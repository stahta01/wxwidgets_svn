/////////////////////////////////////////////////////////////////////////////
// Name:        dataview.h
// Purpose:     documentation for wxDataViewIconText class
// Author:      wxWidgets team
// RCS-ID:      $Id$
// Licence:     wxWindows license
/////////////////////////////////////////////////////////////////////////////

/**
    @class wxDataViewIconText
    @wxheader{dataview.h}

    wxDataViewIconText is used by
    wxDataViewIconTextRenderer
    for data transfer. This class can be converted to a from
    a wxVariant.

    @library{wxbase}
    @category{FIXME}
*/
class wxDataViewIconText : public wxObject
{
public:
    //@{
    /**
        Constructor.
    */
    wxDataViewIconText(const wxString& text = wxEmptyString,
                       const wxIcon& icon = wxNullIcon);
    wxDataViewIconText(const wxDataViewIconText& other);
    //@}

    /**
        Gets the icon.
    */
    const wxIcon GetIcon();

    /**
        Gets the text.
    */
    wxString GetText();

    /**
        Set the icon.
    */
    void SetIcon(const wxIcon& icon);

    /**
        Set the text.
    */
    void SetText(const wxString& text);
};


/**
    @class wxDataViewEvent
    @wxheader{dataview.h}

    wxDataViewEvent - the event class for the wxDataViewCtrl notifications

    @library{wxadv}
    @category{FIXME}
*/
class wxDataViewEvent : public wxNotifyEvent
{
public:
    //@{
    /**
        
    */
    wxDataViewEvent(wxEventType commandType = wxEVT_@NULL,
                    int winid = 0);
    wxDataViewEvent(const wxDataViewEvent& event);
    //@}

    /**
        Used to clone the event.
    */
    wxEvent* Clone();

    /**
        Returns the position of the column in the control or -1
        if no column field was set by the event emitter.
    */
    int GetColumn();

    /**
        Returns a pointer to the wxDataViewColumn from which
        the event was emitted or @NULL.
    */
    wxDataViewColumn* GetDataViewColumn();

    /**
        Returns the wxDataViewModel associated with the event.
    */
    wxDataViewModel* GetModel();

    /**
        Returns a the position of a context menu event in screen coordinates.
    */
    wxPoint GetPosition();

    /**
        Returns a reference to a value.
    */
    const wxVariant GetValue();

    /**
        
    */
    void SetColumn(int col);

    /**
        For wxEVT_DATAVIEW_COLUMN_HEADER_CLICKED only.
    */
    void SetDataViewColumn(wxDataViewColumn* col);

    /**
        
    */
    void SetModel(wxDataViewModel* model);

    /**
        
    */
    void SetValue(const wxVariant& value);
};


/**
    @class wxDataViewIconTextRenderer
    @wxheader{dataview.h}

    The wxDataViewIconTextRenderer class is used to display text with
    a small icon next to it as it is typically done in a file manager.
    This classes uses the wxDataViewIconText
    helper class to store its data. wxDataViewIonText can be converted
    to a from a wxVariant using the left shift
    operator.

    @library{wxadv}
    @category{FIXME}
*/
class wxDataViewIconTextRenderer : public wxDataViewRenderer
{
public:
    /**
        
    */
    wxDataViewIconTextRenderer(const wxString& varianttype = "wxDataViewIconText",
                               wxDataViewCellMode mode = wxDATAVIEW_CELL_INERT);
};


/**
    @class wxDataViewIndexListModel
    @wxheader{dataview.h}

    wxDataViewIndexListModel is a specialized data model which lets
    you address an item by its position (row) rather than its
    wxDataViewItem (which you can obtain from this class).
    This model also provides its own
    wxDataViewIndexListModel::Compare method
    which sorts the model's data by the index.

    This model is special in the it is implemented differently under OS X
    and other platforms. Under OS X a wxDataViewItem is always persistent
    and this is also the case for this class. Under other platforms, the
    meaning of a wxDataViewItem is changed to reflect a row number for
    wxDataViewIndexListModel. The consequence of this is that
    wxDataViewIndexListModel can be used as a virtual model with an
    almost infinate number of items on platforms other than OS X.

    @library{wxbase}
    @category{FIXME}
*/
class wxDataViewIndexListModel : public wxDataViewModel
{
public:
    /**
        Constructor.
    */
    wxDataViewIndexListModel(unsigned int initial_size = 0);

    /**
        Destructor.
    */
    ~wxDataViewIndexListModel();

    /**
        Compare method that sorts the items by their index.
    */
    int Compare(const wxDataViewItem& item1,
                const wxDataViewItem& item2,
                unsigned int column, bool ascending);

    /**
        Oberride this to indicate that the row has special font attributes.
        This only affects the
        wxDataViewTextRendererText renderer.
        
        See also wxDataViewItemAttr.
    */
    bool GetAttr(unsigned int row, unsigned int col,
                 wxDataViewItemAttr& attr);

    /**
        Returns the wxDataViewItem at the given @e row.
    */
    wxDataViewItem GetItem(unsigned int row);

    /**
        Returns the position of given @e item.
    */
    unsigned int GetRow(const wxDataViewItem& item);

    /**
        Override this to allow getting values from the model.
    */
    void GetValue(wxVariant& variant, unsigned int row,
                  unsigned int col);

    /**
        Call this after if the data has to be read again from
        the model. This is useful after major changes when
        calling the methods below (possibly thousands of times)
        doesn't make sense.
    */
    void Reset(unsigned int new_size);

    /**
        Call this after a row has been appended to the model.
    */
    void RowAppended();

    /**
        Call this after a row has been changed.
    */
    void RowChanged(unsigned int row);

    /**
        Call this after a row has been deleted.
    */
    void RowDeleted(unsigned int row);

    /**
        Call this after a row has been inserted at the given position.
    */
    void RowInserted(unsigned int before);

    /**
        Call this after a row has been prepended to the model.
    */
    void RowPrepended();

    /**
        Call this after a value has been changed.
    */
    void RowValueChanged(unsigned int row, unsigned int col);

    /**
        Call this after rows have been deleted. The array will internally
        get copied and sorted in descending order so that the rows with
        the highest position will be deleted first.
    */
    void RowsDeleted(const wxArrayInt& rows);

    /**
        Called in order to set a value in the model.
    */
    bool SetValue(const wxVariant& variant, unsigned int row,
                  unsigned int col);
};


