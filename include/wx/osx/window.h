/////////////////////////////////////////////////////////////////////////////
// Name:        include/wx/mac/carbon/window.h
// Purpose:     wxWindowMac class
// Author:      Stefan Csomor
// Modified by:
// Created:     1998-01-01
// RCS-ID:      $Id$
// Copyright:   (c) Stefan Csomor
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_WINDOW_H_
#define _WX_WINDOW_H_

#include "wx/brush.h"
#include "wx/dc.h"

class WXDLLIMPEXP_FWD_CORE wxButton;
class WXDLLIMPEXP_FWD_CORE wxScrollBar;
class WXDLLIMPEXP_FWD_CORE wxNonOwnedWindow;

class WXDLLIMPEXP_FWD_CORE wxMacControl ;
class WXDLLIMPEXP_FWD_CORE wxWidgetImpl ;

class WXDLLIMPEXP_CORE wxWindowMac: public wxWindowBase
{
    DECLARE_DYNAMIC_CLASS(wxWindowMac)

    friend class wxDC;
    friend class wxPaintDC;

public:
    wxWindowMac();

    wxWindowMac( wxWindowMac *parent,
                wxWindowID id,
                const wxPoint& pos = wxDefaultPosition,
                const wxSize& size = wxDefaultSize,
                long style = 0,
                const wxString& name = wxPanelNameStr );

    virtual ~wxWindowMac();

    bool Create( wxWindowMac *parent,
                wxWindowID id,
                const wxPoint& pos = wxDefaultPosition,
                const wxSize& size = wxDefaultSize,
                long style = 0,
                const wxString& name = wxPanelNameStr );

    // implement base class pure virtuals
    virtual void SetLabel( const wxString& label );
    virtual wxString GetLabel() const;

    virtual void Raise();
    virtual void Lower();

    virtual bool Show( bool show = true );

    virtual bool IsShownOnScreen() const;

    virtual void SetFocus();

    virtual void WarpPointer( int x, int y );

    virtual void Refresh( bool eraseBackground = true,
                          const wxRect *rect = NULL );

    virtual void Update() ;
    virtual void ClearBackground();

    virtual bool SetCursor( const wxCursor &cursor );
    virtual bool SetFont( const wxFont &font );
    virtual bool SetBackgroundColour( const wxColour &colour );
    virtual bool SetForegroundColour( const wxColour &colour );

    virtual int GetCharHeight() const;
    virtual int GetCharWidth() const;
    virtual void GetTextExtent( const wxString& string,
                               int *x, int *y,
                               int *descent = NULL,
                               int *externalLeading = NULL,
                               const wxFont *theFont = NULL )
                               const;
protected:
    virtual void DoEnable( bool enable );
#if wxUSE_MENUS
    virtual bool DoPopupMenu( wxMenu *menu, int x, int y );
#endif

    virtual void DoFreeze();
    virtual void DoThaw();

public:
    virtual void SetScrollbar( int orient, int pos, int thumbVisible,
                               int range, bool refresh = true );
    virtual void SetScrollPos( int orient, int pos, bool refresh = true );
    virtual int GetScrollPos( int orient ) const;
    virtual int GetScrollThumb( int orient ) const;
    virtual int GetScrollRange( int orient ) const;
    virtual void ScrollWindow( int dx, int dy,
                               const wxRect* rect = NULL );
    virtual void AlwaysShowScrollbars(bool horz = true, bool vert = true);
    virtual bool IsScrollbarAlwaysShown(int orient) const
    {
        return orient == wxHORIZONTAL ? m_hScrollBarAlwaysShown
                                      : m_vScrollBarAlwaysShown;
    }

    virtual bool Reparent( wxWindowBase *newParent );

#if wxUSE_DRAG_AND_DROP
    virtual void SetDropTarget( wxDropTarget *dropTarget );
#endif

    // Accept files for dragging
    virtual void DragAcceptFiles( bool accept );

    // implementation from now on
    // --------------------------

    void MacClientToRootWindow( int *x , int *y ) const;

    void MacWindowToRootWindow( int *x , int *y ) const;

    void MacRootWindowToWindow( int *x , int *y ) const;

    virtual wxString MacGetToolTipString( wxPoint &where );

