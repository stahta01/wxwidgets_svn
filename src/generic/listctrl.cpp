/////////////////////////////////////////////////////////////////////////////
// Name:        generic/listctrl.cpp
// Purpose:     generic implementation of wxListCtrl
// Author:      Robert Roebling
//              Vadim Zeitlin (virtual list control support)
// Id:          $Id$
// Copyright:   (c) 1998 Robert Roebling
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

/*
   FIXME for virtual list controls

  +1. clicking on the item with a mouse is awfully slow, what is going on?
      note that selecting with keyboard seems to be much faster
   => fixed HighlightAll() - iterating over 1000000 items *is* slow

   2. background colour is wrong?
 */

/*
   TODO for better virtual list control support:

   1. less dumb line caching, we should cache at least all those visible
      in the control itself and probably twice as many (we might also need to
      cache the first one always for geometry calculations?)

  +2. storing selections: we can't use an array to store the selected indices
      like right now as selecting all in a control with 1000000 items is not
      doable like this - instead, store selections as collection of individual
      items and ranges

   => wxSelectionStore

   3. we need to implement searching/sorting somehow

   4. the idea of storing the line index in the line itself is really stupid,
      we shouldn't need it - but for this we have to get rid of all calles to
      wxListLineData::GetFoo() and replace them with something like
        if ( IsVirtual()
            ... we have it ourselves ...
        else
            line->GetFoo();
 */

// ============================================================================
// declarations
// ============================================================================

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

#ifdef __GNUG__
    #pragma implementation "listctrl.h"
    #pragma implementation "listctrlbase.h"
#endif                                                  

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#if wxUSE_LISTCTRL

#include "wx/dcscreen.h"
#include "wx/app.h"
#include "wx/listctrl.h"
#include "wx/imaglist.h"
#include "wx/dynarray.h"

#ifdef __WXGTK__
#include <gtk/gtk.h>
#include "wx/gtk/win_gtk.h"
#endif

// ----------------------------------------------------------------------------
// events
// ----------------------------------------------------------------------------

DEFINE_EVENT_TYPE(wxEVT_COMMAND_LIST_BEGIN_DRAG)
DEFINE_EVENT_TYPE(wxEVT_COMMAND_LIST_BEGIN_RDRAG)
DEFINE_EVENT_TYPE(wxEVT_COMMAND_LIST_BEGIN_LABEL_EDIT)
DEFINE_EVENT_TYPE(wxEVT_COMMAND_LIST_END_LABEL_EDIT)
DEFINE_EVENT_TYPE(wxEVT_COMMAND_LIST_DELETE_ITEM)
DEFINE_EVENT_TYPE(wxEVT_COMMAND_LIST_DELETE_ALL_ITEMS)
DEFINE_EVENT_TYPE(wxEVT_COMMAND_LIST_GET_INFO)
DEFINE_EVENT_TYPE(wxEVT_COMMAND_LIST_SET_INFO)
DEFINE_EVENT_TYPE(wxEVT_COMMAND_LIST_ITEM_SELECTED)
DEFINE_EVENT_TYPE(wxEVT_COMMAND_LIST_ITEM_DESELECTED)
DEFINE_EVENT_TYPE(wxEVT_COMMAND_LIST_KEY_DOWN)
DEFINE_EVENT_TYPE(wxEVT_COMMAND_LIST_INSERT_ITEM)
DEFINE_EVENT_TYPE(wxEVT_COMMAND_LIST_COL_CLICK)
DEFINE_EVENT_TYPE(wxEVT_COMMAND_LIST_ITEM_RIGHT_CLICK)
DEFINE_EVENT_TYPE(wxEVT_COMMAND_LIST_ITEM_MIDDLE_CLICK)
DEFINE_EVENT_TYPE(wxEVT_COMMAND_LIST_ITEM_ACTIVATED)

// ----------------------------------------------------------------------------
// constants
// ----------------------------------------------------------------------------

// the height of the header window (FIXME: should depend on its font!)
static const int HEADER_HEIGHT = 23;

// the scrollbar units
static const int SCROLL_UNIT_X = 15;
static const int SCROLL_UNIT_Y = 15;

// the spacing between the lines (in report mode)
static const int LINE_SPACING = 0;

// extra margins around the text label
static const int EXTRA_WIDTH = 3;
static const int EXTRA_HEIGHT = 4;

// offset for the header window
static const int HEADER_OFFSET_X = 1;
static const int HEADER_OFFSET_Y = 1;

// when autosizing the columns, add some slack
static const int AUTOSIZE_COL_MARGIN = 10;

// default and minimal widths for the header columns
static const int WIDTH_COL_DEFAULT = 80;
static const int WIDTH_COL_MIN = 10;

// ============================================================================
// private classes
// ============================================================================

// ----------------------------------------------------------------------------
// wxSelectionStore
// ----------------------------------------------------------------------------

int CMPFUNC_CONV wxSizeTCmpFn(size_t n1, size_t n2) { return n1 - n2; }

WX_DEFINE_SORTED_EXPORTED_ARRAY(size_t, wxIndexArray);

// this class is used to store the selected items in the virtual list control
// (but it is not tied to list control and so can be used with other controls
// such as wxListBox in wxUniv)
//
// the idea is to make it really smart later (i.e. store the selections as an
// array of ranes + individual items) but, as I don't have time to do it now
// (this would require writing code to merge/break ranges and much more) keep
// it simple but define a clean interface to it which allows it to be made
// smarter later
class WXDLLEXPORT wxSelectionStore
{
public:
    wxSelectionStore() : m_itemsSel(wxSizeTCmpFn) { Init(); }

    // set the total number of items we handle
    void SetItemCount(size_t count) { m_count = count; }

    // special case of SetItemCount(0)
    void Clear() { m_itemsSel.Clear(); m_count = 0; }

    // must be called when a new item is inserted/added
    void OnItemAdd(size_t item) { wxFAIL_MSG( _T("TODO") ); }

    // must be called when an item is deleted
    void OnItemDelete(size_t item);

    // select one item, use SelectRange() insted if possible!
    //
    // returns true if the items selection really changed
    bool SelectItem(size_t item, bool select = TRUE);

    // select the range of items
    void SelectRange(size_t itemFrom, size_t itemTo, bool select = TRUE);

    // return true if the given item is selected
    bool IsSelected(size_t item) const;

    // return the total number of selected items
    size_t GetSelectedCount() const
    {
        return m_defaultState ? m_count - m_itemsSel.GetCount()
                              : m_itemsSel.GetCount();
    }

private:
    // (re)init
    void Init() { m_defaultState = FALSE; }

    // the total number of items we handle
    size_t m_count;

    // the default state: normally, FALSE (i.e. off) but maybe set to TRUE if
    // there are more selected items than non selected ones - this allows to
    // handle selection of all items efficiently
    bool m_defaultState;

    // the array of items whose selection state is different from default
    wxIndexArray m_itemsSel;

    DECLARE_NO_COPY_CLASS(wxSelectionStore)
};

//-----------------------------------------------------------------------------
//  wxListItemData (internal)
//-----------------------------------------------------------------------------

class WXDLLEXPORT wxListItemData
{
public:
    wxListItemData(wxListMainWindow *owner);
    ~wxListItemData() { delete m_attr; delete m_rect; }

    void SetItem( const wxListItem &info );
    void SetImage( int image ) { m_image = image; }
    void SetData( long data ) { m_data = data; }
    void SetPosition( int x, int y );
    void SetSize( int width, int height );

    bool HasText() const { return !m_text.empty(); }
    const wxString& GetText() const { return m_text; }
    void SetText(const wxString& text) { m_text = text; }

    // we can't use empty string for measuring the string width/height, so
    // always return something
    wxString GetTextForMeasuring() const
    {
        wxString s = GetText();
        if ( s.empty() )
            s = _T('H');

        return s;
    }

    bool IsHit( int x, int y ) const;

    int GetX() const;
    int GetY() const;
    int GetWidth() const;
    int GetHeight() const;

    int GetImage() const { return m_image; }
    bool HasImage() const { return GetImage() != -1; }

    void GetItem( wxListItem &info ) const;

    wxListItemAttr *GetAttributes() const { return m_attr; }

public:
    // the item image or -1
    int m_image;

    // user data associated with the item
    long m_data;

    // the item coordinates are not used in report mode, instead this pointer
    // is NULL and the owner window is used to retrieve the item position and
    // size
    wxRect *m_rect;

    // the list ctrl we are in
    wxListMainWindow *m_owner;

    // custom attributes or NULL
    wxListItemAttr *m_attr;

protected:
    // common part of all ctors
    void Init();

    wxString m_text;
};

//-----------------------------------------------------------------------------
//  wxListHeaderData (internal)
//-----------------------------------------------------------------------------

class WXDLLEXPORT wxListHeaderData : public wxObject
{
protected:
    long      m_mask;
    int       m_image;
    wxString  m_text;
    int       m_format;
    int       m_width;
    int       m_xpos,
              m_ypos;
    int       m_height;

public:
    wxListHeaderData();
    wxListHeaderData( const wxListItem &info );
    void SetItem( const wxListItem &item );
    void SetPosition( int x, int y );
    void SetWidth( int w );
    void SetFormat( int format );
    void SetHeight( int h );
    bool HasImage() const;

    bool HasText() const { return !m_text.empty(); }
    const wxString& GetText() const { return m_text; }
    void SetText(const wxString& text) { m_text = text; }

    void GetItem( wxListItem &item );

    bool IsHit( int x, int y ) const;
    int GetImage() const;
    int GetWidth() const;
    int GetFormat() const;

private:
    DECLARE_DYNAMIC_CLASS(wxListHeaderData);
};

//-----------------------------------------------------------------------------
//  wxListLineData (internal)
//-----------------------------------------------------------------------------

WX_DECLARE_LIST(wxListItemData, wxListItemDataList);
#include "wx/listimpl.cpp"
WX_DEFINE_LIST(wxListItemDataList);

class WXDLLEXPORT wxListLineData
{
public:
    // the list of subitems: only may have more than one item in report mode
    wxListItemDataList m_items;

    // this is not used in report view
    struct GeometryInfo
    {
        // total item rect
        wxRect m_rectAll;

        // label only
        wxRect m_rectLabel;

        // icon only
        wxRect m_rectIcon;

        // the part to be highlighted
        wxRect m_rectHighlight;
    } *m_gi;

    // is this item selected? [NB: not used in virtual mode]
    bool m_highlighted;

    // back pointer to the list ctrl
    wxListMainWindow *m_owner;

public:
    wxListLineData(wxListMainWindow *owner);

    ~wxListLineData() { delete m_gi; }

    // are we in report mode?
    inline bool InReportView() const;

    // are we in virtual report mode?
    inline bool IsVirtual() const;

    // these 2 methods shouldn't be called for report view controls, in that
    // case we determine our position/size ourselves

    // calculate the size of the line
    void CalculateSize( wxDC *dc, int spacing );

    // remember the position this line appears at
    void SetPosition( int x, int y,  int window_width, int spacing );

    // wxListCtrl API

    void SetImage( int image ) { SetImage(0, image); }
    int GetImage() const { return GetImage(0); }
    bool HasImage() const { return GetImage() != -1; }
    bool HasText() const { return !GetText(0).empty(); }

    void SetItem( int index, const wxListItem &info );
    void GetItem( int index, wxListItem &info );

    wxString GetText(int index) const;
    void SetText( int index, const wxString s );

    // return true if the highlighting really changed
    bool Highlight( bool on );

    void ReverseHighlight();

    bool IsHighlighted() const
    {
        wxASSERT_MSG( !IsVirtual(), _T("unexpected call to IsHighlighted") );

        return m_highlighted;
    }

    // draw the line on the given DC in icon/list mode
    void Draw( wxDC *dc );

    // the same in report mode
    void DrawInReportMode( wxDC *dc,
                           const wxRect& rect,
                           const wxRect& rectHL,
                           bool highlighted );

private:
    // set the line to contain num items (only can be > 1 in report mode)
    void InitItems( int num );

    // get the mode (i.e. style)  of the list control
    inline int GetMode() const;

    void SetAttributes(wxDC *dc,
                       const wxListItemAttr *attr,
                       const wxColour& colText,
                       const wxFont& font,
                       bool highlight);

    // these are only used by GetImage/SetImage above, we don't support images
    // with subitems at the public API level yet
    void SetImage( int index, int image );
    int GetImage( int index ) const;
};

WX_DECLARE_EXPORTED_OBJARRAY(wxListLineData, wxListLineDataArray);
#include "wx/arrimpl.cpp"
WX_DEFINE_OBJARRAY(wxListLineDataArray);

//-----------------------------------------------------------------------------
//  wxListHeaderWindow (internal)
//-----------------------------------------------------------------------------

class WXDLLEXPORT wxListHeaderWindow : public wxWindow
{
protected:
    wxListMainWindow  *m_owner;
    wxCursor          *m_currentCursor;
    wxCursor          *m_resizeCursor;
    bool               m_isDragging;

    // column being resized
    int m_column;

    // divider line position in logical (unscrolled) coords
    int m_currentX;

    // minimal position beyond which the divider line can't be dragged in
    // logical coords
    int m_minX;

public:
    wxListHeaderWindow();
    virtual ~wxListHeaderWindow();

    wxListHeaderWindow( wxWindow *win,
                        wxWindowID id,
                        wxListMainWindow *owner,
                        const wxPoint &pos = wxDefaultPosition,
                        const wxSize &size = wxDefaultSize,
                        long style = 0,
                        const wxString &name = "wxlistctrlcolumntitles" );

    void DoDrawRect( wxDC *dc, int x, int y, int w, int h );
    void DrawCurrent();
    void AdjustDC(wxDC& dc);

    void OnPaint( wxPaintEvent &event );
    void OnMouse( wxMouseEvent &event );
    void OnSetFocus( wxFocusEvent &event );

    // needs refresh
    bool m_dirty;

private:
    DECLARE_DYNAMIC_CLASS(wxListHeaderWindow)
    DECLARE_EVENT_TABLE()
};

//-----------------------------------------------------------------------------
// wxListRenameTimer (internal)
//-----------------------------------------------------------------------------

class WXDLLEXPORT wxListRenameTimer: public wxTimer
{
private:
    wxListMainWindow *m_owner;

public:
    wxListRenameTimer( wxListMainWindow *owner );
    void Notify();
};

//-----------------------------------------------------------------------------
//  wxListTextCtrl (internal)
//-----------------------------------------------------------------------------

class WXDLLEXPORT wxListTextCtrl: public wxTextCtrl
{
private:
    bool               *m_accept;
    wxString           *m_res;
    wxListMainWindow   *m_owner;
    wxString            m_startValue;

public:
    wxListTextCtrl() {}
    wxListTextCtrl( wxWindow *parent, const wxWindowID id,
                    bool *accept, wxString *res, wxListMainWindow *owner,
                    const wxString &value = "",
                    const wxPoint &pos = wxDefaultPosition, const wxSize &size = wxDefaultSize,
                    int style = 0,
                    const wxValidator& validator = wxDefaultValidator,
                    const wxString &name = "listctrltextctrl" );
    void OnChar( wxKeyEvent &event );
    void OnKeyUp( wxKeyEvent &event );
    void OnKillFocus( wxFocusEvent &event );

private:
    DECLARE_DYNAMIC_CLASS(wxListTextCtrl);
    DECLARE_EVENT_TABLE()
};

//-----------------------------------------------------------------------------
//  wxListMainWindow (internal)
//-----------------------------------------------------------------------------

WX_DECLARE_LIST(wxListHeaderData, wxListHeaderDataList);
#include "wx/listimpl.cpp"
WX_DEFINE_LIST(wxListHeaderDataList);

class WXDLLEXPORT wxListMainWindow : public wxScrolledWindow
{
public:
    wxListMainWindow();
    wxListMainWindow( wxWindow *parent,
                      wxWindowID id,
                      const wxPoint& pos = wxDefaultPosition,
                      const wxSize& size = wxDefaultSize,
                      long style = 0,
                      const wxString &name = _T("listctrlmainwindow") );

    virtual ~wxListMainWindow();

    bool HasFlag(int flag) const { return m_parent->HasFlag(flag); }

    // return true if this is a virtual list control
    bool IsVirtual() const { return HasFlag(wxLC_VIRTUAL); }

    // return true if the control is in report mode
    bool InReportView() const { return HasFlag(wxLC_REPORT); }

    // return true if we are in single selection mode, false if multi sel
    bool IsSingleSel() const { return HasFlag(wxLC_SINGLE_SEL); }

    // do we have a header window?
    bool HasHeader() const
        { return HasFlag(wxLC_REPORT) && !HasFlag(wxLC_NO_HEADER); }

    void HighlightAll( bool on );

    // all these functions only do something if the line is currently visible

    // change the line "selected" state, return TRUE if it really changed
    bool HighlightLine( size_t line, bool highlight = TRUE);

    // as HighlightLine() but do it for the range of lines: this is incredibly
    // more efficient for virtual list controls!
    //
    // NB: unlike HighlightLine() this one does refresh the lines on screen
    void HighlightLines( size_t lineFrom, size_t lineTo, bool on = TRUE );