/**
    @class wxDataViewModel
    @wxheader{dataview.h}

    wxDataViewModel is the base class for all data model to be
    displayed by a wxDataViewCtrl.
    All other models derive from it and must implement its
    pure virtual functions in order to define a complete
    data model. In detail, you need to override
    wxDataViewModel::IsContainer,
    wxDataViewModel::GetParent,
    wxDataViewModel::GetChildren,
    wxDataViewModel::GetColumnCount,
    wxDataViewModel::GetColumnType and
    wxDataViewModel::GetValue in order to
    define the data model which acts as an interface between
    your actual data and the wxDataViewCtrl. Since you will
    usually also allow the wxDataViewCtrl to change your data
    through its graphical interface, you will also have to override
    wxDataViewModel::SetValue which the
    wxDataViewCtrl will call when a change to some data has been
    commited.

    wxDataViewModel (as indeed the entire wxDataViewCtrl
    code) is using wxVariant to store data and
    its type in a generic way. wxVariant can be extended to contain
    almost any data without changes to the original class.

    The data that is presented through this data model is expected
    to change at run-time. You need to inform the data model when
    a change happened. Depending on what happened you need to call
    one of the following methods:
    wxDataViewModel::ValueChanged,
    wxDataViewModel::ItemAdded,
    wxDataViewModel::ItemDeleted,
    wxDataViewModel::ItemChanged,
    wxDataViewModel::Cleared. There are
    plural forms for notification of addition, change
    or removal of several item at once. See
    wxDataViewModel::ItemsAdded,
    wxDataViewModel::ItemsDeleted,
    wxDataViewModel::ItemsChanged.

    Note that wxDataViewModel does not define the position or
    index of any item in the control because different controls
    might display the same data differently. wxDataViewModel does
    provide a wxDataViewModel::Compare method
    which the wxDataViewCtrl may use to sort the data either
    in conjunction with a column header or without (see
    wxDataViewModel::HasDefaultCompare).

    This class maintains a list of
    wxDataViewModelNotifier
    which link this class to the specific implementations on the
    supported platforms so that e.g. calling
    wxDataViewModel::ValueChanged
    on this model will just call
    wxDataViewModelNotifier::ValueChanged
    for each notifier that has been added. You can also add
    your own notifier in order to get informed about any changes
    to the data in the list model.

    Currently wxWidgets provides the following models apart
    from the base model:
    wxDataViewIndexListModel,
    wxDataViewTreeStore.

    Note that wxDataViewModel is reference counted, derives from
    wxObjectRefData and cannot be deleted
    directly as it can be shared by several wxDataViewCtrls. This
    implies that you need to decrease the reference count after
    associating the model with a control like this:

    @code
    wxDataViewCtrl *musicCtrl = new wxDataViewCtrl( this, ID_MUSIC_CTRL );
        wxDataViewModel *musicModel = new MyMusicModel;
        m_musicCtrl-AssociateModel( musicModel );
        musicModel-DecRef();  // avoid memory leak !!
        // add columns now
    @endcode


    @library{wxadv}
    @category{FIXME}
*/
class wxDataViewModel : public wxObjectRefData
{
public:
    /**
        Constructor.
    */
    wxDataViewModel();

    /**
        Destructor. This should not be called directly. Use DecRef() instead.
    */
    ~wxDataViewModel();

    /**
        Adds a wxDataViewModelNotifier
        to the model.
    */
    void AddNotifier(wxDataViewModelNotifier* notifier);

    /**
        Called to inform the model that all data has been cleared. The
        control will reread the data from the model again.
    */
    virtual bool Cleared();

    /**
        The compare function to be used by control. The default compare function
        sorts by container and other items separately and in ascending order.
        Override this for a different sorting behaviour.
        
        See also HasDefaultCompare().
    */
    virtual int Compare(const wxDataViewItem& item1,
                        const wxDataViewItem& item2,
                        unsigned int column,
                        bool ascending);

    /**
        Oberride this to indicate that the item has special font attributes.
        This only affects the
        wxDataViewTextRendererText renderer.
        
        See also wxDataViewItemAttr.
    */
    bool GetAttr(const wxDataViewItem& item, unsigned int col,
                 wxDataViewItemAttr& attr);

    /**
        Override this so the control can query the child items of
        an item. Returns the number of items.
    */
    virtual unsigned int GetChildren(const wxDataViewItem& item,
                                     wxDataViewItemArray& children);

    /**
        Override this to indicate the number of columns in the model.
    */
    virtual unsigned int GetColumnCount();

    /**
        Override this to indicate what type of data is stored in the
        column specified by @e col. This should return a string
        indicating the type of data as reported by wxVariant.
    */
    virtual wxString GetColumnType(unsigned int col);

    /**
        Override this to indicate which wxDataViewItem representing the parent
        of @e item or an invalid wxDataViewItem if the the root item is
        the parent item.
    */
    virtual wxDataViewItem GetParent(const wxDataViewItem& item);

    /**
        Override this to indicate the value of @e item
        A wxVariant is used to store the data.
    */
    virtual void GetValue(wxVariant& variant,
                          const wxDataViewItem& item,
                          unsigned int col);

    /**
        Override this method to indicate if a container item merely
        acts as a headline (or for categorisation) or if it also
        acts a normal item with entries for futher columns. By
        default returns @e @false.
    */
    virtual bool HasContainerColumns(const wxDataViewItem& item);

    /**
        Override this to indicate that the model provides a default compare
        function that the control should use if no wxDataViewColumn has been
        chosen for sorting. Usually, the user clicks on a column header for
        sorting, the data will be sorted alphanumerically. If any other
        order (e.g. by index or order of appearance) is required, then this
        should be used. See also wxDataViewIndexListModel
        for a model which makes use of this.
    */
    virtual bool HasDefaultCompare();

    /**
        Override this to indicate of @e item is a container, i.e. if
        it can have child items.
    */
    virtual bool IsContainer(const wxDataViewItem& item);

    /**
        Call this to inform the model that an item has been added
        to the data.
    */
    virtual bool ItemAdded(const wxDataViewItem& parent,
                           const wxDataViewItem& item);

    /**
        Call this to inform the model that an item has changed.
        
        This will eventually emit a wxEVT_DATAVIEW_ITEM_VALUE_CHANGED
        event (in which the column fields will not be set) to the user.
    */
    virtual bool ItemChanged(const wxDataViewItem& item);

    /**
        Call this to inform the model that an item has been deleted from the data.
    */
    virtual bool ItemDeleted(const wxDataViewItem& parent,
                             const wxDataViewItem& item);

    /**
        Call this to inform the model that several items have been added
        to the data.
    */
    virtual bool ItemsAdded(const wxDataViewItem& parent,
                            const wxDataViewItemArray& items);

