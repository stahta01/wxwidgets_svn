/////////////////////////////////////////////////////////////////////////////
// Name:        serwnd.cpp
// Purpose:     Serialization: wxWindow classes
// Author:      Guilhem Lavaux
// Modified by:
// Created:     July 1998
// RCS-ID:      $Id$
// Copyright:   (c) 1998 Guilhem Lavaux
// Licence:     wxWindows license
/////////////////////////////////////////////////////////////////////////////

#ifdef __GNUG__
#pragma implementation "serwnd.h"
#endif

#include <wx/window.h>
#include <wx/layout.h>
#include <wx/stream.h>
#include <wx/datstrm.h>
#include <wx/objstrm.h>
#include <wx/utils.h>
#include <wx/frame.h>
#include <wx/panel.h>
#include <wx/menu.h>
#include <wx/dialog.h>
#include <wx/serbase.h>
#include <wx/statusbr.h>
#include <wx/mdi.h>
#include "serwnd.h"


IMPLEMENT_SERIAL_CLASS(wxWindow, wxObject)
IMPLEMENT_SERIAL_CLASS(wxIndividualLayoutConstraint, wxObject)
IMPLEMENT_SERIAL_CLASS(wxLayoutConstraints, wxObject)
IMPLEMENT_ALIAS_SERIAL_CLASS(wxValidator, wxObject)
IMPLEMENT_SERIAL_CLASS(wxFrame, wxWindow)
IMPLEMENT_SERIAL_CLASS(wxPanel, wxWindow)
IMPLEMENT_SERIAL_CLASS(wxDialog, wxWindow)
IMPLEMENT_SERIAL_CLASS(wxMenuBar, wxWindow)
IMPLEMENT_SERIAL_CLASS(wxMenuItem, wxObject)
IMPLEMENT_SERIAL_CLASS(wxMenu, wxObject)

IMPLEMENT_SERIAL_CLASS(wxMDIParentFrame, wxFrame)
IMPLEMENT_SERIAL_CLASS(wxMDIChildFrame, wxFrame)
IMPLEMENT_SERIAL_CLASS(wxMDIClientWindow, wxWindow)

/////////////////////////////////////////////////////////////////////////////

void WXSERIAL(wxWindow)::StoreObject(wxObjectOutputStream& s)
{
  wxWindow *win_object = (wxWindow *)Object();
  wxNode *node = win_object->GetChildren()->First();

  if (s.FirstStage()) {
    s.AddChild(win_object->GetConstraints());
    s.AddChild(win_object->GetValidator());

    // BAD HACK, but I don't have access to the internal variable of wxWindow.
    m_bg_colour = win_object->GetDefaultBackgroundColour();
    m_fg_colour = win_object->GetDefaultForegroundColour();
    s.AddChild(&m_bg_colour);
    s.AddChild(&m_fg_colour);
    s.AddChild(win_object->GetFont());
    while (node) {
      s.AddChild(node->Data());
      node = node->Next();
    }
    return;
  }

  wxDataOutputStream data(s);
  int x,y,w,h;

  data.WriteString(win_object->GetName());
  data.WriteString(win_object->GetLabel());
  data.WriteString(win_object->GetTitle());

  data.Write8(win_object->GetAutoLayout());
  data.Write8(win_object->IsShown());
  data.Write32( win_object->GetWindowStyleFlag() );
  data.Write32(win_object->GetId());

  win_object->GetSize(&w, &h);
  win_object->GetPosition(&x, &y);
  data.Write16(x);
  data.Write16(y);
  data.Write16(w);
  data.Write16(h);
}

