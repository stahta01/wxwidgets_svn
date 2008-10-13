/////////////////////////////////////////////////////////////////////////////
// Name:        listbook.h
// Purpose:     interface of wxListbook
// Author:      wxWidgets team
// RCS-ID:      $Id$
// Licence:     wxWindows license
/////////////////////////////////////////////////////////////////////////////

/**
    @class wxListbook

    wxListbook is a class similar to wxNotebook but which uses a wxListCtrl
    to show the labels instead of the tabs.

    The underlying wxListCtrl displays page labels in a one-column report view
    by default. Calling wxBookCtrl::SetImageList will implicitly switch the
    control to use an icon view.

    For usage documentation of this class, please refer to the base abstract class
    wxBookCtrl. You can also use the @ref page_samples_notebook to see wxListbook in
    action.

    @beginStyleTable
    @style{wxLB_DEFAULT}
           Choose the default location for the labels depending on the current
           platform (left everywhere except Mac where it is top).
    @style{wxLB_TOP}
           Place labels above the page area.
    @style{wxLB_LEFT}
           Place labels on the left side.
    @style{wxLB_RIGHT}
           Place labels on the right side.
    @style{wxLB_BOTTOM}
           Place labels below the page area.
    @endStyleTable

    @beginEventTable{wxBookCtrlEvent}
    @event{EVT_LISTBOOK_PAGE_CHANGED(id, func)}
        The page selection was changed.
        Processes a @c wxEVT_COMMAND_LISTBOOK_PAGE_CHANGED event.
    @event{EVT_LISTBOOK_PAGE_CHANGING(id, func)}
        The page selection is about to be changed.
        Processes a @c wxEVT_COMMAND_LISTBOOK_PAGE_CHANGING event.
        This event can be vetoed.
    @endEventTable

    @library{wxcore}
    @category{miscwnd}

    @see wxBookCtrl(), wxNotebook, @ref page_samples_notebook
*/
class wxListbook : public wxBookCtrlBase
{
public:
    /**
        Default ctor.
    */
    wxListbook();

    /**
        Constructs a listbook control.
    */
    wxListbook(wxWindow* parent, wxWindowID id,
               const wxPoint& pos = wxDefaultPosition,
               const wxSize& size = wxDefaultSize,
               long style = 0,
               const wxString& name = wxEmptyStr);

    /**
        Returns the wxListView associated with the control.
    */
    wxListView* GetListView() const;
};