    /**
        Call this to inform the model that several items have changed.
        
        This will eventually emit wxEVT_DATAVIEW_ITEM_VALUE_CHANGED
        events (in which the column fields will not be set) to the user.
    */
    virtual bool ItemsChanged(const wxDataViewItemArray& items);

    /**
        Call this to inform the model that several items have been deleted.
    */
    virtual bool ItemsDeleted(const wxDataViewItem& parent,
                              const wxDataViewItemArray& items);

    /**
        Remove the @e notifier from the list of notifiers.
    */
    void RemoveNotifier(wxDataViewModelNotifier* notifier);

    /**
        Call this to initiate a resort after the sort function has
        been changed.
    */
    virtual void Resort();

    /**
        This gets called in order to set a value in the data model.
        The most common scenario is that the wxDataViewCtrl calls
        this method after the user changed some data in the view.
        Afterwards ValueChanged()
        has to be called!
    */
    virtual bool SetValue(const wxVariant& variant,
                          const wxDataViewItem& item,
                          unsigned int col);

    /**
        Call this to inform this model that a value in the model has
        been changed. This is also called from wxDataViewCtrl's
        internal editing code, e.g. when editing a text field
        in the control.
        
        This will eventually emit a wxEVT_DATAVIEW_ITEM_VALUE_CHANGED
        event to the user.
    */
    virtual bool ValueChanged(const wxDataViewItem& item,
                              unsigned int col);
};


/**
    @class wxDataViewCustomRenderer
    @wxheader{dataview.h}

    You need to derive a new class from wxDataViewCustomRenderer in
    order to write a new renderer. You need to override at least
    wxDataViewRenderer::SetValue,
    wxDataViewRenderer::GetValue,
    wxDataViewCustomRenderer::GetSize
    and wxDataViewCustomRenderer::Render.

    If you want your renderer to support in-place editing then you
    also need to override
    wxDataViewCustomRenderer::HasEditorCtrl,
    wxDataViewCustomRenderer::CreateEditorCtrl
    and wxDataViewCustomRenderer::GetValueFromEditorCtrl.
    Note that a special event handler will be pushed onto that
    editor control which handles ENTER and focus out events
    in order to end the editing.

    @library{wxadv}
    @category{FIXME}
*/
class wxDataViewCustomRenderer : public wxDataViewRenderer
{
public:
    /**
        Constructor.
    */
    wxDataViewCustomRenderer(const wxString& varianttype = "string",
                             wxDataViewCellMode mode = wxDATAVIEW_CELL_INERT,
                             bool no_init = @false);

    /**
        Destructor.
    */
    ~wxDataViewCustomRenderer();

    /**
        Override this to react to double clicks or ENTER.
    */
    virtual bool Activate(wxRect cell, wxDataViewModel* model,
                          unsigned int col,
                          unsigned int row);

    /**
        Override this to create the actual editor control once editing
        is about to start. @e parent is the parent of the editor
        control, @e labelRect indicates the position and
        size of the editor control and @e value is its initial value:
    */
    virtual wxControl* CreateEditorCtrl(wxWindow * parent,
                                        wxRect labelRect,
                                        const wxVariant & value);

    /**
        Create DC on request. Internal.
    */
#define virtual wxDC* GetDC()     /* implementation is private */

    /**
        Return size required to show content.
    */
    virtual wxSize GetSize();

    /**
        Overrride this so that the renderer can get the value
        from the editor control (pointed to by @e editor):
    */
    virtual bool GetValueFromEditorCtrl(wxControl* editor,
                                        wxVariant & value);

    /**
        Override this and make it return @e @true in order to
        indicate that this renderer supports in-place editing.
    */
    virtual bool HasEditorCtrl();

    /**
        Overrride this to react to a left click.
    */
    virtual bool LeftClick(wxPoint cursor, wxRect cell,
                           wxDataViewModel* model,
                           unsigned int col,
                           unsigned int row);

    /**
        Override this to render the cell. Before this is called,
        wxDataViewRenderer::SetValue was called
        so that this instance knows what to render.
    */
    virtual bool Render(wxRect cell, wxDC* dc, int state);

    /**
        This method should be called from within Render()
        whenever you need to render simple text. This will ensure that the
        correct colour, font and vertical alignment will be chosen so the
        text will look the same as text drawn by native renderers.
    */
    bool RenderText(const wxString& text, int xoffset, wxRect cell,
                    wxDC* dc, int state);

    /**
        Overrride this to react to a right click.
    */
    virtual bool RightClick(wxPoint cursor, wxRect cell,
                            wxDataViewModel* model,
                            unsigned int col,
                            unsigned int row);

    /**
        Overrride this to start a drag operation.
    */
    virtual bool StartDrag(wxPoint cursor, wxRect cell,
                           wxDataViewModel* model,
                           unsigned int col,
                           unsigned int row);
};


/**
    @class wxDataViewBitmapRenderer
    @wxheader{dataview.h}

    wxDataViewBitmapRenderer

    @library{wxadv}
    @category{FIXME}
*/
class wxDataViewBitmapRenderer : public wxDataViewRenderer
{
public:
    /**
        
    */
    wxDataViewBitmapRenderer(const wxString& varianttype = "wxBitmap",
                             wxDataViewCellMode mode = wxDATAVIEW_CELL_INERT);
};


/**
    @class wxDataViewItemAttr
    @wxheader{dataview.h}

    This class is used to indicate to a wxDataViewCtrl
    that a certain Item has extra font attributes
    for its renderer. For this, it is required to override
    wxDataViewModel::GetAttr.

    Attributes are currently only supported by
    wxDataViewTextRendererText.

    @library{wxadv}
    @category{FIXME}
*/
class wxDataViewItemAttr
{
public:
    /**
        Constructor.
    */
    wxDataViewItemAttr();

    /**
        Call this to indicate that the item shall be displayed in bold text.
    */
    void SetBold(bool set);

    /**
        Call this to indicate that the item shall be displayed with
        that colour.
    */
    void SetColour(const wxColour& colour);

    /**
        Call this to indicate that the item shall be displayed in italic text.
    */
    void SetItalic(bool set);
};


