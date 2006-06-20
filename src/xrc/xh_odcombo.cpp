/////////////////////////////////////////////////////////////////////////////
// Name:        src/xrc/xh_odcombo.cpp
// Purpose:     XRC resource for wxRadioBox
// Author:      Alex Bligh - Based on src/xrc/xh_combo.cpp
// Created:     2006/06/19
// RCS-ID:      $Id$
// Copyright:   (c) 2006 Alex Bligh
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#if wxUSE_XRC && wxUSE_ODCOMBOBOX

#include "wx/xrc/xh_odcombo.h"

#ifndef WX_PRECOMP
    #include "wx/intl.h"
#endif

#include "wx/odcombo.h"

IMPLEMENT_DYNAMIC_CLASS(wxOwnerDrawnComboBoxXmlHandler, wxXmlResourceHandler)

wxOwnerDrawnComboBoxXmlHandler::wxOwnerDrawnComboBoxXmlHandler()
                     :wxXmlResourceHandler()
                     ,m_insideBox(false)
{
    XRC_ADD_STYLE(wxCB_SIMPLE);
    XRC_ADD_STYLE(wxCB_SORT);
    XRC_ADD_STYLE(wxCB_READONLY);
    XRC_ADD_STYLE(wxCB_DROPDOWN);
    XRC_ADD_STYLE(wxODCB_STD_CONTROL_PAINT);
    XRC_ADD_STYLE(wxODCB_DCLICK_CYCLES);
    AddWindowStyles();
}

wxObject *wxOwnerDrawnComboBoxXmlHandler::DoCreateResource()
{
    if( m_class == wxT("wxOwnerDrawnComboBox"))
    {
        // find the selection
        long selection = GetLong( wxT("selection"), -1 );

        // need to build the list of strings from children
        m_insideBox = true;
        CreateChildrenPrivately(NULL, GetParamNode(wxT("content")));
        wxString *strings = (wxString *) NULL;
        if (strList.GetCount() > 0)
        {
            strings = new wxString[strList.GetCount()];
            int count = strList.GetCount();
            for (int i = 0; i < count; i++)
                strings[i]=strList[i];
        }

        XRC_MAKE_INSTANCE(control, wxOwnerDrawnComboBox)

        control->Create(m_parentAsWindow,
                        GetID(),
                        GetText(wxT("value")),
                        GetPosition(), GetSize(),
                        strList.GetCount(),
                        strings,
                        GetStyle(),
                        wxDefaultValidator,
                        GetName());

        wxSize ButtonSize=GetSize(wxT("buttonsize"));

        if (ButtonSize != wxDefaultSize)
        control->SetButtonPosition(ButtonSize.GetWidth(), ButtonSize.GetHeight());

        if (selection != -1)
            control->SetSelection(selection);

        SetupWindow(control);

        if (strings != NULL)
            delete[] strings;
        strList.Clear();    // dump the strings

        return control;
    }
    else
    {
        // on the inside now.
        // handle <item>Label</item>

        // add to the list
        wxString str = GetNodeContent(m_node);
        if (m_resource->GetFlags() & wxXRC_USE_LOCALE)
            str = wxGetTranslation(str);
        strList.Add(str);

        return NULL;
    }
}

bool wxOwnerDrawnComboBoxXmlHandler::CanHandle(wxXmlNode *node)
{
//  Avoid GCC bug - this fails on certain GCC 3.x builds for an unknown reason
//  return (IsOfClass(node, wxT("wxOwnerDrawnComboBox")) ||
//         (m_insideBox && node->GetName() == wxT("item")));

    bool fOurClass = node->GetPropVal(wxT("class"), wxEmptyString) == wxT("wxOwnerDrawnComboBox");
    return (fOurClass ||
          (m_insideBox && node->GetName() == wxT("item")));

}

#endif // wxUSE_XRC && wxUSE_ODCOMBOBOX