    // toggle the line state and refresh it
    void ReverseHighlight( size_t line )
        { HighlightLine(line, !IsHighlighted(line)); RefreshLine(line); }

    // refresh one or several lines at once
    void RefreshLine( size_t line );
    void RefreshLines( size_t lineFrom, size_t lineTo );

    // return true if the line is highlighted
    bool IsHighlighted(size_t line) const;

    // the methods which are forwarded to wxListLineData itself in list/icon
    // modes but are here because the lines don't store their positions in the
    // report mode

    // get the bound rect for the entire line
    wxRect GetLineRect(size_t line) const;

    // get the bound rect of the label
    wxRect GetLineLabelRect(size_t line) const;

    // get the bound rect of the items icon (only may be called if we do have
    // an icon!)
    wxRect GetLineIconRect(size_t line) const;

    // get the rect to be highlighted when the item has focus
    wxRect GetLineHighlightRect(size_t line) const;

    // get the size of the total line rect
    wxSize GetLineSize(size_t line) const
        { return GetLineRect(line).GetSize(); }

    // return the hit code for the corresponding position (in this line)
    long HitTestLine(size_t line, int x, int y) const;

    void EditLabel( long item );
    void OnRenameTimer();
    void OnRenameAccept();

    void OnMouse( wxMouseEvent &event );
    void MoveToFocus();

    // called to switch the selection from the current item to newCurrent,
    void OnArrowChar( size_t newCurrent, const wxKeyEvent& event );

    void OnChar( wxKeyEvent &event );
    void OnKeyDown( wxKeyEvent &event );
    void OnSetFocus( wxFocusEvent &event );
    void OnKillFocus( wxFocusEvent &event );
    void OnScroll(wxScrollWinEvent& event) ;

    void OnPaint( wxPaintEvent &event );

    void DrawImage( int index, wxDC *dc, int x, int y );
    void GetImageSize( int index, int &width, int &height ) const;
    int GetTextLength( const wxString &s ) const;

    void SetImageList( wxImageList *imageList, int which );
    void SetItemSpacing( int spacing, bool isSmall = FALSE );
    int GetItemSpacing( bool isSmall = FALSE );

    void SetColumn( int col, wxListItem &item );
    void SetColumnWidth( int col, int width );
    void GetColumn( int col, wxListItem &item ) const;
    int GetColumnWidth( int col ) const;
    int GetColumnCount() const { return m_columns.GetCount(); }

    // returns the sum of the heights of all columns
    int GetHeaderWidth() const;

    int GetCountPerPage() const;

    void SetItem( wxListItem &item );
    void GetItem( wxListItem &item );
    void SetItemState( long item, long state, long stateMask );
    int GetItemState( long item, long stateMask );
    void GetItemRect( long index, wxRect &rect );
    bool GetItemPosition( long item, wxPoint& pos );
    int GetSelectedItemCount();

    // set the scrollbars and update the positions of the items
    void RecalculatePositions();

    // refresh the window and the header
    void RefreshAll();

    long GetNextItem( long item, int geometry, int state );
    void DeleteItem( long index );
    void DeleteAllItems();
    void DeleteColumn( int col );
    void DeleteEverything();
    void EnsureVisible( long index );
    long FindItem( long start, const wxString& str, bool partial = FALSE );
    long FindItem( long start, long data);
    long HitTest( int x, int y, int &flags );
    void InsertItem( wxListItem &item );
    void InsertColumn( long col, wxListItem &item );
    void SortItems( wxListCtrlCompare fn, long data );

    size_t GetItemCount() const;
    bool IsEmpty() const { return GetItemCount() == 0; }
    void SetItemCount(long count);

    void ResetCurrent() { m_current = (size_t)-1; }
    bool HasCurrent() const { return m_current != (size_t)-1; }

    // send out a wxListEvent
    void SendNotify( size_t line,
                     wxEventType command,
                     wxPoint point = wxDefaultPosition );

    // override base class virtual to reset m_lineHeight when the font changes
    virtual bool SetFont(const wxFont& font)
    {
        if ( !wxScrolledWindow::SetFont(font) )
            return FALSE;

        m_lineHeight = 0;

        return TRUE;
    }

    // these are for wxListLineData usage only

    // get the backpointer to the list ctrl
    wxListCtrl *GetListCtrl() const
    {
        return wxStaticCast(GetParent(), wxListCtrl);
    }

    // get the height of all lines (assuming they all do have the same height)
    wxCoord GetLineHeight() const;

    // get the y position of the given line (only for report view)
    wxCoord GetLineY(size_t line) const;

//protected:
    // the array of all line objects for a non virtual list control
    wxListLineDataArray  m_lines;

    // the list of column objects
    wxListHeaderDataList m_columns;

    // currently focused item or -1
    size_t               m_current;

    // the item currently being edited or -1
    size_t               m_currentEdit;

    // the number of lines per page
    int                  m_linesPerPage;

    // this flag is set when something which should result in the window
    // redrawing happens (i.e. an item was added or deleted, or its appearance
    // changed) and OnPaint() doesn't redraw the window while it is set which
    // allows to minimize the number of repaintings when a lot of items are
    // being added. The real repainting occurs only after the next OnIdle()
    // call
    bool                 m_dirty;

    wxBrush             *m_highlightBrush;
    wxColour            *m_highlightColour;
    int                  m_xScroll,
                         m_yScroll;
    wxImageList         *m_small_image_list;
    wxImageList         *m_normal_image_list;
    int                  m_small_spacing;
    int                  m_normal_spacing;
    bool                 m_hasFocus;

    bool                 m_lastOnSame;
    wxTimer             *m_renameTimer;
    bool                 m_renameAccept;
    wxString             m_renameRes;
    bool                 m_isCreated;
    int                  m_dragCount;
    wxPoint              m_dragStart;

    // for double click logic
    size_t m_lineLastClicked,
           m_lineBeforeLastClicked;

protected:
    // the total count of items in a virtual list control
    size_t m_countVirt;

    // the object maintaining the items selection state, only used in virtual
    // controls
    wxSelectionStore m_selStore;

    // common part of all ctors
    void Init();

    // intiialize m_[xy]Scroll
    void InitScrolling();

    // get the line data for the given index
    wxListLineData *GetLine(size_t n) const
    {
        wxASSERT_MSG( n != (size_t)-1, _T("invalid line index") );

        if ( IsVirtual() )
        {
            wxConstCast(this, wxListMainWindow)->CacheLineData(n);

            n = 0;
        }

        return &m_lines[n];
    }

    // get a dummy line which can be used for geometry calculations and such:
    // you must use GetLine() if you want to really draw the line
    wxListLineData *GetDummyLine() const;

    // cache the line data of the n-th line in m_lines[0]
    void CacheLineData(size_t line);

    // get the range of visible lines
    void GetVisibleLinesRange(size_t *from, size_t *to);

    // force us to recalculate the range of visible lines
    void ResetVisibleLinesRange() { m_lineFrom = (size_t)-1; }

    // get the colour to be used for drawing the rules
    wxColour GetRuleColour() const
    {
#ifdef __WXMAC__
        return *wxWHITE;
#else
        return wxSystemSettings::GetSystemColour(wxSYS_COLOUR_3DLIGHT);
#endif
    }

private:
    // initialize the current item if needed
    void UpdateCurrent();

    // called when an item is [un]focuded, i.e. becomes [not] current
    //
    // currently unused
    void OnFocusLine( size_t line );
    void OnUnfocusLine( size_t line );

    // the height of one line using the current font
    wxCoord m_lineHeight;

    // the total header width or 0 if not calculated yet
    wxCoord m_headerWidth;

    // the first and last lines being shown on screen right now (inclusive),
    // both may be -1 if they must be calculated so never access them directly:
    // use GetVisibleLinesRange() above instead
    size_t m_lineFrom,
           m_lineTo;

    DECLARE_DYNAMIC_CLASS(wxListMainWindow);
    DECLARE_EVENT_TABLE()
};

// ============================================================================
// implementation
// ============================================================================

// ----------------------------------------------------------------------------
// wxSelectionStore
// ----------------------------------------------------------------------------

bool wxSelectionStore::IsSelected(size_t item) const
{
    bool isSel = m_itemsSel.Index(item) != wxNOT_FOUND;

    // if the default state is to be selected, being in m_itemsSel means that
    // the item is not selected, so we have to inverse the logic
    return m_defaultState ? !isSel : isSel;
}

bool wxSelectionStore::SelectItem(size_t item, bool select)
{
    // search for the item ourselves as like this we get the index where to
    // insert it later if needed, so we do only one search in the array instead
    // of two (adding item to a sorted array requires a search)
    size_t index = m_itemsSel.IndexForInsert(item);
    bool isSel = index < m_itemsSel.GetCount() && m_itemsSel[index] == item;

    if ( select != m_defaultState )
    {
        if ( !isSel )
        {
            m_itemsSel.AddAt(item, index);

            return TRUE;
        }
    }
    else // reset to default state
    {
        if ( isSel )
        {
            m_itemsSel.RemoveAt(index);
            return TRUE;
        }
    }

    return FALSE;
}

void wxSelectionStore::SelectRange(size_t itemFrom, size_t itemTo, bool select)
{
    wxASSERT_MSG( itemFrom <= itemTo, _T("should be in order") );

    // are we going to have more [un]selected items than the other ones?
    if ( itemTo - itemFrom > m_count / 2 )
    {
        if ( select != m_defaultState )
        {
            // the default state now becomes the same as 'select'
            m_defaultState = select;

            // so all the old selections (which had state select) shouldn't be
            // selected any more, but all the other ones should
            wxIndexArray selOld = m_itemsSel;
            m_itemsSel.Empty();

            // TODO: it should be possible to optimize the searches a bit
            //       knowing the possible range

            size_t item;
            for ( item = 0; item < itemFrom; item++ )
            {
                if ( selOld.Index(item) == wxNOT_FOUND )
                    m_itemsSel.Add(item);
            }

            for ( item = itemTo + 1; item < m_count; item++ )
            {
                if ( selOld.Index(item) == wxNOT_FOUND )
                    m_itemsSel.Add(item);
            }
        }
        else // select == m_defaultState
        {
            // get the inclusive range of items between itemFrom and itemTo
            size_t count = m_itemsSel.GetCount(),
                   start = m_itemsSel.IndexForInsert(itemFrom),
                   end = m_itemsSel.IndexForInsert(itemTo);

            if ( start == count || m_itemsSel[start] < itemFrom )
            {
                start++;
            }

            if ( end == count || m_itemsSel[end] > itemTo )
            {
                end--;
            }

            if ( start <= end )
            {
                // delete all of them (from end to avoid changing indices)
                for ( int i = end; i >= (int)start; i-- )
                {
                    m_itemsSel.RemoveAt(i);
                }
            }
        }
    }
    else // "few" items change state
    {
        // just add the items to the selection
        for ( size_t item = itemFrom; item <= itemTo; item++ )
        {
            SelectItem(item, select);
        }
    }
}

void wxSelectionStore::OnItemDelete(size_t item)
{
    size_t count = m_itemsSel.GetCount(),
           i = m_itemsSel.IndexForInsert(item);

    if ( i < count && m_itemsSel[i] == item )
    {
        // this item itself was in m_itemsSel, remove it from there
        m_itemsSel.RemoveAt(i);

        count--;
    }

    // and adjust the index of all which follow it
    while ( i < count )
    {
        // all following elements must be greater than the one we deleted
        wxASSERT_MSG( m_itemsSel[i] > item, _T("logic error") );

        m_itemsSel[i++]--;
    }
}

//-----------------------------------------------------------------------------
//  wxListItemData
//-----------------------------------------------------------------------------

void wxListItemData::Init()
{
    m_image = -1;
    m_data = 0;

    m_attr = NULL;
}

wxListItemData::wxListItemData(wxListMainWindow *owner)
{
    Init();

    m_owner = owner;

    if ( owner->HasFlag(wxLC_REPORT) )
    {
        m_rect = NULL;
    }
    else
    {
        m_rect = new wxRect;
    }
}

void wxListItemData::SetItem( const wxListItem &info )
{
    if ( info.m_mask & wxLIST_MASK_TEXT )
        SetText(info.m_text);
    if ( info.m_mask & wxLIST_MASK_IMAGE )
        m_image = info.m_image;
    if ( info.m_mask & wxLIST_MASK_DATA )
        m_data = info.m_data;

    if ( info.HasAttributes() )
    {
        if ( m_attr )
            *m_attr = *info.GetAttributes();
        else
            m_attr = new wxListItemAttr(*info.GetAttributes());
    }

    if ( m_rect )
    {
        m_rect->x =
        m_rect->y =
        m_rect->height = 0;
        m_rect->width = info.m_width;
    }
}

void wxListItemData::SetPosition( int x, int y )
{
    wxCHECK_RET( m_rect, _T("unexpected SetPosition() call") );

    m_rect->x = x;
    m_rect->y = y;
}

void wxListItemData::SetSize( int width, int height )
{
    wxCHECK_RET( m_rect, _T("unexpected SetSize() call") );

    if ( width != -1 )
        m_rect->width = width;
    if ( height != -1 )
        m_rect->height = height;
}

bool wxListItemData::IsHit( int x, int y ) const
{
    wxCHECK_MSG( m_rect, FALSE, _T("can't be called in this mode") );

    return wxRect(GetX(), GetY(), GetWidth(), GetHeight()).Inside(x, y);
}

int wxListItemData::GetX() const
{
    wxCHECK_MSG( m_rect, 0, _T("can't be called in this mode") );

    return m_rect->x;
}

int wxListItemData::GetY() const
{
    wxCHECK_MSG( m_rect, 0, _T("can't be called in this mode") );

    return m_rect->y;
}

int wxListItemData::GetWidth() const
{
    wxCHECK_MSG( m_rect, 0, _T("can't be called in this mode") );

    return m_rect->width;
}

int wxListItemData::GetHeight() const
{
    wxCHECK_MSG( m_rect, 0, _T("can't be called in this mode") );

    return m_rect->height;
}

void wxListItemData::GetItem( wxListItem &info ) const
{
    info.m_text = m_text;
    info.m_image = m_image;
    info.m_data = m_data;

    if ( m_attr )
    {
        if ( m_attr->HasTextColour() )
            info.SetTextColour(m_attr->GetTextColour());
        if ( m_attr->HasBackgroundColour() )
            info.SetBackgroundColour(m_attr->GetBackgroundColour());
        if ( m_attr->HasFont() )
            info.SetFont(m_attr->GetFont());
    }
}

//-----------------------------------------------------------------------------
//  wxListHeaderData
//-----------------------------------------------------------------------------

IMPLEMENT_DYNAMIC_CLASS(wxListHeaderData,wxObject);

wxListHeaderData::wxListHeaderData()
{
    m_mask = 0;
    m_image = 0;
    m_format = 0;
    m_width = 0;
    m_xpos = 0;
    m_ypos = 0;
    m_height = 0;
}

wxListHeaderData::wxListHeaderData( const wxListItem &item )
{
    SetItem( item );
    m_xpos = 0;
    m_ypos = 0;
    m_height = 0;
}

void wxListHeaderData::SetItem( const wxListItem &item )
{
    m_mask = item.m_mask;
    m_text = item.m_text;
    m_image = item.m_image;
    m_format = item.m_format;

    SetWidth(item.m_width);
}

void wxListHeaderData::SetPosition( int x, int y )
{
    m_xpos = x;
    m_ypos = y;
}

void wxListHeaderData::SetHeight( int h )
{
    m_height = h;
}

void wxListHeaderData::SetWidth( int w )
{
    m_width = w;
    if (m_width < 0)
        m_width = WIDTH_COL_DEFAULT;
    if (m_width < WIDTH_COL_MIN)
        m_width = WIDTH_COL_MIN;
}

void wxListHeaderData::SetFormat( int format )
{
    m_format = format;
}

bool wxListHeaderData::HasImage() const
{
    return (m_image != 0);
}

bool wxListHeaderData::IsHit( int x, int y ) const
{
    return ((x >= m_xpos) && (x <= m_xpos+m_width) && (y >= m_ypos) && (y <= m_ypos+m_height));
}

void wxListHeaderData::GetItem( wxListItem &item )
{
    item.m_mask = m_mask;
    item.m_text = m_text;
    item.m_image = m_image;
    item.m_format = m_format;
    item.m_width = m_width;
}

int wxListHeaderData::GetImage() const
{
    return m_image;
}

int wxListHeaderData::GetWidth() const
{
    return m_width;
}

int wxListHeaderData::GetFormat() const
{
    return m_format;
}

//-----------------------------------------------------------------------------
//  wxListLineData
//-----------------------------------------------------------------------------

inline int wxListLineData::GetMode() const
{
    return m_owner->GetListCtrl()->GetWindowStyleFlag() & wxLC_MASK_TYPE;
}

inline bool wxListLineData::InReportView() const
{
    return m_owner->HasFlag(wxLC_REPORT);
}

