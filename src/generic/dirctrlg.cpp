/////////////////////////////////////////////////////////////////////////////
// Name:        dirctrlg.cpp
// Purpose:     wxGenericDirCtrl
// Author:      Harm van der Heijden, Robert Roebling, Julian Smart
// Modified by:
// Created:     12/12/98
// RCS-ID:      $Id$
// Copyright:   (c) Harm van der Heijden, Robert Roebling and Julian Smart
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "dirctrlg.h"
#endif

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#if wxUSE_DIRDLG

#include "wx/generic/dirctrlg.h"

#include "wx/module.h"
#include "wx/utils.h"
#include "wx/button.h"
#include "wx/layout.h"
#include "wx/msgdlg.h"
#include "wx/textctrl.h"
#include "wx/textdlg.h"
#include "wx/filefn.h"
#include "wx/cmndata.h"
#include "wx/gdicmn.h"
#include "wx/intl.h"
#include "wx/imaglist.h"
#include "wx/icon.h"
#include "wx/log.h"
#include "wx/sizer.h"
#include "wx/tokenzr.h"
#include "wx/dir.h"
#include "wx/settings.h"
#include "wx/artprov.h"
#include "wx/hash.h"
#include "wx/mimetype.h"
#include "wx/image.h"
#include "wx/choice.h"
#include "wx/filedlg.h"  // for wxFileDialogBase::ParseWildcard

#if wxUSE_STATLINE
    #include "wx/statline.h"
#endif

#if defined(__WXMAC__)
  #include  "wx/mac/private.h"  // includes mac headers
#endif

#ifdef __WXMSW__
#include <windows.h>

// FIXME - Mingw32 1.0 has both _getdrive() and _chdrive(). For now, let's assume
//         older releases don't, but it should be verified and the checks modified
//         accordingly.
#if !defined(__GNUWIN32__) || (defined(__MINGW32_MAJOR_VERSION) && __MINGW32_MAJOR_VERSION >= 1)
#if !defined(__WXWINCE__)
  #include <direct.h>
#endif
  #include <stdlib.h>
  #include <ctype.h>
#endif

#endif

#ifdef __OS2__

#define INCL_BASE
#include <os2.h>
#ifndef __EMX__
#include <direct.h>
#endif
#include <stdlib.h>
#include <ctype.h>
extern bool wxIsDriveAvailable(const wxString& dirName);
#endif // __OS2__

#if defined(__WXMAC__)
#  ifdef __DARWIN__
#    include "MoreFilesX.h"
#  else
#    include "MoreFilesExtras.h"
#  endif
#endif

#ifdef __BORLANDC__
#include "dos.h"
#endif

// If compiled under Windows, this macro can cause problems
#ifdef GetFirstChild
#undef GetFirstChild
#endif

// ----------------------------------------------------------------------------
// wxGetAvailableDrives, for WINDOWS, DOS, OS2, MAC, UNIX (returns "/")
// ----------------------------------------------------------------------------

size_t wxGetAvailableDrives(wxArrayString &paths, wxArrayString &names, wxArrayInt &icon_ids)
{
#if defined(__WINDOWS__) || defined(__DOS__) || defined(__OS2__)

#ifdef __WXWINCE__
    // No logical drives; return "\"
    paths.Add(wxT("\\"));
    names.Add(wxT("\\"));
    return 1;
#elif defined(__WIN32__)
    wxChar driveBuffer[256];
    size_t n = (size_t) GetLogicalDriveStrings(255, driveBuffer);
    size_t i = 0;
    while (i < n)
    {
        wxString path, name;
        path.Printf(wxT("%c:\\"), driveBuffer[i]);
        name.Printf(wxT("%c:"), driveBuffer[i]);

        int imageId;
        int driveType = ::GetDriveType(path);
        switch (driveType)
        {
            case DRIVE_REMOVABLE:
                if (path == wxT("a:\\") || path == wxT("b:\\"))
                    imageId = wxFileIconsTable::floppy;
                else
                    imageId = wxFileIconsTable::removeable;
                break;
            case DRIVE_CDROM:
                imageId = wxFileIconsTable::cdrom;
                break;
            case DRIVE_REMOTE:
            case DRIVE_FIXED:
            default:
                imageId = wxFileIconsTable::drive;
                break;
        }

        paths.Add(path);
        names.Add(name);
        icon_ids.Add(imageId);

        while (driveBuffer[i] != wxT('\0'))
            i ++;
        i ++;
        if (driveBuffer[i] == wxT('\0'))
            break;
    }
#elif defined(__OS2__)
    APIRET rc;
    ULONG ulDriveNum = 0;
    ULONG ulDriveMap = 0;
    rc = ::DosQueryCurrentDisk(&ulDriveNum, &ulDriveMap);
    if ( rc == 0){
        size_t i = 0;
        while (i < 26)
        {
	    if (ulDriveMap & ( 1 << i ))
	    {
		wxString path, name;
		path.Printf(wxT("%c:\\"), 'A' + i);
		name.Printf(wxT("%c:"), 'A' + i);

		int imageId;
		if (path == wxT("A:\\") || path == wxT("B:\\"))
		  imageId = wxFileIconsTable::floppy;
		else
		  imageId = wxFileIconsTable::drive;
		paths.Add(path);
		names.Add(name);
		icon_ids.Add(imageId);
	    }
            i ++;
	}
    } 
#else // !__WIN32__, !__OS2__
    int drive;

    /* If we can switch to the drive, it exists. */
    for( drive = 1; drive <= 26; drive++ )
    {
        wxString path, name;
        path.Printf(wxT("%c:\\"), (char) (drive + 'a' - 1));
        name.Printf(wxT("%c:"), (char) (drive + 'A' - 1));

        if (wxIsDriveAvailable(path))
        {
            paths.Add(path);
            names.Add(name);
            icon_ids.Add((drive <= 2) ? wxFileIconsTable::floppy : wxFileIconsTable::drive);
        }
    }
#endif // __WIN32__/!__WIN32__

#elif defined(__WXMAC__)
#ifdef __DARWIN__
    FSRef     **theVolRefs;
    ItemCount   theVolCount;
    char        thePath[FILENAME_MAX];

    if (FSGetMountedVolumes(&theVolRefs, &theVolCount) == noErr) {
        ItemCount index;
        ::HLock( (Handle)theVolRefs ) ;
        for (index = 0; index < theVolCount; ++index) {
            // get the POSIX path associated with the FSRef
            if ( FSRefMakePath(&((*theVolRefs)[index]),
                                 (UInt8 *)thePath, sizeof(thePath)) != noErr ) {
                continue;
            }
            // add path separator at end if necessary
            wxString path( thePath , wxConvLocal) ;
            if (path.Last() != wxFILE_SEP_PATH) {
                path += wxFILE_SEP_PATH;
            }
            // get Mac volume name for display
            FSVolumeRefNum vRefNum ;
            HFSUniStr255 volumeName ;

            if ( FSGetVRefNum(&((*theVolRefs)[index]), &vRefNum) != noErr ) {
                continue;
            }
            if ( FSGetVInfo(vRefNum, &volumeName, NULL, NULL) != noErr ) {
                continue;
            }
            // get C string from Unicode HFS name
            //   see: http://developer.apple.com/carbon/tipsandtricks.html
            CFStringRef cfstr = CFStringCreateWithCharacters( kCFAllocatorDefault,
                                                              volumeName.unicode,
                                                              volumeName.length );
            //	Do something with str
            char *cstr = NewPtr(CFStringGetLength(cfstr) + 1);
            if (( cstr == NULL ) ||
                !CFStringGetCString(cfstr, cstr, CFStringGetLength(cfstr) + 1,
                                    kCFStringEncodingMacRoman))
            {
                CFRelease( cstr );
                continue;
            }
            wxString name( cstr , wxConvLocal );
            DisposePtr( cstr );
            CFRelease( cfstr );

            GetVolParmsInfoBuffer volParmsInfo;
            UInt32 actualSize;
            if ( FSGetVolParms(vRefNum, sizeof(volParmsInfo), &volParmsInfo, &actualSize) != noErr ) {
                continue;
            }

            paths.Add(path);
            names.Add(name);

            if ( VolIsEjectable(&volParmsInfo) )
                icon_ids.Add(wxFileIconsTable::cdrom);
            else
                icon_ids.Add(wxFileIconsTable::drive);
        }
        ::HUnlock( (Handle)theVolRefs );
        ::DisposeHandle( (Handle)theVolRefs );
    }
#else // !__DARWIN__
    FSSpec volume;
    short index = 1;
    while(1)
    {
        short actualCount = 0 ;
        if (OnLine(&volume, 1, &actualCount, &index ) != noErr || actualCount==0)
        {
            break;
        }

        wxString name = wxMacFSSpec2MacFilename( &volume );
        paths.Add(name + wxFILE_SEP_PATH);
        names.Add(name);
        icon_ids.Add(wxFileIconsTable::drive);
    }
#endif // __DARWIN__

#elif defined(__UNIX__)
    paths.Add(wxT("/"));
    names.Add(wxT("/"));
    icon_ids.Add(wxFileIconsTable::computer);
#else
    #error "Unsupported platform in wxGenericDirCtrl!"
#endif
    return paths.GetCount();
}

