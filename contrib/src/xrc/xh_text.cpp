/////////////////////////////////////////////////////////////////////////////
// Name:        xh_text.cpp
// Purpose:     XRC resource for wxTextCtrl
// Author:      Aleksandras Gluchovas
// Created:     2000/03/21
// RCS-ID:      $Id$
// Copyright:   (c) 2000 Aleksandras Gluchovas
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////
 
#ifdef __GNUG__
#pragma implementation "xh_text.h"
#endif

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#include "wx/xrc/xh_text.h"
#include "wx/textctrl.h"

wxTextCtrlXmlHandler::wxTextCtrlXmlHandler() : wxXmlResourceHandler() 
{
    ADD_STYLE(wxTE_PROCESS_ENTER);
    ADD_STYLE(wxTE_PROCESS_TAB);
    ADD_STYLE(wxTE_MULTILINE);
    ADD_STYLE(wxTE_PASSWORD);
    ADD_STYLE(wxTE_READONLY);
    ADD_STYLE(wxHSCROLL);
    AddWindowStyles();
}

wxObject *wxTextCtrlXmlHandler::DoCreateResource()
{ 
   wxTextCtrl *text = wxStaticCast(m_instance, wxTextCtrl);

   if ( !text )
       text = new wxTextCtrl;

   text->Create( m_parentAsWindow,
                GetID(),
                GetText(wxT("value")),
                GetPosition(), GetSize(),
                GetStyle(),
                wxDefaultValidator,
                GetName() );

    SetupWindow(text);
    
    return text;
}



bool wxTextCtrlXmlHandler::CanHandle(wxXmlNode *node)
{
    return IsOfClass(node, wxT("wxTextCtrl"));
}