inline bool wxListLineData::IsVirtual() const
{
    return m_owner->IsVirtual();
}

wxListLineData::wxListLineData( wxListMainWindow *owner )
{
    m_owner = owner;
    m_items.DeleteContents( TRUE );

    if ( InReportView() )
    {
        m_gi = NULL;
    }
    else // !report
    {
        m_gi = new GeometryInfo;
    }

    m_highlighted = FALSE;

    InitItems( GetMode() == wxLC_REPORT ? m_owner->GetColumnCount() : 1 );
}

void wxListLineData::CalculateSize( wxDC *dc, int spacing )
{
    wxListItemDataList::Node *node = m_items.GetFirst();
    wxCHECK_RET( node, _T("no subitems at all??") );

    wxListItemData *item = node->GetData();

    switch ( GetMode() )
    {
        case wxLC_ICON:
        case wxLC_SMALL_ICON:
            {
                m_gi->m_rectAll.width = spacing;

                wxString s = item->GetText();

                wxCoord lw, lh;
                if ( s.empty() )
                {
                    lh =
                    m_gi->m_rectLabel.width =
                    m_gi->m_rectLabel.height = 0;
                }
                else // has label
                {
                    dc->GetTextExtent( s, &lw, &lh );
                    if (lh < SCROLL_UNIT_Y)
                        lh = SCROLL_UNIT_Y;
                    lw += EXTRA_WIDTH;
                    lh += EXTRA_HEIGHT;

                    m_gi->m_rectAll.height = spacing + lh;
                    if (lw > spacing)
                        m_gi->m_rectAll.width = lw;

                    m_gi->m_rectLabel.width = lw;
                    m_gi->m_rectLabel.height = lh;
                }

                if (item->HasImage())
                {
                    int w, h;
                    m_owner->GetImageSize( item->GetImage(), w, h );
                    m_gi->m_rectIcon.width = w + 8;
                    m_gi->m_rectIcon.height = h + 8;

                    if ( m_gi->m_rectIcon.width > m_gi->m_rectAll.width )
                        m_gi->m_rectAll.width = m_gi->m_rectIcon.width;
                    if ( m_gi->m_rectIcon.height + lh > m_gi->m_rectAll.height - 4 )
                        m_gi->m_rectAll.height = m_gi->m_rectIcon.height + lh + 4;
                }

                if ( item->HasText() )
                {
                    m_gi->m_rectHighlight.width = m_gi->m_rectLabel.width;
                    m_gi->m_rectHighlight.height = m_gi->m_rectLabel.height;
                }
                else // no text, highlight the icon
                {
                    m_gi->m_rectHighlight.width = m_gi->m_rectIcon.width;
                    m_gi->m_rectHighlight.height = m_gi->m_rectIcon.height;
                }
            }
            break;

        case wxLC_LIST:
            {
                wxString s = item->GetTextForMeasuring();

                wxCoord lw,lh;
                dc->GetTextExtent( s, &lw, &lh );
                if (lh < SCROLL_UNIT_Y)
                    lh = SCROLL_UNIT_Y;
                lw += EXTRA_WIDTH;
                lh += EXTRA_HEIGHT;

                m_gi->m_rectLabel.width = lw;
                m_gi->m_rectLabel.height = lh;

                m_gi->m_rectAll.width = lw;
                m_gi->m_rectAll.height = lh;

                if (item->HasImage())
                {
                    int w, h;
                    m_owner->GetImageSize( item->GetImage(), w, h );
                    m_gi->m_rectIcon.width = w;
                    m_gi->m_rectIcon.height = h;

                    m_gi->m_rectAll.width += 4 + w;
                    if (h > m_gi->m_rectAll.height)
                        m_gi->m_rectAll.height = h;
                }

                m_gi->m_rectHighlight.width = m_gi->m_rectAll.width;
                m_gi->m_rectHighlight.height = m_gi->m_rectAll.height;
            }
            break;

        case wxLC_REPORT:
            wxFAIL_MSG( _T("unexpected call to SetSize") );
            break;

        default:
            wxFAIL_MSG( _T("unknown mode") );
    }
}

void wxListLineData::SetPosition( int x, int y,
                                  int window_width,
                                  int spacing )
{
    wxListItemDataList::Node *node = m_items.GetFirst();
    wxCHECK_RET( node, _T("no subitems at all??") );

    wxListItemData *item = node->GetData();

    switch ( GetMode() )
    {
        case wxLC_ICON:
        case wxLC_SMALL_ICON:
            m_gi->m_rectAll.x = x;
            m_gi->m_rectAll.y = y;

            if ( item->HasImage() )
            {
                m_gi->m_rectIcon.x = m_gi->m_rectAll.x + 4
                                    + (spacing - m_gi->m_rectIcon.width)/2;
                m_gi->m_rectIcon.y = m_gi->m_rectAll.y + 4;
            }

            if ( item->HasText() )
            {
                if (m_gi->m_rectAll.width > spacing)
                    m_gi->m_rectLabel.x = m_gi->m_rectAll.x + 2;
                else
                    m_gi->m_rectLabel.x = m_gi->m_rectAll.x + 2 + (spacing/2) - (m_gi->m_rectLabel.width/2);
                m_gi->m_rectLabel.y = m_gi->m_rectAll.y + m_gi->m_rectAll.height + 2 - m_gi->m_rectLabel.height;
                m_gi->m_rectHighlight.x = m_gi->m_rectLabel.x - 2;
                m_gi->m_rectHighlight.y = m_gi->m_rectLabel.y - 2;
            }
            else // no text, highlight the icon
            {
                m_gi->m_rectHighlight.x = m_gi->m_rectIcon.x - 4;
                m_gi->m_rectHighlight.y = m_gi->m_rectIcon.y - 4;
            }
            break;

        case wxLC_LIST:
            m_gi->m_rectAll.x = x;
            m_gi->m_rectAll.y = y;

            m_gi->m_rectHighlight.x = m_gi->m_rectAll.x;
            m_gi->m_rectHighlight.y = m_gi->m_rectAll.y;
            m_gi->m_rectLabel.y = m_gi->m_rectAll.y + 2;

            if (item->HasImage())
            {
                m_gi->m_rectIcon.x = m_gi->m_rectAll.x + 2;
                m_gi->m_rectIcon.y = m_gi->m_rectAll.y + 2;
                m_gi->m_rectLabel.x = m_gi->m_rectAll.x + 6 + m_gi->m_rectIcon.width;
            }
            else
            {
                m_gi->m_rectLabel.x = m_gi->m_rectAll.x + 2;
            }
            break;

        case wxLC_REPORT:
            wxFAIL_MSG( _T("unexpected call to SetPosition") );
            break;

        default:
            wxFAIL_MSG( _T("unknown mode") );
    }
}

void wxListLineData::InitItems( int num )
{
    for (int i = 0; i < num; i++)
        m_items.Append( new wxListItemData(m_owner) );
}

void wxListLineData::SetItem( int index, const wxListItem &info )
{
    wxListItemDataList::Node *node = m_items.Item( index );
    wxCHECK_RET( node, _T("invalid column index in SetItem") );

    wxListItemData *item = node->GetData();
    item->SetItem( info );
}

void wxListLineData::GetItem( int index, wxListItem &info )
{
    wxListItemDataList::Node *node = m_items.Item( index );
    if (node)
    {
        wxListItemData *item = node->GetData();
        item->GetItem( info );
    }
}

wxString wxListLineData::GetText(int index) const
{
    wxString s;

    wxListItemDataList::Node *node = m_items.Item( index );
    if (node)
    {
        wxListItemData *item = node->GetData();
        s = item->GetText();
    }

    return s;
}

void wxListLineData::SetText( int index, const wxString s )
{
    wxListItemDataList::Node *node = m_items.Item( index );
    if (node)
    {
        wxListItemData *item = node->GetData();
        item->SetText( s );
    }
}

void wxListLineData::SetImage( int index, int image )
{
    wxListItemDataList::Node *node = m_items.Item( index );
    wxCHECK_RET( node, _T("invalid column index in SetImage()") );

    wxListItemData *item = node->GetData();
    item->SetImage(image);
}

int wxListLineData::GetImage( int index ) const
{
    wxListItemDataList::Node *node = m_items.Item( index );
    wxCHECK_MSG( node, -1, _T("invalid column index in GetImage()") );

    wxListItemData *item = node->GetData();
    return item->GetImage();
}

void wxListLineData::SetAttributes(wxDC *dc,
                                   const wxListItemAttr *attr,
                                   const wxColour& colText,
                                   const wxFont& font,
                                   bool highlight)
{
    // don't use foregroud colour for drawing highlighted items - this might
    // make them completely invisible (and there is no way to do bit
    // arithmetics on wxColour, unfortunately)
    if ( !highlight && attr && attr->HasTextColour() )
    {
        dc->SetTextForeground(attr->GetTextColour());
    }
    else
    {
        dc->SetTextForeground(colText);
    }

    if ( attr && attr->HasFont() )
    {
        dc->SetFont(attr->GetFont());
    }
    else
    {
        dc->SetFont(font);
    }
}

void wxListLineData::Draw( wxDC *dc )
{
    wxListItemDataList::Node *node = m_items.GetFirst();
    wxCHECK_RET( node, _T("no subitems at all??") );

    wxListItemData *item = node->GetData();
    if (item->HasImage())
    {
        wxRect rectIcon = m_gi->m_rectIcon;
        m_owner->DrawImage( item->GetImage(), dc,
                            rectIcon.x, rectIcon.y );
    }

    if (item->HasText())
    {
        wxRect rectLabel = m_gi->m_rectLabel;
        dc->DrawText( item->GetText(), rectLabel.x, rectLabel.y );
    }
}

void wxListLineData::DrawInReportMode( wxDC *dc,
                                       const wxRect& rect,
                                       const wxRect& rectHL,
                                       bool highlighted )
{
    // use our own flag if we maintain it
    if ( !IsVirtual() )
        highlighted = m_highlighted;

    // default foreground colour
    wxWindow *listctrl = m_owner->GetParent();
    wxColour colText;
    if ( highlighted )
    {
        colText = wxSystemSettings::GetSystemColour( wxSYS_COLOUR_HIGHLIGHTTEXT );
    }
    else
    {
        colText = listctrl->GetForegroundColour();
    }

    // default font
    wxFont font = listctrl->GetFont();

    // VZ: currently we set the colours/fonts only once, but like this (i.e.
    //     using SetAttributes() inside the loop), it will be trivial to
    //     customize the subitems (in report mode) too.
    wxListItemData *item = m_items.GetFirst()->GetData();
    wxListItemAttr *attr = item->GetAttributes();
    SetAttributes(dc, attr, colText, font, highlighted);

    bool hasBgCol = attr && attr->HasBackgroundColour();
    if ( highlighted || hasBgCol )
    {
        if ( highlighted )
        {
            dc->SetBrush( *m_owner->m_highlightBrush );
        }
        else
        {
            if ( hasBgCol )
                dc->SetBrush(wxBrush(attr->GetBackgroundColour(), wxSOLID));
            else
                dc->SetBrush( * wxWHITE_BRUSH );
        }

        dc->SetPen( * wxTRANSPARENT_PEN );
        dc->DrawRectangle( rectHL );
    }

    wxListItemDataList::Node *node = m_items.GetFirst();
    wxCHECK_RET( node, _T("no subitems at all??") );

    size_t col = 0;
    wxCoord x = rect.x + HEADER_OFFSET_X,
            y = rect.y + (LINE_SPACING + EXTRA_HEIGHT) / 2;

    while ( node )
    {
        wxListItemData *item = node->GetData();

        int xOld = x;

        if ( item->HasImage() )
        {
            int ix, iy;
            m_owner->DrawImage( item->GetImage(), dc, x, y );
            m_owner->GetImageSize( item->GetImage(), ix, iy );
            x += ix + 5; // FIXME: what is "5"?
        }

        int width = m_owner->GetColumnWidth(col++);

        wxDCClipper clipper(*dc, x, y, width, rect.height);

        if ( item->HasText() )
        {
            dc->DrawText( item->GetText(), x, y );
        }

        x = xOld + width;

        node = node->GetNext();
    }
}

bool wxListLineData::Highlight( bool on )
{
    wxCHECK_MSG( !m_owner->IsVirtual(), FALSE, _T("unexpected call to Highlight") );

    if ( on == m_highlighted )
        return FALSE;

    m_highlighted = on;

    return TRUE;
}

void wxListLineData::ReverseHighlight( void )
{
    Highlight(!IsHighlighted());
}

//-----------------------------------------------------------------------------
//  wxListHeaderWindow
//-----------------------------------------------------------------------------

IMPLEMENT_DYNAMIC_CLASS(wxListHeaderWindow,wxWindow);

BEGIN_EVENT_TABLE(wxListHeaderWindow,wxWindow)
    EVT_PAINT         (wxListHeaderWindow::OnPaint)
    EVT_MOUSE_EVENTS  (wxListHeaderWindow::OnMouse)
    EVT_SET_FOCUS     (wxListHeaderWindow::OnSetFocus)
END_EVENT_TABLE()

wxListHeaderWindow::wxListHeaderWindow( void )
{
    m_owner = (wxListMainWindow *) NULL;
    m_currentCursor = (wxCursor *) NULL;
    m_resizeCursor = (wxCursor *) NULL;
    m_isDragging = FALSE;
}

wxListHeaderWindow::wxListHeaderWindow( wxWindow *win, wxWindowID id, wxListMainWindow *owner,
      const wxPoint &pos, const wxSize &size,
      long style, const wxString &name ) :
  wxWindow( win, id, pos, size, style, name )
{
    m_owner = owner;
//  m_currentCursor = wxSTANDARD_CURSOR;
    m_currentCursor = (wxCursor *) NULL;
    m_resizeCursor = new wxCursor( wxCURSOR_SIZEWE );
    m_isDragging = FALSE;
    m_dirty = FALSE;

    SetBackgroundColour( wxSystemSettings::GetSystemColour( wxSYS_COLOUR_BTNFACE ) );
}

wxListHeaderWindow::~wxListHeaderWindow( void )
{
    delete m_resizeCursor;
}

void wxListHeaderWindow::DoDrawRect( wxDC *dc, int x, int y, int w, int h )
{
#ifdef __WXGTK__
    GtkStateType state = m_parent->IsEnabled() ? GTK_STATE_NORMAL
                                               : GTK_STATE_INSENSITIVE;

    x = dc->XLOG2DEV( x );

    gtk_paint_box (m_wxwindow->style, GTK_PIZZA(m_wxwindow)->bin_window,
                   state, GTK_SHADOW_OUT,
                   (GdkRectangle*) NULL, m_wxwindow, "button",
                   x-1, y-1, w+2, h+2);
#elif defined( __WXMAC__  )
    const int m_corner = 1;

    dc->SetBrush( *wxTRANSPARENT_BRUSH );

    dc->SetPen( wxPen( wxSystemSettings::GetSystemColour( wxSYS_COLOUR_BTNSHADOW ) , 1 , wxSOLID ) );
    dc->DrawLine( x+w-m_corner+1, y, x+w, y+h );  // right (outer)
    dc->DrawRectangle( x, y+h, w+1, 1 );          // bottom (outer)

    wxPen pen( wxColour( 0x88 , 0x88 , 0x88 ), 1, wxSOLID );

    dc->SetPen( pen );
    dc->DrawLine( x+w-m_corner, y, x+w-1, y+h );  // right (inner)
    dc->DrawRectangle( x+1, y+h-1, w-2, 1 );      // bottom (inner)

    dc->SetPen( *wxWHITE_PEN );
    dc->DrawRectangle( x, y, w-m_corner+1, 1 );   // top (outer)
    dc->DrawRectangle( x, y, 1, h );              // left (outer)
    dc->DrawLine( x, y+h-1, x+1, y+h-1 );
    dc->DrawLine( x+w-1, y, x+w-1, y+1 );
#else // !GTK, !Mac
    const int m_corner = 1;

    dc->SetBrush( *wxTRANSPARENT_BRUSH );

    dc->SetPen( *wxBLACK_PEN );
    dc->DrawLine( x+w-m_corner+1, y, x+w, y+h );  // right (outer)
    dc->DrawRectangle( x, y+h, w+1, 1 );          // bottom (outer)

    wxPen pen( wxSystemSettings::GetSystemColour( wxSYS_COLOUR_BTNSHADOW ), 1, wxSOLID );

    dc->SetPen( pen );
    dc->DrawLine( x+w-m_corner, y, x+w-1, y+h );  // right (inner)
    dc->DrawRectangle( x+1, y+h-1, w-2, 1 );      // bottom (inner)

    dc->SetPen( *wxWHITE_PEN );
    dc->DrawRectangle( x, y, w-m_corner+1, 1 );   // top (outer)
    dc->DrawRectangle( x, y, 1, h );              // left (outer)
    dc->DrawLine( x, y+h-1, x+1, y+h-1 );
    dc->DrawLine( x+w-1, y, x+w-1, y+1 );
#endif
}