    // simple accessors
    // ----------------

    virtual WXWidget GetHandle() const;

    virtual bool SetTransparent(wxByte alpha);
    virtual bool CanSetTransparent();
    virtual wxByte GetTransparent() const ;

    // event handlers
    // --------------
    void OnPaint( wxPaintEvent& event );
    void OnNcPaint( wxNcPaintEvent& event );
    void OnEraseBackground(wxEraseEvent& event );
    void OnMouseEvent( wxMouseEvent &event );

    void MacOnScroll( wxScrollEvent&event );

    virtual bool AcceptsFocus() const;

    virtual bool IsDoubleBuffered() const { return true; }

public:
    static long         MacRemoveBordersFromStyle( long style ) ;

public:
    void OnInternalIdle();

    // For implementation purposes:
    // sometimes decorations make the client area smaller
    virtual wxPoint GetClientAreaOrigin() const;

    wxWindowMac *FindItem(long id) const;
    wxWindowMac *FindItemByHWND(WXHWND hWnd, bool controlOnly = false) const;

    virtual void        MacHandleControlClick( WXWidget control , wxInt16 controlpart , bool mouseStillDown ) ;
    virtual bool        MacDoRedraw( void* updatergn , long time ) ;

    // this should not be overriden in classes above wxWindowMac
    // because it is called from its destructor via DeleteChildren
    virtual void        RemoveChild( wxWindowBase *child );
    virtual void        MacPaintBorders( int left , int top ) ;
    void                MacPaintGrowBox();

    // invalidates the borders and focus area around the control;
    // must not be virtual as it will be called during destruction
    void                MacInvalidateBorders() ;

    WXWindow            MacGetTopLevelWindowRef() const ;
    wxNonOwnedWindow*   MacGetTopLevelWindow() const ;

    virtual long        MacGetLeftBorderSize() const ;
    virtual long        MacGetRightBorderSize() const ;
    virtual long        MacGetTopBorderSize() const ;
    virtual long        MacGetBottomBorderSize() const ;

    virtual void        MacSuperChangedPosition() ;

    // absolute coordinates of this window's root have changed
    virtual void        MacTopLevelWindowChangedPosition() ;

    virtual void        MacChildAdded() ;
    virtual void        MacVisibilityChanged() ;
    virtual void        MacEnabledStateChanged() ;
    virtual void        MacHiliteChanged() ;
    virtual wxInt32     MacControlHit( WXEVENTHANDLERREF handler , WXEVENTREF event ) ;

    bool                MacIsReallyEnabled() ;
    bool                MacIsReallyHilited() ;

    bool                MacIsUserPane() { return m_macIsUserPane; }

    virtual bool        MacSetupCursor( const wxPoint& pt ) ;

    // return the rectangle that would be visible of this control,
    // regardless whether controls are hidden
    // only taking into account clipping by parent windows
    const wxRect&       MacGetClippedClientRect() const ;
    const wxRect&       MacGetClippedRect() const ;
    const wxRect&       MacGetClippedRectWithOuterStructure() const ;

    // returns the visible region of this control in window ie non-client coordinates
    const wxRegion&     MacGetVisibleRegion( bool includeOuterStructures = false ) ;

    // returns true if children have to clipped to the content area
    // (e.g., scrolled windows)
    bool                MacClipChildren() const { return m_clipChildren ; }
    void                MacSetClipChildren( bool clip ) { m_clipChildren = clip ; }

    // returns true if the grandchildren need to be clipped to the children's content area
    // (e.g., splitter windows)
    virtual bool        MacClipGrandChildren() const { return false ; }
    bool                MacIsWindowScrollbar( const wxWindow* sb ) const
    { return ((wxWindow*)m_hScrollBar == sb || (wxWindow*)m_vScrollBar == sb) ; }
    virtual bool IsClientAreaChild(const wxWindow *child) const
    {
        return !MacIsWindowScrollbar(child) &&
               wxWindowBase::IsClientAreaChild(child);
    }

    void                MacPostControlCreate(const wxPoint& pos, const wxSize& size) ;
    wxList&             GetSubcontrols() { return m_subControls; }

