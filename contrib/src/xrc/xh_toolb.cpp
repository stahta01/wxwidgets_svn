/////////////////////////////////////////////////////////////////////////////
// Name:        xh_toolb.cpp
// Purpose:     XML resource for wxBoxSizer
// Author:      Vaclav Slavik
// Created:     2000/08/11
// RCS-ID:      $Id$
// Copyright:   (c) 2000 Vaclav Slavik
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////
 
#ifdef __GNUG__
#pragma implementation "xh_toolb.h"
#endif

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#include "wx/xrc/xh_toolb.h"
#include "wx/toolbar.h"


#if wxUSE_TOOLBAR

wxToolBarXmlHandler::wxToolBarXmlHandler() 
: wxXmlResourceHandler(), m_isInside(FALSE), m_toolbar(NULL)
{
    ADD_STYLE(wxTB_FLAT);
    ADD_STYLE(wxTB_DOCKABLE);
    ADD_STYLE(wxTB_VERTICAL);
    ADD_STYLE(wxTB_HORIZONTAL);
}



wxObject *wxToolBarXmlHandler::DoCreateResource()
{ 
    if (m_class == wxT("tool"))
    {
        wxCHECK_MSG(m_toolbar, NULL, wxT("Incorrect syntax of XML resource: tool not within a toolbar!"));
        m_toolbar->AddTool(GetID(),
                           GetBitmap(wxT("bitmap")),
                           GetBitmap(wxT("bitmap2")),
                           GetBool(wxT("toggle")),
                           GetPosition().x,
                           GetPosition().y,
                           NULL,
                           GetText(wxT("tooltip")),
                           GetText(wxT("longhelp")));
        return m_toolbar; // must return non-NULL
    }
    
    else if (m_class == wxT("separator"))
    {
        wxCHECK_MSG(m_toolbar, NULL, wxT("Incorrect syntax of XML resource: separator not within a toolbar!"));
        m_toolbar->AddSeparator();
        return m_toolbar; // must return non-NULL
    }
    
    else /*<object class="wxToolBar">*/
    {
        int style = GetStyle(wxT("style"), wxNO_BORDER | wxTB_HORIZONTAL);
#ifdef __WXMSW__
        if (!(style & wxNO_BORDER)) style |= wxNO_BORDER;
#endif
        wxToolBar *toolbar = new wxToolBar(m_parentAsWindow,
                                    GetID(),
                                    GetPosition(),
                                    GetSize(),
                                    style,
                                    GetName());

        wxSize bmpsize = GetSize(wxT("bitmapsize"));
        if (!(bmpsize == wxDefaultSize))
            toolbar->SetToolBitmapSize(bmpsize);
        wxSize margins = GetSize(wxT("margins"));
        if (!(margins == wxDefaultSize))
            toolbar->SetMargins(margins.x, margins.y);
        long packing = GetLong(wxT("packing"), -1);
        if (packing != -1)
            toolbar->SetToolPacking(packing);
        long separation = GetLong(wxT("separation"), -1);
        if (separation != -1)
            toolbar->SetToolSeparation(separation);

        wxXmlNode *children_node = GetParamNode(wxT("object"));
        if (children_node == NULL) return toolbar;

        m_isInside = TRUE;
        m_toolbar = toolbar;

        wxXmlNode *n = children_node;

        while (n)
        {
            if (n->GetType() == wxXML_ELEMENT_NODE && 
                n->GetName() == wxT("object"))
            {
                wxObject *created = CreateResFromNode(n, toolbar, NULL);
                wxControl *control = wxDynamicCast(created, wxControl);
                if (!IsOfClass(n, wxT("tool")) &&
                    !IsOfClass(n, wxT("separator")) &&
                    control != NULL)
                    toolbar->AddControl(control);
            }
            n = n->GetNext();
        }

        m_isInside = FALSE;
        m_toolbar = NULL;

        toolbar->Realize();
        return toolbar;
    }
}



bool wxToolBarXmlHandler::CanHandle(wxXmlNode *node)
{
    return ((!m_isInside && IsOfClass(node, wxT("wxToolBar"))) ||
            (m_isInside && IsOfClass(node, wxT("tool"))) || 
            (m_isInside && IsOfClass(node, wxT("separator"))));
}

#endif