// shift the DC origin to match the position of the main window horz
// scrollbar: this allows us to always use logical coords
void wxListHeaderWindow::AdjustDC(wxDC& dc)
{
    int xpix;
    m_owner->GetScrollPixelsPerUnit( &xpix, NULL );

    int x;
    m_owner->GetViewStart( &x, NULL );

    // account for the horz scrollbar offset
    dc.SetDeviceOrigin( -x * xpix, 0 );
}

void wxListHeaderWindow::OnPaint( wxPaintEvent &WXUNUSED(event) )
{
#ifdef __WXGTK__
    wxClientDC dc( this );
#else
    wxPaintDC dc( this );
#endif

    PrepareDC( dc );
    AdjustDC( dc );

    dc.BeginDrawing();

    dc.SetFont( GetFont() );

    // width and height of the entire header window
    int w, h;
    GetClientSize( &w, &h );
    m_owner->CalcUnscrolledPosition(w, 0, &w, NULL);

    dc.SetBackgroundMode(wxTRANSPARENT);

    // do *not* use the listctrl colour for headers - one day we will have a
    // function to set it separately
    //dc.SetTextForeground( *wxBLACK );
    dc.SetTextForeground(wxSystemSettings::
                            GetSystemColour( wxSYS_COLOUR_WINDOWTEXT ));

    int x = HEADER_OFFSET_X;

    int numColumns = m_owner->GetColumnCount();
    wxListItem item;
    for (int i = 0; i < numColumns; i++)
    {
        m_owner->GetColumn( i, item );
        int wCol = item.m_width;
        int cw = wCol - 2; // the width of the rect to draw

        int xEnd = x + wCol;

        dc.SetPen( *wxWHITE_PEN );

        DoDrawRect( &dc, x, HEADER_OFFSET_Y, cw, h-2 );
        dc.SetClippingRegion( x, HEADER_OFFSET_Y, cw-5, h-4 );
        dc.DrawText( item.GetText(), x + EXTRA_WIDTH, HEADER_OFFSET_Y + EXTRA_HEIGHT );
        dc.DestroyClippingRegion();
        x += wCol;

        if (xEnd > w+5)
            break;
    }
    dc.EndDrawing();
}

void wxListHeaderWindow::DrawCurrent()
{
    int x1 = m_currentX;
    int y1 = 0;
    ClientToScreen( &x1, &y1 );

    int x2 = m_currentX-1;
    int y2 = 0;
    m_owner->GetClientSize( NULL, &y2 );
    m_owner->ClientToScreen( &x2, &y2 );

    wxScreenDC dc;
    dc.SetLogicalFunction( wxINVERT );
    dc.SetPen( wxPen( *wxBLACK, 2, wxSOLID ) );
    dc.SetBrush( *wxTRANSPARENT_BRUSH );

    AdjustDC(dc);

    dc.DrawLine( x1, y1, x2, y2 );

    dc.SetLogicalFunction( wxCOPY );

    dc.SetPen( wxNullPen );
    dc.SetBrush( wxNullBrush );
}

void wxListHeaderWindow::OnMouse( wxMouseEvent &event )
{
    // we want to work with logical coords
    int x;
    m_owner->CalcUnscrolledPosition(event.GetX(), 0, &x, NULL);
    int y = event.GetY();

    if (m_isDragging)
    {
        // we don't draw the line beyond our window, but we allow dragging it
        // there
        int w = 0;
        GetClientSize( &w, NULL );
        m_owner->CalcUnscrolledPosition(w, 0, &w, NULL);
        w -= 6;

        // erase the line if it was drawn
        if ( m_currentX < w )
            DrawCurrent();

        if (event.ButtonUp())
        {
            ReleaseMouse();
            m_isDragging = FALSE;
            m_dirty = TRUE;
            m_owner->SetColumnWidth( m_column, m_currentX - m_minX );
        }
        else
        {
            if (x > m_minX + 7)
                m_currentX = x;
            else
                m_currentX = m_minX + 7;

            // draw in the new location
            if ( m_currentX < w )
                DrawCurrent();
        }
    }
    else // not dragging
    {
        m_minX = 0;
        bool hit_border = FALSE;

        // end of the current column
        int xpos = 0;

        // find the column where this event occured
        int countCol = m_owner->GetColumnCount();
        for (int col = 0; col < countCol; col++)
        {
            xpos += m_owner->GetColumnWidth( col );
            m_column = col;

            if ( (abs(x-xpos) < 3) && (y < 22) )
            {
                // near the column border
                hit_border = TRUE;
                break;
            }

            if ( x < xpos )
            {
                // inside the column
                break;
            }

            m_minX = xpos;
        }

        if (event.LeftDown())
        {
            if (hit_border)
            {
                m_isDragging = TRUE;
                m_currentX = x;
                DrawCurrent();
                CaptureMouse();
            }
            else
            {
                wxWindow *parent = GetParent();
                wxListEvent le( wxEVT_COMMAND_LIST_COL_CLICK, parent->GetId() );
                le.SetEventObject( parent );
                le.m_col = m_column;
                parent->GetEventHandler()->ProcessEvent( le );
            }
        }
        else if (event.Moving())
        {
            bool setCursor;
            if (hit_border)
            {
                setCursor = m_currentCursor == wxSTANDARD_CURSOR;
                m_currentCursor = m_resizeCursor;
            }
            else
            {
                setCursor = m_currentCursor != wxSTANDARD_CURSOR;
                m_currentCursor = wxSTANDARD_CURSOR;
            }

            if ( setCursor )
                SetCursor(*m_currentCursor);
        }
    }
}

void wxListHeaderWindow::OnSetFocus( wxFocusEvent &WXUNUSED(event) )
{
    m_owner->SetFocus();
}

//-----------------------------------------------------------------------------
// wxListRenameTimer (internal)
//-----------------------------------------------------------------------------

wxListRenameTimer::wxListRenameTimer( wxListMainWindow *owner )
{
    m_owner = owner;
}

void wxListRenameTimer::Notify()
{
    m_owner->OnRenameTimer();
}

//-----------------------------------------------------------------------------
// wxListTextCtrl (internal)
//-----------------------------------------------------------------------------

IMPLEMENT_DYNAMIC_CLASS(wxListTextCtrl,wxTextCtrl);

BEGIN_EVENT_TABLE(wxListTextCtrl,wxTextCtrl)
    EVT_CHAR           (wxListTextCtrl::OnChar)
    EVT_KEY_UP         (wxListTextCtrl::OnKeyUp)
    EVT_KILL_FOCUS     (wxListTextCtrl::OnKillFocus)
END_EVENT_TABLE()

wxListTextCtrl::wxListTextCtrl( wxWindow *parent,
                                const wxWindowID id,
                                bool *accept,
                                wxString *res,
                                wxListMainWindow *owner,
                                const wxString &value,
                                const wxPoint &pos,
                                const wxSize &size,
                                int style,
                                const wxValidator& validator,
                                const wxString &name )
              : wxTextCtrl( parent, id, value, pos, size, style, validator, name )
{
    m_res = res;
    m_accept = accept;
    m_owner = owner;
    (*m_accept) = FALSE;
    (*m_res) = "";
    m_startValue = value;
}

void wxListTextCtrl::OnChar( wxKeyEvent &event )
{
    if (event.m_keyCode == WXK_RETURN)
    {
        (*m_accept) = TRUE;
        (*m_res) = GetValue();

        if (!wxPendingDelete.Member(this))
            wxPendingDelete.Append(this);

        if ((*m_accept) && ((*m_res) != m_startValue))
            m_owner->OnRenameAccept();

        return;
    }
    if (event.m_keyCode == WXK_ESCAPE)
    {
        (*m_accept) = FALSE;
        (*m_res) = "";

        if (!wxPendingDelete.Member(this))
            wxPendingDelete.Append(this);

        return;
    }

    event.Skip();
}

void wxListTextCtrl::OnKeyUp( wxKeyEvent &event )
{
    // auto-grow the textctrl:
    wxSize parentSize = m_owner->GetSize();
    wxPoint myPos = GetPosition();
    wxSize mySize = GetSize();
    int sx, sy;
    GetTextExtent(GetValue() + _T("MM"), &sx, &sy); // FIXME: MM??
    if (myPos.x + sx > parentSize.x)
        sx = parentSize.x - myPos.x;
    if (mySize.x > sx)
        sx = mySize.x;
    SetSize(sx, -1);

    event.Skip();
}

void wxListTextCtrl::OnKillFocus( wxFocusEvent &WXUNUSED(event) )
{
    if (!wxPendingDelete.Member(this))
        wxPendingDelete.Append(this);

    if ((*m_accept) && ((*m_res) != m_startValue))
        m_owner->OnRenameAccept();
}

//-----------------------------------------------------------------------------
//  wxListMainWindow
//-----------------------------------------------------------------------------

IMPLEMENT_DYNAMIC_CLASS(wxListMainWindow,wxScrolledWindow);

BEGIN_EVENT_TABLE(wxListMainWindow,wxScrolledWindow)
  EVT_PAINT          (wxListMainWindow::OnPaint)
  EVT_MOUSE_EVENTS   (wxListMainWindow::OnMouse)
  EVT_CHAR           (wxListMainWindow::OnChar)
  EVT_KEY_DOWN       (wxListMainWindow::OnKeyDown)
  EVT_SET_FOCUS      (wxListMainWindow::OnSetFocus)
  EVT_KILL_FOCUS     (wxListMainWindow::OnKillFocus)
  EVT_SCROLLWIN      (wxListMainWindow::OnScroll)
END_EVENT_TABLE()

void wxListMainWindow::Init()
{
    m_columns.DeleteContents( TRUE );
    m_dirty = TRUE;
    m_countVirt = 0;
    m_lineFrom =
    m_lineTo = (size_t)-1;
    m_linesPerPage = 0;

    m_headerWidth =
    m_lineHeight = 0;

    m_small_image_list = (wxImageList *) NULL;
    m_normal_image_list = (wxImageList *) NULL;

    m_small_spacing = 30;
    m_normal_spacing = 40;

    m_hasFocus = FALSE;
    m_dragCount = 0;
    m_isCreated = FALSE;

    m_lastOnSame = FALSE;
    m_renameTimer = new wxListRenameTimer( this );
    m_renameAccept = FALSE;

    m_current =
    m_currentEdit =
    m_lineLastClicked =
    m_lineBeforeLastClicked = (size_t)-1;
}

void wxListMainWindow::InitScrolling()
{
    if ( HasFlag(wxLC_REPORT) )
    {
        m_xScroll = SCROLL_UNIT_X;
        m_yScroll = SCROLL_UNIT_Y;
    }
    else
    {
        m_xScroll = SCROLL_UNIT_Y;
        m_yScroll = 0;
    }
}

wxListMainWindow::wxListMainWindow()
{
    Init();

    m_highlightBrush = (wxBrush *) NULL;

    m_xScroll =
    m_yScroll = 0;
}

wxListMainWindow::wxListMainWindow( wxWindow *parent,
                                    wxWindowID id,
                                    const wxPoint& pos,
                                    const wxSize& size,
                                    long style,
                                    const wxString &name )
                : wxScrolledWindow( parent, id, pos, size,
                                    style | wxHSCROLL | wxVSCROLL, name )
{
    Init();

    m_highlightBrush = new wxBrush( wxSystemSettings::GetSystemColour(wxSYS_COLOUR_HIGHLIGHT), wxSOLID );
    wxSize sz = size;
    sz.y = 25;

    InitScrolling();
    SetScrollbars( m_xScroll, m_yScroll, 0, 0, 0, 0 );

    SetBackgroundColour( wxSystemSettings::GetSystemColour( wxSYS_COLOUR_LISTBOX ) );
}

wxListMainWindow::~wxListMainWindow()
{
    DeleteEverything();

    delete m_highlightBrush;

    delete m_renameTimer;
}

void wxListMainWindow::CacheLineData(size_t line)
{
    wxListCtrl *listctrl = GetListCtrl();

    wxListLineData *ld = GetDummyLine();

    size_t countCol = GetColumnCount();
    for ( size_t col = 0; col < countCol; col++ )
    {
        ld->SetText(col, listctrl->OnGetItemText(line, col));
    }

    ld->SetImage(listctrl->OnGetItemImage(line));
}

wxListLineData *wxListMainWindow::GetDummyLine() const
{
    wxASSERT_MSG( !IsEmpty(), _T("invalid line index") );

    if ( m_lines.IsEmpty() )
    {
        // normal controls are supposed to have something in m_lines
        // already if it's not empty
        wxASSERT_MSG( IsVirtual(), _T("logic error") );

        wxListMainWindow *self = wxConstCast(this, wxListMainWindow);
        wxListLineData *line = new wxListLineData(self);
        self->m_lines.Add(line);
    }

    return &m_lines[0];
}

// ----------------------------------------------------------------------------
// line geometry (report mode only)
// ----------------------------------------------------------------------------

wxCoord wxListMainWindow::GetLineHeight() const
{
    wxASSERT_MSG( HasFlag(wxLC_REPORT), _T("only works in report mode") );

    // we cache the line height as calling GetTextExtent() is slow
    if ( !m_lineHeight )
    {
        wxListMainWindow *self = wxConstCast(this, wxListMainWindow);

        wxClientDC dc( self );
        dc.SetFont( GetFont() );

        wxCoord y;
        dc.GetTextExtent(_T("H"), NULL, &y);

        if ( y < SCROLL_UNIT_Y )
            y = SCROLL_UNIT_Y;
        y += EXTRA_HEIGHT;

        self->m_lineHeight = y + LINE_SPACING;
    }

    return m_lineHeight;
}

wxCoord wxListMainWindow::GetLineY(size_t line) const
{
    wxASSERT_MSG( HasFlag(wxLC_REPORT), _T("only works in report mode") );

    return LINE_SPACING + line*GetLineHeight();
}

wxRect wxListMainWindow::GetLineRect(size_t line) const
{
    if ( !InReportView() )
        return GetLine(line)->m_gi->m_rectAll;

    wxRect rect;
    rect.x = HEADER_OFFSET_X;
    rect.y = GetLineY(line);
    rect.width = GetHeaderWidth();
    rect.height = GetLineHeight();

    return rect;
}

wxRect wxListMainWindow::GetLineLabelRect(size_t line) const
{
    if ( !InReportView() )
        return GetLine(line)->m_gi->m_rectLabel;

    wxRect rect;
    rect.x = HEADER_OFFSET_X;
    rect.y = GetLineY(line);
    rect.width = GetColumnWidth(0);
    rect.height = GetLineHeight();

    return rect;
}

wxRect wxListMainWindow::GetLineIconRect(size_t line) const
{
    if ( !InReportView() )
        return GetLine(line)->m_gi->m_rectIcon;

    wxListLineData *ld = GetLine(line);
    wxASSERT_MSG( ld->HasImage(), _T("should have an image") );

    wxRect rect;
    rect.x = HEADER_OFFSET_X;
    rect.y = GetLineY(line);
    GetImageSize(ld->GetImage(), rect.width, rect.height);

    return rect;
}

wxRect wxListMainWindow::GetLineHighlightRect(size_t line) const
{
    return InReportView() ? GetLineRect(line)
                          : GetLine(line)->m_gi->m_rectHighlight;
}

long wxListMainWindow::HitTestLine(size_t line, int x, int y) const
{
    wxListLineData *ld = GetLine(line);

    if ( ld->HasImage() && GetLineIconRect(line).Inside(x, y) )
        return wxLIST_HITTEST_ONITEMICON;

    if ( ld->HasText() )
    {
        wxRect rect = InReportView() ? GetLineRect(line)
                                     : GetLineLabelRect(line);

        if ( rect.Inside(x, y) )
            return wxLIST_HITTEST_ONITEMLABEL;
    }

    return 0;
}

// ----------------------------------------------------------------------------
// highlight (selection) handling
// ----------------------------------------------------------------------------

bool wxListMainWindow::IsHighlighted(size_t line) const
{
    if ( IsVirtual() )
    {
        return m_selStore.IsSelected(line);
    }
    else // !virtual
    {
        wxListLineData *ld = GetLine(line);
        wxCHECK_MSG( ld, FALSE, _T("invalid index in IsHighlighted") );

        return ld->IsHighlighted();
    }
}

void wxListMainWindow::HighlightLines( size_t lineFrom, size_t lineTo, bool highlight )
{
    if ( IsVirtual() )
    {
        m_selStore.SelectRange(lineFrom, lineTo, highlight);
        RefreshLines(lineFrom, lineTo);
    }
    else
    {
        // do it the dumb way
        bool needsRefresh = FALSE;
        for ( size_t line = lineFrom; line <= lineTo; line++ )
        {
            if ( HighlightLine(line, highlight) )
                needsRefresh = TRUE;
        }

        if ( needsRefresh )
            RefreshLines(lineFrom, lineTo);
    }
}

