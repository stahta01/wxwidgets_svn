///////////////////////////////////////////////////////////////////////////////
// Name:        dnd.h
// Purpose:     declaration of the wxDropTarget class
// Author:      Robert Roebling
// RCS-ID:      $Id$
// Copyright:   (c) 1998 Vadim Zeitlin, Robert Roebling
// Licence:     wxWindows license
///////////////////////////////////////////////////////////////////////////////


#ifndef __GTKDNDH__
#define __GTKDNDH__

#ifdef __GNUG__
#pragma interface
#endif

#if wxUSE_DRAG_AND_DROP

#include "wx/object.h"
#include "wx/string.h"
#include "wx/dataobj.h"
#include "wx/cursor.h"
#include "wx/icon.h"
#include "wx/gdicmn.h"

//-------------------------------------------------------------------------
// classes
//-------------------------------------------------------------------------

class wxWindow;

class wxDropTarget;
class wxTextDropTarget;
class wxFileDropTarget;

class wxDropSource;

//-------------------------------------------------------------------------
// wxDropTarget
//-------------------------------------------------------------------------

class wxDropTarget: public wxDropTargetBase
{
public:
    wxDropTarget(wxDataObject *dataObject = (wxDataObject*) NULL );
    
    virtual wxDragResult OnDragOver(wxCoord x, wxCoord y, wxDragResult def);
    virtual bool OnDrop(wxCoord x, wxCoord y);
    virtual bool OnData(wxCoord x, wxCoord y);
    virtual bool GetData();

  // implementation

    GdkAtom GetMatchingPair();

    void RegisterWidget( GtkWidget *widget );
    void UnregisterWidget( GtkWidget *widget );

    GdkDragContext     *m_dragContext;
    GtkWidget          *m_dragWidget;
    GtkSelectionData   *m_dragData;
    guint               m_dragTime;
    bool                m_firstMotion;     /* gdk has no "gdk_drag_enter" event */

    void SetDragContext( GdkDragContext *dc ) { m_dragContext = dc; }
    void SetDragWidget( GtkWidget *w ) { m_dragWidget = w; }
    void SetDragData( GtkSelectionData *sd ) { m_dragData = sd; }
    void SetDragTime( guint time ) { m_dragTime = time; }
};

// ----------------------------------------------------------------------------
// A simple wxDropTarget derived class for text data: you only need to
// override OnDropText() to get something working
// ----------------------------------------------------------------------------

class wxTextDropTarget : public wxDropTarget
{
public:
    wxTextDropTarget();

    virtual bool OnDropText(wxCoord x, wxCoord y, const wxString& text) = 0;

    virtual bool OnData(wxCoord x, wxCoord y);
};

// ----------------------------------------------------------------------------
// A drop target which accepts files (dragged from File Manager or Explorer)
// ----------------------------------------------------------------------------

class wxFileDropTarget : public wxDropTarget
{
public:
    wxFileDropTarget();

    // parameters are the number of files and the array of file names
    virtual bool OnDropFiles(wxCoord x, wxCoord y,
                             const wxArrayString& filenames) = 0;

    virtual bool OnData(wxCoord x, wxCoord y);
};

//-------------------------------------------------------------------------
// wxDropSource
//-------------------------------------------------------------------------

class wxDropSource: public wxDropSourceBase
{
public:
    /* constructor. set data later with SetData() */
    wxDropSource( wxWindow *win,
                  const wxIcon &go = wxNullIcon,
                  const wxIcon &stop = wxNullIcon );

    /* constructor for setting one data object */
    wxDropSource( wxDataObject& data,
                  wxWindow *win,
                  const wxIcon &go = wxNullIcon,
                  const wxIcon &stop = wxNullIcon );

    ~wxDropSource();

    /* start drag action */
    virtual wxDragResult DoDragDrop( bool bAllowMove = FALSE );

    /* GTK implementation */
    void RegisterWindow();
    void UnregisterWindow();

    GtkWidget     *m_widget;
    wxWindow      *m_window;
    wxDragResult   m_retValue;

    wxCursor      m_defaultCursor;
    wxCursor      m_goaheadCursor;

    wxIcon        m_goIcon;
    wxIcon        m_stopIcon;

    bool          m_waiting;
};

#endif

   // wxUSE_DRAG_AND_DROP

#endif
       //__GTKDNDH__