// ----------------------------------------------------------------------------
// wxIsDriveAvailable
// ----------------------------------------------------------------------------

#if defined(__DOS__)

bool wxIsDriveAvailable(const wxString& dirName)
{
    // FIXME_MGL - this method leads to hang up under Watcom for some reason
#ifndef __WATCOMC__
    if ( dirName.Len() == 3 && dirName[1u] == wxT(':') )
    {
        wxString dirNameLower(dirName.Lower());
        // VS: always return TRUE for removable media, since Win95 doesn't
        //     like it when MS-DOS app accesses empty floppy drive
        return (dirNameLower[0u] == wxT('a') ||
                dirNameLower[0u] == wxT('b') ||
                wxPathExists(dirNameLower));
    }
    else
#endif
        return TRUE;
}

#elif defined(__WINDOWS__) || defined(__OS2__)

int setdrive(int drive)
{
#ifdef __WXWINCE__
    return 0;
#elif defined(__GNUWIN32__) && \
    (defined(__MINGW32_MAJOR_VERSION) && __MINGW32_MAJOR_VERSION >= 1)
    return _chdrive(drive);
#else
	wxChar  newdrive[4];

	if (drive < 1 || drive > 31)
		return -1;
	newdrive[0] = (wxChar)(wxT('A') + drive - 1);
	newdrive[1] = wxT(':');
#ifdef __OS2__
	newdrive[2] = wxT('\\');
	newdrive[3] = wxT('\0');
#else
	newdrive[2] = wxT('\0');
#endif
#if defined(__WXMSW__)
	if (::SetCurrentDirectory(newdrive))
#else
    // VA doesn't know what LPSTR is and has its own set
	if (!DosSetCurrentDir((PSZ)newdrive))
#endif
		return 0;
	else
		return -1;
#endif // !GNUWIN32
}

bool wxIsDriveAvailable(const wxString& dirName)
{
#ifdef __WXWINCE__
    return FALSE;
#else
#ifdef __WIN32__
    UINT errorMode = SetErrorMode(SEM_FAILCRITICALERRORS | SEM_NOOPENFILEERRORBOX);
#endif
    bool success = TRUE;

    // Check if this is a root directory and if so,
    // whether the drive is available.
    if (dirName.Len() == 3 && dirName[(size_t)1] == wxT(':'))
    {
        wxString dirNameLower(dirName.Lower());
#if defined(__GNUWIN32__) && !(defined(__MINGW32_MAJOR_VERSION) && __MINGW32_MAJOR_VERSION >= 1)
        success = wxPathExists(dirNameLower);
#else
        #if defined(__OS2__)
        // Avoid changing to drive since no media may be inserted.
        if (dirNameLower[(size_t)0] == 'a' || dirNameLower[(size_t)0] == 'b')
            return success;
        #endif
        int currentDrive = _getdrive();
        int thisDrive = (int) (dirNameLower[(size_t)0] - 'a' + 1) ;
        int err = setdrive( thisDrive ) ;
        setdrive( currentDrive );

        if (err == -1)
        {
            success = FALSE;
        }
#endif
    }
#ifdef __WIN32__
    (void) SetErrorMode(errorMode);
#endif

    return success;
#endif
}
#endif // __WINDOWS__ || __OS2__


// Function which is called by quick sort. We want to override the default wxArrayString behaviour,
// and sort regardless of case.
static int wxCMPFUNC_CONV wxDirCtrlStringCompareFunction(wxString* strFirst, wxString* strSecond)
{
    return strFirst->CmpNoCase(*strSecond);
}

//-----------------------------------------------------------------------------
// wxDirItemData
//-----------------------------------------------------------------------------

wxDirItemData::wxDirItemData(const wxString& path, const wxString& name,
                             bool isDir)
{
    m_path = path;
    m_name = name;
    /* Insert logic to detect hidden files here
     * In UnixLand we just check whether the first char is a dot
     * For FileNameFromPath read LastDirNameInThisPath ;-) */
    // m_isHidden = (bool)(wxFileNameFromPath(*m_path)[0] == '.');
    m_isHidden = FALSE;
    m_isExpanded = FALSE;
    m_isDir = isDir;
}

wxDirItemData::~wxDirItemData()
{
}

void wxDirItemData::SetNewDirName(const wxString& path)
{
    m_path = path;
    m_name = wxFileNameFromPath(path);
}