void WXSERIAL(wxWindow)::LoadObject(wxObjectInputStream& s)
{
  wxDataInputStream data_s(s);
  wxWindow *win_object = (wxWindow *)Object();

  m_parent = (wxWindow *)s.GetParent();

  m_name  = data_s.ReadString();
  m_label = data_s.ReadString();
  m_title = data_s.ReadString();

  m_auto_layout = data_s.Read8();
  m_shown       = data_s.Read8();
  m_style       = data_s.Read32();
  m_id          = data_s.Read32();

  m_x = data_s.Read16();
  m_y = data_s.Read16();
  m_w = data_s.Read16();
  m_h = data_s.Read16();

  /* I assume we will never create raw wxWindow object */

  m_validator = (wxValidator *)s.GetChild();
  win_object->SetDefaultBackgroundColour(*((wxColour *)s.GetChild()));
  win_object->SetDefaultForegroundColour(*((wxColour *)s.GetChild()));
  win_object->SetFont(*((wxFont *)s.GetChild()));

  return;
}

/////////////////////////////////////////////////////////////////////////////

void WXSERIAL(wxIndividualLayoutConstraint)::StoreObject
                                           (wxObjectOutputStream& s)
{
  wxIndividualLayoutConstraint *lay_object =
                                     (wxIndividualLayoutConstraint *)Object();
  if (s.FirstStage())
    return;

  wxDataOutputStream data_s(s);

  data_s.WriteString(s.GetObjectName(lay_object->GetOtherWindow()));
  data_s.Write8(lay_object->GetMyEdge());
  data_s.Write8(lay_object->GetRelationship());
  data_s.Write16(lay_object->GetMargin());
  data_s.Write16(lay_object->GetValue());
  data_s.Write8(lay_object->GetPercent());
  data_s.Write8(lay_object->GetOtherEdge());
}

void WXSERIAL(wxIndividualLayoutConstraint)::
                              LoadObject(wxObjectInputStream& s)
{
  wxIndividualLayoutConstraint *lay_object =
                                     (wxIndividualLayoutConstraint *)Object();
  wxDataInputStream data_s(s);
  wxString win_name;

  win_name = data_s.ReadString();
  lay_object->otherWin     = (wxWindow *)s.SolveName(win_name);
  lay_object->myEdge       = (wxEdge)data_s.Read8();
  lay_object->relationship = (wxRelationship)data_s.Read8();
  lay_object->margin       = data_s.Read16();
  lay_object->value        = data_s.Read16();
  lay_object->percent      = data_s.Read8();
  lay_object->otherEdge    = (wxEdge)data_s.Read8();
}

/////////////////////////////////////////////////////////////////////////////

void WXSERIAL(wxLayoutConstraints)::StoreObject(wxObjectOutputStream& s)
{
  wxLayoutConstraints *lay_object = (wxLayoutConstraints *)Object();
  WXSERIAL(wxIndividualLayoutConstraint) c;

#define STORE(obj) c.SetObject(&(lay_object->obj)); c.StoreObject(s);

  // I simplify the process for this object
  STORE(left);
  STORE(right);
  STORE(bottom);
  STORE(top);

  STORE(width);
  STORE(height);

  STORE(centreX);
  STORE(centreY);

#undef STORE
}

void WXSERIAL(wxLayoutConstraints)::LoadObject(wxObjectInputStream& s)
{
  wxLayoutConstraints *lay_object = (wxLayoutConstraints *)Object();
  WXSERIAL(wxIndividualLayoutConstraint) c;

#define LOAD(obj) c.SetObject(&(lay_object->obj)); c.LoadObject(s);

  // I simplify the process for this object
  LOAD(left);
  LOAD(right);
  LOAD(bottom);
  LOAD(top);

  LOAD(width);
  LOAD(height);

  LOAD(centreX);
  LOAD(centreY);

#undef LOAD

  // Initialize constraints
  ((wxWindow *)s.GetParent())->SetConstraints(lay_object);
}

/////////////////////////////////////////////////////////////////////////////

