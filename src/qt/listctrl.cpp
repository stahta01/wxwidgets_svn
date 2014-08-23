/////////////////////////////////////////////////////////////////////////////
// Name:        src/qt/listctrl.cpp
// Author:      Peter Most
// Copyright:   (c) Peter Most
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#include "wx/listctrl.h"
#include "wx/qt/private/winevent.h"

#include <QHeaderView>


class wxQtTreeWidget : public wxQtEventSignalHandler< QTreeWidget, wxListCtrl >
{
public:
    wxQtTreeWidget( wxWindow *parent, wxListCtrl *handler );

    void EmitListEvent(wxEventType typ, QTreeWidgetItem *qitem, int column) const;

private:
    void itemClicked(QTreeWidgetItem * item, int column);
    void itemActivated(QTreeWidgetItem * item, int column);
    void itemPressed(QTreeWidgetItem * item, int column);
};

wxQtTreeWidget::wxQtTreeWidget( wxWindow *parent, wxListCtrl *handler )
    : wxQtEventSignalHandler< QTreeWidget, wxListCtrl >( parent, handler )
{
    connect(this, &QTreeWidget::itemClicked, this, &wxQtTreeWidget::itemClicked);
    connect(this, &QTreeWidget::itemPressed, this, &wxQtTreeWidget::itemPressed);
    connect(this, &QTreeWidget::itemActivated, this, &wxQtTreeWidget::itemActivated);
}

void wxQtTreeWidget::EmitListEvent(wxEventType typ, QTreeWidgetItem *qitem, int column) const
{
    wxListCtrl *handler = GetHandler();
    if ( handler )
    {
        // prepare the event
        // -----------------
        wxListEvent event;
        event.SetEventType(typ);
        event.SetId(handler->GetId());
        event.m_itemIndex = this->indexFromItem(qitem, column).row();
        event.m_item.SetId(event.m_itemIndex);
        event.m_item.SetMask(wxLIST_MASK_TEXT |
                             wxLIST_MASK_IMAGE |
                             wxLIST_MASK_DATA);
        handler->GetItem(event.m_item);
        EmitEvent(event);
    }
}

void wxQtTreeWidget::itemClicked(QTreeWidgetItem *qitem, int column)
{
    EmitListEvent(wxEVT_LIST_ITEM_SELECTED, qitem, column);
}

void wxQtTreeWidget::itemPressed(QTreeWidgetItem *qitem, int column)
{
    EmitListEvent(wxEVT_LIST_ITEM_SELECTED, qitem, column);
}

void wxQtTreeWidget::itemActivated(QTreeWidgetItem *qitem, int column)
{
    EmitListEvent(wxEVT_LIST_ITEM_ACTIVATED, qitem, column);
}


Qt::AlignmentFlag wxQtConvertTextAlign(wxListColumnFormat align)
{
    switch (align)
    {
        case wxLIST_FORMAT_LEFT:
            return Qt::AlignLeft;
        case wxLIST_FORMAT_RIGHT:
            return Qt::AlignRight;
        case wxLIST_FORMAT_CENTRE:
            return Qt::AlignCenter;
    }
}

wxListColumnFormat wxQtConvertAlignFlag(int align)
{
    switch (align)
    {
        case Qt::AlignLeft:
            return wxLIST_FORMAT_LEFT;
        case Qt::AlignRight:
            return wxLIST_FORMAT_RIGHT;
        case Qt::AlignCenter:
            return wxLIST_FORMAT_CENTRE;
    }
}


wxListCtrl::wxListCtrl()
{
}

wxListCtrl::wxListCtrl(wxWindow *parent,
           wxWindowID id,
           const wxPoint& pos,
           const wxSize& size,
           long style,
           const wxValidator& validator,
           const wxString& name)
{
    Create( parent, id, pos, size, style, validator, name );
}


bool wxListCtrl::Create(wxWindow *parent,
            wxWindowID id,
            const wxPoint& pos,
            const wxSize& size,
            long style,
            const wxValidator& validator,
            const wxString& name)
{
    m_qtTreeWidget = new wxQtTreeWidget( parent, this );

    return QtCreateControl( parent, id, pos, size, style, validator, name );
}

bool wxListCtrl::SetForegroundColour(const wxColour& col)
{
    return wxListCtrlBase::SetForegroundColour(col);
}