/**
    @class wxDataViewItem
    @wxheader{dataview.h}

    wxDataViewItem is a small opaque class that represents an
    item in a wxDataViewCtrl in a
    persistent way, i.e. indepent of the position of the
    item in the control or changes to its contents. It must
    hold a unique ID of type @e void* in its only field
    and can be converted to a from it.

    If the ID is @e @NULL the wxDataViewItem is invalid and
    wxDataViewItem::IsOk will return @e @false
    which used in many places in the API of wxDataViewCtrl
    to indicate that e.g. no item was found. An ID of @NULL
    is also used to indicate the invisible root. Examples
    for this are
    wxDataViewModel::GetParent and
    wxDataViewModel::GetChildren.

    @library{wxadv}
    @category{FIXME}
*/
class wxDataViewItem
{
public:
    //@{
    /**
        
    */
    wxDataViewItem(void* id = @NULL);
    wxDataViewItem(const wxDataViewItem& item);
    //@}

    /**
        Returns the ID.
    */
#define void* GetID()     /* implementation is private */

    /**
        Returns @true if the ID is not @e @NULL.
    */
#define bool IsOk()     /* implementation is private */
};


/**
    @class wxDataViewCtrl
    @wxheader{dataview.h}

    wxDataViewCtrl is a control to display data either
    in a tree like fashion or in a tabular form or both.
    If you only need to display a simple tree structure
    with an API more like the older wxTreeCtrl class,
    then the specialized wxDataViewTreeCtrl
    can be used.

    A wxDataViewItem is used
    to represent a (visible) item in the control.

    Unlike wxListCtrl wxDataViewCtrl doesn't
    get its data from the user through virtual functions or by
    setting it directly. Instead you need to write your own
    wxDataViewModel and associate
    it with this control. Then you need to add a number of
    wxDataViewColumn to this control to
    define what each column shall display. Each wxDataViewColumn
    in turn owns 1 instance of a
    wxDataViewRenderer to render its
    cells. A number of standard renderers for rendering text, dates,
    images, toggle, a progress bar etc. are provided. Additionally,
    the user can write custom renderes deriving from
    wxDataViewCustomRenderer
    for displaying anything.

    All data transfer from the control to the model and the user
    code is done through wxVariant which can
    be extended to support more data formats as necessary.
    Accordingly, all type information uses the strings returned
    from wxVariant::GetType.

    @beginStyleTable
    @style{wxDV_SINGLE}:
           Single selection mode. This is the default.
    @style{wxDV_MULTIPLE}:
           Multiple selection mode.
    @style{wxDV_ROW_LINES}:
           Use alternating colours for rows if supported by platform and theme.
    @style{wxDV_HORIZ_RULES}:
           Display fine rules between row if supported.
    @style{wxDV_VERT_RULES}:
           Display fine rules between columns is supported.
    @endStyleTable

    @library{wxadv}
    @category{ctrl}
    @appearance{dataviewctrl.png}
*/
class wxDataViewCtrl : public wxControl
{
public:
    //@{
    /**
        Constructor. Calls Create().
    */
    wxDataViewCtrl();
    wxDataViewCtrl(wxWindow* parent, wxWindowID id,
                   const wxPoint& pos = wxDefaultPosition,
                   const wxSize& size = wxDefaultSize,
                   long style = 0,
                   const wxValidator& validator = wxDefaultValidator);
    //@}

    /**
        Destructor.
    */
    ~wxDataViewCtrl();

    //@{
    /**
        Appends a column for rendering a bitmap. Returns the wxDataViewColumn
        created in the function or @NULL on failure.
    */
    wxDataViewColumn* AppendBitmapColumn(const wxString& label,
                                         unsigned int model_column,
                                         wxDataViewCellMode mode = wxDATAVIEW_CELL_INERT,
                                         int width = -1,
                                         wxAlignment align = wxALIGN_CENTER,
                                         int flags = wxDATAVIEW_COL_RESIZABLE);
    wxDataViewColumn* AppendBitmapColumn(const wxBitmap& label,
                                         unsigned int model_column,
                                         wxDataViewCellMode mode = wxDATAVIEW_CELL_INERT,
                                         int width = -1,
                                         wxAlignment align = wxALIGN_CENTER,
                                         int flags = wxDATAVIEW_COL_RESIZABLE);
    //@}

    /**
        Add a wxDataViewColumn to the control. Returns
        @e @true on success.
        
        Note that there is a number of short cut methods which implicitly create
        a wxDataViewColumn and a
        wxDataViewRenderer for it (see below).
    */
    virtual bool AppendColumn(wxDataViewColumn* col);

    //@{
    /**
        Appends a column for rendering a date. Returns the wxDataViewColumn
        created in the function or @NULL on failure.
    */
    wxDataViewColumn* AppendDateColumn(const wxString& label,
                                       unsigned int model_column,
                                       wxDataViewCellMode mode = wxDATAVIEW_CELL_ACTIVATABLE,
                                       int width = -1,
                                       wxAlignment align = wxALIGN_CENTER,
                                       int flags = wxDATAVIEW_COL_RESIZABLE);
    wxDataViewColumn* AppendDateColumn(const wxBitmap& label,
                                       unsigned int model_column,
                                       wxDataViewCellMode mode = wxDATAVIEW_CELL_ACTIVATABLE,
                                       int width = -1,
                                       wxAlignment align = wxALIGN_CENTER,
                                       int flags = wxDATAVIEW_COL_RESIZABLE);
    //@}

    //@{
    /**
        Appends a column for rendering text with an icon. Returns the wxDataViewColumn
        created in the function or @NULL on failure. This uses the
        wxDataViewIconTextRenderer.
    */
    wxDataViewColumn* AppendIconTextColumn(const wxString& label,
                                           unsigned int model_column,
                                           wxDataViewCellMode mode = wxDATAVIEW_CELL_INERT,
                                           int width = -1,
                                           wxAlignment align = wxALIGN_LEFT,
                                           int flags = wxDATAVIEW_COL_RESIZABLE);
    wxDataViewColumn* AppendIconTextColumn(const wxBitmap& label,
                                           unsigned int model_column,
                                           wxDataViewCellMode mode = wxDATAVIEW_CELL_INERT,
                                           int width = -1,
                                           wxAlignment align = wxALIGN_LEFT,
                                           int flags = wxDATAVIEW_COL_RESIZABLE);
    //@}

    //@{
    /**
        Appends a column for rendering a progress indicator. Returns the
        wxDataViewColumn
        created in the function or @NULL on failure.
    */
    wxDataViewColumn* AppendProgressColumn(const wxString& label,
                                           unsigned int model_column,
                                           wxDataViewCellMode mode = wxDATAVIEW_CELL_INERT,
                                           int width = 80,
                                           wxAlignment align = wxALIGN_CENTER,
                                           int flags = wxDATAVIEW_COL_RESIZABLE);
    wxDataViewColumn* AppendProgressColumn(const wxBitmap& label,
                                           unsigned int model_column,
                                           wxDataViewCellMode mode = wxDATAVIEW_CELL_INERT,
                                           int width = 80,
                                           wxAlignment align = wxALIGN_CENTER,
                                           int flags = wxDATAVIEW_COL_RESIZABLE);
    //@}

