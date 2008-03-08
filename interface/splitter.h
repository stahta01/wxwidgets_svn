/////////////////////////////////////////////////////////////////////////////
// Name:        splitter.h
// Purpose:     documentation for wxSplitterWindow class
// Author:      wxWidgets team
// RCS-ID:      $Id$
// Licence:     wxWindows license
/////////////////////////////////////////////////////////////////////////////

/**
    @class wxSplitterWindow
    @wxheader{splitter.h}

    @ref overview_wxsplitterwindowoverview "wxSplitterWindow overview"

    This class manages up to two subwindows. The current view can be
    split into two programmatically (perhaps from a menu command), and unsplit
    either programmatically or via the wxSplitterWindow user interface.

    @beginStyleTable
    @style{wxSP_3D}:
           Draws a 3D effect border and sash.
    @style{wxSP_3DSASH}:
           Draws a 3D effect sash.
    @style{wxSP_3DBORDER}:
           Synonym for wxSP_BORDER.
    @style{wxSP_BORDER}:
           Draws a standard border.
    @style{wxSP_NOBORDER}:
           No border (default).
    @style{wxSP_NO_XP_THEME}:
           Under Windows XP, switches off the attempt to draw the splitter
           using Windows XP theming, so the borders and sash will take on the
           pre-XP look.
    @style{wxSP_PERMIT_UNSPLIT}:
           Always allow to unsplit, even with the minimum pane size other than
           zero.
    @style{wxSP_LIVE_UPDATE}:
           Don't draw XOR line but resize the child windows immediately.
    @endStyleTable

    @library{wxcore}
    @category{miscwnd}

    @seealso
    wxSplitterEvent
*/
class wxSplitterWindow : public wxWindow
{
public:
    //@{
    /**
        Constructor for creating the window.
        
        @param parent
        The parent of the splitter window.
        
        @param id
        The window identifier.
        
        @param pos
        The window position.
        
        @param size
        The window size.
        
        @param style
        The window style. See wxSplitterWindow.
        
        @param name
        The window name.
        
        @remarks After using this constructor, you must create either one or two
                   subwindows with the splitter window as parent, and
                   then call one of Initialize(),
                   SplitVertically() and
                   SplitHorizontally() in order to set
                   the pane(s).
        
        @sa Initialize(), SplitVertically(),
              SplitHorizontally(), Create()
    */
    wxSplitterWindow();
    wxSplitterWindow(wxWindow* parent, wxWindowID id,
                     const wxPoint& point = wxDefaultPosition,
                     const wxSize& size = wxDefaultSize,
                     long style=wxSP_3D,
                     const wxString& name = "splitterWindow");
    //@}

    /**
        Destroys the wxSplitterWindow and its children.
    */
    ~wxSplitterWindow();

    /**
        Creation function, for two-step construction. See wxSplitterWindow() for
        details.
    */
    bool Create(wxWindow* parent, wxWindowID id,
                const wxPoint& point = wxDefaultPosition,
                const wxSize& size = wxDefaultSize,
                long style=wxSP_3D,
                const wxString& name = "splitterWindow");

    /**
        Returns the current minimum pane size (defaults to zero).
        
        @sa SetMinimumPaneSize()
    */
    int GetMinimumPaneSize();

    /**
        Returns the current sash gravity.
        
        @sa SetSashGravity()
    */
    double GetSashGravity();

    /**
        Returns the current sash position.
        
        @sa SetSashPosition()
    */
    int GetSashPosition();

    /**
        Gets the split mode.
        
        @sa SetSplitMode(), SplitVertically(),
              SplitHorizontally().
    */
    int GetSplitMode();

    /**
        Returns the left/top or only pane.
    */
    wxWindow* GetWindow1();

    /**
        Returns the right/bottom pane.
    */
    wxWindow* GetWindow2();

    /**
        Initializes the splitter window to have one pane.  The child window is
        shown if it is currently hidden.
        
        @param window
        The pane for the unsplit window.
        
        @remarks This should be called if you wish to initially view only a
                   single pane in the splitter window.
        
        @sa SplitVertically(), SplitHorizontally()
    */
    void Initialize(wxWindow* window);

    /**
        Returns @true if the window is split, @false otherwise.
    */
    bool IsSplit();

