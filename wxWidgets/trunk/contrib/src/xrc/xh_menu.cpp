/////////////////////////////////////////////////////////////////////////////
// Name:        xh_menu.cpp
// Purpose:     XRC resource for menus and menubars
// Author:      Vaclav Slavik
// Created:     2000/03/05
// RCS-ID:      $Id$
// Copyright:   (c) 2000 Vaclav Slavik
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////
 
#ifdef __GNUG__
#pragma implementation "xh_menu.h"
#endif

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#include "wx/xrc/xh_menu.h"
#include "wx/menu.h"


wxMenuXmlHandler::wxMenuXmlHandler() : 
        wxXmlResourceHandler(), m_insideMenu(FALSE)
{
    ADD_STYLE(wxMENU_TEAROFF);
}



wxObject *wxMenuXmlHandler::DoCreateResource()
{
    if (m_class == wxT("wxMenu"))
    {
        wxMenu *menu = new wxMenu(GetStyle());
        wxString title = GetText(wxT("label"));
        wxString help = GetText(wxT("help"));
        
        bool oldins = m_insideMenu;
        m_insideMenu = TRUE;
        CreateChildren(menu, TRUE/*only this handler*/);
        m_insideMenu = oldins;

        wxMenuBar *p_bar = wxDynamicCast(m_parent, wxMenuBar);
        if (p_bar)
            p_bar->Append(menu, title);
        else
        {
            wxMenu *p_menu = wxDynamicCast(m_parent, wxMenu);
            if (p_menu)
                p_menu->Append(GetID(), title, menu, help);
        }

        return menu;
    }

    else
    {
        wxMenu *p_menu = wxDynamicCast(m_parent, wxMenu);
        
        if (m_class == wxT("separator"))
            p_menu->AppendSeparator();
        else if (m_class == wxT("break"))
            p_menu->Break();
        else /*wxMenuItem*/
        {
            int id = GetID();
            bool checkable = GetBool(wxT("checkable"));
            wxString label = GetText(wxT("label"));
            wxString accel = GetText(wxT("accel"));
            wxString fullLabel = label;
            if (!accel.IsEmpty())
                fullLabel << wxT("\t") << accel;

            wxMenuItem *mitem = new wxMenuItem(p_menu, id, fullLabel,
                                               GetText(wxT("help")), checkable);
                                               
#if wxCHECK_VERSION(2,3,0) || defined(__WXMSW__)
                if (HasParam(wxT("bitmap")))
                    mitem->SetBitmap(GetBitmap(wxT("bitmap")));
#endif
            p_menu->Append(mitem);
            mitem->Enable(GetBool(wxT("enabled"), TRUE));
            if (checkable) mitem->Check(GetBool(wxT("checked")));
        }
        return NULL;
    }
}



bool wxMenuXmlHandler::CanHandle(wxXmlNode *node)
{
    return IsOfClass(node, wxT("wxMenu")) ||
           (m_insideMenu && 
               (IsOfClass(node, wxT("wxMenuItem")) ||
                IsOfClass(node, wxT("break")) ||
                IsOfClass(node, wxT("separator")))
           );
}











wxMenuBarXmlHandler::wxMenuBarXmlHandler() : wxXmlResourceHandler()
{
    ADD_STYLE(wxMB_DOCKABLE);
}



wxObject *wxMenuBarXmlHandler::DoCreateResource()
{
    wxMenuBar *menubar = new wxMenuBar(GetStyle());
    CreateChildren(menubar);
    return menubar;
}



bool wxMenuBarXmlHandler::CanHandle(wxXmlNode *node)
{
    return IsOfClass(node, wxT("wxMenuBar"));
}