bool wxListMainWindow::HighlightLine( size_t line, bool highlight )
{
    bool changed;

    if ( IsVirtual() )
    {
        changed = m_selStore.SelectItem(line, highlight);
    }
    else // !virtual
    {
        wxListLineData *ld = GetLine(line);
        wxCHECK_MSG( ld, FALSE, _T("invalid index in IsHighlighted") );

        changed = ld->Highlight(highlight);
    }

    if ( changed )
    {
        SendNotify( line, highlight ? wxEVT_COMMAND_LIST_ITEM_SELECTED
                                    : wxEVT_COMMAND_LIST_ITEM_DESELECTED );
    }

    return changed;
}

void wxListMainWindow::RefreshLine( size_t line )
{
    wxRect rect = GetLineRect(line);

    CalcScrolledPosition( rect.x, rect.y, &rect.x, &rect.y );
    RefreshRect( rect );
}

void wxListMainWindow::RefreshLines( size_t lineFrom, size_t lineTo )
{
    // we suppose that they are ordered by caller
    wxASSERT_MSG( lineFrom <= lineTo, _T("indices in disorder") );

    if ( HasFlag(wxLC_REPORT) )
    {
        size_t visibleFrom, visibleTo;
        GetVisibleLinesRange(&visibleFrom, &visibleTo);

        if ( lineFrom < visibleFrom )
            lineFrom = visibleFrom;
        if ( lineTo > visibleTo )
            lineTo = visibleTo;

        wxRect rect;
        rect.x = 0;
        rect.y = GetLineY(lineFrom);
        rect.width = GetClientSize().x;
        rect.height = GetLineY(lineTo) - rect.y;

        CalcScrolledPosition( rect.x, rect.y, &rect.x, &rect.y );
        RefreshRect( rect );
    }
    else // !report
    {
        // TODO: this should be optimized...
        for ( size_t line = lineFrom; line <= lineTo; line++ )
        {
            RefreshLine(line);
        }
    }
}

void wxListMainWindow::OnPaint( wxPaintEvent &WXUNUSED(event) )
{
    // Note: a wxPaintDC must be constructed even if no drawing is
    // done (a Windows requirement).
    wxPaintDC dc( this );

    if ( IsEmpty() )
    {
        // empty control. nothing to draw
        return;
    }

    PrepareDC( dc );

    int dev_x, dev_y;
    CalcScrolledPosition( 0, 0, &dev_x, &dev_y );

    dc.BeginDrawing();

    dc.SetFont( GetFont() );

    if ( HasFlag(wxLC_REPORT) )
    {
        int lineHeight = GetLineHeight();

        size_t visibleFrom, visibleTo;
        GetVisibleLinesRange(&visibleFrom, &visibleTo);
        for ( size_t line = visibleFrom; line <= visibleTo; line++ )
        {
            GetLine(line)->DrawInReportMode( &dc,
                                             GetLineRect(line),
                                             GetLineHighlightRect(line),
                                             IsHighlighted(line) );
        }

        if ( HasFlag(wxLC_HRULES) )
        {
            wxPen pen(GetRuleColour(), 1, wxSOLID);
            wxSize clientSize = GetClientSize();

            for ( size_t i = visibleFrom; i <= visibleTo; i++ )
            {
                dc.SetPen(pen);
                dc.SetBrush( *wxTRANSPARENT_BRUSH );
                dc.DrawLine(0 - dev_x, i*lineHeight,
                            clientSize.x - dev_x, i*lineHeight);
            }

            // Draw last horizontal rule
            if ( visibleTo > visibleFrom )
            {
                dc.SetPen(pen);
                dc.SetBrush( *wxTRANSPARENT_BRUSH );
                dc.DrawLine(0 - dev_x, m_lineTo*lineHeight,
                            clientSize.x - dev_x , m_lineTo*lineHeight );
            }
        }

        // Draw vertical rules if required
        if ( HasFlag(wxLC_VRULES) && !IsEmpty() )
        {
            wxPen pen(GetRuleColour(), 1, wxSOLID);

            int col = 0;
            wxRect firstItemRect;
            wxRect lastItemRect;
            GetItemRect(0, firstItemRect);
            GetItemRect(GetItemCount() - 1, lastItemRect);
            int x = firstItemRect.GetX();
            dc.SetPen(pen);
            dc.SetBrush(* wxTRANSPARENT_BRUSH);
            for (col = 0; col < GetColumnCount(); col++)
            {
                int colWidth = GetColumnWidth(col);
                x += colWidth;
                dc.DrawLine(x - dev_x, firstItemRect.GetY() - 1 - dev_y,
                            x - dev_x, lastItemRect.GetBottom() + 1 - dev_y);
            }
        }
    }
    else // !report
    {
        size_t count = GetItemCount();
        for ( size_t i = 0; i < count; i++ )
        {
            GetLine(i)->Draw( &dc );
        }
    }

    if ( HasCurrent() && m_hasFocus )
    {
#ifdef __WXMAC__
        // no rect outline, we already have the background color
#else
        dc.SetPen( *wxBLACK_PEN );
        dc.SetBrush( *wxTRANSPARENT_BRUSH );
        dc.DrawRectangle( GetLineHighlightRect(m_current) );
#endif
    }

    dc.EndDrawing();
}

void wxListMainWindow::HighlightAll( bool on )
{
    if ( IsSingleSel() )
    {
        wxASSERT_MSG( !on, _T("can't do this in a single sel control") );

        // we just have one item to turn off
        if ( HasCurrent() && IsHighlighted(m_current) )
        {
            HighlightLine(m_current, FALSE);
            RefreshLine(m_current);
        }
    }
    else // multi sel
    {
        HighlightLines(0, GetItemCount() - 1, on);
    }
}

void wxListMainWindow::SendNotify( size_t line,
                                   wxEventType command,
                                   wxPoint point )
{
    wxListEvent le( command, GetParent()->GetId() );
    le.SetEventObject( GetParent() );
    le.m_itemIndex = line;

    // set only for events which have position
    if ( point != wxDefaultPosition )
        le.m_pointDrag = point;

    GetLine(line)->GetItem( 0, le.m_item );
    GetParent()->GetEventHandler()->ProcessEvent( le );
}

void wxListMainWindow::OnFocusLine( size_t WXUNUSED(line) )
{
//  SendNotify( line, wxEVT_COMMAND_LIST_ITEM_FOCUSSED );
}

void wxListMainWindow::OnUnfocusLine( size_t WXUNUSED(line) )
{
//  SendNotify( line, wxEVT_COMMAND_LIST_ITEM_UNFOCUSSED );
}

void wxListMainWindow::EditLabel( long item )
{
    wxCHECK_RET( (item >= 0) && ((size_t)item < GetItemCount()),
                 wxT("wrong index in wxListCtrl::EditLabel()") );

    m_currentEdit = (size_t)item;

    wxListEvent le( wxEVT_COMMAND_LIST_BEGIN_LABEL_EDIT, GetParent()->GetId() );
    le.SetEventObject( GetParent() );
    le.m_itemIndex = item;
    wxListLineData *data = GetLine(m_currentEdit);
    wxCHECK_RET( data, _T("invalid index in EditLabel()") );
    data->GetItem( 0, le.m_item );
    GetParent()->GetEventHandler()->ProcessEvent( le );

    if (!le.IsAllowed())
        return;

    // We have to call this here because the label in question might just have
    // been added and no screen update taken place.
    if (m_dirty)
        wxSafeYield();

    wxClientDC dc(this);
    PrepareDC( dc );

    wxString s = data->GetText(0);
    wxRect rectLabel = GetLineLabelRect(m_currentEdit);

    rectLabel.x = dc.LogicalToDeviceX( rectLabel.x );
    rectLabel.y = dc.LogicalToDeviceY( rectLabel.y );

    wxListTextCtrl *text = new wxListTextCtrl
                               (
                                this, -1,
                                &m_renameAccept,
                                &m_renameRes,
                                this,
                                s,
                                wxPoint(rectLabel.x-4,rectLabel.y-4),
                                wxSize(rectLabel.width+11,rectLabel.height+8)
                               );
    text->SetFocus();
}

void wxListMainWindow::OnRenameTimer()
{
    wxCHECK_RET( HasCurrent(), wxT("unexpected rename timer") );

    EditLabel( m_current );
}

void wxListMainWindow::OnRenameAccept()
{
    wxListEvent le( wxEVT_COMMAND_LIST_END_LABEL_EDIT, GetParent()->GetId() );
    le.SetEventObject( GetParent() );
    le.m_itemIndex = m_currentEdit;

    wxListLineData *data = GetLine(m_currentEdit);
    wxCHECK_RET( data, _T("invalid index in OnRenameAccept()") );

    data->GetItem( 0, le.m_item );
    le.m_item.m_text = m_renameRes;
    GetParent()->GetEventHandler()->ProcessEvent( le );

    if (!le.IsAllowed()) return;

    wxListItem info;
    info.m_mask = wxLIST_MASK_TEXT;
    info.m_itemId = le.m_itemIndex;
    info.m_text = m_renameRes;
    info.SetTextColour(le.m_item.GetTextColour());
    SetItem( info );
}

void wxListMainWindow::OnMouse( wxMouseEvent &event )
{
    event.SetEventObject( GetParent() );
    if ( GetParent()->GetEventHandler()->ProcessEvent( event) )
        return;

    if ( !HasCurrent() || IsEmpty() )
        return;

    if (m_dirty)
        return;

    if ( !(event.Dragging() || event.ButtonDown() || event.LeftUp() ||
        event.ButtonDClick()) )
        return;

    int x = event.GetX();
    int y = event.GetY();
    CalcUnscrolledPosition( x, y, &x, &y );

    /* Did we actually hit an item ? */
    long hitResult = 0;

    size_t count = GetItemCount(),
           current;

    if ( HasFlag(wxLC_REPORT) )
    {
        current = y / GetLineHeight();
        hitResult = HitTestLine(current, x, y);
    }
    else // !report
    {
        // TODO: optimize it too! this is less simple than for report view but
        //       enumerating all items is still not a way to do it!!
        for ( current = 0; current < count && !hitResult; current++ )
        {
            hitResult = HitTestLine(current, x, y);
        }
    }

    if (event.Dragging())
    {
        if (m_dragCount == 0)
            m_dragStart = wxPoint(x,y);

        m_dragCount++;

        if (m_dragCount != 3)
            return;

        int command = event.RightIsDown() ? wxEVT_COMMAND_LIST_BEGIN_RDRAG
                                          : wxEVT_COMMAND_LIST_BEGIN_DRAG;

        wxListEvent le( command, GetParent()->GetId() );
        le.SetEventObject( GetParent() );
        le.m_pointDrag = m_dragStart;
        GetParent()->GetEventHandler()->ProcessEvent( le );

        return;
    }
    else
    {
        m_dragCount = 0;
    }

    if ( !hitResult )
    {
        // outside of any item
        return;
    }

    bool forceClick = FALSE;
    if (event.ButtonDClick())
    {
        m_renameTimer->Stop();
        m_lastOnSame = FALSE;

        if ( current == m_lineBeforeLastClicked )
        {
            SendNotify( current, wxEVT_COMMAND_LIST_ITEM_ACTIVATED );

            return;
        }
        else
        {
            // the first click was on another item, so don't interpret this as
            // a double click, but as a simple click instead
            forceClick = TRUE;
        }
    }

    if (event.LeftUp() && m_lastOnSame)
    {
        if ((current == m_current) &&
            (hitResult == wxLIST_HITTEST_ONITEMLABEL) &&
            HasFlag(wxLC_EDIT_LABELS)  )
        {
            m_renameTimer->Start( 100, TRUE );
        }
        m_lastOnSame = FALSE;
    }
    else if (event.RightDown())
    {
        SendNotify( current, wxEVT_COMMAND_LIST_ITEM_RIGHT_CLICK,
                    event.GetPosition() );
    }
    else if (event.MiddleDown())
    {
        SendNotify( current, wxEVT_COMMAND_LIST_ITEM_MIDDLE_CLICK );
    }
    else if ( event.LeftDown() || forceClick )
    {
        m_lineBeforeLastClicked = m_lineLastClicked;
        m_lineLastClicked = current;

        size_t oldCurrent = m_current;

        if ( IsSingleSel() || !(event.ControlDown() || event.ShiftDown()) )
        {
            HighlightAll( FALSE );
            m_current = current;

            ReverseHighlight(m_current);
        }
        else // multi sel & either ctrl or shift is down
        {
            if (event.ControlDown())
            {
                m_current = current;

                ReverseHighlight(m_current);
            }
            else if (event.ShiftDown())
            {
                m_current = current;

                size_t lineFrom = oldCurrent,
                       lineTo = current;

                if ( lineTo < lineFrom )
                {
                    lineTo = lineFrom;
                    lineFrom = m_current;
                }

                HighlightLines(lineFrom, lineTo);
            }
            else // !ctrl, !shift
            {
                // test in the enclosing if should make it impossible
                wxFAIL_MSG( _T("how did we get here?") );
            }
        }

        if (m_current != oldCurrent)
        {
            RefreshLine( oldCurrent );
            OnUnfocusLine( oldCurrent );
            OnFocusLine( m_current );
        }

        // forceClick is only set if the previous click was on another item
        m_lastOnSame = !forceClick && (m_current == oldCurrent);
    }
}

void wxListMainWindow::MoveToFocus()
{
    if ( !HasCurrent() )
        return;

    wxRect rect = GetLineRect(m_current);

    int client_w, client_h;
    GetClientSize( &client_w, &client_h );

    int view_x = m_xScroll*GetScrollPos( wxHORIZONTAL );
    int view_y = m_yScroll*GetScrollPos( wxVERTICAL );

    if ( HasFlag(wxLC_REPORT) )
    {
        // the next we need the range of lines shown it might be different, so
        // recalculate it
        ResetVisibleLinesRange();

        if (rect.y < view_y )
            Scroll( -1, rect.y/m_yScroll );
        if (rect.y+rect.height+5 > view_y+client_h)
            Scroll( -1, (rect.y+rect.height-client_h+SCROLL_UNIT_Y)/m_yScroll );
    }
    else // !report
    {
        if (rect.x-view_x < 5)
            Scroll( (rect.x-5)/m_xScroll, -1 );
        if (rect.x+rect.width-5 > view_x+client_w)
            Scroll( (rect.x+rect.width-client_w+SCROLL_UNIT_X)/m_xScroll, -1 );
    }
}

// ----------------------------------------------------------------------------
// keyboard handling
// ----------------------------------------------------------------------------

void wxListMainWindow::OnArrowChar(size_t newCurrent, const wxKeyEvent& event)
{
    wxCHECK_RET( newCurrent < (size_t)GetItemCount(),
                 _T("invalid item index in OnArrowChar()") );

    size_t oldCurrent = m_current;

    // in single selection we just ignore Shift as we can't select several
    // items anyhow
    if ( event.ShiftDown() && !IsSingleSel() )
    {
        m_current = newCurrent;

        // select all the items between the old and the new one
        if ( oldCurrent > newCurrent )
        {
            newCurrent = oldCurrent;
            oldCurrent = m_current;
        }

        HighlightLines(oldCurrent, newCurrent);
    }
    else // !shift
    {
        // all previously selected items are unselected unless ctrl is held
        if ( !event.ControlDown() )
            HighlightAll(FALSE);

        m_current = newCurrent;

        HighlightLine( oldCurrent, FALSE );
        RefreshLine( oldCurrent );

        if ( !event.ControlDown() )
        {
            HighlightLine( m_current, TRUE );
        }
    }

    OnUnfocusLine( oldCurrent );
    OnFocusLine( m_current );
    RefreshLine( m_current );

    MoveToFocus();
}

void wxListMainWindow::OnKeyDown( wxKeyEvent &event )
{
    wxWindow *parent = GetParent();

    /* we propagate the key event up */
    wxKeyEvent ke( wxEVT_KEY_DOWN );
    ke.m_shiftDown = event.m_shiftDown;
    ke.m_controlDown = event.m_controlDown;
    ke.m_altDown = event.m_altDown;
    ke.m_metaDown = event.m_metaDown;
    ke.m_keyCode = event.m_keyCode;
    ke.m_x = event.m_x;
    ke.m_y = event.m_y;
    ke.SetEventObject( parent );
    if (parent->GetEventHandler()->ProcessEvent( ke )) return;

    event.Skip();
}

