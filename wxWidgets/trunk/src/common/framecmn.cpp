/////////////////////////////////////////////////////////////////////////////
// Name:        framecmn.cpp
// Purpose:     common (for all platforms) wxFrame functions
// Author:      Julian Smart, Vadim Zeitlin
// Created:     01/02/97
// Id:          $Id$
// Copyright:   (c) 1998 Robert Roebling, Julian Smart and Markus Holzem
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#include "wx/frame.h"
#include "wx/menu.h"
#include "wx/menuitem.h"

#ifndef __WXGTK__
void wxFrame::OnIdle(wxIdleEvent& WXUNUSED(event) )
{
  DoMenuUpdates();
}
#endif

// update all menus
void wxFrame::DoMenuUpdates()
{
  wxMenuBar* bar = GetMenuBar();
  if ( bar != NULL ) {
    int nCount = bar->GetMenuCount();
    for (int n = 0; n < nCount; n++)
      DoMenuUpdates(bar->GetMenu(n));
  }
}

// update a menu and all submenus recursively
void wxFrame::DoMenuUpdates(wxMenu* menu)
{
  wxNode* node = menu->GetItems().First();
  while (node)
  {
    wxMenuItem* item = (wxMenuItem*) node->Data();
    if ( !item->IsSeparator() )
    {
      wxWindowID id = item->GetId();
      wxUpdateUIEvent event(id);
      event.SetEventObject( this );

      if (GetEventHandler()->ProcessEvent(event))
      {
        if (event.GetSetText())
          menu->SetLabel(id, event.GetText());
        if (event.GetSetChecked())
          menu->Check(id, event.GetChecked());
        if (event.GetSetEnabled())
          menu->Enable(id, event.GetEnabled());
      }

      if (item->GetSubMenu())
        DoMenuUpdates(item->GetSubMenu());
    }
    node = node->Next();
  }
}