    //@{
    /**
        Appends a column for rendering text. Returns the wxDataViewColumn
        created in the function or @NULL on failure.
    */
    wxDataViewColumn* AppendTextColumn(const wxString& label,
                                       unsigned int model_column,
                                       wxDataViewCellMode mode = wxDATAVIEW_CELL_INERT,
                                       int width = -1,
                                       wxAlignment align = wxALIGN_LEFT,
                                       int flags = wxDATAVIEW_COL_RESIZABLE);
    wxDataViewColumn* AppendTextColumn(const wxBitmap& label,
                                       unsigned int model_column,
                                       wxDataViewCellMode mode = wxDATAVIEW_CELL_INERT,
                                       int width = -1,
                                       wxAlignment align = wxALIGN_LEFT,
                                       int flags = wxDATAVIEW_COL_RESIZABLE);
    //@}

    //@{
    /**
        Appends a column for rendering a toggle. Returns the wxDataViewColumn
        created in the function or @NULL on failure.
    */
    wxDataViewColumn* AppendToggleColumn(const wxString& label,
                                         unsigned int model_column,
                                         wxDataViewCellMode mode = wxDATAVIEW_CELL_INERT,
                                         int width = 30,
                                         wxAlignment align = wxALIGN_CENTER,
                                         int flags = wxDATAVIEW_COL_RESIZABLE);
    wxDataViewColumn* AppendToggleColumn(const wxBitmap& label,
                                         unsigned int model_column,
                                         wxDataViewCellMode mode = wxDATAVIEW_CELL_INERT,
                                         int width = 30,
                                         wxAlignment align = wxALIGN_CENTER,
                                         int flags = wxDATAVIEW_COL_RESIZABLE);
    //@}

    /**
        Associates a wxDataViewModel with the
        control. This increases the reference count of the model by 1.
    */
    virtual bool AssociateModel(wxDataViewModel* model);

    /**
        Removes all columns.
    */
    virtual bool ClearColumns();

    /**
        Unselects all rows.
    */
    void ClearSelection();

    /**
        Collapses the item.
    */
    void Collapse(const wxDataViewItem & item);

    /**
        Create the control. Useful for two step creation.
    */
    bool Create(wxWindow* parent, wxWindowID id,
                const wxPoint& pos = wxDefaultPosition,
                const wxSize& size = wxDefaultSize,
                long style = 0,
                const wxValidator& validator = wxDefaultValidator);

    /**
        Deletes given column.
    */
    virtual bool DeleteColumn(const wxDataViewColumn* column);

    /**
        Call this to ensure that the given item is visible.
    */
    void EnsureVisible(const wxDataViewItem & item,
                       const wxDataViewColumn* column = @NULL);

    /**
        Expands the item.
    */
    void Expand(const wxDataViewItem & item);

    /**
        Returns pointer to the column. @e pos refers to the
        position in the control which may change after reordering
        columns by the user.
    */
    virtual wxDataViewColumn* GetColumn(unsigned int pos);

    /**
        Returns the number of columns.
    */
    virtual unsigned int GetColumnCount();

    /**
        Returns the position of the column or -1 if not found in the control.
    */
    virtual int GetColumnPosition(const wxDataViewColumn* column);

    /**
        Returns column containing the expanders.
    */
    wxDataViewColumn * GetExpanderColumn();

    /**
        Returns indentation.
    */
    int GetIndent();

    /**
        Returns item rect.
    */
    wxRect GetItemRect(const wxDataViewItem& item,
                       const wxDataViewColumn * col = @NULL);

    /**
        Returns pointer to the data model associated with the
        control (if any).
    */
    virtual wxDataViewModel* GetModel();

    /**
        Returns first selected item or an invalid item if none is selected.
    */
    wxDataViewItem GetSelection();

    /**
        Fills @e sel with currently selected items and returns
        their number.
    */
    int GetSelections(wxDataViewItemArray & sel);

    /**
        Returns the wxDataViewColumn currently responsible for sorting
        or @NULL if none has been selected.
    */
    virtual wxDataViewColumn* GetSortingColumn();

    /**
        Hittest.
    */
    void HitTest(const wxPoint& point, wxDataViewItem& item,
                 wxDataViewColumn *& col);

    /**
        Return @true if the item is selected.
    */
    bool IsSelected(const wxDataViewItem & item);

    /**
        Select the given item.
    */
    void Select(const wxDataViewItem & item);

    /**
        Select all items.
    */
    void SelectAll();

    /**
        Set which column shall contain the tree-like expanders.
    */
    void SetExpanderColumn(wxDataViewColumn * col);

    /**
        Sets the indendation.
    */
    void SetIndent(int indent);

    /**
        Sets the selection to the array of wxDataViewItems.
    */
    void SetSelections(const wxDataViewItemArray & sel);

    /**
        Unselect the given item.
    */
    void Unselect(const wxDataViewItem & item);

    /**
        Unselect all item. This method only has effect if multiple
        selections are allowed.
    */
    void UnselectAll();
};


/**
    @class wxDataViewModelNotifier
    @wxheader{dataview.h}

    A wxDataViewModelNotifier instance is owned by a
    wxDataViewModel
    and mirrors its notification interface. See
    the documentation of that class for further
    information.

    @library{wxbase}
    @category{FIXME}
*/
class wxDataViewModelNotifier
{
public:
    /**
        Constructor.
    */
    wxDataViewModelNotifier();

    /**
        Destructor.
    */
    ~wxDataViewModelNotifier();

    /**
        Called by owning model.
    */
    bool Cleared();

    /**
        Get owning wxDataViewModel.
    */
    wxDataViewModel* GetOwner();

    /**
        Called by owning model.
    */
    bool ItemAdded(const wxDataViewItem& parent,
                   const wxDataViewItem& item);

    /**
        Called by owning model.
    */
    bool ItemChanged(const wxDataViewItem& item);

    /**
        Called by owning model.
    */
    bool ItemDeleted(const wxDataViewItem& parent,
                     const wxDataViewItem& item);

    /**
        Called by owning model.
    */
    bool ItemsAdded(const wxDataViewItem& parent,
                    const wxDataViewItemArray& items);

    /**
        Called by owning model.
    */
    bool ItemsChanged(const wxDataViewItemArray& items);