void wxListMainWindow::OnChar( wxKeyEvent &event )
{
    wxWindow *parent = GetParent();

    /* we send a list_key event up */
    if ( HasCurrent() )
    {
        wxListEvent le( wxEVT_COMMAND_LIST_KEY_DOWN, GetParent()->GetId() );
        le.m_itemIndex = m_current;
        GetLine(m_current)->GetItem( 0, le.m_item );
        le.m_code = (int)event.KeyCode();
        le.SetEventObject( parent );
        parent->GetEventHandler()->ProcessEvent( le );
    }

    /* we propagate the char event up */
    wxKeyEvent ke( wxEVT_CHAR );
    ke.m_shiftDown = event.m_shiftDown;
    ke.m_controlDown = event.m_controlDown;
    ke.m_altDown = event.m_altDown;
    ke.m_metaDown = event.m_metaDown;
    ke.m_keyCode = event.m_keyCode;
    ke.m_x = event.m_x;
    ke.m_y = event.m_y;
    ke.SetEventObject( parent );
    if (parent->GetEventHandler()->ProcessEvent( ke )) return;

    if (event.KeyCode() == WXK_TAB)
    {
        wxNavigationKeyEvent nevent;
        nevent.SetWindowChange( event.ControlDown() );
        nevent.SetDirection( !event.ShiftDown() );
        nevent.SetEventObject( GetParent()->GetParent() );
        nevent.SetCurrentFocus( m_parent );
        if (GetParent()->GetParent()->GetEventHandler()->ProcessEvent( nevent )) return;
    }

    /* no item -> nothing to do */
    if (!HasCurrent())
    {
        event.Skip();
        return;
    }

    switch (event.KeyCode())
    {
        case WXK_UP:
            if ( m_current > 0 )
                OnArrowChar( m_current - 1, event );
            break;

        case WXK_DOWN:
            if ( m_current < (size_t)GetItemCount() - 1 )
                OnArrowChar( m_current + 1, event );
            break;

        case WXK_END:
            if (!IsEmpty())
                OnArrowChar( GetItemCount() - 1, event );
            break;

        case WXK_HOME:
            if (!IsEmpty())
                OnArrowChar( 0, event );
            break;

        case WXK_PRIOR:
            {
                int steps = 0;
                if ( HasFlag(wxLC_REPORT) )
                {
                    steps = m_linesPerPage - 1;
                }
                else
                {
                    steps = m_current % m_linesPerPage;
                }

                int index = m_current - steps;
                if (index < 0)
                    index = 0;

                OnArrowChar( index, event );
            }
            break;

        case WXK_NEXT:
            {
                int steps = 0;
                if ( HasFlag(wxLC_REPORT) )
                {
                    steps = m_linesPerPage - 1;
                }
                else
                {
                    steps = m_linesPerPage - (m_current % m_linesPerPage) - 1;
                }

                size_t index = m_current + steps;
                size_t count = GetItemCount();
                if ( index >= count )
                    index = count - 1;

                OnArrowChar( index, event );
            }
            break;

        case WXK_LEFT:
            if ( !HasFlag(wxLC_REPORT) )
            {
                int index = m_current - m_linesPerPage;
                if (index < 0)
                    index = 0;

                OnArrowChar( index, event );
            }
            break;

        case WXK_RIGHT:
            if ( !HasFlag(wxLC_REPORT) )
            {
                size_t index = m_current + m_linesPerPage;

                size_t count = GetItemCount();
                if ( index >= count )
                    index = count - 1;

                OnArrowChar( index, event );
            }
            break;

        case WXK_SPACE:
            if ( IsSingleSel() )
            {
                wxListEvent le( wxEVT_COMMAND_LIST_ITEM_ACTIVATED,
                                GetParent()->GetId() );
                le.SetEventObject( GetParent() );
                le.m_itemIndex = m_current;
                GetLine(m_current)->GetItem( 0, le.m_item );
                GetParent()->GetEventHandler()->ProcessEvent( le );
            }
            else
            {
                ReverseHighlight(m_current);
            }
            break;

        case WXK_RETURN:
        case WXK_EXECUTE:
            {
                wxListEvent le( wxEVT_COMMAND_LIST_ITEM_ACTIVATED,
                                GetParent()->GetId() );
                le.SetEventObject( GetParent() );
                le.m_itemIndex = m_current;
                GetLine(m_current)->GetItem( 0, le.m_item );
                GetParent()->GetEventHandler()->ProcessEvent( le );
            }
            break;

        default:
            event.Skip();
    }
}

// ----------------------------------------------------------------------------
// focus handling
// ----------------------------------------------------------------------------

#ifdef __WXGTK__
extern wxWindow *g_focusWindow;
#endif

void wxListMainWindow::OnSetFocus( wxFocusEvent &WXUNUSED(event) )
{
    m_hasFocus = TRUE;

    if ( HasCurrent() )
        RefreshLine( m_current );

    if (!GetParent())
        return;

#ifdef __WXGTK__
    g_focusWindow = GetParent();
#endif

    wxFocusEvent event( wxEVT_SET_FOCUS, GetParent()->GetId() );
    event.SetEventObject( GetParent() );
    GetParent()->GetEventHandler()->ProcessEvent( event );
}

void wxListMainWindow::OnKillFocus( wxFocusEvent &WXUNUSED(event) )
{
    m_hasFocus = FALSE;

    if ( HasCurrent() )
        RefreshLine( m_current );
}

void wxListMainWindow::DrawImage( int index, wxDC *dc, int x, int y )
{
    if ( HasFlag(wxLC_ICON) && (m_normal_image_list))
    {
        m_normal_image_list->Draw( index, *dc, x, y, wxIMAGELIST_DRAW_TRANSPARENT );
    }
    else if ( HasFlag(wxLC_SMALL_ICON) && (m_small_image_list))
    {
        m_small_image_list->Draw( index, *dc, x, y, wxIMAGELIST_DRAW_TRANSPARENT );
    }
    else if ( HasFlag(wxLC_LIST) && (m_small_image_list))
    {
        m_small_image_list->Draw( index, *dc, x, y, wxIMAGELIST_DRAW_TRANSPARENT );
    }
    else if ( HasFlag(wxLC_REPORT) && (m_small_image_list))
    {
        m_small_image_list->Draw( index, *dc, x, y, wxIMAGELIST_DRAW_TRANSPARENT );
    }
}

void wxListMainWindow::GetImageSize( int index, int &width, int &height ) const
{
    if ( HasFlag(wxLC_ICON) && m_normal_image_list )
    {
        m_normal_image_list->GetSize( index, width, height );
    }
    else if ( HasFlag(wxLC_SMALL_ICON) && m_small_image_list )
    {
        m_small_image_list->GetSize( index, width, height );
    }
    else if ( HasFlag(wxLC_LIST) && m_small_image_list )
    {
        m_small_image_list->GetSize( index, width, height );
    }
    else if ( HasFlag(wxLC_REPORT) && m_small_image_list )
    {
        m_small_image_list->GetSize( index, width, height );
    }
    else
    {
        width =
        height = 0;
    }
}

int wxListMainWindow::GetTextLength( const wxString &s ) const
{
    wxClientDC dc( wxConstCast(this, wxListMainWindow) );
    dc.SetFont( GetFont() );

    wxCoord lw;
    dc.GetTextExtent( s, &lw, NULL );

    return lw + AUTOSIZE_COL_MARGIN;
}

void wxListMainWindow::SetImageList( wxImageList *imageList, int which )
{
    m_dirty = TRUE;

    // calc the spacing from the icon size
    int width = 0,
        height = 0;
    if ((imageList) && (imageList->GetImageCount()) )
    {
        imageList->GetSize(0, width, height);
    }

    if (which == wxIMAGE_LIST_NORMAL)
    {
        m_normal_image_list = imageList;
        m_normal_spacing = width + 8;
    }

    if (which == wxIMAGE_LIST_SMALL)
    {
        m_small_image_list = imageList;
        m_small_spacing = width + 14;
    }
}

void wxListMainWindow::SetItemSpacing( int spacing, bool isSmall )
{
    m_dirty = TRUE;
    if (isSmall)
    {
        m_small_spacing = spacing;
    }
    else
    {
        m_normal_spacing = spacing;
    }
}

int wxListMainWindow::GetItemSpacing( bool isSmall )
{
    return isSmall ? m_small_spacing : m_normal_spacing;
}

// ----------------------------------------------------------------------------
// columns
// ----------------------------------------------------------------------------

void wxListMainWindow::SetColumn( int col, wxListItem &item )
{
    wxListHeaderDataList::Node *node = m_columns.Item( col );

    wxCHECK_RET( node, _T("invalid column index in SetColumn") );

    if ( item.m_width == wxLIST_AUTOSIZE_USEHEADER )
        item.m_width = GetTextLength( item.m_text );

    wxListHeaderData *column = node->GetData();
    column->SetItem( item );

    wxListHeaderWindow *headerWin = GetListCtrl()->m_headerWin;
    if ( headerWin )
        headerWin->m_dirty = TRUE;

    m_dirty = TRUE;

    // invalidate it as it has to be recalculated
    m_headerWidth = 0;
}

void wxListMainWindow::SetColumnWidth( int col, int width )
{
    wxCHECK_RET( col >= 0 && col < GetColumnCount(),
                 _T("invalid column index") );

    wxCHECK_RET( HasFlag(wxLC_REPORT),
                 _T("SetColumnWidth() can only be called in report mode.") );

    m_dirty = TRUE;

    wxListHeaderDataList::Node *node = m_columns.Item( col );
    wxCHECK_RET( node, _T("no column?") );

    wxListHeaderData *column = node->GetData();

    size_t count = GetItemCount();

    if (width == wxLIST_AUTOSIZE_USEHEADER)
    {
        width = GetTextLength(column->GetText());
    }
    else if ( width == wxLIST_AUTOSIZE )
    {
        if ( IsVirtual() )
        {
            // TODO: determine the max width somehow...
            width = WIDTH_COL_DEFAULT;
        }
        else // !virtual
        {
            wxClientDC dc(this);
            dc.SetFont( GetFont() );

            int max = AUTOSIZE_COL_MARGIN;

            for ( size_t i = 0; i < count; i++ )
            {
                wxListLineData *line = GetLine(i);
                wxListItemDataList::Node *n = line->m_items.Item( col );

                wxCHECK_RET( n, _T("no subitem?") );

                wxListItemData *item = n->GetData();
                int current = 0;

                if (item->HasImage())
                {
                    int ix, iy;
                    GetImageSize( item->GetImage(), ix, iy );
                    current += ix + 5;
                }

                if (item->HasText())
                {
                    wxCoord w;
                    dc.GetTextExtent( item->GetText(), &w, NULL );
                    current += w;
                }

                if (current > max)
                    max = current;
            }

            width = max + AUTOSIZE_COL_MARGIN;
        }
    }

    column->SetWidth( width );

    // invalidate it as it has to be recalculated
    m_headerWidth = 0;
}

int wxListMainWindow::GetHeaderWidth() const
{
    if ( !m_headerWidth )
    {
        wxListMainWindow *self = wxConstCast(this, wxListMainWindow);

        size_t count = GetColumnCount();
        for ( size_t col = 0; col < count; col++ )
        {
            self->m_headerWidth += GetColumnWidth(col);
        }
    }

    return m_headerWidth;
}

void wxListMainWindow::GetColumn( int col, wxListItem &item ) const
{
    wxListHeaderDataList::Node *node = m_columns.Item( col );
    wxCHECK_RET( node, _T("invalid column index in GetColumn") );

    wxListHeaderData *column = node->GetData();
    column->GetItem( item );
}

int wxListMainWindow::GetColumnWidth( int col ) const
{
    wxListHeaderDataList::Node *node = m_columns.Item( col );
    wxCHECK_MSG( node, 0, _T("invalid column index") );

    wxListHeaderData *column = node->GetData();
    return column->GetWidth();
}

// ----------------------------------------------------------------------------
// item state
// ----------------------------------------------------------------------------

void wxListMainWindow::SetItem( wxListItem &item )
{
    long id = item.m_itemId;
    wxCHECK_RET( id >= 0 && (size_t)id < GetItemCount(),
                 _T("invalid item index in SetItem") );

    if ( IsVirtual() )
    {
        // just refresh the line to show the new value of the text/image
        RefreshLine((size_t)id);
    }
    else // !virtual
    {
        m_dirty = TRUE;

        wxListLineData *line = GetLine((size_t)id);
        if ( HasFlag(wxLC_REPORT) )
            item.m_width = GetColumnWidth( item.m_col );
        line->SetItem( item.m_col, item );
    }
}

void wxListMainWindow::SetItemState( long litem, long state, long stateMask )
{
     wxCHECK_RET( litem >= 0 && (size_t)litem < GetItemCount(),
                  _T("invalid list ctrl item index in SetItem") );

    size_t oldCurrent = m_current;
    size_t item = (size_t)litem;    // sdafe because of the check above

    if ( stateMask & wxLIST_STATE_FOCUSED )
    {
        if ( state & wxLIST_STATE_FOCUSED )
        {
            // don't do anything if this item is already focused
            if ( item != m_current )
            {
                OnUnfocusLine( m_current );
                m_current = item;
                OnFocusLine( m_current );

                if ( IsSingleSel() && (oldCurrent != (size_t)-1) )
                {
                    HighlightLine(oldCurrent, FALSE);
                    RefreshLine(oldCurrent);
                }

                RefreshLine( m_current );
            }
        }
        else // unfocus
        {
            // don't do anything if this item is not focused
            if ( item == m_current )
            {
                OnUnfocusLine( m_current );
                m_current = (size_t)-1;
            }
        }
    }

    if ( stateMask & wxLIST_STATE_SELECTED )
    {
        bool on = (state & wxLIST_STATE_SELECTED) != 0;

        if ( IsSingleSel() )
        {
            if ( on )
            {
                // selecting the item also makes it the focused one in the
                // single sel mode
                if ( m_current != item )
                {
                    OnUnfocusLine( m_current );
                    m_current = item;
                    OnFocusLine( m_current );

                    if ( oldCurrent != (size_t)-1 )
                    {
                        HighlightLine( oldCurrent, FALSE );
                        RefreshLine( oldCurrent );
                    }
                }
            }
            else // off
            {
                // only the current item may be selected anyhow
                if ( item != m_current )
                    return;
            }
        }

        if ( HighlightLine(item, on) )
        {
            RefreshLine(item);
        }
    }
}

int wxListMainWindow::GetItemState( long item, long stateMask )
{
    wxCHECK_MSG( item >= 0 && (size_t)item < GetItemCount(), 0,
                 _T("invalid list ctrl item index in GetItemState()") );

    int ret = wxLIST_STATE_DONTCARE;

    if ( stateMask & wxLIST_STATE_FOCUSED )
    {
        if ( (size_t)item == m_current )
            ret |= wxLIST_STATE_FOCUSED;
    }

    if ( stateMask & wxLIST_STATE_SELECTED )
    {
        if ( IsHighlighted(item) )
            ret |= wxLIST_STATE_SELECTED;
    }

    return ret;
}

void wxListMainWindow::GetItem( wxListItem &item )
{
    wxCHECK_RET( item.m_itemId >= 0 && (size_t)item.m_itemId < GetItemCount(),
                 _T("invalid item index in GetItem") );

    wxListLineData *line = GetLine((size_t)item.m_itemId);
    line->GetItem( item.m_col, item );
}

// ----------------------------------------------------------------------------
// item count
// ----------------------------------------------------------------------------

size_t wxListMainWindow::GetItemCount() const
{
    return IsVirtual() ? m_countVirt : m_lines.GetCount();
}

void wxListMainWindow::SetItemCount(long count)
{
    m_selStore.SetItemCount(count);
    m_countVirt = count;

    Refresh();
}

int wxListMainWindow::GetSelectedItemCount()
{
    // deal with the quick case first
    if ( IsSingleSel() )
    {
        return HasCurrent() ? IsHighlighted(m_current) : FALSE;
    }

    // virtual controls remmebers all its selections itself
    if ( IsVirtual() )
        return m_selStore.GetSelectedCount();

    // TODO: we probably should maintain the number of items selected even for
    //       non virtual controls as enumerating all lines is really slow...
    size_t countSel = 0;
    size_t count = GetItemCount();
    for ( size_t line = 0; line < count; line++ )
    {
        if ( GetLine(line)->IsHighlighted() )
            countSel++;
    }

    return countSel;
}

// ----------------------------------------------------------------------------
// item position/size
// ----------------------------------------------------------------------------

void wxListMainWindow::GetItemRect( long index, wxRect &rect )
{
    wxCHECK_RET( index >= 0 && (size_t)index < GetItemCount(),
                 _T("invalid index in GetItemRect") );

    rect = GetLineRect((size_t)index);

    CalcScrolledPosition(rect.x, rect.y, &rect.x, &rect.y);
}

bool wxListMainWindow::GetItemPosition(long item, wxPoint& pos)
{
    wxRect rect;
    GetItemRect(item, rect);

    pos.x = rect.x;
    pos.y = rect.y;

    return TRUE;
}

// ----------------------------------------------------------------------------
// geometry calculation
// ----------------------------------------------------------------------------

