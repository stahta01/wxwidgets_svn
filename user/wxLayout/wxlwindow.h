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

#ifndef USE_PCH
#  include   <wx/wx.h>
#endif

#include   "wxllist.h"

enum
{
   WXLOWIN_MENU_LARGER = 12000,
   WXLOWIN_MENU_SMALLER,
   WXLOWIN_MENU_UNDERLINE,
   WXLOWIN_MENU_BOLD,
   WXLOWIN_MENU_ITALICS,
   WXLOWIN_MENU_ROMAN,
   WXLOWIN_MENU_TYPEWRITER,
   WXLOWIN_MENU_SANSSERIF,
   WXLOWIN_MENU_RCLICK,
   WXLOWIN_MENU_LCLICK,
   WXLOWIN_MENU_DBLCLICK
   
};

class wxLayoutWindow : public wxScrolledWindow
{
public:
   /** Constructor.
       @param parent parent window to display this panel in
   */
   wxLayoutWindow(wxWindow *parent);

   /* Returns a reference to the wxLayoutList object.
      @return the list
   */
   wxLayoutList & GetLayoutList(void) { return m_llist; }

   // clears the window and sets default parameters:
   void Clear(int family = wxROMAN, int size=12, int style=wxNORMAL, int weight=wxNORMAL,
              int underline=0, char const *fg="black", char const
              *bg="white")
      {
         GetLayoutList().Clear(family,size,style,weight,underline,fg,bg);
         SetBackgroundColour( *GetLayoutList().GetDefaults()->GetBGColour());

         m_bDirty = FALSE;
      }

   // callbacks
   // NB: these functions are used as event handlers and must not be virtual
   void OnPaint(wxPaintEvent &event);
   
   void OnLeftMouseClick(wxMouseEvent& event)
      { OnMouse(WXLOWIN_MENU_LCLICK, event); }
   void OnRightMouseClick(wxMouseEvent& event)
      { OnMouse(WXLOWIN_MENU_RCLICK, event); }
   void OnMouseDblClick(wxMouseEvent& event)
      { OnMouse(WXLOWIN_MENU_DBLCLICK, event); }

   void OnChar(wxKeyEvent& event);
   void OnMenu(wxCommandEvent& event);

   void EnablePopup(bool enable = true) { m_DoPopupMenu = enable; }
   /// gets called by either Update() or OnPaint()
   void DoPaint(wxDC &dc);

#ifdef __WXMSW__
   virtual long MSWGetDlgCode();
#endif //MSW

   void UpdateScrollbars(void);
   void Print(void);
   wxMenu * wxLayoutWindow::MakeFormatMenu(void);

   /// if the flag is true, we send events when user clicks on embedded objects
   inline void SetMouseTracking(bool doIt = true) { m_doSendEvents = doIt; }

   virtual ~wxLayoutWindow() { if(m_PopupMenu) delete m_PopupMenu; }

   // dirty flag access
   bool IsDirty() const { return m_llist.IsDirty(); }
   void ResetDirty()    { m_llist.ResetDirty();     }

protected:
   /// generic function for mouse events processing
   void OnMouse(int eventId, wxMouseEvent& event);

   /// repaint if needed
   void Update(void);

   /// for sending events
   wxWindow *m_Parent;
   bool m_doSendEvents;

   /// the layout list to be displayed
   wxLayoutList m_llist;

   /// have we already set the scrollbars?
   bool m_ScrollbarsSet;
   /// Where does the current view start?
   int m_ViewStartX; int m_ViewStartY;
   
   /// do we have unsaved data?
   bool m_bDirty;

   /// do we handle clicks of the right mouse button?
   bool m_DoPopupMenu;
   /// the menu
   wxMenu * m_PopupMenu;
private:
   DECLARE_EVENT_TABLE()
};

#endif
