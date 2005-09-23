/////////////////////////////////////////////////////////////////////////////
// Name:        xh_panel.cpp
// Purpose:     XRC resource for panels
// Author:      Vaclav Slavik
// Created:     2000/03/05
// RCS-ID:      $Id$
// Copyright:   (c) 2000 Vaclav Slavik
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#if wxUSE_XRC

#include "wx/xrc/xh_panel.h"
#include "wx/panel.h"
#include "wx/frame.h"  // to get wxNO_3D

IMPLEMENT_DYNAMIC_CLASS(wxPanelXmlHandler, wxXmlResourceHandler)

wxPanelXmlHandler::wxPanelXmlHandler() : wxXmlResourceHandler()
{
    XRC_ADD_STYLE(wxNO_3D);
    XRC_ADD_STYLE(wxTAB_TRAVERSAL);
    XRC_ADD_STYLE(wxWS_EX_VALIDATE_RECURSIVELY);
    
    AddWindowStyles();
}

wxObject *wxPanelXmlHandler::DoCreateResource()
{
    XRC_MAKE_INSTANCE(panel, wxPanel)

    panel->Create(m_parentAsWindow,
                  GetID(),
                  GetPosition(), GetSize(),
                  GetStyle(wxT("style"), wxTAB_TRAVERSAL),
                  GetName());

    SetupWindow(panel);
    CreateChildren(panel);

    return panel;
}

bool wxPanelXmlHandler::CanHandle(wxXmlNode *node)
{
    return IsOfClass(node, wxT("wxPanel"));
}

#endif // wxUSE_XRC
