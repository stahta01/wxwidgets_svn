/////////////////////////////////////////////////////////////////////////////
// Name:        xh_sizer.cpp
// Purpose:     XML resource for wxBoxSizer
// Author:      Vaclav Slavik
// Created:     2000/03/21
// RCS-ID:      $Id$
// Copyright:   (c) 2000 Vaclav Slavik
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////
 
#ifdef __GNUG__
#pragma implementation "xh_sizer.h"
#endif

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#include "wx/xml/xh_sizer.h"
#include "wx/sizer.h"
#include "wx/log.h"
#include "wx/statbox.h"
#include "wx/notebook.h"
#include "wx/tokenzr.h"

bool wxSizerXmlHandler::IsSizerNode(wxXmlNode *node)
{
    return (IsOfClass(node, _T("wxBoxSizer"))) ||
           (IsOfClass(node, _T("wxStaticBoxSizer"))) ||
           (IsOfClass(node, _T("wxGridSizer"))) ||
           (IsOfClass(node, _T("wxFlexGridSizer")));
}



wxSizerXmlHandler::wxSizerXmlHandler() 
: wxXmlResourceHandler(), m_IsInside(FALSE), m_ParentSizer(NULL)
{
    ADD_STYLE(wxHORIZONTAL);
    ADD_STYLE(wxVERTICAL);

    // and flags
    ADD_STYLE(wxLEFT);
    ADD_STYLE(wxRIGHT);
    ADD_STYLE(wxTOP);
    ADD_STYLE(wxBOTTOM);
    ADD_STYLE(wxNORTH);
    ADD_STYLE(wxSOUTH);
    ADD_STYLE(wxEAST);
    ADD_STYLE(wxWEST);
    ADD_STYLE(wxALL);

    ADD_STYLE(wxGROW);
    ADD_STYLE(wxEXPAND);
    ADD_STYLE(wxSHAPED);
    ADD_STYLE(wxSTRETCH_NOT);

    ADD_STYLE(wxALIGN_CENTER);
    ADD_STYLE(wxALIGN_CENTRE);
    ADD_STYLE(wxALIGN_LEFT);
    ADD_STYLE(wxALIGN_TOP);
    ADD_STYLE(wxALIGN_RIGHT);
    ADD_STYLE(wxALIGN_BOTTOM);
    ADD_STYLE(wxALIGN_CENTER_HORIZONTAL);
    ADD_STYLE(wxALIGN_CENTRE_HORIZONTAL);
    ADD_STYLE(wxALIGN_CENTER_VERTICAL);
    ADD_STYLE(wxALIGN_CENTRE_VERTICAL);
}