void wxListMainWindow::RecalculatePositions()
{
    if ( IsEmpty() )
        return;

    wxClientDC dc( this );
    dc.SetFont( GetFont() );

    int iconSpacing;
    if ( HasFlag(wxLC_ICON) )
        iconSpacing = m_normal_spacing;
    else if ( HasFlag(wxLC_SMALL_ICON) )
        iconSpacing = m_small_spacing;
    else
        iconSpacing = 0;

    int clientWidth,
        clientHeight;
    GetClientSize( &clientWidth, &clientHeight );

    if ( HasFlag(wxLC_REPORT) )
    {
        // all lines have the same height
        int lineHeight = GetLineHeight();

        // scroll one line per step
        m_yScroll = lineHeight;

        size_t lineCount = GetItemCount();
        int entireHeight = lineCount*lineHeight + LINE_SPACING;

        m_linesPerPage = clientHeight / lineHeight;

        ResetVisibleLinesRange();

        SetScrollbars( m_xScroll, m_yScroll,
                       (GetHeaderWidth() + m_xScroll - 1)/m_xScroll,
                       (entireHeight + m_yScroll - 1)/m_yScroll,
                       GetScrollPos(wxHORIZONTAL),
                       GetScrollPos(wxVERTICAL),
                       TRUE );
    }
    else // !report
    {
        // at first we try without any scrollbar. if the items don't
        // fit into the window, we recalculate after subtracting an
        // approximated 15 pt for the horizontal scrollbar

        clientHeight -= 4;  // sunken frame

        int entireWidth = 0;

        for (int tries = 0; tries < 2; tries++)
        {
            entireWidth = 0;
            int x = 2;
            int y = 2;
            int maxWidth = 0;
            m_linesPerPage = 0;
            int currentlyVisibleLines = 0;

            size_t count = GetItemCount();
            for (size_t i = 0; i < count; i++)
            {
                currentlyVisibleLines++;
                wxListLineData *line = GetLine(i);
                line->CalculateSize( &dc, iconSpacing );
                line->SetPosition( x, y, clientWidth, iconSpacing );

                wxSize sizeLine = GetLineSize(i);

                if ( maxWidth < sizeLine.x )
                    maxWidth = sizeLine.x;

                y += sizeLine.y;
                if (currentlyVisibleLines > m_linesPerPage)
                    m_linesPerPage = currentlyVisibleLines;

                // assume that the size of the next one is the same... (FIXME)
                if ( y + sizeLine.y - 6 >= clientHeight )
                {
                    currentlyVisibleLines = 0;
                    y = 2;
                    x += maxWidth+6;
                    entireWidth += maxWidth+6;
                    maxWidth = 0;
                }
                if ( i == count - 1 )
                    entireWidth += maxWidth;
                if ((tries == 0) && (entireWidth > clientWidth))
                {
                    clientHeight -= 15; // scrollbar height
                    m_linesPerPage = 0;
                    currentlyVisibleLines = 0;
                    break;
                }
                if ( i == count - 1 )
                    tries = 1;  // everything fits, no second try required
            }
        }

        int scroll_pos = GetScrollPos( wxHORIZONTAL );
        SetScrollbars( m_xScroll, m_yScroll, (entireWidth+SCROLL_UNIT_X) / m_xScroll, 0, scroll_pos, 0, TRUE );
    }

    // FIXME: why should we call it from here?
    UpdateCurrent();

    RefreshAll();
}

void wxListMainWindow::RefreshAll()
{
    m_dirty = FALSE;
    Refresh();

    wxListHeaderWindow *headerWin = GetListCtrl()->m_headerWin;
    if ( headerWin )
    {
        headerWin->m_dirty = FALSE;
        headerWin->Refresh();
    }
}

void wxListMainWindow::UpdateCurrent()
{
    if ( !HasCurrent() && !IsEmpty() )
    {
        m_current = 0;
    }

    if ( m_current != (size_t)-1 )
    {
        OnFocusLine( m_current );
    }
}

long wxListMainWindow::GetNextItem( long item,
                                    int WXUNUSED(geometry),
                                    int state )
{
    long ret = item,
         max = GetItemCount();
    wxCHECK_MSG( (ret == -1) || (ret < max), -1,
                 _T("invalid listctrl index in GetNextItem()") );

    // notice that we start with the next item (or the first one if item == -1)
    // and this is intentional to allow writing a simple loop to iterate over
    // all selected items
    ret++;
    if ( ret == max )
    {
        // this is not an error because the index was ok initially, just no
        // such item
        return -1;
    }

    if ( !state )
    {
        // any will do
        return (size_t)ret;
    }

    size_t count = GetItemCount();
    for ( size_t line = (size_t)ret; line < count; line++ )
    {
        if ( (state & wxLIST_STATE_FOCUSED) && (line == m_current) )
            return line;

        if ( (state & wxLIST_STATE_SELECTED) && IsHighlighted(line) )
            return line;
    }

    return -1;
}

// ----------------------------------------------------------------------------
// deleting stuff
// ----------------------------------------------------------------------------

void wxListMainWindow::DeleteItem( long lindex )
{
    size_t count = GetItemCount();

    wxCHECK_RET( (lindex >= 0) && ((size_t)lindex < count),
                 _T("invalid item index in DeleteItem") );

    size_t index = (size_t)lindex;

    m_dirty = TRUE;

    // select the next item when the selected one is deleted
    if ( m_current == index )
    {
        // the last valid index after deleting the item will be count-2
        if ( m_current == count - 1 )
        {
            m_current--;
        }
    }

    SendNotify( index, wxEVT_COMMAND_LIST_DELETE_ITEM );

    if ( InReportView() )
    {
        if ( m_lineTo == GetItemCount() - 1 )
        {
            m_lineTo--;
        }
    }

    // refresh before removing the line
    RefreshLines(index, GetItemCount() - 1);

    if ( IsVirtual() )
    {
        m_countVirt--;

        m_selStore.OnItemDelete(index);
    }
    else
    {
        m_lines.RemoveAt( index );
    }
}

void wxListMainWindow::DeleteColumn( int col )
{
    wxListHeaderDataList::Node *node = m_columns.Item( col );

    wxCHECK_RET( node, wxT("invalid column index in DeleteColumn()") );

    m_dirty = TRUE;
    m_columns.DeleteNode( node );
}

void wxListMainWindow::DeleteAllItems()
{
    if ( IsEmpty() )
    {
        // nothing to do - in particular, don't send the event
        return;
    }

    m_dirty = TRUE;

    ResetCurrent();

    // to make the deletion of all items faster, we don't send the
    // notifications for each item deletion in this case but only one event
    // for all of them: this is compatible with wxMSW and documented in
    // DeleteAllItems() description

    wxListEvent event( wxEVT_COMMAND_LIST_DELETE_ALL_ITEMS, GetParent()->GetId() );
    event.SetEventObject( GetParent() );
    GetParent()->GetEventHandler()->ProcessEvent( event );

    if ( IsVirtual() )
    {
        m_countVirt = 0;

        ResetVisibleLinesRange();
    }

    m_lines.Clear();

    m_selStore.Clear();
}

void wxListMainWindow::DeleteEverything()
{
    DeleteAllItems();

    m_columns.Clear();
}

// ----------------------------------------------------------------------------
// scanning for an item
// ----------------------------------------------------------------------------

void wxListMainWindow::EnsureVisible( long index )
{
    wxCHECK_RET( index >= 0 && (size_t)index < GetItemCount(),
                 _T("invalid index in EnsureVisible") );

    // We have to call this here because the label in question might just have
    // been added and no screen update taken place.
    if (m_dirty)
        wxSafeYield();

    size_t oldCurrent = m_current;
    m_current = (size_t)index;
    MoveToFocus();
    m_current = oldCurrent;
}

long wxListMainWindow::FindItem(long start, const wxString& str, bool WXUNUSED(partial) )
{
    long pos = start;
    wxString tmp = str;
    if (pos < 0)
        pos = 0;

    size_t count = GetItemCount();
    for ( size_t i = (size_t)pos; i < count; i++ )
    {
        wxListLineData *line = GetLine(i);
        if ( line->GetText(0) == tmp )
            return i;
    }

    return wxNOT_FOUND;
}

long wxListMainWindow::FindItem(long start, long data)
{
    long pos = start;
    if (pos < 0)
        pos = 0;

    size_t count = GetItemCount();
    for (size_t i = (size_t)pos; i < count; i++)
    {
        wxListLineData *line = GetLine(i);
        wxListItem item;
        line->GetItem( 0, item );
        if (item.m_data == data)
            return i;
    }

    return wxNOT_FOUND;
}

long wxListMainWindow::HitTest( int x, int y, int &flags )
{
    CalcUnscrolledPosition( x, y, &x, &y );

    if ( HasFlag(wxLC_REPORT) )
    {
        size_t current = y / GetLineHeight();
        flags = HitTestLine(current, x, y);
        if ( flags )
            return current;
    }
    else // !report
    {
        // TODO: optimize it too! this is less simple than for report view but
        //       enumerating all items is still not a way to do it!!
        size_t count = GetItemCount();
        for ( size_t current = 0; current < count; current++ )
        {
            flags = HitTestLine(current, x, y);
            if ( flags )
                return current;
        }
    }

    return wxNOT_FOUND;
}

// ----------------------------------------------------------------------------
// adding stuff
// ----------------------------------------------------------------------------

void wxListMainWindow::InsertItem( wxListItem &item )
{
    wxASSERT_MSG( !IsVirtual(), _T("can't be used with virtual control") );

    size_t count = GetItemCount();
    wxCHECK_RET( item.m_itemId >= 0 && (size_t)item.m_itemId <= count,
                 _T("invalid item index") );

    size_t id = item.m_itemId;

    m_dirty = TRUE;

    int mode = 0;
    if ( HasFlag(wxLC_REPORT) )
        mode = wxLC_REPORT;
    else if ( HasFlag(wxLC_LIST) )
        mode = wxLC_LIST;
    else if ( HasFlag(wxLC_ICON) )
        mode = wxLC_ICON;
    else if ( HasFlag(wxLC_SMALL_ICON) )
        mode = wxLC_ICON;  // no typo
    else
    {
        wxFAIL_MSG( _T("unknown mode") );
    }

    wxListLineData *line = new wxListLineData(this);

    line->SetItem( 0, item );

    m_lines.Insert( line, id );

    RefreshLines(id, GetItemCount() - 1);
}

void wxListMainWindow::InsertColumn( long col, wxListItem &item )
{
    m_dirty = TRUE;
    if ( HasFlag(wxLC_REPORT) )
    {
        if (item.m_width == wxLIST_AUTOSIZE_USEHEADER)
            item.m_width = GetTextLength( item.m_text );
        wxListHeaderData *column = new wxListHeaderData( item );
        if ((col >= 0) && (col < (int)m_columns.GetCount()))
        {
            wxListHeaderDataList::Node *node = m_columns.Item( col );
            m_columns.Insert( node, column );
        }
        else
        {
            m_columns.Append( column );
        }
    }
}

// ----------------------------------------------------------------------------
// sorting
// ----------------------------------------------------------------------------

wxListCtrlCompare list_ctrl_compare_func_2;
long              list_ctrl_compare_data;

int LINKAGEMODE list_ctrl_compare_func_1( wxListLineData **arg1, wxListLineData **arg2 )
{
    wxListLineData *line1 = *arg1;
    wxListLineData *line2 = *arg2;
    wxListItem item;
    line1->GetItem( 0, item );
    long data1 = item.m_data;
    line2->GetItem( 0, item );
    long data2 = item.m_data;
    return list_ctrl_compare_func_2( data1, data2, list_ctrl_compare_data );
}

void wxListMainWindow::SortItems( wxListCtrlCompare fn, long data )
{
    list_ctrl_compare_func_2 = fn;
    list_ctrl_compare_data = data;
    m_lines.Sort( list_ctrl_compare_func_1 );
    m_dirty = TRUE;
}

// ----------------------------------------------------------------------------
// scrolling
// ----------------------------------------------------------------------------

void wxListMainWindow::OnScroll(wxScrollWinEvent& event)
{
    // update our idea of which lines are shown when we redraw the window the
    // next time
    ResetVisibleLinesRange();

    // FIXME
#if defined(__WXGTK__) && !defined(__WXUNIVERSAL__)
    wxScrolledWindow::OnScroll(event);
#else
    HandleOnScroll( event );
#endif

    if ( event.GetOrientation() == wxHORIZONTAL && HasHeader() )
    {
        wxListCtrl* lc = GetListCtrl();
        wxCHECK_RET( lc, _T("no listctrl window?") );

        lc->m_headerWin->Refresh() ;
#ifdef __WXMAC__
        lc->m_headerWin->MacUpdateImmediately() ;
#endif
    }
}

int wxListMainWindow::GetCountPerPage() const
{
    if ( !m_linesPerPage )
    {
        wxConstCast(this, wxListMainWindow)->
            m_linesPerPage = GetClientSize().y / GetLineHeight();
    }

    return m_linesPerPage;
}

void wxListMainWindow::GetVisibleLinesRange(size_t *from, size_t *to)
{
    wxASSERT_MSG( HasFlag(wxLC_REPORT), _T("this is for report mode only") );

    if ( m_lineFrom == (size_t)-1 )
    {
        m_lineFrom = GetScrollPos(wxVERTICAL);

        size_t count = GetItemCount();

        wxASSERT_MSG( m_lineFrom < count, _T("invalid scroll position?") );

        // we redraw one extra line but this is needed to make the redrawing
        // logic work when there is a fractional number of lines on screen
        m_lineTo = m_lineFrom + m_linesPerPage;
        if ( m_lineTo >= count )
            m_lineTo = count - 1;
    }

    if ( from )
        *from = m_lineFrom;
    if ( to )
        *to = m_lineTo;
}

// -------------------------------------------------------------------------------------
// wxListItem
// -------------------------------------------------------------------------------------

IMPLEMENT_DYNAMIC_CLASS(wxListItem, wxObject)

wxListItem::wxListItem()
{
    m_mask = 0;
    m_itemId = 0;
    m_col = 0;
    m_state = 0;
    m_stateMask = 0;
    m_image = 0;
    m_data = 0;
    m_format = wxLIST_FORMAT_CENTRE;
    m_width = 0;

    m_attr = NULL;
}

void wxListItem::Clear()
{
    m_mask = 0;
    m_itemId = 0;
    m_col = 0;
    m_state = 0;
    m_stateMask = 0;
    m_image = 0;
    m_data = 0;
    m_format = wxLIST_FORMAT_CENTRE;
    m_width = 0;
    m_text = _T("");

    ClearAttributes();
}

void wxListItem::ClearAttributes()
{
    if (m_attr)
    {
        delete m_attr;
        m_attr = NULL;
    }
}

// -------------------------------------------------------------------------------------
// wxListEvent
// -------------------------------------------------------------------------------------

IMPLEMENT_DYNAMIC_CLASS(wxListEvent, wxNotifyEvent)

wxListEvent::wxListEvent( wxEventType commandType, int id )
           : wxNotifyEvent( commandType, id )
{
    m_code = 0;
    m_itemIndex = 0;
    m_oldItemIndex = 0;
    m_col = 0;
    m_cancelled = FALSE;
    m_pointDrag.x = 0;
    m_pointDrag.y = 0;
}

void wxListEvent::CopyObject(wxObject& object_dest) const
{
    wxListEvent *obj = (wxListEvent *)&object_dest;

    wxNotifyEvent::CopyObject(object_dest);

    obj->m_code = m_code;
    obj->m_itemIndex = m_itemIndex;
    obj->m_oldItemIndex = m_oldItemIndex;
    obj->m_col = m_col;
    obj->m_cancelled = m_cancelled;
    obj->m_pointDrag = m_pointDrag;
    obj->m_item.m_mask = m_item.m_mask;
    obj->m_item.m_itemId = m_item.m_itemId;
    obj->m_item.m_col = m_item.m_col;
    obj->m_item.m_state = m_item.m_state;
    obj->m_item.m_stateMask = m_item.m_stateMask;
    obj->m_item.m_text = m_item.m_text;
    obj->m_item.m_image = m_item.m_image;
    obj->m_item.m_data = m_item.m_data;
    obj->m_item.m_format = m_item.m_format;
    obj->m_item.m_width = m_item.m_width;

    if ( m_item.HasAttributes() )
    {
        obj->m_item.SetTextColour(m_item.GetTextColour());
    }
}

// -------------------------------------------------------------------------------------
// wxListCtrl
// -------------------------------------------------------------------------------------

IMPLEMENT_DYNAMIC_CLASS(wxListCtrl, wxControl)

BEGIN_EVENT_TABLE(wxListCtrl,wxControl)
  EVT_SIZE(wxListCtrl::OnSize)
  EVT_IDLE(wxListCtrl::OnIdle)
END_EVENT_TABLE()

wxListCtrl::wxListCtrl()
{
    m_imageListNormal = (wxImageList *) NULL;
    m_imageListSmall = (wxImageList *) NULL;
    m_imageListState = (wxImageList *) NULL;

    m_ownsImageListNormal =
    m_ownsImageListSmall =
    m_ownsImageListState = FALSE;

    m_mainWin = (wxListMainWindow*) NULL;
    m_headerWin = (wxListHeaderWindow*) NULL;
}

wxListCtrl::~wxListCtrl()
{
    if ( m_mainWin )
        m_mainWin->ResetCurrent();

    if (m_ownsImageListNormal)
        delete m_imageListNormal;
    if (m_ownsImageListSmall)
        delete m_imageListSmall;
    if (m_ownsImageListState)
        delete m_imageListState;
}

void wxListCtrl::CreateHeaderWindow()
{
    m_headerWin = new wxListHeaderWindow
                      (
                        this, -1, m_mainWin,
                        wxPoint(0, 0),
                        wxSize(GetClientSize().x, HEADER_HEIGHT),
                        wxTAB_TRAVERSAL
                      );
}