void WXSERIAL(wxFrame)::StoreObject(wxObjectOutputStream& s)
{
  wxFrame *frame = (wxFrame *)Object();

  if (s.FirstStage()) {
    s.AddChild(frame->GetMenuBar());
    WXSERIAL(wxWindow)::StoreObject(s);
    return;
  }

  WXSERIAL(wxWindow)::StoreObject(s);

  wxDataOutputStream data_s(s);
  wxStatusBar *statbar = frame->GetStatusBar();

  // AAARGH !! I absolutely need to be able to modify internal fields of
  // wxFrame (wxToolBar and wxStatusBar)
 
  if (statbar)
    data_s.Write8(statbar->GetFieldsCount());
  else
    data_s.Write8(0);
  // HOW CAN I ACCESS TO THIS FIELD ?
  // for (...) { data_s.Write8(statbar->m_statusWidths[i]); }
}

void WXSERIAL(wxFrame)::LoadObject(wxObjectInputStream& s)
{
  wxFrame *frame = (wxFrame *)Object();
  wxMenuBar *mbar = (wxMenuBar *)s.GetChild();

  WXSERIAL(wxWindow)::LoadObject(s);

  wxDataInputStream data_s(s);

  frame->SetMenuBar(mbar);
  if (frame->GetClassInfo() == CLASSINFO(wxFrame))
    frame->Create(m_parent, m_id, m_title, wxPoint(m_x, m_y),
                  wxSize(m_w, m_h), m_style, m_name);

  frame->CreateStatusBar(data_s.Read8());
}

/////////////////////////////////////////////////////////////////////////////

void WXSERIAL(wxMenuBar)::StoreObject(wxObjectOutputStream& s)
{
  wxMenuBar *mbar = (wxMenuBar *)Object();
  int i, mcount = mbar->GetMenuCount(); 

  if (s.FirstStage()) {
    for (i=0;i<mcount;i++)
      s.AddChild( mbar->GetMenu(i) );
    WXSERIAL(wxWindow)::StoreObject(s);
    return;
  }

  wxDataOutputStream data_s(s);
  data_s.Write8( mcount );

  // It isn't necessary for this object.
  // WXSERIAL(wxWindow)::StoreObject(s);
}

void WXSERIAL(wxMenuBar)::LoadObject(wxObjectInputStream& s)
{
  wxMenuBar *mbar = (wxMenuBar *)Object();
  int i, mcount;
  wxDataInputStream data_s(s);

  mcount = data_s.Read8();
  for (i=0;i<mcount;i++) {
    wxMenu *menu = (wxMenu *)s.GetChild();
    mbar->Append( menu, menu->GetTitle() );
  }

  // It isn't necessary for this object.
  // WXSERIAL(wxWindow)::LoadObject(s);
}

/////////////////////////////////////////////////////////////////////////////

void WXSERIAL(wxMenu)::StoreObject(wxObjectOutputStream& s)
{
  wxMenu *menu = (wxMenu *)Object();

  if (s.FirstStage()) {
    s.AddChild( &menu->GetItems() );
    return;
  }

  wxDataOutputStream data_s(s);
  data_s.WriteString( menu->GetTitle() );
}

void WXSERIAL(wxMenu)::LoadObject(wxObjectInputStream& s)
{
  wxMenu *menu = (wxMenu *)Object();
  wxList *items = (wxList *)s.GetChild();
  wxNode *node = items->First();

  wxDataInputStream data_s(s);

  menu->SetTitle( data_s.ReadString() );

  while (node) {
// NOT IMPLEMENTED in wxGTK
//    menu->Append( (wxMenuItem *)node->Data() );
    node = node->Next();
  }
}

/////////////////////////////////////////////////////////////////////////////

void WXSERIAL(wxMenuItem)::StoreObject(wxObjectOutputStream& s)
{
  wxMenuItem *item = (wxMenuItem *)Object();

  if (s.FirstStage()) {
    s.AddChild(item->GetSubMenu());
    return;
  }

  wxDataOutputStream data_s(s);

  data_s.Write8( item->GetId() );
  data_s.WriteString( item->GetText() );
  data_s.Write8( item->IsCheckable() );
  data_s.Write8( item->IsEnabled() );
  data_s.Write8( item->IsChecked() );
}