bool wxListCtrl::SetBackgroundColour(const wxColour& col)
{
    return wxListCtrlBase::SetBackgroundColour(col);
}

bool wxListCtrl::GetColumn(int col, wxListItem& info) const
{
    QTreeWidgetItem *qitem = m_qtTreeWidget->headerItem();
    if ( qitem != NULL )
    {
        info.SetText(wxQtConvertString(qitem->text(col)));
        info.SetAlign(wxQtConvertAlignFlag(qitem->textAlignment(col)));
        info.SetWidth(m_qtTreeWidget->columnWidth(col));
        return true;
    }
    else
        return false;
}

bool wxListCtrl::SetColumn(int col, const wxListItem& info)
{
    DoInsertColumn(col, info);
    return true;
}

int wxListCtrl::GetColumnWidth(int col) const
{
    return m_qtTreeWidget->columnWidth(col);
}

bool wxListCtrl::SetColumnWidth(int col, int width)
{
    m_qtTreeWidget->setColumnWidth(col, width);
    return true;
}

int wxListCtrl::GetColumnOrder(int col) const
{
    return col;
}

int wxListCtrl::GetColumnIndexFromOrder(int order) const
{
    return order;
}

wxArrayInt wxListCtrl::GetColumnsOrder() const
{
    return wxArrayInt();
}

bool wxListCtrl::SetColumnsOrder(const wxArrayInt& orders)
{
    return false;
}

int wxListCtrl::GetCountPerPage() const
{
    // this may not be exact but should be a good aproximation:
    return m_qtTreeWidget->height() /
            m_qtTreeWidget->visualItemRect(m_qtTreeWidget->headerItem()).height();
}

wxRect wxListCtrl::GetViewRect() const
{
    // this may not be exact but should be a good aproximation:
    wxRect rect = wxQtConvertRect(m_qtTreeWidget->rect());
    int h = m_qtTreeWidget->header()->defaultSectionSize();
    rect.SetTop(h);
    rect.SetHeight(rect.GetHeight() - h);
    return rect;
}

wxTextCtrl* wxListCtrl::GetEditControl() const
{
    return NULL;
}

QTreeWidgetItem *wxListCtrl::QtGetItem(int id) const
{
    wxCHECK_MSG( id >= 0 && id < GetItemCount(), NULL,
                 wxT("invalid item index in wxListCtrl") );
    QModelIndex index = m_qtTreeWidget->model()->index(id, 0);
    // note that itemFromIndex(index) is protected
    return (QTreeWidgetItem*)index.internalPointer();
}

bool wxListCtrl::GetItem(wxListItem& info) const
{
    const long id = info.GetId();
    QTreeWidgetItem *qitem = QtGetItem(id);
    if ( qitem != NULL )
    {
        if ( !info.m_mask )
            // by default, get everything for backwards compatibility
            info.m_mask = -1;
        if ( info.m_mask & wxLIST_MASK_TEXT )
            info.SetText(wxQtConvertString(qitem->text(info.GetColumn())));
        if ( info.m_mask & wxLIST_MASK_DATA )
        {
            QVariant variant = qitem->data(0, Qt::UserRole);
            info.SetData(variant.value<long>());
        }
        if ( info.m_mask & wxLIST_MASK_STATE )
        {
            info.m_state = wxLIST_STATE_DONTCARE;
            if ( info.m_stateMask & wxLIST_STATE_FOCUSED )
            {
                if ( m_qtTreeWidget->currentIndex().row() == id )
                    info.m_state |= wxLIST_STATE_FOCUSED;
            }
            if ( info.m_stateMask & wxLIST_STATE_SELECTED )
            {
                if ( qitem->isSelected() )
                    info.m_state |= wxLIST_STATE_SELECTED;
            }
        }
        return true;
    }
    else
        return false;
}