wxObject *wxSizerXmlHandler::DoCreateResource()
{ 
    if (m_Class == _T("sizeritem"))
    {
        wxXmlNode *n = GetParamNode(_T("object"));

        if (n)
        {
            bool old_ins = m_IsInside;
            wxSizer *old_par = m_ParentSizer;
            m_IsInside = FALSE;
            if (!IsSizerNode(n)) m_ParentSizer = NULL;
            wxObject *item = CreateResFromNode(n, m_Parent, NULL);
            m_IsInside = old_ins;
            m_ParentSizer = old_par;
            wxSizer *sizer = wxDynamicCast(item, wxSizer);
            wxWindow *wnd = wxDynamicCast(item, wxWindow);
            wxSize minsize = GetSize(_T("minsize"));

            if (sizer)
            {
                m_ParentSizer->Add(sizer, GetLong(_T("option")), 
                                   GetStyle(_T("flag")), GetDimension(_T("border")));
                if (!(minsize == wxDefaultSize))
                    m_ParentSizer->SetItemMinSize(sizer, minsize.x, minsize.y);
            }
            else if (wnd)
            {
                m_ParentSizer->Add(wnd, GetLong(_T("option")), 
                                   GetStyle(_T("flag")), GetDimension(_T("border")));
                if (!(minsize == wxDefaultSize))
                    m_ParentSizer->SetItemMinSize(wnd, minsize.x, minsize.y);
            }
            else 
                wxLogError(_T("Error in resource."));

            return item;
        }
        else /*n == NULL*/
        {
            wxLogError(_T("Error in resource: no control/sizer within sizer's <item> tag."));
            return NULL;
        }
    }
    
    else if (m_Class == _T("spacer"))
    {
        wxCHECK_MSG(m_ParentSizer, NULL, _T("Incorrect syntax of XML resource: spacer not within sizer!"));
        wxSize sz = GetSize();
        m_ParentSizer->Add(sz.x, sz.y,
            GetLong(_T("option")), GetStyle(_T("flag")), GetDimension(_T("border")));
        return NULL;
    }
    

    else {
        wxSizer *sizer = NULL;
        
        wxXmlNode *parentNode = m_Node->GetParent();

        wxCHECK_MSG(m_ParentSizer != NULL ||
                ((IsOfClass(parentNode, _T("wxPanel")) ||
                  IsOfClass(parentNode, _T("wxDialog"))) &&
                 parentNode->GetType() == wxXML_ELEMENT_NODE), NULL,
                _T("Incorrect use of sizer: parent is not 'wxDialog' or 'wxPanel'."));

        if (m_Class == _T("wxBoxSizer"))
            sizer = new wxBoxSizer(GetStyle(_T("orient"), wxHORIZONTAL));

        else if (m_Class == _T("wxStaticBoxSizer"))
        {
            sizer = new wxStaticBoxSizer(
                         new wxStaticBox(m_ParentAsWindow, -1, GetText(_T("label"))),
                         GetStyle(_T("orient"), wxHORIZONTAL));
        }
        
        else if (m_Class == _T("wxGridSizer"))
            sizer = new wxGridSizer(GetLong(_T("rows")), GetLong(_T("cols")),
                                    GetDimension(_T("vgap")), GetDimension(_T("hgap")));
                                    
        else if (m_Class == _T("wxFlexGridSizer"))
        {
            wxFlexGridSizer *fsizer = 
                  new wxFlexGridSizer(GetLong(_T("rows")), GetLong(_T("cols")),
                      GetDimension(_T("vgap")), GetDimension(_T("hgap")));
            sizer = fsizer;
            wxStringTokenizer tkn;
            unsigned long l;
            tkn.SetString(GetParamValue(_T("growablerows")), _T(","));
            while (tkn.HasMoreTokens())
            {
                if (!tkn.GetNextToken().ToULong(&l))
                    wxLogError(_T("growablerows must be comma-separated list of row numbers"));
                else
                    fsizer->AddGrowableRow(l);
            }
            tkn.SetString(GetParamValue(_T("growablecols")), _T(","));
            while (tkn.HasMoreTokens())
            {
                if (!tkn.GetNextToken().ToULong(&l))
                    wxLogError(_T("growablecols must be comma-separated list of column numbers"));
                else
                    fsizer->AddGrowableCol(l);
            }
        }

        wxSize minsize = GetSize(_T("minsize"));
        if (!(minsize == wxDefaultSize))
            sizer->SetMinSize(minsize);

        wxSizer *old_par = m_ParentSizer;
        m_ParentSizer = sizer;
        bool old_ins = m_IsInside;
        m_IsInside = TRUE;
        CreateChildren(m_Parent, TRUE/*only this handler*/);
        m_IsInside = old_ins;
        m_ParentSizer = old_par;
        
        if (m_ParentSizer == NULL) // setup window:
        {
            m_ParentAsWindow->SetAutoLayout(TRUE);
            m_ParentAsWindow->SetSizer(sizer);

            wxXmlNode *nd = m_Node;
            m_Node = parentNode;
            if (GetSize() == wxDefaultSize)
                sizer->Fit(m_ParentAsWindow);
            m_Node = nd;

            if (m_ParentAsWindow->GetWindowStyle() & (wxRESIZE_BOX | wxRESIZE_BORDER))
                sizer->SetSizeHints(m_ParentAsWindow);
        }
        
        return sizer;
    }
}



bool wxSizerXmlHandler::CanHandle(wxXmlNode *node)
{
    return ((!m_IsInside && IsSizerNode(node)) ||
            (m_IsInside && IsOfClass(node, _T("sizeritem"))) ||
            (m_IsInside && IsOfClass(node, _T("spacer"))));
}
