/////////////////////////////////////////////////////////////////////////////
// Name:        xmlres.cpp
// Purpose:     XML resources
// Author:      Vaclav Slavik
// Created:     2000/03/05
// RCS-ID:      $Id$
// Copyright:   (c) 2000 Vaclav Slavik
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////
 
#ifdef __GNUG__
#pragma implementation "xmlres.h"
#endif

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#include "wx/dialog.h"
#include "wx/panel.h"
#include "wx/wfstream.h"
#include "wx/filesys.h"
#include "wx/log.h"
#include "wx/intl.h"
#include "wx/tokenzr.h"
#include "wx/module.h"
#include "wx/bitmap.h"
#include "wx/image.h"

#include "wx/xml/xml.h"
#include "wx/xml/xmlres.h"

#include "wx/arrimpl.cpp"
WX_DEFINE_OBJARRAY(wxXmlResourceDataRecords);


wxXmlResource::wxXmlResource()
{
    m_Handlers.DeleteContents(TRUE);
}
    
wxXmlResource::wxXmlResource(const wxString& filemask)
{
    m_Handlers.DeleteContents(TRUE);
    Load(filemask);
}

wxXmlResource::~wxXmlResource()
{
    ClearHandlers();
}


bool wxXmlResource::Load(const wxString& filemask)
{
    wxString fnd;
    wxXmlResourceDataRecord *drec;
    bool iswild = wxIsWild(filemask);
    
#if wxUSE_FILESYSTEM
    wxFileSystem fsys;
#   define wxXmlFindFirst  fsys.FindFirst(filemask, wxFILE)
#   define wxXmlFindNext   fsys.FindNext()
#else
#   define wxXmlFindFirst  wxFindFirstFile(filemask, wxFILE)
#   define wxXmlFindNext   wxFindNextFile()
#endif
    if (iswild)
        fnd = wxXmlFindFirst;
    else
        fnd = filemask;
    while (!!fnd)
    {
#if wxUSE_FILESYSTEM
        if (filemask.Lower().Matches("*.zip") ||
            filemask.Lower().Matches("*.rsc"))
        {
            wxFileSystem fs2;
            wxString fnd2;
            
            fnd2 = fs2.FindFirst(fnd + wxT("#zip:*.xmb"), wxFILE);
            while (!!fnd2)
            {
                drec = new wxXmlResourceDataRecord;
                drec->File = fnd2;
                m_Data.Add(drec);
                fnd2 = fs2.FindNext();
            }
        }
        else
#endif
        {
            drec = new wxXmlResourceDataRecord;
            drec->File = fnd;
            m_Data.Add(drec);
        }

        if (iswild)
            fnd = wxXmlFindNext;
        else
            fnd = wxEmptyString;
    }
#   undef wxXmlFindFirst
#   undef wxXmlFindNext
    return TRUE;
}



void wxXmlResource::AddHandler(wxXmlResourceHandler *handler)
{
    m_Handlers.Append(handler);
    handler->SetParentResource(this);
}



void wxXmlResource::ClearHandlers()
{
    m_Handlers.Clear();
}



wxMenu *wxXmlResource::LoadMenu(const wxString& name)
{
    return (wxMenu*)CreateResFromNode(FindResource(name, wxT("menu")), NULL, NULL);
}



wxMenuBar *wxXmlResource::LoadMenuBar(const wxString& name)
{
    return (wxMenuBar*)CreateResFromNode(FindResource(name, wxT("menubar")), NULL, NULL);
}



wxToolBar *wxXmlResource::LoadToolBar(wxWindow *parent, const wxString& name)
{
    return (wxToolBar*)CreateResFromNode(FindResource(name, wxT("toolbar")), parent, NULL);
}



wxDialog *wxXmlResource::LoadDialog(wxWindow *parent, const wxString& name)
{
    wxDialog *dialog = new wxDialog;
    if (!LoadDialog(dialog, parent, name))
        { delete dialog; return NULL; }
    else return dialog;
}