bool wxListCtrl::SetItem(wxListItem& info)
{
    const long id = info.GetId();
    QTreeWidgetItem *qitem = QtGetItem(id);
    if ( qitem != NULL )
    {
        if ((info.m_mask & wxLIST_MASK_TEXT) && !info.GetText().IsNull() )
            qitem->setText(info.GetColumn(), wxQtConvertString(info.GetText()));
        qitem->setTextAlignment(info.GetColumn(), wxQtConvertTextAlign(info.GetAlign()));

        if ( info.m_mask & wxLIST_MASK_DATA )
        {
            QVariant variant = qVariantFromValue(info.GetData());
            qitem->setData(0, Qt::UserRole, variant);
        }
        if (info.m_mask & wxLIST_MASK_STATE)
        {
            if ((info.m_stateMask & wxLIST_STATE_FOCUSED) &&
                (info.m_state & wxLIST_STATE_FOCUSED))
                    m_qtTreeWidget->setCurrentItem(qitem, 0);
            if (info.m_stateMask & wxLIST_STATE_SELECTED)
                qitem->setSelected(info.m_state & wxLIST_STATE_SELECTED);
        }
        for (int col=0; col<GetColumnCount(); col++)
        {
            if ( info.GetFont().IsOk() )
                qitem->setFont(col, info.GetFont().GetHandle() );
            if ( info.GetTextColour().IsOk() )
                qitem->setTextColor(col, info.GetTextColour().GetHandle());
            if ( info.GetBackgroundColour().IsOk() )
                qitem->setBackgroundColor(col, info.GetBackgroundColour().GetHandle());
        }
        return true;
    }
    else
        return false;
}

long wxListCtrl::SetItem(long index, int col, const wxString& label, int imageId)
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
    }
    return SetItem(info);
}

int  wxListCtrl::GetItemState(long item, long stateMask) const
{
    wxListItem info;

    info.m_mask = wxLIST_MASK_STATE;
    info.m_stateMask = stateMask;
    info.m_itemId = item;

    if (!GetItem(info))
        return 0;

    return info.m_state;
}

bool wxListCtrl::SetItemState(long item, long state, long stateMask)
{
    wxListItem info;

    info.m_mask = wxLIST_MASK_STATE;
    info.m_stateMask = stateMask;
    info.m_state = state;
    info.m_itemId = item;

    return SetItem(info);
}

bool wxListCtrl::SetItemImage(long item, int image, int selImage)
{
    return false;
}

bool wxListCtrl::SetItemColumnImage(long item, long column, int image)
{
    return false;
}

wxString wxListCtrl::GetItemText(long item, int col) const
{
    QTreeWidgetItem *qitem = QtGetItem(item);
    if ( qitem )
        return wxQtConvertString( qitem->text(0) );
    else
        return wxString();
}

void wxListCtrl::SetItemText(long item, const wxString& str)
{
    QTreeWidgetItem *qitem = QtGetItem(item);
    if ( qitem )
        qitem->setText( 0, wxQtConvertString( str ) );
}

wxUIntPtr wxListCtrl::GetItemData(long item) const
{
    QTreeWidgetItem *qitem = QtGetItem(item);
    if ( qitem != NULL )
    {
        QVariant variant = qitem->data(0, Qt::UserRole);
        return variant.value<wxUIntPtr>();
    }
    else
        return 0;
}

bool wxListCtrl::SetItemPtrData(long item, wxUIntPtr data)
{
    QTreeWidgetItem *qitem = QtGetItem(item);
    if ( qitem != NULL )
    {
        QVariant variant = qVariantFromValue(data);
        qitem->setData(0, Qt::UserRole, variant);
        return true;
    }
    else
        return false;
}

bool wxListCtrl::SetItemData(long item, long data)
{
    QTreeWidgetItem *qitem = QtGetItem(item);
    if ( qitem != NULL )
    {
        QVariant variant = qVariantFromValue(data);
        qitem->setData(0, Qt::UserRole, variant);
        return true;
    }
    else
        return false;
}

bool wxListCtrl::GetItemRect(long item, wxRect& rect, int code) const
{
    QTreeWidgetItem *qitem = QtGetItem(item);
    if ( qitem != NULL )
    {
        rect = wxQtConvertRect( m_qtTreeWidget->visualItemRect(qitem) );
        return true;
    }
    else
        return false;
}

bool wxListCtrl::GetSubItemRect(long item, long subItem, wxRect& rect, int WXUNUSED(code)) const
{
    QTreeWidgetItem *qitem = QtGetItem(item);
    if ( qitem != NULL )
    {
        wxCHECK_MSG( item >= 0 && item < GetItemCount(), NULL,
                     wxT("invalid row index in GetSubItemRect") );
        wxCHECK_MSG( subItem >= 0 && subItem < GetColumnCount(), NULL,
                     wxT("invalid column index in GetSubItemRect") );
        QModelIndex qindex = m_qtTreeWidget->model()->index(item, subItem);
        rect = wxQtConvertRect( m_qtTreeWidget->visualRect(qindex) );
        return true;
    }
    else
        return false;
}