bool wxDirItemData::HasSubDirs() const
{
    if (m_path.IsEmpty())
        return FALSE;

    wxDir dir;
    {
        wxLogNull nolog;
        if ( !dir.Open(m_path) )
            return FALSE;
    }

    return dir.HasSubDirs();
}

bool wxDirItemData::HasFiles(const wxString& WXUNUSED(spec)) const
{
    if (m_path.IsEmpty())
        return FALSE;

    wxDir dir;
    {
        wxLogNull nolog;
        if ( !dir.Open(m_path) )
            return FALSE;
    }

    return dir.HasFiles();
}

//-----------------------------------------------------------------------------
// wxGenericDirCtrl
//-----------------------------------------------------------------------------


#if wxUSE_EXTENDED_RTTI
WX_DEFINE_FLAGS( wxGenericDirCtrlStyle )

wxBEGIN_FLAGS( wxGenericDirCtrlStyle )
    // new style border flags, we put them first to
    // use them for streaming out
    wxFLAGS_MEMBER(wxBORDER_SIMPLE)
    wxFLAGS_MEMBER(wxBORDER_SUNKEN)
    wxFLAGS_MEMBER(wxBORDER_DOUBLE)
    wxFLAGS_MEMBER(wxBORDER_RAISED)
    wxFLAGS_MEMBER(wxBORDER_STATIC)
    wxFLAGS_MEMBER(wxBORDER_NONE)
    
    // old style border flags
    wxFLAGS_MEMBER(wxSIMPLE_BORDER)
    wxFLAGS_MEMBER(wxSUNKEN_BORDER)
    wxFLAGS_MEMBER(wxDOUBLE_BORDER)
    wxFLAGS_MEMBER(wxRAISED_BORDER)
    wxFLAGS_MEMBER(wxSTATIC_BORDER)
    wxFLAGS_MEMBER(wxBORDER)

    // standard window styles
    wxFLAGS_MEMBER(wxTAB_TRAVERSAL)
    wxFLAGS_MEMBER(wxCLIP_CHILDREN)
    wxFLAGS_MEMBER(wxTRANSPARENT_WINDOW)
    wxFLAGS_MEMBER(wxWANTS_CHARS)
    wxFLAGS_MEMBER(wxFULL_REPAINT_ON_RESIZE)
    wxFLAGS_MEMBER(wxALWAYS_SHOW_SB )
    wxFLAGS_MEMBER(wxVSCROLL)
    wxFLAGS_MEMBER(wxHSCROLL)

    wxFLAGS_MEMBER(wxDIRCTRL_DIR_ONLY)
    wxFLAGS_MEMBER(wxDIRCTRL_3D_INTERNAL)
    wxFLAGS_MEMBER(wxDIRCTRL_SELECT_FIRST)
    wxFLAGS_MEMBER(wxDIRCTRL_SHOW_FILTERS)

wxEND_FLAGS( wxGenericDirCtrlStyle )

IMPLEMENT_DYNAMIC_CLASS_XTI(wxGenericDirCtrl, wxControl,"wx/dirctrl.h")

wxBEGIN_PROPERTIES_TABLE(wxGenericDirCtrl)
    wxHIDE_PROPERTY( Children )
	wxPROPERTY( DefaultPath , wxString , SetDefaultPath , GetDefaultPath  , , 0 /*flags*/ , wxT("Helpstring") , wxT("group"))
	wxPROPERTY( Filter , wxString , SetFilter , GetFilter  ,, 0 /*flags*/ , wxT("Helpstring") , wxT("group") )
	wxPROPERTY( DefaultFilter , int , SetFilterIndex, GetFilterIndex,, 0 /*flags*/ , wxT("Helpstring") , wxT("group") )
wxEND_PROPERTIES_TABLE()

wxBEGIN_HANDLERS_TABLE(wxGenericDirCtrl)
wxEND_HANDLERS_TABLE()

wxCONSTRUCTOR_8( wxGenericDirCtrl , wxWindow* , Parent , wxWindowID , Id , wxString , DefaultPath , 
                 wxPoint , Position , wxSize , Size , long , WindowStyle , wxString , Filter , int , DefaultFilter ) 
#else
IMPLEMENT_DYNAMIC_CLASS(wxGenericDirCtrl, wxControl)
#endif

BEGIN_EVENT_TABLE(wxGenericDirCtrl, wxControl)
  EVT_TREE_ITEM_EXPANDING     (-1, wxGenericDirCtrl::OnExpandItem)
  EVT_TREE_ITEM_COLLAPSED     (-1, wxGenericDirCtrl::OnCollapseItem)
  EVT_TREE_BEGIN_LABEL_EDIT   (-1, wxGenericDirCtrl::OnBeginEditItem)
  EVT_TREE_END_LABEL_EDIT     (-1, wxGenericDirCtrl::OnEndEditItem)
  EVT_SIZE                    (wxGenericDirCtrl::OnSize)
END_EVENT_TABLE()

wxGenericDirCtrl::wxGenericDirCtrl(void)
{
    Init();
}

bool wxGenericDirCtrl::Create(wxWindow *parent,
                              const wxWindowID id,
                              const wxString& dir,
                              const wxPoint& pos,
                              const wxSize& size,
                              long style,
                              const wxString& filter,
                              int defaultFilter,
                              const wxString& name)
{
    if (!wxControl::Create(parent, id, pos, size, style, wxDefaultValidator, name))
        return FALSE;

    SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_3DFACE));

    Init();

    long treeStyle = wxTR_HAS_BUTTONS | wxTR_HIDE_ROOT;

    if (style & wxDIRCTRL_EDIT_LABELS)
        treeStyle |= wxTR_EDIT_LABELS;

    if ((style & wxDIRCTRL_3D_INTERNAL) == 0)
        treeStyle |= wxNO_BORDER;
    else
        treeStyle |= wxBORDER_SUNKEN;

    long filterStyle = 0;
    if ((style & wxDIRCTRL_3D_INTERNAL) == 0)
        filterStyle |= wxNO_BORDER;
    else
        filterStyle |= wxBORDER_SUNKEN;

    m_treeCtrl = new wxTreeCtrl(this, wxID_TREECTRL, pos, size, treeStyle);

    if (!filter.IsEmpty() && (style & wxDIRCTRL_SHOW_FILTERS))
        m_filterListCtrl = new wxDirFilterListCtrl(this, wxID_FILTERLISTCTRL, wxDefaultPosition, wxDefaultSize, filterStyle);

    m_defaultPath = dir;
    m_filter = filter;

    SetFilterIndex(defaultFilter);

    if (m_filterListCtrl)
        m_filterListCtrl->FillFilterList(filter, defaultFilter);

    m_treeCtrl->SetImageList(wxTheFileIconsTable->GetSmallImageList());

    m_showHidden = FALSE;
    wxDirItemData* rootData = new wxDirItemData(wxT(""), wxT(""), TRUE);

    wxString rootName;