bool wxListCtrl::Create(wxWindow *parent,
                        wxWindowID id,
                        const wxPoint &pos,
                        const wxSize &size,
                        long style,
                        const wxValidator &validator,
                        const wxString &name)
{
    m_imageListNormal =
    m_imageListSmall =
    m_imageListState = (wxImageList *) NULL;
    m_ownsImageListNormal =
    m_ownsImageListSmall =
    m_ownsImageListState = FALSE;

    m_mainWin = (wxListMainWindow*) NULL;
    m_headerWin = (wxListHeaderWindow*) NULL;

    if ( !(style & wxLC_MASK_TYPE) )
    {
        style = style | wxLC_LIST;
    }

    if ( !wxControl::Create( parent, id, pos, size, style, validator, name ) )
        return FALSE;

    // don't create the inner window with the border
    style &= ~wxSUNKEN_BORDER;

    m_mainWin = new wxListMainWindow( this, -1, wxPoint(0,0), size, style );

    if ( HasFlag(wxLC_REPORT) )
    {
        CreateHeaderWindow();

        if ( HasFlag(wxLC_NO_HEADER) )
        {
            // VZ: why do we create it at all then?
            m_headerWin->Show( FALSE );
        }
    }

    return TRUE;
}

void wxListCtrl::SetSingleStyle( long style, bool add )
{
    wxASSERT_MSG( !(style & wxLC_VIRTUAL),
                  _T("wxLC_VIRTUAL can't be [un]set") );

    long flag = GetWindowStyle();

    if (add)
    {
        if (style & wxLC_MASK_TYPE)
            flag &= ~(wxLC_MASK_TYPE | wxLC_VIRTUAL);
        if (style & wxLC_MASK_ALIGN)
            flag &= ~wxLC_MASK_ALIGN;
        if (style & wxLC_MASK_SORT)
            flag &= ~wxLC_MASK_SORT;
    }

    if (add)
    {
        flag |= style;
    }
    else
    {
        flag &= ~style;
    }

    SetWindowStyleFlag( flag );
}

void wxListCtrl::SetWindowStyleFlag( long flag )
{
    if (m_mainWin)
    {
        m_mainWin->DeleteEverything();

        int width = 0;
        int height = 0;
        GetClientSize( &width, &height );

        if (flag & wxLC_REPORT)
        {
            if (!HasFlag(wxLC_REPORT))
            {
                if (!m_headerWin)
                {
                    CreateHeaderWindow();

                    if (HasFlag(wxLC_NO_HEADER))
                        m_headerWin->Show( FALSE );
                }
                else
                {
                    if (flag & wxLC_NO_HEADER)
                        m_headerWin->Show( FALSE );
                    else
                        m_headerWin->Show( TRUE );
                }
            }
        }
        else // !report
        {
            if ( m_mainWin->HasHeader() )
            {
                m_headerWin->Show( FALSE );
            }
        }
    }

    wxWindow::SetWindowStyleFlag( flag );
}

bool wxListCtrl::GetColumn(int col, wxListItem &item) const
{
    m_mainWin->GetColumn( col, item );
    return TRUE;
}

bool wxListCtrl::SetColumn( int col, wxListItem& item )
{
    m_mainWin->SetColumn( col, item );
    return TRUE;
}

int wxListCtrl::GetColumnWidth( int col ) const
{
    return m_mainWin->GetColumnWidth( col );
}

bool wxListCtrl::SetColumnWidth( int col, int width )
{
    m_mainWin->SetColumnWidth( col, width );
    return TRUE;
}

int wxListCtrl::GetCountPerPage() const
{
  return m_mainWin->GetCountPerPage();  // different from Windows ?
}

bool wxListCtrl::GetItem( wxListItem &info ) const
{
    m_mainWin->GetItem( info );
    return TRUE;
}

bool wxListCtrl::SetItem( wxListItem &info )
{
    m_mainWin->SetItem( info );
    return TRUE;
}

long wxListCtrl::SetItem( long index, int col, const wxString& label, int imageId )
{
    wxListItem info;
    info.m_text = label;
    info.m_mask = wxLIST_MASK_TEXT;
    info.m_itemId = index;
    info.m_col = col;
    if ( imageId > -1 )
    {
        info.m_image = imageId;
        info.m_mask |= wxLIST_MASK_IMAGE;
    };
    m_mainWin->SetItem(info);
    return TRUE;
}

int wxListCtrl::GetItemState( long item, long stateMask ) const
{
    return m_mainWin->GetItemState( item, stateMask );
}

bool wxListCtrl::SetItemState( long item, long state, long stateMask )
{
    m_mainWin->SetItemState( item, state, stateMask );
    return TRUE;
}

bool wxListCtrl::SetItemImage( long item, int image, int WXUNUSED(selImage) )
{
    wxListItem info;
    info.m_image = image;
    info.m_mask = wxLIST_MASK_IMAGE;
    info.m_itemId = item;
    m_mainWin->SetItem( info );
    return TRUE;
}

wxString wxListCtrl::GetItemText( long item ) const
{
    wxListItem info;
    info.m_itemId = item;
    m_mainWin->GetItem( info );
    return info.m_text;
}

void wxListCtrl::SetItemText( long item, const wxString &str )
{
    wxListItem info;
    info.m_mask = wxLIST_MASK_TEXT;
    info.m_itemId = item;
    info.m_text = str;
    m_mainWin->SetItem( info );
}

long wxListCtrl::GetItemData( long item ) const
{
    wxListItem info;
    info.m_itemId = item;
    m_mainWin->GetItem( info );
    return info.m_data;
}

bool wxListCtrl::SetItemData( long item, long data )
{
    wxListItem info;
    info.m_mask = wxLIST_MASK_DATA;
    info.m_itemId = item;
    info.m_data = data;
    m_mainWin->SetItem( info );
    return TRUE;
}

bool wxListCtrl::GetItemRect( long item, wxRect &rect,  int WXUNUSED(code) ) const
{
    m_mainWin->GetItemRect( item, rect );
    return TRUE;
}

bool wxListCtrl::GetItemPosition( long item, wxPoint& pos ) const
{
    m_mainWin->GetItemPosition( item, pos );
    return TRUE;
}

bool wxListCtrl::SetItemPosition( long WXUNUSED(item), const wxPoint& WXUNUSED(pos) )
{
    return 0;
}

int wxListCtrl::GetItemCount() const
{
    return m_mainWin->GetItemCount();
}

int wxListCtrl::GetColumnCount() const
{
    return m_mainWin->GetColumnCount();
}

void wxListCtrl::SetItemSpacing( int spacing, bool isSmall )
{
    m_mainWin->SetItemSpacing( spacing, isSmall );
}

int wxListCtrl::GetItemSpacing( bool isSmall ) const
{
    return m_mainWin->GetItemSpacing( isSmall );
}

int wxListCtrl::GetSelectedItemCount() const
{
    return m_mainWin->GetSelectedItemCount();
}

wxColour wxListCtrl::GetTextColour() const
{
    return GetForegroundColour();
}

void wxListCtrl::SetTextColour(const wxColour& col)
{
    SetForegroundColour(col);
}

long wxListCtrl::GetTopItem() const
{
    return 0;
}

long wxListCtrl::GetNextItem( long item, int geom, int state ) const
{
    return m_mainWin->GetNextItem( item, geom, state );
}

wxImageList *wxListCtrl::GetImageList(int which) const
{
    if (which == wxIMAGE_LIST_NORMAL)
    {
        return m_imageListNormal;
    }
    else if (which == wxIMAGE_LIST_SMALL)
    {
        return m_imageListSmall;
    }
    else if (which == wxIMAGE_LIST_STATE)
    {
        return m_imageListState;
    }
    return (wxImageList *) NULL;
}

void wxListCtrl::SetImageList( wxImageList *imageList, int which )
{
    if ( which == wxIMAGE_LIST_NORMAL )
    {
        if (m_ownsImageListNormal) delete m_imageListNormal;
        m_imageListNormal = imageList;
        m_ownsImageListNormal = FALSE;
    }
    else if ( which == wxIMAGE_LIST_SMALL )
    {
        if (m_ownsImageListSmall) delete m_imageListSmall;
        m_imageListSmall = imageList;
        m_ownsImageListSmall = FALSE;
    }
    else if ( which == wxIMAGE_LIST_STATE )
    {
        if (m_ownsImageListState) delete m_imageListState;
        m_imageListState = imageList;
        m_ownsImageListState = FALSE;
    }

    m_mainWin->SetImageList( imageList, which );
}

void wxListCtrl::AssignImageList(wxImageList *imageList, int which)
{
    SetImageList(imageList, which);
    if ( which == wxIMAGE_LIST_NORMAL )
        m_ownsImageListNormal = TRUE;
    else if ( which == wxIMAGE_LIST_SMALL )
        m_ownsImageListSmall = TRUE;
    else if ( which == wxIMAGE_LIST_STATE )
        m_ownsImageListState = TRUE;
}

bool wxListCtrl::Arrange( int WXUNUSED(flag) )
{
    return 0;
}

bool wxListCtrl::DeleteItem( long item )
{
    m_mainWin->DeleteItem( item );
    return TRUE;
}

bool wxListCtrl::DeleteAllItems()
{
    m_mainWin->DeleteAllItems();
    return TRUE;
}

bool wxListCtrl::DeleteAllColumns()
{
    size_t count = m_mainWin->m_columns.GetCount();
    for ( size_t n = 0; n < count; n++ )
        DeleteColumn(n);

    return TRUE;
}

void wxListCtrl::ClearAll()
{
    m_mainWin->DeleteEverything();
}

bool wxListCtrl::DeleteColumn( int col )
{
    m_mainWin->DeleteColumn( col );
    return TRUE;
}

void wxListCtrl::Edit( long item )
{
    m_mainWin->EditLabel( item );
}

bool wxListCtrl::EnsureVisible( long item )
{
    m_mainWin->EnsureVisible( item );
    return TRUE;
}

long wxListCtrl::FindItem( long start, const wxString& str,  bool partial )
{
    return m_mainWin->FindItem( start, str, partial );
}

long wxListCtrl::FindItem( long start, long data )
{
    return m_mainWin->FindItem( start, data );
}

long wxListCtrl::FindItem( long WXUNUSED(start), const wxPoint& WXUNUSED(pt),
                           int WXUNUSED(direction))
{
    return 0;
}

long wxListCtrl::HitTest( const wxPoint &point, int &flags )
{
    return m_mainWin->HitTest( (int)point.x, (int)point.y, flags );
}

long wxListCtrl::InsertItem( wxListItem& info )
{
    m_mainWin->InsertItem( info );
    return info.m_itemId;
}

long wxListCtrl::InsertItem( long index, const wxString &label )
{
    wxListItem info;
    info.m_text = label;
    info.m_mask = wxLIST_MASK_TEXT;
    info.m_itemId = index;
    return InsertItem( info );
}

long wxListCtrl::InsertItem( long index, int imageIndex )
{
    wxListItem info;
    info.m_mask = wxLIST_MASK_IMAGE;
    info.m_image = imageIndex;
    info.m_itemId = index;
    return InsertItem( info );
}

long wxListCtrl::InsertItem( long index, const wxString &label, int imageIndex )
{
    wxListItem info;
    info.m_text = label;
    info.m_image = imageIndex;
    info.m_mask = wxLIST_MASK_TEXT | wxLIST_MASK_IMAGE;
    info.m_itemId = index;
    return InsertItem( info );
}

long wxListCtrl::InsertColumn( long col, wxListItem &item )
{
    wxASSERT( m_headerWin );
    m_mainWin->InsertColumn( col, item );
    m_headerWin->Refresh();

    return 0;
}

long wxListCtrl::InsertColumn( long col, const wxString &heading,
                               int format, int width )
{
    wxListItem item;
    item.m_mask = wxLIST_MASK_TEXT | wxLIST_MASK_FORMAT;
    item.m_text = heading;
    if (width >= -2)
    {
        item.m_mask |= wxLIST_MASK_WIDTH;
        item.m_width = width;
    }
    item.m_format = format;

    return InsertColumn( col, item );
}

bool wxListCtrl::ScrollList( int WXUNUSED(dx), int WXUNUSED(dy) )
{
    return 0;
}

// Sort items.
// fn is a function which takes 3 long arguments: item1, item2, data.
// item1 is the long data associated with a first item (NOT the index).
// item2 is the long data associated with a second item (NOT the index).
// data is the same value as passed to SortItems.
// The return value is a negative number if the first item should precede the second
// item, a positive number of the second item should precede the first,
// or zero if the two items are equivalent.
// data is arbitrary data to be passed to the sort function.

bool wxListCtrl::SortItems( wxListCtrlCompare fn, long data )
{
    m_mainWin->SortItems( fn, data );
    return TRUE;
}

// ----------------------------------------------------------------------------
// event handlers
// ----------------------------------------------------------------------------

void wxListCtrl::OnSize(wxSizeEvent& event)
{
    if ( !m_mainWin )
        return;

    int cw, ch;
    GetClientSize( &cw, &ch );

    if ( m_mainWin->HasHeader() )
    {
        m_headerWin->SetSize( 0, 0, cw, HEADER_HEIGHT );
        m_mainWin->SetSize( 0, HEADER_HEIGHT + 1, cw, ch - HEADER_HEIGHT - 1 );
    }
    else // no header window
    {
        m_mainWin->SetSize( 0, 0, cw, ch );
    }

    m_mainWin->RecalculatePositions();
}

void wxListCtrl::OnIdle( wxIdleEvent & event )
{
    event.Skip();

    // do it only if needed
    if ( !m_mainWin->m_dirty )
        return;

    m_mainWin->RecalculatePositions();
}

// ----------------------------------------------------------------------------
// font/colours
// ----------------------------------------------------------------------------

bool wxListCtrl::SetBackgroundColour( const wxColour &colour )
{
    if (m_mainWin)
    {
        m_mainWin->SetBackgroundColour( colour );
        m_mainWin->m_dirty = TRUE;
    }

    return TRUE;
}

bool wxListCtrl::SetForegroundColour( const wxColour &colour )
{
    if ( !wxWindow::SetForegroundColour( colour ) )
        return FALSE;

    if (m_mainWin)
    {
        m_mainWin->SetForegroundColour( colour );
        m_mainWin->m_dirty = TRUE;
    }

    if (m_headerWin)
    {
        m_headerWin->SetForegroundColour( colour );
    }

    return TRUE;
}

bool wxListCtrl::SetFont( const wxFont &font )
{
    if ( !wxWindow::SetFont( font ) )
        return FALSE;

    if (m_mainWin)
    {
        m_mainWin->SetFont( font );
        m_mainWin->m_dirty = TRUE;
    }

    if (m_headerWin)
    {
        m_headerWin->SetFont( font );
    }

    return TRUE;
}

// ----------------------------------------------------------------------------
// methods forwarded to m_mainWin
// ----------------------------------------------------------------------------

#if wxUSE_DRAG_AND_DROP

void wxListCtrl::SetDropTarget( wxDropTarget *dropTarget )
{
    m_mainWin->SetDropTarget( dropTarget );
}

wxDropTarget *wxListCtrl::GetDropTarget() const
{
    return m_mainWin->GetDropTarget();
}

#endif // wxUSE_DRAG_AND_DROP

bool wxListCtrl::SetCursor( const wxCursor &cursor )
{
    return m_mainWin ? m_mainWin->wxWindow::SetCursor(cursor) : FALSE;
}

wxColour wxListCtrl::GetBackgroundColour() const
{
    return m_mainWin ? m_mainWin->GetBackgroundColour() : wxColour();
}

wxColour wxListCtrl::GetForegroundColour() const
{
    return m_mainWin ? m_mainWin->GetForegroundColour() : wxColour();
}

bool wxListCtrl::DoPopupMenu( wxMenu *menu, int x, int y )
{
#if wxUSE_MENUS
    return m_mainWin->PopupMenu( menu, x, y );
#else
    return FALSE;
#endif // wxUSE_MENUS
}

void wxListCtrl::SetFocus()
{
    /* The test in window.cpp fails as we are a composite
       window, so it checks against "this", but not m_mainWin. */
    if ( FindFocus() != this )
        m_mainWin->SetFocus();
}

// ----------------------------------------------------------------------------
// virtual list control support
// ----------------------------------------------------------------------------

wxString wxListCtrl::OnGetItemText(long item, long col) const
{
    // this is a pure virtual function, in fact - which is not really pure
    // because the controls which are not virtual don't need to implement it
    wxFAIL_MSG( _T("not supposed to be called") );

    return wxEmptyString;
}

int wxListCtrl::OnGetItemImage(long item) const
{
    // same as above
    wxFAIL_MSG( _T("not supposed to be called") );

    return -1;
}

void wxListCtrl::SetItemCount(long count)
{
    wxASSERT_MSG( IsVirtual(), _T("this is for virtual controls only") );

    m_mainWin->SetItemCount(count);
}

#endif // wxUSE_LISTCTRL