bool wxListCtrl::GetItemPosition(long item, wxPoint& pos) const
{
    wxRect rect;
    GetItemRect(item, rect);

    pos.x = rect.x;
    pos.y = rect.y;

    return true;
}

bool wxListCtrl::SetItemPosition(long item, const wxPoint& pos)
{
    return false;
}

int wxListCtrl::GetItemCount() const
{
    return m_qtTreeWidget->topLevelItemCount();
}

int wxListCtrl::GetColumnCount() const
{
    return m_qtTreeWidget->columnCount();
}

wxSize wxListCtrl::GetItemSpacing() const
{
    return wxSize();
}

void wxListCtrl::SetItemTextColour( long item, const wxColour& col)
{
}

wxColour wxListCtrl::GetItemTextColour( long item ) const
{
    return wxColour();
}

void wxListCtrl::SetItemBackgroundColour( long item, const wxColour &col)
{
}

wxColour wxListCtrl::GetItemBackgroundColour( long item ) const
{
    return wxColour();
}

void wxListCtrl::SetItemFont( long item, const wxFont &f)
{
}

wxFont wxListCtrl::GetItemFont( long item ) const
{
    return wxFont();
}

int wxListCtrl::GetSelectedItemCount() const
{
    return m_qtTreeWidget->selectedItems().length();
}

wxColour wxListCtrl::GetTextColour() const
{
    return wxColour();
}

void wxListCtrl::SetTextColour(const wxColour& col)
{
}

long wxListCtrl::GetTopItem() const
{
    return 0;
}

void wxListCtrl::SetSingleStyle(long style, bool add)
{
}

void wxListCtrl::SetWindowStyleFlag(long style)
{
}

long wxListCtrl::GetNextItem(long item, int WXUNUSED(geometry), int state) const
{
    wxListItem info;
    long ret = item,
         max = GetItemCount();
    wxCHECK_MSG( (ret == -1) || (ret < max), -1,
                 wxT("invalid listctrl index in GetNextItem()") );

    // notice that we start with the next item (or the first one if item == -1)
    // and this is intentional to allow writing a simple loop to iterate over
    // all selected items
    ret++;
    if ( ret == max )
        // this is not an error because the index was OK initially,
        // just no such item
        return -1;

    if ( !state )
        // any will do
        return (size_t)ret;

    size_t count = GetItemCount();
    for ( size_t line = (size_t)ret; line < count; line++ )
    {
        if ( GetItemState(line, state) )
            return line;
    }

    return -1;
}

wxImageList *wxListCtrl::GetImageList(int which) const
{
    return NULL;
}


void wxListCtrl::SetImageList(wxImageList *imageList, int which)
{
}

void wxListCtrl::AssignImageList(wxImageList *imageList, int which)
{
}

bool wxListCtrl::InReportView() const
{
    return true;
}

bool wxListCtrl::IsVirtual() const
{
    return false;
}

void wxListCtrl::RefreshItem(long item)
{
}

void wxListCtrl::RefreshItems(long itemFrom, long itemTo)
{
}

bool wxListCtrl::Arrange(int flag)
{
    return false;
}

bool wxListCtrl::DeleteItem(long item)
{
    QTreeWidgetItem *qitem = QtGetItem(item);
    if ( qitem != NULL )
    {
        delete qitem;
        return true;
    }
    else
        return false;
}

bool wxListCtrl::DeleteAllItems()
{
    m_qtTreeWidget->clear();
    return true;
}

bool wxListCtrl::DeleteColumn(int col)
{
    // Qt cannot easily add or remove columns, so only the last one can be deleted
    if ( col == GetColumnCount() - 1 )
    {
        m_qtTreeWidget->setColumnCount(col);
        return true;
    }
    else
        return false;
}

bool wxListCtrl::DeleteAllColumns()
{
    m_qtTreeWidget->setColumnCount(0);
    return true;
}

void wxListCtrl::ClearAll()
{
    m_qtTreeWidget->clear();
}

wxTextCtrl* wxListCtrl::EditLabel(long item, wxClassInfo* WXUNUSED(textControlClass))
{
    QTreeWidgetItem *qitem = QtGetItem(item);
    if ( qitem != NULL )
    {
        m_qtTreeWidget->openPersistentEditor(qitem);
    }
    return NULL;
}