#if defined(__WINDOWS__) || defined(__OS2__) || defined(__DOS__)
    rootName = _("Computer");
#else
    rootName = _("Sections");
#endif

    m_rootId = m_treeCtrl->AddRoot( rootName, 3, -1, rootData);
    m_treeCtrl->SetItemHasChildren(m_rootId);
    ExpandDir(m_rootId); // automatically expand first level

    // Expand and select the default path
    if (!m_defaultPath.IsEmpty())
        ExpandPath(m_defaultPath);

    DoResize();

    return TRUE;
}

wxGenericDirCtrl::~wxGenericDirCtrl()
{
}

void wxGenericDirCtrl::Init()
{
    m_showHidden = FALSE;
    m_currentFilter = 0;
    m_currentFilterStr = wxEmptyString; // Default: any file
    m_treeCtrl = NULL;
    m_filterListCtrl = NULL;
}

void wxGenericDirCtrl::ShowHidden( bool show )
{
    m_showHidden = show;

    wxString path = GetPath();
    ReCreateTree();
    SetPath(path);
}

const wxTreeItemId
wxGenericDirCtrl::AddSection(const wxString& path, const wxString& name, int imageId)
{
    wxDirItemData *dir_item = new wxDirItemData(path,name,TRUE);

    wxTreeItemId id = AppendItem( m_rootId, name, imageId, -1, dir_item);

    m_treeCtrl->SetItemHasChildren(id);

    return id;
}

void wxGenericDirCtrl::SetupSections()
{
    wxArrayString paths, names;
    wxArrayInt icons;

    size_t n, count = wxGetAvailableDrives(paths, names, icons);

    for (n = 0; n < count; n++)
    {
        AddSection(paths[n], names[n], icons[n]);
    }
}

void wxGenericDirCtrl::OnBeginEditItem(wxTreeEvent &event)
{
    // don't rename the main entry "Sections"
    if (event.GetItem() == m_rootId)
    {
        event.Veto();
        return;
    }

    // don't rename the individual sections
    if (m_treeCtrl->GetItemParent( event.GetItem() ) == m_rootId)
    {
        event.Veto();
        return;
    }
}

void wxGenericDirCtrl::OnEndEditItem(wxTreeEvent &event)
{
    if ((event.GetLabel().IsEmpty()) ||
        (event.GetLabel() == _(".")) ||
        (event.GetLabel() == _("..")) ||
        (event.GetLabel().Find(wxT('/')) != wxNOT_FOUND) ||
        (event.GetLabel().Find(wxT('\\')) != wxNOT_FOUND) ||
        (event.GetLabel().Find(wxT('|')) != wxNOT_FOUND))
    {
        wxMessageDialog dialog(this, _("Illegal directory name."), _("Error"), wxOK | wxICON_ERROR );
        dialog.ShowModal();
        event.Veto();
        return;
    }

    wxTreeItemId id = event.GetItem();
    wxDirItemData *data = (wxDirItemData*)m_treeCtrl->GetItemData( id );
    wxASSERT( data );

    wxString new_name( wxPathOnly( data->m_path ) );
    new_name += wxString(wxFILE_SEP_PATH);
    new_name += event.GetLabel();

    wxLogNull log;

    if (wxFileExists(new_name))
    {
        wxMessageDialog dialog(this, _("File name exists already."), _("Error"), wxOK | wxICON_ERROR );
        dialog.ShowModal();
        event.Veto();
    }

    if (wxRenameFile(data->m_path,new_name))
    {
        data->SetNewDirName( new_name );
    }
    else
    {
        wxMessageDialog dialog(this, _("Operation not permitted."), _("Error"), wxOK | wxICON_ERROR );
        dialog.ShowModal();
        event.Veto();
    }
}

void wxGenericDirCtrl::OnExpandItem(wxTreeEvent &event)
{
    wxTreeItemId parentId = event.GetItem();

    // VS: this is needed because the event handler is called from wxTreeCtrl
    //     ctor when wxTR_HIDE_ROOT was specified

    if (!m_rootId.IsOk())

        m_rootId = m_treeCtrl->GetRootItem();

    ExpandDir(parentId);
}

void wxGenericDirCtrl::OnCollapseItem(wxTreeEvent &event )
{
    CollapseDir(event.GetItem());
}

void wxGenericDirCtrl::CollapseDir(wxTreeItemId parentId)
{
    wxTreeItemId child;

    wxDirItemData *data = (wxDirItemData *) m_treeCtrl->GetItemData(parentId);
    if (!data->m_isExpanded)
        return;

    data->m_isExpanded = FALSE;
    wxTreeItemIdValue cookie;
    /* Workaround because DeleteChildren has disapeared (why?) and
     * CollapseAndReset doesn't work as advertised (deletes parent too) */
    child = m_treeCtrl->GetFirstChild(parentId, cookie);
    while (child.IsOk())
    {
        m_treeCtrl->Delete(child);
        /* Not GetNextChild below, because the cookie mechanism can't
         * handle disappearing children! */
        child = m_treeCtrl->GetFirstChild(parentId, cookie);
    }
}

