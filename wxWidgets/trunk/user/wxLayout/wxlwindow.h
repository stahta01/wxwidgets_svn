/*-*- c++ -*-********************************************************
 * wxLwindow.h : a scrolled Window for displaying/entering rich text*
 *                                                                  *
 * (C) 1998 by Karsten Ball�der (Ballueder@usa.net)                 *
 *                                                                  *
 * $Id$
 *******************************************************************/
#ifndef WXLWINDOW_H
#define WXLWINDOW_H

#ifdef __GNUG__
#   pragma interface "wxlwindow.h"
#endif

#include   <wx/wx.h>

#include   "wxllist.h"

#define   BROKEN_COMPILER

#ifdef   BROKEN_COMPILER
#   define   virtual
#endif

class wxLayoutWindow : public wxScrolledWindow
{
public:
   wxLayoutWindow(wxWindow *parent);

   wxLayoutList & GetLayoutList(void) { return m_llist; }

   // clears the window and sets default parameters:
   void Clear(int family = wxROMAN, int size=12, int style=wxNORMAL, int weight=wxNORMAL,
              int underline=0, char const *fg="black", char const
              *bg="white")
      {
         GetLayoutList().Clear(family,size,style,weight,underline,fg,bg);
         SetBackgroundColour( *GetLayoutList().GetDefaults()->GetBGColour());
      }

   //virtual void OnDraw(wxDC &dc);
   void OnPaint(wxPaintEvent &WXUNUSED(event));
   virtual void OnMouse(wxMouseEvent& event);
   virtual void OnChar(wxKeyEvent& event);
   void UpdateScrollbars(void);
   void Print(void);
   void Erase(void)
      { m_llist.Clear(); Clear(); }
   void SetEventId(int id) { m_EventId = id; }
   wxPoint const &GetClickPosition(void) const { return
                                                    m_ClickPosition; }
   virtual ~wxLayoutWindow() {} ;
private:
   /// for sending events
   wxWindow *m_Parent;
   int m_EventId;
   /// the layout list to be displayed
   wxLayoutList m_llist;
   /// have we already set the scrollbars?
   bool m_ScrollbarsSet;
   /// if we want to find an object:
   wxPoint m_FindPos;
   wxLayoutObjectBase *m_FoundObject;
   wxPoint m_ClickPosition;
   DECLARE_EVENT_TABLE()
};

#ifdef   BROKEN_COMPILER
#undef   virtual
#endif

#endif