    /**
        Application-overridable function called when the sash is double-clicked with
        the left mouse button.
        
        @param x
        The x position of the mouse cursor.
        
        @param y
        The y position of the mouse cursor.
        
        @remarks The default implementation of this function calls Unsplit if the
                   minimum pane size is zero.
        
        @sa Unsplit()
    */
    virtual void OnDoubleClickSash(int x, int y);

    /**
        Application-overridable function called when the sash position is changed by
        user. It may return @false to prevent the change or @true to allow it.
        
        @param newSashPosition
        The new sash position (always positive or zero)
        
        @remarks The default implementation of this function verifies that the
                   sizes of both  panes of the splitter are greater than
                   minimum pane size.
    */
    virtual bool OnSashPositionChange(int newSashPosition);

    /**
        Application-overridable function called when the window is unsplit, either
        programmatically or using the wxSplitterWindow user interface.
        
        @param removed
        The window being removed.
        
        @remarks The default implementation of this function simply hides
                   removed. You may wish to delete the window.
    */
    virtual void OnUnsplit(wxWindow* removed);

    /**
        This function replaces one of the windows managed by the wxSplitterWindow with
        another one. It is in general better to use it instead of calling Unsplit()
        and then resplitting the window back because it will provoke much less flicker
        (if any). It is valid to call this function whether the splitter has two
        windows or only one.
        
        Both parameters should be non-@NULL and @e winOld must specify one of the
        windows managed by the splitter. If the parameters are incorrect or the window
        couldn't be replaced, @false is returned. Otherwise the function will return
        @true, but please notice that it will not delete the replaced window and you
        may wish to do it yourself.
        
        @sa GetMinimumPaneSize()
    */
    bool ReplaceWindow(wxWindow * winOld, wxWindow * winNew);

    /**
        Sets the minimum pane size.
        
        @param paneSize
        Minimum pane size in pixels.
        
        @remarks The default minimum pane size is zero, which means that either
                   pane can be reduced to zero by dragging the sash,
                   thus removing one of the panes. To prevent this
                   behaviour (and veto out-of-range sash dragging), set
                   a minimum size, for example 20 pixels. If the
                   wxSP_PERMIT_UNSPLIT style is used when a splitter
                   window is created, the window may be unsplit even if
                   minimum size is non-zero.
        
        @sa GetMinimumPaneSize()
    */
    void SetMinimumPaneSize(int paneSize);

    /**
        Sets the sash gravity.
        
        @param gravity
        The sash gravity. Value between 0.0 and 1.0.
        
        @sa GetSashGravity()
    */
    void SetSashGravity(double gravity);

    /**
        Sets the sash position.
        
        @param position
        The sash position in pixels.
        
        @param redraw
        If @true, resizes the panes and redraws the sash and border.
        
        @remarks Does not currently check for an out-of-range value.
        
        @sa GetSashPosition()
    */
    void SetSashPosition(int position, const bool redraw = @true);

    /**
        Sets the sash size. Normally, the sash size is determined according to the
        metrics
        of each platform, but the application can override this, for example to show
        a thin sash that the user is not expected to drag. If @e size is more -1,
        the custom sash size will be used.
    */
    void SetSashSize(int size);

    /**
        Sets the split mode.
        
        @param mode
        Can be wxSPLIT_VERTICAL or wxSPLIT_HORIZONTAL.
        
        @remarks Only sets the internal variable; does not update the display.
        
        @sa GetSplitMode(), SplitVertically(),
              SplitHorizontally().
    */
    void SetSplitMode(int mode);

    /**
        Initializes the top and bottom panes of the splitter window.  The
        child windows are shown if they are currently hidden.
        
        @param window1
        The top pane.
        
        @param window2
        The bottom pane.
        
        @param sashPosition
        The initial position of the sash. If this value is
        positive, it specifies the size of the upper pane. If it is negative, its
        absolute value gives the size of the lower pane. Finally, specify 0 (default)
        to choose the default position (half of the total window height).
        
        @returns @true if successful, @false otherwise (the window was already
                   split).
        
        @remarks This should be called if you wish to initially view two panes.
                   It can also be called at any subsequent time, but the
                   application should check that the window is not
                   currently split using IsSplit.
        
        @sa SplitVertically(), IsSplit(),
              Unsplit()
    */
    bool SplitHorizontally(wxWindow* window1, wxWindow* window2,
                           int sashPosition = 0);