void wxGenericDirCtrl::ExpandDir(wxTreeItemId parentId)
{
    wxDirItemData *data = (wxDirItemData *) m_treeCtrl->GetItemData(parentId);

    if (data->m_isExpanded)
        return;

    data->m_isExpanded = TRUE;

    if (parentId == m_treeCtrl->GetRootItem())
    {
        SetupSections();
        return;
    }

    wxASSERT(data);

    wxString search,path,filename;

    wxString dirName(data->m_path);

#if defined(__WINDOWS__) || defined(__DOS__) || defined(__OS2__)
    // Check if this is a root directory and if so,
    // whether the drive is avaiable.
    if (!wxIsDriveAvailable(dirName))
    {
        data->m_isExpanded = FALSE;
        //wxMessageBox(wxT("Sorry, this drive is not available."));
        return;
    }
#endif

    // This may take a longish time. Go to busy cursor
    wxBusyCursor busy;

#if defined(__WINDOWS__) || defined(__DOS__) || defined(__OS2__)
    if (dirName.Last() == ':')
        dirName += wxString(wxFILE_SEP_PATH);
#endif

    wxArrayString dirs;
    wxArrayString filenames;

    wxDir d;
    wxString eachFilename;

    wxLogNull log;
    d.Open(dirName);

    if (d.IsOpened())
    {
        int style = wxDIR_DIRS;
        if (m_showHidden) style |= wxDIR_HIDDEN;
        if (d.GetFirst(& eachFilename, wxEmptyString, style))
        {
            do
            {
                if ((eachFilename != wxT(".")) && (eachFilename != wxT("..")))
                {
                    dirs.Add(eachFilename);
                }
            }
            while (d.GetNext(& eachFilename));
        }
    }
    dirs.Sort(wxDirCtrlStringCompareFunction);

    // Now do the filenames -- but only if we're allowed to
    if ((GetWindowStyle() & wxDIRCTRL_DIR_ONLY) == 0)
    {
        wxLogNull log;

        d.Open(dirName);

        if (d.IsOpened())
        {
            if (d.GetFirst(& eachFilename, m_currentFilterStr, wxDIR_FILES))
            {
                do
                {
                    if ((eachFilename != wxT(".")) && (eachFilename != wxT("..")))
                    {
                        filenames.Add(eachFilename);
                    }
                }
                while (d.GetNext(& eachFilename));
            }
        }
        filenames.Sort(wxDirCtrlStringCompareFunction);
    }

    // Add the sorted dirs
    size_t i;
    for (i = 0; i < dirs.Count(); i++)
    {
        wxString eachFilename(dirs[i]);
        path = dirName;
        if (!wxEndsWithPathSeparator(path))
            path += wxString(wxFILE_SEP_PATH);
        path += eachFilename;

        wxDirItemData *dir_item = new wxDirItemData(path,eachFilename,TRUE);
        wxTreeItemId id = AppendItem( parentId, eachFilename,
                                      wxFileIconsTable::folder, -1, dir_item);
        m_treeCtrl->SetItemImage( id, wxFileIconsTable::folder_open,
                                  wxTreeItemIcon_Expanded );

        // Has this got any children? If so, make it expandable.
        // (There are two situations when a dir has children: either it
        // has subdirectories or it contains files that weren't filtered
        // out. The latter only applies to dirctrl with files.)
        if ( dir_item->HasSubDirs() ||
             (((GetWindowStyle() & wxDIRCTRL_DIR_ONLY) == 0) &&
               dir_item->HasFiles(m_currentFilterStr)) )
        {
            m_treeCtrl->SetItemHasChildren(id);
        }
    }

    // Add the sorted filenames
    if ((GetWindowStyle() & wxDIRCTRL_DIR_ONLY) == 0)
    {
        for (i = 0; i < filenames.Count(); i++)
        {
            wxString eachFilename(filenames[i]);
            path = dirName;
            if (!wxEndsWithPathSeparator(path))
                path += wxString(wxFILE_SEP_PATH);
            path += eachFilename;
            //path = dirName + wxString(wxT("/")) + eachFilename;
            wxDirItemData *dir_item = new wxDirItemData(path,eachFilename,FALSE);
            int image_id = wxFileIconsTable::file;
            if (eachFilename.Find(wxT('.')) != wxNOT_FOUND)
                image_id = wxTheFileIconsTable->GetIconID(eachFilename.AfterLast(wxT('.')));
            (void) AppendItem( parentId, eachFilename, image_id, -1, dir_item);
        }
    }
}

void wxGenericDirCtrl::ReCreateTree()
{
    CollapseDir(m_treeCtrl->GetRootItem());
    ExpandDir(m_treeCtrl->GetRootItem());
}

// Find the child that matches the first part of 'path'.
// E.g. if a child path is "/usr" and 'path' is "/usr/include"
// then the child for /usr is returned.
wxTreeItemId wxGenericDirCtrl::FindChild(wxTreeItemId parentId, const wxString& path, bool& done)
{
    wxString path2(path);

    // Make sure all separators are as per the current platform
    path2.Replace(wxT("\\"), wxString(wxFILE_SEP_PATH));
    path2.Replace(wxT("/"), wxString(wxFILE_SEP_PATH));

    // Append a separator to foil bogus substring matching
    path2 += wxString(wxFILE_SEP_PATH);

    // In MSW or PM, case is not significant
#if defined(__WINDOWS__) || defined(__DOS__) || defined(__OS2__)
    path2.MakeLower();
#endif

    wxTreeItemIdValue cookie;
    wxTreeItemId childId = m_treeCtrl->GetFirstChild(parentId, cookie);
    while (childId.IsOk())
    {
        wxDirItemData* data = (wxDirItemData*) m_treeCtrl->GetItemData(childId);

        if (data && !data->m_path.IsEmpty())
        {
            wxString childPath(data->m_path);
            if (!wxEndsWithPathSeparator(childPath))
                childPath += wxString(wxFILE_SEP_PATH);

            // In MSW and PM, case is not significant
#if defined(__WINDOWS__) || defined(__DOS__) || defined(__OS2__)
            childPath.MakeLower();
#endif

            if (childPath.Len() <= path2.Len())
            {
                wxString path3 = path2.Mid(0, childPath.Len());
                if (childPath == path3)
                {
                    if (path3.Len() == path2.Len())
                        done = TRUE;
                    else
                        done = FALSE;
                    return childId;
                }
            }
        }

        childId = m_treeCtrl->GetNextChild(parentId, cookie);
    }
    wxTreeItemId invalid;
    return invalid;
}

// Try to expand as much of the given path as possible,
// and select the given tree item.
bool wxGenericDirCtrl::ExpandPath(const wxString& path)
{
    bool done = FALSE;
    wxTreeItemId id = FindChild(m_rootId, path, done);
    wxTreeItemId lastId = id; // The last non-zero id
    while (id.IsOk() && !done)
    {
        ExpandDir(id);

        id = FindChild(id, path, done);
        if (id.IsOk())
            lastId = id;
    }
    if (lastId.IsOk())
    {
        wxDirItemData *data = (wxDirItemData *) m_treeCtrl->GetItemData(lastId);
        if (data->m_isDir)
        {
            m_treeCtrl->Expand(lastId);
        }
        if ((GetWindowStyle() & wxDIRCTRL_SELECT_FIRST) && data->m_isDir)
        {
            // Find the first file in this directory
            wxTreeItemIdValue cookie;
            wxTreeItemId childId = m_treeCtrl->GetFirstChild(lastId, cookie);
            bool selectedChild = FALSE;
            while (childId.IsOk())
            {
                wxDirItemData* data = (wxDirItemData*) m_treeCtrl->GetItemData(childId);

                if (data && data->m_path != wxT("") && !data->m_isDir)
                {
                    m_treeCtrl->SelectItem(childId);
                    m_treeCtrl->EnsureVisible(childId);
                    selectedChild = TRUE;
                    break;
                }
                childId = m_treeCtrl->GetNextChild(lastId, cookie);
            }
            if (!selectedChild)
            {
                m_treeCtrl->SelectItem(lastId);
                m_treeCtrl->EnsureVisible(lastId);
            }
        }
        else
        {
            m_treeCtrl->SelectItem(lastId);
            m_treeCtrl->EnsureVisible(lastId);
        }

        return TRUE;
    }
    else
        return FALSE;
}

