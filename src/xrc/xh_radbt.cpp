/////////////////////////////////////////////////////////////////////////////
// Name:        xh_radbt.cpp
// Purpose:     XRC resource for wxRadioButton
// Author:      Bob Mitchell
// Created:     2000/03/21
// RCS-ID:      $Id$
// Copyright:   (c) 2000 Bob Mitchell and Verant Interactive
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifdef __GNUG__
#pragma implementation "xh_radbt.h"
#endif

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#if wxUSE_XRC && wxUSE_RADIOBOX

#include "wx/xrc/xh_radbt.h"
#include "wx/radiobut.h"

IMPLEMENT_DYNAMIC_CLASS(wxRadioButtonXmlHandler, wxXmlResourceHandler)

wxRadioButtonXmlHandler::wxRadioButtonXmlHandler()
: wxXmlResourceHandler()
{
    XRC_ADD_STYLE(wxRB_GROUP);
    XRC_ADD_STYLE(wxRB_SINGLE);
    AddWindowStyles();
}

wxObject *wxRadioButtonXmlHandler::DoCreateResource()
{
    /* BOBM - implementation note.
     * once the wxBitmapRadioButton is implemented.
     * look for a bitmap property. If not null,
     * make it a wxBitmapRadioButton instead of the
     * normal radio button.
     */

    XRC_MAKE_INSTANCE(control, wxRadioButton)

    control->Create(m_parentAsWindow,
                    GetID(),
                    GetText(wxT("label")),
                    GetPosition(), GetSize(),
                    GetStyle(),
                    wxDefaultValidator,
                    GetName());

    control->SetValue(GetBool(wxT("value"), 0));
    SetupWindow(control);

    return control;
}

bool wxRadioButtonXmlHandler::CanHandle(wxXmlNode *node)
{
    return IsOfClass(node, wxT("wxRadioButton"));
}

#endif // wxUSE_XRC && wxUSE_NOTEBOOK