    /**
        Called by owning model.
    */
    bool ItemsDeleted(const wxDataViewItem& parent,
                      const wxDataViewItemArray& items);

    /**
        Called by owning model.
    */
    void Resort();

    /**
        Set owner of this notifier. Used internally.
    */
    void SetOwner(wxDataViewModel* owner);

    /**
        Called by owning model.
    */
    bool ValueChanged(const wxDataViewItem& item, unsigned int col);
};


/**
    @class wxDataViewRenderer
    @wxheader{dataview.h}

    This class is used by wxDataViewCtrl to
    render the individual cells. One instance of a renderer class is
    owned by wxDataViewColumn. There is
    a number of ready-to-use renderers provided:
    wxDataViewTextRenderer,
    wxDataViewTextRendererAttr,
    wxDataViewIconTextRenderer,
    wxDataViewToggleRenderer,
    wxDataViewProgressRenderer,
    wxDataViewBitmapRenderer,
    wxDataViewDateRenderer.
    wxDataViewSpinRenderer.

    Additionally, the user can write own renderers by deriving from
    wxDataViewCustomRenderer.

    The @e wxDataViewCellMode flag controls, what actions
    the cell data allows. @e wxDATAVIEW_CELL_ACTIVATABLE
    indicates that the user can double click the cell and
    something will happen (e.g. a window for editing a date
    will pop up). @e wxDATAVIEW_CELL_EDITABLE indicates
    that the user can edit the data in-place, i.e. an control
    will show up after a slow click on the cell. This behaviour
    is best known from changing the filename in most file
    managers etc.


    @code
    enum wxDataViewCellMode
    {
        wxDATAVIEW_CELL_INERT,
        wxDATAVIEW_CELL_ACTIVATABLE,
        wxDATAVIEW_CELL_EDITABLE
    };
    @endcode

    The @e wxDataViewCellRenderState flag controls how the
    the renderer should display its contents in a cell:

    @code
    enum wxDataViewCellRenderState
    {
        wxDATAVIEW_CELL_SELECTED    = 1,
        wxDATAVIEW_CELL_PRELIT      = 2,
        wxDATAVIEW_CELL_INSENSITIVE = 4,
        wxDATAVIEW_CELL_FOCUSED     = 8
    };
    @endcode


    @library{wxadv}
    @category{FIXME}
*/
class wxDataViewRenderer : public wxObject
{
public:
    /**
        Constructor.
    */
    wxDataViewRenderer(const wxString& varianttype,
                       wxDataViewCellMode mode = wxDATAVIEW_CELL_INERT,
                       int alignment = wxALIGN_LEFT|wxALIGN_CENTRE_VERTICAL);

    /**
        Returns the cell mode.
    */
    virtual wxDataViewCellMode GetMode();

    /**
        Returns pointer to the owning wxDataViewColumn.
    */
    virtual wxDataViewColumn* GetOwner();

    /**
        This methods retrieves the value from the renderer in order to
        transfer the value back to the data model. Returns @e @false
        on failure.
    */
    virtual bool GetValue(wxVariant& value);

    /**
        Returns a string with the type of the wxVariant
        supported by this renderer.
    */
    virtual wxString GetVariantType();

    /**
        Sets the owning wxDataViewColumn. This
        is usually called from within wxDataViewColumn.
    */
    virtual void SetOwner(wxDataViewColumn* owner);

    /**
        Set the value of the renderer (and thus its cell) to @e value.
        The internal code will then render this cell with this data.
    */
    virtual bool SetValue(const wxVariant& value);

    /**
        Before data is committed to the data model, it is passed to this
        method where it can be checked for validity. This can also be
        used for checking a valid range or limiting the user input in
        a certain aspect (e.g. max number of characters or only alphanumeric
        input, ASCII only etc.). Return @e @false if the value is
        not valid.
        
        Please note that due to implementation limitations, this validation
        is done after the editing control already is destroyed and the
        editing process finished.
    */
    virtual bool Validate(wxVariant& value);
};


/**
    @class wxDataViewTextRenderer
    @wxheader{dataview.h}

    wxDataViewTextRenderer is used for rendering text. It supports
    in-place editing if desired.

    @library{wxadv}
    @category{FIXME}
*/
class wxDataViewTextRenderer : public wxDataViewRenderer
{
public:
    /**
        
    */
    wxDataViewTextRenderer(const wxString& varianttype = "string",
                           wxDataViewCellMode mode = wxDATAVIEW_CELL_INERT);
};


/**
    @class wxDataViewProgressRenderer
    @wxheader{dataview.h}

    wxDataViewProgressRenderer

    @library{wxadv}
    @category{FIXME}
*/
class wxDataViewProgressRenderer : public wxDataViewRenderer
{
public:
    /**
        
    */
    wxDataViewProgressRenderer(const wxString& label = wxEmptyString,
                               const wxString& varianttype = "long",
                               wxDataViewCellMode mode = wxDATAVIEW_CELL_INERT);
};


/**
    @class wxDataViewSpinRenderer
    @wxheader{dataview.h}

    This is a specialized renderer for rendering integer values. It
    supports modifying the values in-place by using a wxSpinCtrl.
    The renderer only support variants of type @e long.

    @library{wxbase}
    @category{FIXME}
*/
class wxDataViewSpinRenderer : public wxDataViewCustomRenderer
{
public:
    /**
        Constructor. @e min and @e max indicate the minimum und
        maximum values of for the wxSpinCtrl.
    */
    wxDataViewSpinRenderer(int min, int max,
                           wxDataViewCellMode mode = wxDATAVIEW_CELL_EDITABLE,
                           int alignment = wxDVR_DEFAULT_ALIGNMENT);
};


/**
    @class wxDataViewToggleRenderer
    @wxheader{dataview.h}

    wxDataViewToggleRenderer

    @library{wxadv}
    @category{FIXME}
*/
class wxDataViewToggleRenderer : public wxDataViewRenderer
{
public:
    /**
        
    */
    wxDataViewToggleRenderer(const wxString& varianttype = "bool",
                             wxDataViewCellMode mode = wxDATAVIEW_CELL_INERT);
};