wxString wxGenericDirCtrl::GetPath() const
{
    wxTreeItemId id = m_treeCtrl->GetSelection();
    if (id)
    {
        wxDirItemData* data = (wxDirItemData*) m_treeCtrl->GetItemData(id);
        return data->m_path;
    }
    else
        return wxEmptyString;
}

wxString wxGenericDirCtrl::GetFilePath() const
{
    wxTreeItemId id = m_treeCtrl->GetSelection();
    if (id)
    {
        wxDirItemData* data = (wxDirItemData*) m_treeCtrl->GetItemData(id);
        if (data->m_isDir)
            return wxEmptyString;
        else
            return data->m_path;
    }
    else
        return wxEmptyString;
}

void wxGenericDirCtrl::SetPath(const wxString& path)
{
    m_defaultPath = path;
    if (m_rootId)
        ExpandPath(path);
}

// Not used
#if 0
void wxGenericDirCtrl::FindChildFiles(wxTreeItemId id, int dirFlags, wxArrayString& filenames)
{
    wxDirItemData *data = (wxDirItemData *) m_treeCtrl->GetItemData(id);

    // This may take a longish time. Go to busy cursor
    wxBusyCursor busy;

    wxASSERT(data);

    wxString search,path,filename;

    wxString dirName(data->m_path);

#if defined(__WXMSW__) || defined(__OS2__)
    if (dirName.Last() == ':')
        dirName += wxString(wxFILE_SEP_PATH);
#endif

    wxDir d;
    wxString eachFilename;

    wxLogNull log;
    d.Open(dirName);

    if (d.IsOpened())
    {
        if (d.GetFirst(& eachFilename, m_currentFilterStr, dirFlags))
        {
            do
            {
                if ((eachFilename != wxT(".")) && (eachFilename != wxT("..")))
                {
                    filenames.Add(eachFilename);
                }
            }
            while (d.GetNext(& eachFilename)) ;
        }
    }
}
#endif

void wxGenericDirCtrl::SetFilterIndex(int n)
{
    m_currentFilter = n;

    wxString f, d;
    if (ExtractWildcard(m_filter, n, f, d))
        m_currentFilterStr = f;
    else
        m_currentFilterStr = wxT("*.*");
}

void wxGenericDirCtrl::SetFilter(const wxString& filter)
{
    m_filter = filter;

    wxString f, d;
    if (ExtractWildcard(m_filter, m_currentFilter, f, d))
        m_currentFilterStr = f;
    else
        m_currentFilterStr = wxT("*.*");
}

// Extract description and actual filter from overall filter string
bool wxGenericDirCtrl::ExtractWildcard(const wxString& filterStr, int n, wxString& filter, wxString& description)
{
    wxArrayString filters, descriptions;
    int count = ParseFilter(filterStr, filters, descriptions);
    if (count > 0 && n < count)
    {
        filter = filters[n];
        description = descriptions[n];
        return TRUE;
    }

    return FALSE;
}

// Parses the global filter, returning the number of filters.
// Returns 0 if none or if there's a problem.
// filterStr is in the form: "All files (*.*)|*.*|JPEG Files (*.jpeg)|*.jpg"

int wxGenericDirCtrl::ParseFilter(const wxString& filterStr, wxArrayString& filters, wxArrayString& descriptions)
{
    return wxFileDialogBase::ParseWildcard(filterStr, descriptions, filters );
}

void wxGenericDirCtrl::DoResize()
{
    wxSize sz = GetClientSize();
    int verticalSpacing = 3;
    if (m_treeCtrl)
    {
        wxSize filterSz ;
        if (m_filterListCtrl)
        {
#ifdef __WXMSW__
            // For some reason, this is required in order for the
            // correct control height to always be returned, rather
            // than the drop-down list height which is sometimes returned.
            wxSize oldSize = m_filterListCtrl->GetSize();
            m_filterListCtrl->SetSize(-1, -1, oldSize.x+10, -1, wxSIZE_USE_EXISTING);
            m_filterListCtrl->SetSize(-1, -1, oldSize.x, -1, wxSIZE_USE_EXISTING);
#endif
            filterSz = m_filterListCtrl->GetSize();
            sz.y -= (filterSz.y + verticalSpacing);
        }
        m_treeCtrl->SetSize(0, 0, sz.x, sz.y);
        if (m_filterListCtrl)
        {
            m_filterListCtrl->SetSize(0, sz.y + verticalSpacing, sz.x, filterSz.y);
            // Don't know why, but this needs refreshing after a resize (wxMSW)
            m_filterListCtrl->Refresh();
        }
    }
}


void wxGenericDirCtrl::OnSize(wxSizeEvent& WXUNUSED(event))
{
    DoResize();
}

wxTreeItemId wxGenericDirCtrl::AppendItem (const wxTreeItemId & parent,
					   const wxString & text,
					   int image, int selectedImage,
					   wxTreeItemData * data)
{
  wxTreeCtrl *treeCtrl = GetTreeCtrl ();

  wxASSERT (treeCtrl);

  if (treeCtrl)
  {
    return treeCtrl->AppendItem (parent, text, image, selectedImage, data);
  }
  else
  {
    return wxTreeItemId();
  }
}


//-----------------------------------------------------------------------------
// wxDirFilterListCtrl
//-----------------------------------------------------------------------------

IMPLEMENT_CLASS(wxDirFilterListCtrl, wxChoice)

BEGIN_EVENT_TABLE(wxDirFilterListCtrl, wxChoice)
    EVT_CHOICE(-1, wxDirFilterListCtrl::OnSelFilter)
END_EVENT_TABLE()

bool wxDirFilterListCtrl::Create(wxGenericDirCtrl* parent, const wxWindowID id,
              const wxPoint& pos,
              const wxSize& size,
              long style)
{
    m_dirCtrl = parent;
    return wxChoice::Create(parent, id, pos, size, 0, NULL, style);
}

void wxDirFilterListCtrl::Init()
{
    m_dirCtrl = NULL;
}

void wxDirFilterListCtrl::OnSelFilter(wxCommandEvent& WXUNUSED(event))
{
    int sel = GetSelection();

    wxString currentPath = m_dirCtrl->GetPath();

    m_dirCtrl->SetFilterIndex(sel);

    // If the filter has changed, the view is out of date, so
    // collapse the tree.
    m_dirCtrl->ReCreateTree();

    // Try to restore the selection, or at least the directory
    m_dirCtrl->ExpandPath(currentPath);
}

void wxDirFilterListCtrl::FillFilterList(const wxString& filter, int defaultFilter)
{
    Clear();
    wxArrayString descriptions, filters;
    size_t n = (size_t) m_dirCtrl->ParseFilter(filter, filters, descriptions);

    if (n > 0 && defaultFilter < (int) n)
    {
        for (size_t i = 0; i < n; i++)
            Append(descriptions[i]);
        SetSelection(defaultFilter);
    }
}


// ----------------------------------------------------------------------------
// wxFileIconsTable icons
// ----------------------------------------------------------------------------