bool wxListCtrl::EndEditLabel(bool cancel)
{
    int item = GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_FOCUSED);
    if (item > 0)
    {
        QTreeWidgetItem *qitem = QtGetItem(item);
        if ( qitem != NULL )
        {
            m_qtTreeWidget->closePersistentEditor(qitem);
            return true;
        }
    }
    return false;
}

bool wxListCtrl::EnsureVisible(long item)
{
    QTreeWidgetItem *qitem = QtGetItem(item);
    if ( qitem != NULL )
    {
         m_qtTreeWidget->scrollToItem(qitem);
         return true;
    }
    else
        return false;
}

long wxListCtrl::FindItem(long start, const wxString& str, bool partial)
{
    return 0;
}

long wxListCtrl::FindItem(long start, wxUIntPtr data)
{
    return 0;
}

long wxListCtrl::FindItem(long start, const wxPoint& pt, int direction)
{
    return 0;
}

long wxListCtrl::HitTest(const wxPoint& point, int &flags, long* ptrSubItem) const
{
    QModelIndex index = m_qtTreeWidget->indexAt(wxQtConvertPoint(point));
    if ( index.isValid() )
    {
        flags = wxLIST_HITTEST_ONITEM;
        *ptrSubItem = index.column();
    }
    else
    {
        flags = wxLIST_HITTEST_NOWHERE;
        *ptrSubItem = 0;
    }
    return index.row();
}

long wxListCtrl::InsertItem(const wxListItem& info)
{
    QTreeWidgetItem *qitem = new QTreeWidgetItem(m_qtTreeWidget);
    if ( qitem != NULL )
    {
        qitem->setText(info.GetColumn(), wxQtConvertString(info.GetText()));
        qitem->setTextAlignment(info.GetColumn(), wxQtConvertTextAlign(info.GetAlign()));
        for (int col=0; col<GetColumnCount();col++)
        {
            if ( info.GetFont().IsOk() )
                qitem->setFont(col, info.GetFont().GetHandle() );
            if ( info.GetTextColour().IsOk() )
                qitem->setTextColor(col, info.GetTextColour().GetHandle());
            if ( info.GetBackgroundColour().IsOk() )
                qitem->setBackgroundColor(col, info.GetBackgroundColour().GetHandle());
        }
        return GetItemCount() - 1;
    }
    else
        return -1;
}

long wxListCtrl::InsertItem(long index, const wxString& label)
{
    wxListItem info;
    info.m_text = label;
    info.m_mask = wxLIST_MASK_TEXT;
    info.m_itemId = index;
    return InsertItem(info);
}

long wxListCtrl::InsertItem(long index, int imageIndex)
{
    wxListItem info;
    info.m_image = imageIndex;
    info.m_mask = wxLIST_MASK_IMAGE;
    info.m_itemId = index;
    return InsertItem(info);
}

long wxListCtrl::InsertItem(long index, const wxString& label, int imageIndex)
{
    wxListItem info;
    //info.m_image = imageIndex == -1 ? I_IMAGENONE : imageIndex;
    info.m_text = label;
    info.m_mask = wxLIST_MASK_TEXT | wxLIST_MASK_IMAGE;
    info.m_itemId = index;
    return InsertItem(info);
}

long wxListCtrl::DoInsertColumn(long col, const wxListItem& info)
{
    QTreeWidgetItem *qitem = m_qtTreeWidget->headerItem();
    if ( qitem != NULL )
    {
        qitem->setText(col, wxQtConvertString(info.GetText()));
        qitem->setTextAlignment(col, wxQtConvertTextAlign(info.GetAlign()));
        if (info.GetWidth())
            m_qtTreeWidget->setColumnWidth(col, info.GetWidth());
        return col;
    }
    else
        return -1;
}


void wxListCtrl::SetItemCount(long count)
{
}

bool wxListCtrl::ScrollList(int dx, int dy)
{
    // aproximate, as scrollContentsBy is protected
    m_qtTreeWidget->scroll(dx, dy);
}

bool wxListCtrl::SortItems(wxListCtrlCompare fn, wxIntPtr data)
{
    return false;
}


QTreeWidget *wxListCtrl::GetHandle() const
{
    return m_qtTreeWidget;
}