bool wxXmlResource::LoadDialog(wxDialog *dlg, wxWindow *parent, const wxString& name)
{
    return CreateResFromNode(FindResource(name, wxT("dialog")), parent, dlg) != NULL;
}



wxPanel *wxXmlResource::LoadPanel(wxWindow *parent, const wxString& name)
{
    wxPanel *panel = new wxPanel;
    if (!LoadPanel(panel, parent, name))
        { delete panel; return NULL; }
    else return panel;
}

bool wxXmlResource::LoadPanel(wxPanel *panel, wxWindow *parent, const wxString& name)
{
    return CreateResFromNode(FindResource(name, wxT("panel")), parent, panel) != NULL;
}



void wxXmlResource::ProcessPlatformProperty(wxXmlNode *node)
{
    wxString s;
    bool isok;
    
    wxXmlNode *c = node->GetChildren();
    while (c)
    {
        isok = FALSE;
        if (!c->GetPropVal(_T("platform"), &s)) 
            isok = TRUE;
        else
        {
            wxStringTokenizer tkn(s, " |");

            while (tkn.HasMoreTokens())
            {
                s = tkn.GetNextToken();    
                if (
#ifdef __WXMSW__
                    s == wxString(_T("win"))
#elif defined(__UNIX__)
                    s == wxString(_T("unix"))
#elif defined(__MAC__)
                    s == wxString(_T("mac"))
#elif defined(__OS2__)
                    s == wxString(_T("os2"))
#else
                    FALSE
#endif
              ) isok = TRUE;
            }
        }
        
        if (isok)
            ProcessPlatformProperty(c);
        else
        {
            node->RemoveChild(c);
            delete c;
        }
        
        c = c->GetNext();
    }
}



void wxXmlResource::UpdateResources()
{
    bool modif;
#   if wxUSE_FILESYSTEM
    wxFSFile *file;
    wxFileSystem fsys;
#   endif

    for (size_t i = 0; i < m_Data.GetCount(); i++)
    {
        modif = (m_Data[i].Doc == NULL);

        if (!modif)
        {
#           if wxUSE_FILESYSTEM
            file = fsys.OpenFile(m_Data[i].File);
            modif = file && file->GetModificationTime() > m_Data[i].Time;
            if (!file)
                wxLogError(_("Cannot open file '%s'."), m_Data[i].File.c_str());
            delete file;
#           else
            modif = wxDateTime(wxFileModificationTime(m_Data[i].File)) > m_Data[i].Time;
#           endif
        }
        
        if (modif)
        {
            wxInputStream *stream;

#           if wxUSE_FILESYSTEM
            file = fsys.OpenFile(m_Data[i].File);
            stream = file->GetStream();
#           else
            stream = new wxFileInputStream(m_Data[i].File);
#           endif
            
            if (stream) 
            {
                delete m_Data[i].Doc;
                m_Data[i].Doc = new wxXmlDocument;
            }
            if (!stream || !m_Data[i].Doc->Load(*stream))
            {
                wxLogError(_("Cannot load resources from file '%s'."), m_Data[i].File.c_str());
                delete m_Data[i].Doc;
                m_Data[i].Doc = NULL;
            }
            else if (m_Data[i].Doc->GetRoot()->GetName() != _T("resource")) 
            {
                wxLogError(_("Invalid XML resource '%s': doesn't have root node 'resource'."), m_Data[i].File.c_str());
                delete m_Data[i].Doc;
                m_Data[i].Doc = NULL;
            }
            else
                ProcessPlatformProperty(m_Data[i].Doc->GetRoot());

#           if wxUSE_FILESYSTEM
            delete file;
#           else
            delete stream;
#           endif
        }
    }
}