/* Open folder */
static const char * file_icons_tbl_folder_open_xpm[] = {
/* width height ncolors chars_per_pixel */
"16 16 6 1",
/* colors */
"   s None  c None",
".  c #000000",
"+  c #c0c0c0",
"@  c #808080",
"#  c #ffff00",
"$  c #ffffff",
/* pixels */
"                ",
"   @@@@@        ",
"  @$$$$$@       ",
" @$#+#+#$@@@@@@ ",
" @$+#+#+$$$$$$@.",
" @$#+#+#+#+#+#@.",
"@@@@@@@@@@@@@#@.",
"@$$$$$$$$$$@@+@.",
"@$#+#+#+#+##.@@.",
" @$#+#+#+#+#+.@.",
" @$+#+#+#+#+#.@.",
"  @$+#+#+#+##@..",
"  @@@@@@@@@@@@@.",
"   .............",
"                ",
"                "};

/* Computer */
static const char * file_icons_tbl_computer_xpm[] = {
"16 16 7 1",
"     s None    c None",
".    c #808080",
"X    c #c0c0c0",
"o    c Black",
"O    c Gray100",
"+    c #008080",
"@    c Blue",
"    ........... ",
"   .XXXXXXXXXX.o",
"   .OOOOOOOOO..o",
"   .OoooooooX..o",
"   .Oo+...@+X..o",
"   .Oo+XXX.+X..o",
"   .Oo+....+X..o",
"   .Oo++++++X..o",
"   .OXXXXXXXX.oo",
"   ..........o.o",
"   ...........Xo",
"   .XXXXXXXXXX.o",
"  .o.o.o.o.o...o",
" .oXoXoXoXoXo.o ",
".XOXXXXXXXXX.o  ",
"............o   "};

/* Drive */
static const char * file_icons_tbl_drive_xpm[] = {
"16 16 7 1",
"     s None    c None",
".    c #808080",
"X    c #c0c0c0",
"o    c Black",
"O    c Gray100",
"+    c Green",
"@    c #008000",
"                ",
"                ",
"                ",
"                ",
"  ............. ",
" .XXXXXXXXXXXX.o",
".OOOOOOOOOOOO..o",
".XXXXXXXXX+@X..o",
".XXXXXXXXXXXX..o",
".X..........X..o",
".XOOOOOOOOOOX..o",
"..............o ",
" ooooooooooooo  ",
"                ",
"                ",
"                "};

/* CD-ROM */
static const char *file_icons_tbl_cdrom_xpm[] = {
"16 16 10 1",
"     s None    c None",
".    c #808080",
"X    c #c0c0c0",
"o    c Yellow",
"O    c Blue",
"+    c Black",
"@    c Gray100",
"#    c #008080",
"$    c Green",
"%    c #008000",
"        ...     ",
"      ..XoX..   ",
"     .O.XoXXX+  ",
"    ...O.oXXXX+ ",
"    .O..X.XXXX+ ",
"   ....X.+..XXX+",
"   .XXX.+@+.XXX+",
"   .X@XX.+.X@@X+",
" .....X...#XX@+ ",
".@@@...XXo.O@X+ ",
".@XXX..XXoXOO+  ",
".@++++..XoX+++  ",
".@$%@@XX+++X.+  ",
".............+  ",
" ++++++++++++   ",
"                "};

/* Floppy */
static const char * file_icons_tbl_floppy_xpm[] = {
"16 16 7 1",
"     s None    c None",
".    c #808080",
"X    c Gray100",
"o    c #c0c0c0",
"O    c Black",
"+    c Cyan",
"@    c Red",
"         ......X",
"        .ooooooO",
"        .+++++OO",
"        .++++++O",
"        .++++++O",
"        .ooooooO",
"  .......o....oO",
" .oooooo.o.O.XoO",
".XXXXXXXXOOOOOO ",
".ooooooooo@o..O ",
".ooo....oooo..O ",
".o..OOOO...o..O ",
".oooXXXXoooo..O ",
".............O  ",
" OOOOOOOOOOOO   ",
"                "};

/* Removeable */
static const char * file_icons_tbl_removeable_xpm[] = {
"16 16 7 1",
"     s None    c None",
".    c #808080",
"X    c #c0c0c0",
"o    c Black",
"O    c Gray100",
"+    c Red",
"@    c #800000",
"                ",
"                ",
"                ",
"  ............. ",
" .XXXXXXXXXXXX.o",
".OOOOOOOOOOOO..o",
".OXXXXXXXXXXX..o",
".O+@.oooooo.X..o",
".OXXOooooooOX..o",
".OXXXOOOOOOXX..o",
".OXXXXXXXXXXX..o",
".O............o ",
" ooooooooooooo  ",
"                ",
"                ",
"                "};

// ----------------------------------------------------------------------------
// wxFileIconsTable & friends
// ----------------------------------------------------------------------------

// global instance of a wxFileIconsTable
wxFileIconsTable* wxTheFileIconsTable = (wxFileIconsTable *)NULL;

// A module to allow icons table cleanup

class wxFileIconsTableModule: public wxModule
{
DECLARE_DYNAMIC_CLASS(wxFileIconsTableModule)
public:
    wxFileIconsTableModule() {}
    bool OnInit() { wxTheFileIconsTable = new wxFileIconsTable; return TRUE; }
    void OnExit()
    {
        if (wxTheFileIconsTable)
        {
            delete wxTheFileIconsTable;
            wxTheFileIconsTable = NULL;
        }
    }
};

IMPLEMENT_DYNAMIC_CLASS(wxFileIconsTableModule, wxModule)

class wxFileIconEntry : public wxObject
{
public:
    wxFileIconEntry(int i) { id = i; }

    int id;
};

wxFileIconsTable::wxFileIconsTable()
{
    m_HashTable = NULL;
    m_smallImageList = NULL;
}

wxFileIconsTable::~wxFileIconsTable()
{
    if (m_HashTable)
    {
        WX_CLEAR_HASH_TABLE(*m_HashTable);
        delete m_HashTable;
    }
    if (m_smallImageList) delete m_smallImageList;
}