    // translate wxWidgets coords into ones suitable
    // to be passed to CreateControl calls
    //
    // returns true if non-default coords are returned, false otherwise
    bool                MacGetBoundsForControl(const wxPoint& pos,
                                           const wxSize& size,
                                           int& x, int& y,
                                           int& w, int& h , bool adjustForOrigin ) const ;

    // the 'true' OS level control for this wxWindow
#if wxOSX_USE_CARBON
    wxMacControl*       GetPeer() const { return m_peer ; }
#else
    wxWidgetImpl*       GetPeer() const { return m_peer ; }
#endif
 
    void *              MacGetCGContextRef() { return m_cgContextRef ; }
    void                MacSetCGContextRef(void * cg) { m_cgContextRef = cg ; }

    // osx specific event handling common for all osx-ports
    
    virtual bool        HandleClicked( double timestampsec );
protected:
    // For controls like radio buttons which are genuinely composite
    wxList              m_subControls;

#if wxOSX_USE_CARBON
    // the peer object, allowing for cleaner API support
    wxMacControl *       m_peer ;
#else
    wxWidgetImpl *       m_peer ;
#endif
    void *              m_cgContextRef ;

    // cache the clipped rectangles within the window hierarchy
    void                MacUpdateClippedRects() const ;

    mutable bool        m_cachedClippedRectValid ;
    mutable wxRect      m_cachedClippedRectWithOuterStructure ;
    mutable wxRect      m_cachedClippedRect ;
    mutable wxRect      m_cachedClippedClientRect ;
    mutable wxRegion    m_cachedClippedRegionWithOuterStructure ;
    mutable wxRegion    m_cachedClippedRegion ;
    mutable wxRegion    m_cachedClippedClientRegion ;

    // true if is is not a native control but a wxWindow control
    bool                m_macIsUserPane ;

    // insets of the mac control from the wx top left corner
    wxPoint             m_macTopLeftInset ;
    wxPoint             m_macBottomRightInset ;
    wxByte              m_macAlpha ;

    wxScrollBar*        m_hScrollBar ;
    wxScrollBar*        m_vScrollBar ;
    bool                m_hScrollBarAlwaysShown;
    bool                m_vScrollBarAlwaysShown;
    wxString            m_label ;

    // set to true if we do a sharp clip at the content area of this window
    // must be dynamic as eg a panel normally is not clipping precisely, but if
    // it becomes the target window of a scrolled window it has to...
    bool                m_clipChildren ;

    virtual bool        MacIsChildOfClientArea( const wxWindow* child ) const ;

    bool                MacHasScrollBarCorner() const;
    void                MacCreateScrollBars( long style ) ;
    void                MacRepositionScrollBars() ;
    void                MacUpdateControlFont() ;

    // implement the base class pure virtuals
    virtual wxSize DoGetBestSize() const;
    virtual wxSize DoGetSizeFromClientSize( const wxSize & size ) const;
    virtual void DoClientToScreen( int *x, int *y ) const;
    virtual void DoScreenToClient( int *x, int *y ) const;
    virtual void DoGetPosition( int *x, int *y ) const;
    virtual void DoGetSize( int *width, int *height ) const;
    virtual void DoGetClientSize( int *width, int *height ) const;
    virtual void DoSetSize(int x, int y,
                           int width, int height,
                           int sizeFlags = wxSIZE_AUTO);
    virtual void DoSetClientSize(int width, int height);

    virtual void DoCaptureMouse();
    virtual void DoReleaseMouse();

    // move the window to the specified location and resize it: this is called
    // from both DoSetSize() and DoSetClientSize() and would usually just call
    // ::MoveWindow() except for composite controls which will want to arrange
    // themselves inside the given rectangle
    virtual void DoMoveWindow( int x, int y, int width, int height );
    virtual void DoSetWindowVariant( wxWindowVariant variant );

#if wxUSE_TOOLTIPS
    virtual void DoSetToolTip( wxToolTip *tip );
#endif

private:
    // common part of all ctors
    void Init();

    // show/hide scrollbars as needed, common part of SetScrollbar() and
    // AlwaysShowScrollbars()
    void DoUpdateScrollbarVisibility();


    DECLARE_NO_COPY_CLASS(wxWindowMac)
    DECLARE_EVENT_TABLE()
};

#endif // _WX_WINDOW_H_