wxXmlNode *wxXmlResource::FindResource(const wxString& name, const wxString& type)
{
    UpdateResources(); //ensure everything is up-to-date
    
    wxString dummy;   
    for (size_t f = 0; f < m_Data.GetCount(); f++)
    {
        if (m_Data[f].Doc == NULL || m_Data[f].Doc->GetRoot() == NULL) continue;
        for (wxXmlNode *node = m_Data[f].Doc->GetRoot()->GetChildren(); 
                                      node; node = node->GetNext())
            if (    node->GetType() == wxXML_ELEMENT_NODE &&
                    (!type || node->GetName() == type) &&
                    node->GetPropVal(wxT("name"), &dummy) &&
                    dummy == name)
            {
#if wxUSE_FILESYSTEM
                m_CurFileSystem.ChangePathTo(m_Data[f].File);
#endif
                return node;
            }
    }

    wxLogError(_("XML resource '%s' (type '%s') not found!"), 
               name.c_str(), type.c_str());
    return NULL;
}



wxObject *wxXmlResource::CreateResFromNode(wxXmlNode *node, wxObject *parent, wxObject *instance)
{
    if (node == NULL) return NULL;
    
    wxXmlResourceHandler *handler;
    wxObject *ret;
    wxNode * ND = m_Handlers.GetFirst();
    while (ND)
    {
        handler = (wxXmlResourceHandler*)ND->GetData();
        if (handler->CanHandle(node))
        {
            ret = handler->CreateResource(node, parent, instance);
            if (ret) return ret;
        }
        ND = ND->GetNext();
    }

    wxLogError(_("No handler found for XML node '%s'!"), node->GetName().c_str());
    return NULL;
}









wxXmlResourceHandler::wxXmlResourceHandler()
        : m_Node(NULL), m_Parent(NULL), m_Instance(NULL), 
          m_ParentAsWindow(NULL), m_InstanceAsWindow(NULL)
{}



wxObject *wxXmlResourceHandler::CreateResource(wxXmlNode *node, wxObject *parent, wxObject *instance)
{
    wxXmlNode *myNode = m_Node;
    wxObject *myParent = m_Parent, *myInstance = m_Instance;
    wxWindow *myParentAW = m_ParentAsWindow, *myInstanceAW = m_InstanceAsWindow;
    
    m_Node = node;
    m_Parent = parent;
    m_Instance = instance;
    m_ParentAsWindow = wxDynamicCast(m_Parent, wxWindow);
    m_InstanceAsWindow = wxDynamicCast(m_Instance, wxWindow);
    
    wxObject *returned = DoCreateResource();
    
    m_Node = myNode;
    m_Parent = myParent; m_ParentAsWindow = myParentAW;
    m_Instance = myInstance; m_InstanceAsWindow = myInstanceAW;
    
    return returned;
}


void wxXmlResourceHandler::AddStyle(const wxString& name, int value)
{
    m_StyleNames.Add(name);
    m_StyleValues.Add(value);
}



void wxXmlResourceHandler::AddWindowStyles()
{
    ADD_STYLE(wxSIMPLE_BORDER);
    ADD_STYLE(wxSUNKEN_BORDER);
    ADD_STYLE(wxDOUBLE_BORDER);
    ADD_STYLE(wxRAISED_BORDER);
    ADD_STYLE(wxSTATIC_BORDER);
    ADD_STYLE(wxTRANSPARENT_WINDOW);
    ADD_STYLE(wxWANTS_CHARS);
    ADD_STYLE(wxNO_FULL_REPAINT_ON_RESIZE);
}



bool wxXmlResourceHandler::HasParam(const wxString& param)
{
    return (GetParamNode(param) != NULL);
}


int wxXmlResourceHandler::GetStyle(const wxString& param, int defaults)
{
    wxString s = GetParamValue(param);
    
    if (!s) return defaults;
    
    wxStringTokenizer tkn(s, _T("| "), wxTOKEN_STRTOK);
    int style = 0;
    int index;
    wxString fl;
    while (tkn.HasMoreTokens())
    {
        fl = tkn.GetNextToken();
        index = m_StyleNames.Index(fl);
        if (index != wxNOT_FOUND)
            style |= m_StyleValues[index];
        else
            wxLogError(_("Unknown style flag ") + fl);
    }
    return style;
}