// delayed initialization - wait until first use (wxArtProv not created yet)
void wxFileIconsTable::Create()
{
    wxCHECK_RET(!m_smallImageList && !m_HashTable, wxT("creating icons twice"));
    m_HashTable = new wxHashTable(wxKEY_STRING);
    m_smallImageList = new wxImageList(16, 16);

    // folder:
    m_smallImageList->Add(wxArtProvider::GetBitmap(wxART_FOLDER, wxART_CMN_DIALOG));
    // folder_open
    m_smallImageList->Add(wxIcon(file_icons_tbl_folder_open_xpm));
    // computer
    m_smallImageList->Add(wxIcon(file_icons_tbl_computer_xpm));
    // drive
    m_smallImageList->Add(wxIcon(file_icons_tbl_drive_xpm));
    // cdrom
    m_smallImageList->Add(wxIcon(file_icons_tbl_cdrom_xpm));
    // floppy
    m_smallImageList->Add(wxIcon(file_icons_tbl_floppy_xpm));
    // removeable
    m_smallImageList->Add(wxIcon(file_icons_tbl_removeable_xpm));
    // file
    m_smallImageList->Add(wxArtProvider::GetBitmap(wxART_NORMAL_FILE, wxART_CMN_DIALOG));
    // executable
    if (GetIconID(wxEmptyString, _T("application/x-executable")) == file)
    {
        m_smallImageList->Add(wxArtProvider::GetBitmap(wxART_EXECUTABLE_FILE, wxART_CMN_DIALOG));
        delete m_HashTable->Get(_T("exe"));
        m_HashTable->Delete(_T("exe"));
        m_HashTable->Put(_T("exe"), new wxFileIconEntry(executable));
    }
    /* else put into list by GetIconID
       (KDE defines application/x-executable for *.exe and has nice icon)
     */
}

wxImageList *wxFileIconsTable::GetSmallImageList()
{
    if (!m_smallImageList)
        Create();

    return m_smallImageList;
}

#if wxUSE_MIMETYPE
// VS: we don't need this function w/o wxMimeTypesManager because we'll only have
//     one icon and we won't resize it

static wxBitmap CreateAntialiasedBitmap(const wxImage& img)
{
    wxImage smallimg (16, 16);
    unsigned char *p1, *p2, *ps;
    unsigned char mr = img.GetMaskRed(),
                  mg = img.GetMaskGreen(),
                  mb = img.GetMaskBlue();

    unsigned x, y;
    unsigned sr, sg, sb, smask;

    p1 = img.GetData(), p2 = img.GetData() + 3 * 32, ps = smallimg.GetData();
    smallimg.SetMaskColour(mr, mr, mr);

    for (y = 0; y < 16; y++)
    {
        for (x = 0; x < 16; x++)
        {
            sr = sg = sb = smask = 0;
            if (p1[0] != mr || p1[1] != mg || p1[2] != mb)
                sr += p1[0], sg += p1[1], sb += p1[2];
            else smask++;
            p1 += 3;
            if (p1[0] != mr || p1[1] != mg || p1[2] != mb)
                sr += p1[0], sg += p1[1], sb += p1[2];
            else smask++;
            p1 += 3;
            if (p2[0] != mr || p2[1] != mg || p2[2] != mb)
                sr += p2[0], sg += p2[1], sb += p2[2];
            else smask++;
            p2 += 3;
            if (p2[0] != mr || p2[1] != mg || p2[2] != mb)
                sr += p2[0], sg += p2[1], sb += p2[2];
            else smask++;
            p2 += 3;

            if (smask > 2)
                ps[0] = ps[1] = ps[2] = mr;
            else
                ps[0] = sr >> 2, ps[1] = sg >> 2, ps[2] = sb >> 2;
            ps += 3;
        }
        p1 += 32 * 3, p2 += 32 * 3;
    }

    return wxBitmap(smallimg);
}

// finds empty borders and return non-empty area of image:
static wxImage CutEmptyBorders(const wxImage& img)
{
    unsigned char mr = img.GetMaskRed(),
                  mg = img.GetMaskGreen(),
                  mb = img.GetMaskBlue();
    unsigned char *dt = img.GetData(), *dttmp;
    unsigned w = img.GetWidth(), h = img.GetHeight();

    unsigned top, bottom, left, right, i;
    bool empt;

#define MK_DTTMP(x,y)      dttmp = dt + ((x + y * w) * 3)
#define NOEMPTY_PIX(empt)  if (dttmp[0] != mr || dttmp[1] != mg || dttmp[2] != mb) {empt = FALSE; break;}

    for (empt = TRUE, top = 0; empt && top < h; top++)
    {
        MK_DTTMP(0, top);
        for (i = 0; i < w; i++, dttmp+=3)
            NOEMPTY_PIX(empt)
    }
    for (empt = TRUE, bottom = h-1; empt && bottom > top; bottom--)
    {
        MK_DTTMP(0, bottom);
        for (i = 0; i < w; i++, dttmp+=3)
            NOEMPTY_PIX(empt)
    }
    for (empt = TRUE, left = 0; empt && left < w; left++)
    {
        MK_DTTMP(left, 0);
        for (i = 0; i < h; i++, dttmp+=3*w)
            NOEMPTY_PIX(empt)
    }
    for (empt = TRUE, right = w-1; empt && right > left; right--)
    {
        MK_DTTMP(right, 0);
        for (i = 0; i < h; i++, dttmp+=3*w)
            NOEMPTY_PIX(empt)
    }
    top--, left--, bottom++, right++;

    return img.GetSubImage(wxRect(left, top, right - left + 1, bottom - top + 1));
}
#endif // wxUSE_MIMETYPE

int wxFileIconsTable::GetIconID(const wxString& extension, const wxString& mime)
{
    if (!m_smallImageList)
        Create();

#if wxUSE_MIMETYPE
    if (!extension.IsEmpty())
    {
        wxFileIconEntry *entry = (wxFileIconEntry*) m_HashTable->Get(extension);
        if (entry) return (entry -> id);
    }

    wxFileType *ft = (mime.IsEmpty()) ?
                   wxTheMimeTypesManager -> GetFileTypeFromExtension(extension) :
                   wxTheMimeTypesManager -> GetFileTypeFromMimeType(mime);

    wxIconLocation iconLoc;
    wxIcon ic;
    if ( ft && ft->GetIcon(&iconLoc) )
    {
        ic = wxIcon(iconLoc);
    }

    delete ft;

    if ( !ic.Ok() )
    {
        int newid = file;
        m_HashTable->Put(extension, new wxFileIconEntry(newid));
        return newid;
    }

    wxBitmap bmp;
    bmp.CopyFromIcon(ic);

    int id = m_smallImageList->GetImageCount();
    if ((bmp.GetWidth() == 16) && (bmp.GetHeight() == 16))
        m_smallImageList->Add(bmp);
    else
    {
        wxImage img = bmp.ConvertToImage();

        if ((img.GetWidth() != 32) || (img.GetHeight() != 32))
            m_smallImageList->Add(CreateAntialiasedBitmap(CutEmptyBorders(img).Rescale(32, 32)));
        else
            m_smallImageList->Add(CreateAntialiasedBitmap(img));
    }
    m_HashTable->Put(extension, new wxFileIconEntry(id));
    return id;

#else // !wxUSE_MIMETYPE

    if (extension == wxT("exe"))
        return executable;
    else
        return file;
#endif // wxUSE_MIMETYPE/!wxUSE_MIMETYPE
}

#endif // wxUSE_DIRDLG
