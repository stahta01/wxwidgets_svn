/////////////////////////////////////////////////////////////////////////////
// Name:        notebook.h
// Purpose:     interface of wxNotebook
// Author:      wxWidgets team
// RCS-ID:      $Id$
// Licence:     wxWindows license
/////////////////////////////////////////////////////////////////////////////

/**
    @class wxNotebook

    This class represents a notebook control, which manages multiple windows with
    associated tabs.

    To use the class, create a wxNotebook object and call wxNotebook::AddPage
    or wxNotebook::InsertPage, passing a window to be used as the page.
    Do not explicitly delete the window for a page that is currently managed by
    wxNotebook.

    @b wxNotebookPage is a typedef for wxWindow.

    @beginStyleTable
    @style{wxNB_TOP}
           Place tabs on the top side.
    @style{wxNB_LEFT}
           Place tabs on the left side.
    @style{wxNB_RIGHT}
           Place tabs on the right side.
    @style{wxNB_BOTTOM}
           Place tabs under instead of above the notebook pages.
    @style{wxNB_FIXEDWIDTH}
           (Windows only) All tabs will have same width.
    @style{wxNB_MULTILINE}
           (Windows only) There can be several rows of tabs.
    @style{wxNB_NOPAGETHEME}
           (Windows only) Display a solid colour on notebook pages, and not a
           gradient, which can reduce performance.
    @style{wxNB_FLAT}
           (Windows CE only) Show tabs in a flat style.
    @endStyleTable

    The styles wxNB_LEFT, RIGHT and BOTTOM are not supported under
    Microsoft Windows XP when using visual themes.

    @beginEventTable{wxBookCtrlEvent}
    @event{EVT_NOTEBOOK_PAGE_CHANGED(id, func)}
        The page selection was changed.
        Processes a @c wxEVT_COMMAND_NOTEBOOK_PAGE_CHANGED event.
    @event{EVT_NOTEBOOK_PAGE_CHANGING(id, func)}
        The page selection is about to be changed.
        Processes a @c wxEVT_COMMAND_NOTEBOOK_PAGE_CHANGING event.
        This event can be vetoed.
    @endEventTable


    @section notebook_bg Page backgrounds

    On Windows XP, the default theme paints a gradient on the notebook's pages.
    If you wish to suppress this theme, for aesthetic or performance reasons,
    there are three ways of doing it.
    You can use @c wxNB_NOPAGETHEME to disable themed drawing for a particular
    notebook, you can call wxSystemOptions::SetOption to disable it for the
    whole application, or you can disable it for individual pages by using
    SetBackgroundColour().

    To disable themed pages globally:
    @code
    wxSystemOptions::SetOption(wxT("msw.notebook.themed-background"), 0);
    @endcode

    Set the value to 1 to enable it again.
    To give a single page a solid background that more or less fits in with the
    overall theme, use:
    @code
    wxColour col = notebook->GetThemeBackgroundColour();
    if (col.Ok())
    {
        page->SetBackgroundColour(col);
    }
    @endcode

    On platforms other than Windows, or if the application is not using Windows
    themes, GetThemeBackgroundColour() will return an uninitialised colour object,
    and the above code will therefore work on all platforms.


    @library{wxcore}
    @category{miscwnd}
    @appearance{notebook.png}

    @see wxBookCtrl, wxBookCtrlEvent, wxImageList, @ref page_samples_notebook
*/
class wxNotebook : public wxBookCtrlBase
{
public:

    /**
        Constructs a notebook control.
    */
    wxNotebook();

    /**
        Constructs a notebook control.
        Note that sometimes you can reduce flicker by passing the wxCLIP_CHILDREN
        window style.

        @param parent
            The parent window. Must be non-@NULL.
        @param id
            The window identifier.
        @param pos
            The window position.
        @param size
            The window size.
        @param style
            The window style. See wxNotebook.
        @param name
            The name of the control.
    */
    wxNotebook(wxWindow* parent, wxWindowID id,
               const wxPoint& pos = wxDefaultPosition,
               const wxSize& size = wxDefaultSize,
               long style = 0,
               const wxString& name = wxNotebookNameStr);

    /**
        Destroys the wxNotebook object.
    */
    virtual ~wxNotebook();

    /**
        Creates a notebook control.
        See wxNotebook() for a description of the parameters.
    */
    bool Create(wxWindow* parent, wxWindowID id,
                const wxPoint& pos = wxDefaultPosition,
                const wxSize& size = wxDefaultSize,
                long style = 0,
                const wxString& name = wxNotebookNameStr);


    /**
        Returns the number of rows in the notebook control.
    */
    virtual int GetRowCount() const;

    /**
        If running under Windows and themes are enabled for the application, this
        function returns a suitable colour for painting the background of a notebook
        page, and can be passed to SetBackgroundColour().

        Otherwise, an uninitialised colour will be returned.
    */
    virtual wxColour GetThemeBackgroundColour() const;

    /**
        An event handler function, called when the page selection is changed.

        @see wxBookCtrlEvent
    */
    void OnSelChange(wxBookCtrlEvent& event);

    /**
        Sets the amount of space around each page's icon and label, in pixels.

        @note The vertical padding cannot be changed in wxGTK.
    */
    virtual void SetPadding(const wxSize& padding);
};