wxString wxXmlResourceHandler::GetText(const wxString& param)
{
    wxString str1 = GetParamValue(param);
    wxString str2;
    const wxChar *dt;

    for (dt = str1.c_str(); *dt; dt++)
    {
        // Remap $ to &, map $$ to $ (for things like "&File..." -- 
        // this is illegal in XML, so we use "$File..."):
        if (*dt == '$')
            switch (*(++dt))
            {
                case '$' : str2 << '$'; break;
                default  : str2 << '&' << *dt; break;
            }
        // Remap \n to CR, \r LF, \t to TAB:
        else if (*dt == '\\')
            switch (*(++dt))
            {
                case 'n' : str2 << '\n'; break;
                case 't' : str2 << '\t'; break;
                case 'r' : str2 << '\r'; break;
                default  : str2 << '\\' << *dt; break;
            }
        else str2 << *dt;
    }
    return str2;
}



long wxXmlResourceHandler::GetLong(const wxString& param, long defaultv)
{
    long value;
    wxString str1 = GetParamValue(param);

    if (!str1.ToLong(&value)) 
        value = defaultv;

    return value;
}


int wxXmlResourceHandler::GetID()
{
    wxString sid = GetName();
    long num;
    
    if (sid == _T("-1")) return -1;
    else if (sid.IsNumber() && sid.ToLong(&num)) return num;
#define stdID(id) else if (sid == _T(#id)) return id
    stdID(wxID_OPEN); stdID(wxID_CLOSE); stdID(wxID_NEW);
    stdID(wxID_SAVE); stdID(wxID_SAVEAS); stdID(wxID_REVERT);
    stdID(wxID_EXIT); stdID(wxID_UNDO); stdID(wxID_REDO);
    stdID(wxID_HELP); stdID(wxID_PRINT); stdID(wxID_PRINT_SETUP);
    stdID(wxID_PREVIEW); stdID(wxID_ABOUT); stdID(wxID_HELP_CONTENTS);
    stdID(wxID_HELP_COMMANDS); stdID(wxID_HELP_PROCEDURES); 
    stdID(wxID_CUT); stdID(wxID_COPY); stdID(wxID_PASTE); 
    stdID(wxID_CLEAR); stdID(wxID_FIND); stdID(wxID_DUPLICATE);
    stdID(wxID_SELECTALL); stdID(wxID_OK); stdID(wxID_CANCEL);
    stdID(wxID_APPLY); stdID(wxID_YES); stdID(wxID_NO);
    stdID(wxID_STATIC); stdID(wxID_FORWARD); stdID(wxID_BACKWARD);
    stdID(wxID_DEFAULT); stdID(wxID_MORE); stdID(wxID_SETUP);
    stdID(wxID_RESET); stdID(wxID_HELP_CONTEXT); 
#undef stdID
    else return XMLID(sid.c_str());
}


wxString wxXmlResourceHandler::GetName()
{
    return m_Node->GetPropVal(_T("name"), _T("-1"));
}



bool wxXmlResourceHandler::GetBool(const wxString& param, bool defaultv)
{
    wxString v = GetParamValue(param);
    v.MakeLower();
    if (!v) return defaultv;
    else return (v == _T("1"));
}



wxColour wxXmlResourceHandler::GetColour(const wxString& param)
{
    wxString v = GetParamValue(param);
    unsigned long tmp = 0; 
    
    if (v.Length() != 7 || v[0] != _T('#') ||
        wxSscanf(v.c_str(), _T("#%lX"), &tmp) != 1)
    {
        wxLogError(_("XML resource: Incorrect colour specification '%s' for property '%s'."),
                   v.c_str(), param.c_str());
        return wxNullColour;
    }
    
    return wxColour((tmp & 0xFF0000) >> 16 , 
                    (tmp & 0x00FF00) >> 8, 
                    (tmp & 0x0000FF));
}