/**
    @class wxDataViewTreeCtrl
    @wxheader{dataview.h}

    This class is a wxDataViewCtrl which internally
    uses a wxDataViewTreeStore and forwards
    most of its API to that class. Additionally, it uses a wxImageList
    to store a list of icons. The main purpose of this class is to look
    like a wxTreeCtrl to make a transition from it
    to the wxDataViewCtrl class simpler.

    @library{wxbase}
    @category{ctrl}
    @appearance{dataviewtreectrl.png}
*/
class wxDataViewTreeCtrl : public wxDataViewCtrl
{
public:
    //@{
    /**
        Constructor. Calls Create().
    */
    wxDataViewTreeCtrl();
    wxDataViewTreeCtrl(wxWindow* parent, wxWindowID id,
                       const wxPoint& pos = wxDefaultPosition,
                       const wxSize& size = wxDefaultSize,
                       long style = wxDV_NO_HEADER,
                       const wxValidator& validator = wxDefaultValidator);
    //@}

    /**
        Destructor. Deletes the image list if any.
    */
    ~wxDataViewTreeCtrl();

    /**
        
    */
    wxDataViewItem AppendContainer(const wxDataViewItem& parent,
                                   const wxString& text,
                                   int icon = -1,
                                   int expanded = -1,
                                   wxClientData* data = @NULL);

    /**
        
    */
    wxDataViewItem AppendItem(const wxDataViewItem& parent,
                              const wxString& text,
                              int icon = -1,
                              wxClientData* data = @NULL);

    /**
        Creates the control and a wxDataViewTreeStore as
        its internal model.
    */
    bool Create(wxWindow* parent, wxWindowID id,
                const wxPoint& pos = wxDefaultPosition,
                const wxSize& size = wxDefaultSize,
                long style = wxDV_NO_HEADER,
                const wxValidator& validator = wxDefaultValidator);

    /**
        Calls the identical method from wxDataViewTreeStore.
    */
    void DeleteAllItems();

    /**
        Calls the identical method from wxDataViewTreeStore.
    */
    void DeleteChildren(const wxDataViewItem& item);

    /**
        Calls the identical method from wxDataViewTreeStore.
    */
    void DeleteItem(const wxDataViewItem& item);

    /**
        Calls the identical method from wxDataViewTreeStore.
    */
    int GetChildCount(const wxDataViewItem& parent);

    /**
        Returns the image list.
    */
    wxImageList* GetImageList();

    /**
        Calls the identical method from wxDataViewTreeStore.
    */
    wxClientData* GetItemData(const wxDataViewItem& item);

    /**
        Calls the identical method from wxDataViewTreeStore.
    */
    const wxIcon GetItemExpandedIcon(const wxDataViewItem& item);

    /**
        Calls the identical method from wxDataViewTreeStore.
    */
    const wxIcon GetItemIcon(const wxDataViewItem& item);

    /**
        Calls the identical method from wxDataViewTreeStore.
    */
    wxString GetItemText(const wxDataViewItem& item);

    /**
        Calls the identical method from wxDataViewTreeStore.
    */
    wxDataViewItem GetNthChild(const wxDataViewItem& parent,
                               unsigned int pos);

    //@{
    /**
        Returns the store.
    */
    wxDataViewTreeStore* GetStore();
    const wxDataViewTreeStore* GetStore();
    //@}

    /**
        Calls the same method from wxDataViewTreeStore but uess
        and index position in the image list instead of a wxIcon.
    */
    wxDataViewItem InsertContainer(const wxDataViewItem& parent,
                                   const wxDataViewItem& previous,
                                   const wxString& text,
                                   int icon = -1,
                                   int expanded = -1,
                                   wxClientData* data = @NULL);

    /**
        Calls the same method from wxDataViewTreeStore but uess
        and index position in the image list instead of a wxIcon.
    */
    wxDataViewItem InsertItem(const wxDataViewItem& parent,
                              const wxDataViewItem& previous,
                              const wxString& text,
                              int icon = -1,
                              wxClientData* data = @NULL);

    /**
        Calls the same method from wxDataViewTreeStore but uess
        and index position in the image list instead of a wxIcon.
    */
    wxDataViewItem PrependContainer(const wxDataViewItem& parent,
                                    const wxString& text,
                                    int icon = -1,
                                    int expanded = -1,
                                    wxClientData* data = @NULL);

    /**
        Calls the same method from wxDataViewTreeStore but uess
        and index position in the image list instead of a wxIcon.
    */
    wxDataViewItem PrependItem(const wxDataViewItem& parent,
                               const wxString& text,
                               int icon = -1,
                               wxClientData* data = @NULL);

    /**
        Sets the image list.
    */
    void SetImageList(wxImageList* imagelist);

    /**
        Calls the identical method from wxDataViewTreeStore.
    */
    void SetItemData(const wxDataViewItem& item, wxClientData* data);

    /**
        Calls the identical method from wxDataViewTreeStore.
    */
    void SetItemExpandedIcon(const wxDataViewItem& item,
                             const wxIcon& icon);

    /**
        Calls the identical method from wxDataViewTreeStore.
    */
    void SetItemIcon(const wxDataViewItem& item, const wxIcon& icon);

    /**
        Calls the identical method from wxDataViewTreeStore.
    */
    void SetItemText(const wxDataViewItem& item,
                     const wxString& text);
};


/**
    @class wxDataViewTreeStore
    @wxheader{dataview.h}

    wxDataViewTreeStore is a specialised wxDataViewModel
    for displaying simple trees very much like wxTreeCtrl
    does and it offers a similar API. This class actually stores the entire
    tree (therefore its name) and implements all virtual methods from the base
    class so it can be used directly without having to derive any class from it.
    This comes at the price of much reduced flexibility.

    @library{wxadv}
    @category{FIXME}
*/
class wxDataViewTreeStore : public wxDataViewModel
{
public:
    /**
        Constructor. Creates the invisible root node internally.
    */
    wxDataViewTreeStore();

    /**
        Destructor.
    */
    ~wxDataViewTreeStore();

    /**
        Append a container.
    */
    wxDataViewItem AppendContainer(const wxDataViewItem& parent,
                                   const wxString& text,
                                   const wxIcon& icon = wxNullIcon,
                                   const wxIcon& expanded = wxNullIcon,
                                   wxClientData* data = @NULL);

    /**
        Append an item.
    */
    wxDataViewItem AppendItem(const wxDataViewItem& parent,
                              const wxString& text,
                              const wxIcon& icon = wxNullIcon,
                              wxClientData* data = @NULL);

    /**
        Delete all item in the model.
    */
    void DeleteAllItems();

    /**
        Delete all children of the item, but not the item itself.
    */
    void DeleteChildren(const wxDataViewItem& item);

    /**
        Delete this item.
    */
    void DeleteItem(const wxDataViewItem& item);

    /**
        Return the number of children of item.
    */
    int GetChildCount(const wxDataViewItem& parent);