    /**
        Initializes the left and right panes of the splitter window.  The
        child windows are shown if they are currently hidden.
        
        @param window1
        The left pane.
        
        @param window2
        The right pane.
        
        @param sashPosition
        The initial position of the sash. If this value is
        positive, it specifies the size of the left pane. If it is negative, it is
        absolute value gives the size of the right pane. Finally, specify 0 (default)
        to choose the default position (half of the total window width).
        
        @returns @true if successful, @false otherwise (the window was already
                   split).
        
        @remarks This should be called if you wish to initially view two panes.
                   It can also be called at any subsequent time, but the
                   application should check that the window is not
                   currently split using IsSplit.
        
        @sa SplitHorizontally(), IsSplit(),
              Unsplit().
    */
    bool SplitVertically(wxWindow* window1, wxWindow* window2,
                         int sashPosition = 0);

    /**
        Unsplits the window.
        
        @param toRemove
        The pane to remove, or @NULL to remove the right or bottom pane.
        
        @returns @true if successful, @false otherwise (the window was not split).
        
        @remarks This call will not actually delete the pane being removed; it
                   calls OnUnsplit which can be overridden for the
                   desired behaviour. By default, the pane being removed
                   is hidden.
        
        @sa SplitHorizontally(), SplitVertically(),
              IsSplit(), OnUnsplit()
    */
    bool Unsplit(wxWindow* toRemove = @NULL);

    /**
        Causes any pending sizing of the sash and child panes to take place
        immediately.
        
        Such resizing normally takes place in idle time, in order
        to wait for layout to be completed. However, this can cause
        unacceptable flicker as the panes are resized after the window has been
        shown. To work around this, you can perform window layout (for
        example by sending a size event to the parent window), and then
        call this function, before showing the top-level window.
    */
    void UpdateSize();
};


/**
    @class wxSplitterEvent
    @wxheader{splitter.h}

    This class represents the events generated by a splitter control. Also there is
    only one event class, the data associated to the different events is not the
    same and so not all accessor functions may be called for each event. The
    documentation mentions the kind of event(s) for which the given accessor
    function makes sense: calling it for other types of events will result
    in assert failure (in debug mode) and will return meaningless results.

    @library{wxcore}
    @category{events}

    @seealso
    wxSplitterWindow, @ref overview_eventhandlingoverview "Event handling overview"
*/
class wxSplitterEvent : public wxNotifyEvent
{
public:
    /**
        Constructor. Used internally by wxWidgets only.
    */
    wxSplitterEvent(wxEventType eventType = wxEVT_@NULL,
                    wxSplitterWindow * splitter = @NULL);

    /**
        Returns the new sash position.
        
        May only be called while processing
        wxEVT_COMMAND_SPLITTER_SASH_POS_CHANGING and
        wxEVT_COMMAND_SPLITTER_SASH_POS_CHANGED events.
    */
    int GetSashPosition();

    /**
        Returns a pointer to the window being removed when a splitter window
        is unsplit.
        
        May only be called while processing
        wxEVT_COMMAND_SPLITTER_UNSPLIT events.
    */
    wxWindow* GetWindowBeingRemoved();

    /**
        Returns the x coordinate of the double-click point.
        
        May only be called while processing
        wxEVT_COMMAND_SPLITTER_DOUBLECLICKED events.
    */
#define int GetX()     /* implementation is private */

    /**
        Returns the y coordinate of the double-click point.
        
        May only be called while processing
        wxEVT_COMMAND_SPLITTER_DOUBLECLICKED events.
    */
#define int GetY()     /* implementation is private */

    /**
        In the case of wxEVT_COMMAND_SPLITTER_SASH_POS_CHANGED events,
        sets the new sash position. In the case of
        wxEVT_COMMAND_SPLITTER_SASH_POS_CHANGING events, sets the new
        tracking bar position so visual feedback during dragging will
        represent that change that will actually take place. Set to -1 from
        the event handler code to prevent repositioning.
        
        May only be called while processing
        wxEVT_COMMAND_SPLITTER_SASH_POS_CHANGING and
        wxEVT_COMMAND_SPLITTER_SASH_POS_CHANGED events.
        
        @param pos
        New sash position.
    */
    void SetSashPosition(int pos);
};