wxBitmap wxXmlResourceHandler::GetBitmap(const wxString& param, wxSize size)
{
    wxString name = GetParamValue(param);
    if (name.IsEmpty()) return wxNullBitmap;
#if wxUSE_FILESYSTEM
    wxFSFile *fsfile = GetCurFileSystem().OpenFile(name);
    if (fsfile == NULL)
    {
        wxLogError(_("XML resource: Cannot create bitmap from '%s'."), param.mb_str());
        return wxNullBitmap;
    }    
    wxImage img(*(fsfile->GetStream()));
    delete fsfile;
#else
    wxImage img(GetParamValue(_T("bitmap")));
#endif
    if (!img.Ok()) 
    {
        wxLogError(_("XML resource: Cannot create bitmap from '%s'."), param.mb_str());
        return wxNullBitmap;
    }
    if (!(size == wxDefaultSize)) img.Rescale(size.x, size.y);
    return img.ConvertToBitmap();
}



wxIcon wxXmlResourceHandler::GetIcon(const wxString& param, wxSize size)
{
#if wxCHECK_VERSION(2,3,0) || defined(__WXMSW__)
    wxIcon icon;
    icon.CopyFromBitmap(GetBitmap(param, size));
#else
    wxIcon *iconpt;
    wxBitmap bmppt = GetBitmap(param, size);
    iconpt = (wxIcon*)(&bmppt);
    wxIcon icon(*iconpt);
#endif
    return icon;
}



wxXmlNode *wxXmlResourceHandler::GetParamNode(const wxString& param)
{
    wxXmlNode *n = m_Node->GetChildren();
    
    while (n)
    {
        if (n->GetType() == wxXML_ELEMENT_NODE && n->GetName() == param)
            return n;
        n = n->GetNext();
    }
    return NULL;
}


wxString wxXmlResourceHandler::GetNodeContent(wxXmlNode *node)
{
    wxXmlNode *n = node;
    if (n == NULL) return wxEmptyString;
    n = n->GetChildren();
    
    while (n)
    {
        if (n->GetType() == wxXML_TEXT_NODE || 
            n->GetType() == wxXML_CDATA_SECTION_NODE)
            return n->GetContent();
        n = n->GetNext();
    }
    return wxEmptyString;
}



wxString wxXmlResourceHandler::GetParamValue(const wxString& param)
{
    return GetNodeContent(GetParamNode(param));
}



wxSize wxXmlResourceHandler::GetSize(const wxString& param)
{
    wxString s = GetParamValue(param);
    if (s.IsEmpty()) s = _T("-1,-1");
    bool is_dlg;
    long sx, sy;
    
    is_dlg = s[s.Length()-1] == _T('d');
    if (is_dlg) s.RemoveLast();
    
    if (!s.BeforeFirst(_T(',')).ToLong(&sx) ||
        !s.AfterLast(_T(',')).ToLong(&sy))
    {
        wxLogError(_("Cannot parse coordinates from '%s'."), s.mb_str());
        return wxDefaultSize;
    }
    
    if (is_dlg)
    {
        if (m_InstanceAsWindow)
            return wxDLG_UNIT(m_InstanceAsWindow, wxSize(sx, sy));
        else if (m_ParentAsWindow)
            return wxDLG_UNIT(m_ParentAsWindow, wxSize(sx, sy));
        else
        {
            wxLogError(_("Cannot convert dialog units: dialog unknown."));
            return wxDefaultSize;
        }
    }
    else return wxSize(sx, sy);
}



wxPoint wxXmlResourceHandler::GetPosition(const wxString& param)
{
    wxSize sz = GetSize(param);
    return wxPoint(sz.x, sz.y);
}



wxCoord wxXmlResourceHandler::GetDimension(const wxString& param, wxCoord defaultv)
{
    wxString s = GetParamValue(param);
    if (s.IsEmpty()) return defaultv;
    bool is_dlg;
    long sx;
    
    is_dlg = s[s.Length()-1] == _T('d');
    if (is_dlg) s.RemoveLast();
    
    if (!s.ToLong(&sx))
    {
        wxLogError(_("Cannot parse dimension from '%s'."), s.mb_str());
        return defaultv;
    }
    
    if (is_dlg)
    {
        if (m_InstanceAsWindow)
            return wxDLG_UNIT(m_InstanceAsWindow, wxSize(sx, 0)).x;
        else if (m_ParentAsWindow)
            return wxDLG_UNIT(m_ParentAsWindow, wxSize(sx, 0)).x;
        else
        {
            wxLogError(_("Cannot convert dialog units: dialog unknown."));
            return defaultv;
        }
    }
    else return sx;
}