    /**
        Returns the client data asoociated with the item.
    */
    wxClientData* GetItemData(const wxDataViewItem& item);

    /**
        Returns the icon to display in expanded containers.
    */
    const wxIcon GetItemExpandedIcon(const wxDataViewItem& item);

    /**
        Returns the icon of the item.
    */
    const wxIcon GetItemIcon(const wxDataViewItem& item);

    /**
        Returns the text of the item.
    */
    wxString GetItemText(const wxDataViewItem& item);

    /**
        Returns the nth child item of item.
    */
    wxDataViewItem GetNthChild(const wxDataViewItem& parent,
                               unsigned int pos);

    /**
        Inserts a container after @e previous.
    */
    wxDataViewItem InsertContainer(const wxDataViewItem& parent,
                                   const wxDataViewItem& previous,
                                   const wxString& text,
                                   const wxIcon& icon = wxNullIcon,
                                   const wxIcon& expanded = wxNullIcon,
                                   wxClientData* data = @NULL);

    /**
        Inserts an item after @e previous.
    */
    wxDataViewItem InsertItem(const wxDataViewItem& parent,
                              const wxDataViewItem& previous,
                              const wxString& text,
                              const wxIcon& icon = wxNullIcon,
                              wxClientData* data = @NULL);

    /**
        Inserts a container before the first child item or @e parent.
    */
    wxDataViewItem PrependContainer(const wxDataViewItem& parent,
                                    const wxString& text,
                                    const wxIcon& icon = wxNullIcon,
                                    const wxIcon& expanded = wxNullIcon,
                                    wxClientData* data = @NULL);

    /**
        Inserts an item before the first child item or @e parent.
    */
    wxDataViewItem PrependItem(const wxDataViewItem& parent,
                               const wxString& text,
                               const wxIcon& icon = wxNullIcon,
                               wxClientData* data = @NULL);

    /**
        Sets the client data associated with the item.
    */
    void SetItemData(const wxDataViewItem& item, wxClientData* data);

    /**
        Sets the expanded icon for the item.
    */
    void SetItemExpandedIcon(const wxDataViewItem& item,
                             const wxIcon& icon);

    /**
        Sets the icon for the item.
    */
    void SetItemIcon(const wxDataViewItem& item, const wxIcon& icon);
};


/**
    @class wxDataViewDateRenderer
    @wxheader{dataview.h}

    wxDataViewDateRenderer

    @library{wxadv}
    @category{FIXME}
*/
class wxDataViewDateRenderer : public wxDataViewRenderer
{
public:
    /**
        
    */
    wxDataViewDateRenderer(const wxString& varianttype = "datetime",
                           wxDataViewCellMode mode = wxDATAVIEW_CELL_ACTIVATABLE);
};


/**
    @class wxDataViewTextRendererAttr
    @wxheader{dataview.h}

    The same as wxDataViewTextRenderer but with
    support for font attributes. Font attributes are currently only supported
    under GTK+ and MSW.

    See also wxDataViewModel::GetAttr and
    wxDataViewItemAttr.

    @library{wxadv}
    @category{FIXME}
*/
class wxDataViewTextRendererAttr : public wxDataViewTextRenderer
{
public:
    /**
        
    */
    wxDataViewTextRendererAttr(const wxString& varianttype = "string",
                               wxDataViewCellMode mode = wxDATAVIEW_CELL_INERT,
                               int align = wxDVR_DEFAULT_ALIGNMENT);
};


/**
    @class wxDataViewColumn
    @wxheader{dataview.h}

    This class represents a column in a wxDataViewCtrl.
    One wxDataViewColumn is bound to one column in the data model,
    to which the wxDataViewCtrl has been associated.

    An instance of wxDataViewRenderer is used by
    this class to render its data.

    @library{wxadv}
    @category{FIXME}
*/
class wxDataViewColumn : public wxObject
{
public:
    //@{
    /**
        Constructors.
    */
    wxDataViewColumn(const wxString& title,
                     wxDataViewRenderer* renderer,
                     unsigned int model_column,
                     int width = wxDVC_DEFAULT_WIDTH,
                     wxAlignment align = wxALIGN_CENTRE,
                     int flags = wxDATAVIEW_COL_RESIZABLE);
    wxDataViewColumn(const wxBitmap& bitmap,
                     wxDataViewRenderer* renderer,
                     unsigned int model_column,
                     int width = wxDVC_DEFAULT_WIDTH,
                     wxAlignment align = wxALIGN_CENTRE,
                     int flags = wxDATAVIEW_COL_RESIZABLE);
    //@}

    /**
        Destructor.
    */
    ~wxDataViewColumn();

    /**
        Returns the bitmap in the header of the column, if any.
    */
    const wxBitmap GetBitmap();

    /**
        Returns the index of the column of the model, which this
        wxDataViewColumn is displaying.
    */
    unsigned int GetModelColumn();

    /**
        Returns the owning wxDataViewCtrl.
    */
    wxDataViewCtrl* GetOwner();

    /**
        Returns the renderer of this wxDataViewColumn.
        
        See also wxDataViewRenderer.
    */
    wxDataViewRenderer* GetRenderer();

    /**
        Returns @true if the column is reorderable.
    */
    bool GetReorderable();

    /**
        Returns @true if the column is sortable.
        
        See SetSortable()
    */
    bool GetSortable();

    /**
        Returns the width of the column.
    */
    int GetWidth();

    /**
        Returns @true, if the sort order is ascending.
        
        See also SetSortOrder()
    */
    bool IsSortOrderAscending();

    /**
        Set the alignment of the column header.
    */
    void SetAlignment(wxAlignment align);

    /**
        Set the bitmap of the column header.
    */
    void SetBitmap(const wxBitmap& bitmap);

    /**
        Indicate wether the column can be reordered by the
        user using the mouse. This is typically implemented
        visually by dragging the header button around.
    */
    void SetReorderable(bool reorderable);

    /**
        Indicate the sort order if the implementation of the
        wxDataViewCtrl supports it, most commonly by showing
        a little arrow.
    */
    void SetSortOrder(bool ascending);

    /**
        Indicate that the column is sortable. This does
        not show any sorting indicate yet, but it does
        make the column header clickable. Call
        SetSortOrder()
        afterwards to actually make the sort indicator appear.
        If @e sortable is @false, the column header is
        no longer clickable and the sort indicator (little
        arrow) will disappear.
    */
    void SetSortable(bool sortable);

    /**
        Set the title of the column header to @e title.
    */
    void SetTitle(const wxString& title);
};