void WXSERIAL(wxMenuItem)::LoadObject(wxObjectInputStream& s)
{
  wxMenuItem *item = (wxMenuItem *)Object();
  wxDataInputStream data_s(s);

  item->SetId( data_s.Read8() );
  item->SetText( data_s.ReadString() );
  item->SetCheckable( data_s.Read8() );
  item->Enable( data_s.Read8() );
  item->Check( data_s.Read8() );
  item->SetSubMenu( (wxMenu *)s.GetChild() );
}

/////////////////////////////////////////////////////////////////////////////

void WXSERIAL(wxPanel)::StoreObject(wxObjectOutputStream& s)
{
  WXSERIAL(wxWindow)::StoreObject(s);
}

void WXSERIAL(wxPanel)::LoadObject(wxObjectInputStream& s)
{
  WXSERIAL(wxWindow)::LoadObject(s);

  ((wxPanel *)Object())->Create(m_parent, m_id, wxPoint(m_x, m_y),
                                wxSize(m_w, m_h), m_style, m_name);
}

/////////////////////////////////////////////////////////////////////////////

void WXSERIAL(wxDialog)::StoreObject(wxObjectOutputStream& s)
{
  WXSERIAL(wxWindow)::StoreObject(s);
}

void WXSERIAL(wxDialog)::LoadObject(wxObjectInputStream& s)
{
  WXSERIAL(wxWindow)::LoadObject(s);

  ((wxDialog *)Object())->Create(m_parent, m_id, m_title, wxPoint(m_x, m_y),
                                 wxSize(m_w, m_h), m_style, m_name);
}

/////////////////////////////////////////////////////////////////////////////

void WXSERIAL(wxMDIParentFrame)::StoreObject(wxObjectOutputStream& s)
{
  wxMDIParentFrame *frame = (wxMDIParentFrame *)Object();

  if (s.FirstStage()) {
    s.AddChild(frame->GetClientWindow());
    WXSERIAL(wxMDIParentFrame)::StoreObject(s);
    return;
  }

  WXSERIAL(wxMDIParentFrame)::StoreObject(s);
}

void WXSERIAL(wxMDIParentFrame)::LoadObject(wxObjectInputStream& s)
{
  wxMDIParentFrame *frame = (wxMDIParentFrame *)Object();
  wxMDIClientWindow *client;

  client = (wxMDIClientWindow *) s.GetChild();

  frame->Create(m_parent, m_id, m_title, wxPoint(m_x, m_y),
                wxSize(m_w, m_h), m_style, m_name);

  WXSERIAL(wxFrame)::LoadObject(s);
}

/////////////////////////////////////////////////////////////////////////////

void WXSERIAL(wxMDIChildFrame)::StoreObject(wxObjectOutputStream& s)
{
  WXSERIAL(wxFrame)::StoreObject(s);
}

void WXSERIAL(wxMDIChildFrame)::LoadObject(wxObjectInputStream& s)
{
  WXSERIAL(wxFrame)::LoadObject(s);

  ((wxMDIChildFrame *)Object())->Create((wxMDIParentFrame *)m_parent,
                                        m_id, m_title,
                                        wxPoint(m_x, m_y), wxSize(m_w, m_h),
                                        m_style, m_name);
}

/////////////////////////////////////////////////////////////////////////////

void WXSERIAL(wxMDIClientWindow)::StoreObject(wxObjectOutputStream& s)
{
  WXSERIAL(wxWindow)::StoreObject(s);
}

void WXSERIAL(wxMDIClientWindow)::LoadObject(wxObjectInputStream& s)
{
  WXSERIAL(wxWindow)::LoadObject(s);

  ((wxMDIClientWindow *)Object())->CreateClient((wxMDIParentFrame *)m_parent, m_style);
}