void wxXmlResourceHandler::SetupWindow(wxWindow *wnd)
{
    //FIXME : add font, cursor
    
    if (HasParam(_T("exstyle")))
        wnd->SetExtraStyle(GetStyle(_T("exstyle")));
    if (HasParam(_T("bg")))
        wnd->SetBackgroundColour(GetColour(_T("bg")));
    if (HasParam(_T("fg")))
        wnd->SetForegroundColour(GetColour(_T("fg")));
    if (GetBool(_T("enabled"), 1) == 0)
        wnd->Enable(FALSE);
    if (GetBool(_T("focused"), 0) == 1)
        wnd->SetFocus();
    if (GetBool(_T("hidden"), 0) == 1)
        wnd->Show(FALSE);
#if wxUSE_TOOLTIPS
    if (HasParam(_T("tooltip")))
        wnd->SetToolTip(GetText(_T("tooltip")));
#endif      
}


void wxXmlResourceHandler::CreateChildren(wxObject *parent, 
                bool only_this_handler, wxXmlNode *children_node)
{
    if (children_node == NULL) children_node = GetParamNode(_T("children"));
    if (children_node == NULL) return;

    wxXmlNode *n = children_node->GetChildren();

    while (n)
    {
        if (n->GetType() == wxXML_ELEMENT_NODE)
        {        
            if (only_this_handler)
            {
                if (CanHandle(n))
                    CreateResource(n, parent, NULL);
            }
            else
                m_Resource->CreateResFromNode(n, parent, NULL);
        }
        n = n->GetNext();
    }
}









// --------------- XMLID implementation -----------------------------

#define XMLID_TABLE_SIZE     1024


struct XMLID_record
{
    int id;
    char *key;
    XMLID_record *next;
};

static XMLID_record *XMLID_Records[XMLID_TABLE_SIZE] = {NULL};
static int XMLID_LastID = wxID_HIGHEST;

/*static*/ int wxXmlResource::GetXMLID(const char *str_id)
{
    int index = 0;
    
    for (const char *c = str_id; *c != '\0'; c++) index += (int)*c;
    index %= XMLID_TABLE_SIZE;
    
    XMLID_record *oldrec = NULL;
    int matchcnt = 0;
    for (XMLID_record *rec = XMLID_Records[index]; rec; rec = rec->next)
    {
        if (strcmp(rec->key, str_id) == 0)
        {
            return rec->id;
        }
        matchcnt++;
        oldrec = rec;
    }
    
    XMLID_record **rec_var = (oldrec == NULL) ? 
                              &XMLID_Records[index] : &oldrec->next;
    *rec_var = new XMLID_record;
    (*rec_var)->id = ++XMLID_LastID;
    (*rec_var)->key = strdup(str_id);
    (*rec_var)->next = NULL;
    
    return (*rec_var)->id;
}


static void CleanXMLID_Record(XMLID_record *rec)
{
    if (rec)
    {
        CleanXMLID_Record(rec->next);
        free (rec->key);
        delete rec;
    }
}

static void CleanXMLID_Records()
{
    for (int i = 0; i < XMLID_TABLE_SIZE; i++)
        CleanXMLID_Record(XMLID_Records[i]);
}








// --------------- module and globals -----------------------------


static wxXmlResource gs_XmlResource;

wxXmlResource *wxTheXmlResource = &gs_XmlResource;


class wxXmlResourceModule: public wxModule
{
DECLARE_DYNAMIC_CLASS(wxXmlResourceModule)
public:
    wxXmlResourceModule() {}
    bool OnInit() {return TRUE;}
    void OnExit() 
    {
        wxTheXmlResource->ClearHandlers();
        CleanXMLID_Records();
    }
};

IMPLEMENT_DYNAMIC_CLASS(wxXmlResourceModule, wxModule)
