/////////////////////////////////////////////////////////////////////////////
// Name:        treectrl.cpp
// Purpose:     generic tree control implementation
// Author:      Robert Roebling
// Created:     01/02/97
// Modified:    22/10/98 - almost total rewrite, simpler interface (VZ)
// Id:          $Id$
// Copyright:   (c) 1998 Robert Roebling, Julian Smart and Markus Holzem
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// =============================================================================
// declarations
// =============================================================================

// -----------------------------------------------------------------------------
// headers
// -----------------------------------------------------------------------------

#ifdef __GNUG__
  #pragma implementation "treectrl.h"
#endif

#include "wx/treectrl.h"
#include "wx/settings.h"
#include "wx/log.h"
#include "wx/intl.h"
#include "wx/dynarray.h"
#include "wx/dcclient.h"
#include "wx/imaglist.h"

// -----------------------------------------------------------------------------
// array types
// -----------------------------------------------------------------------------

WX_DEFINE_ARRAY(wxGenericTreeItem *, wxArrayTreeItems);

// -----------------------------------------------------------------------------
// private classes
// -----------------------------------------------------------------------------

// a tree item
class WXDLLEXPORT wxGenericTreeItem
{
public:
  // ctors & dtor
  wxGenericTreeItem() { m_data = NULL; }
  wxGenericTreeItem( wxGenericTreeItem *parent,
                     const wxString& text,
                     wxDC& dc,
                     int image, int selImage,
                     wxTreeItemData *data );

  inline ~wxGenericTreeItem();

  // trivial accessors
  wxArrayTreeItems& GetChildren() { return m_children; }

  const wxString& GetText() const { return m_text; }
  int GetImage() const { return m_image; }
  int GetSelectedImage() const { return m_selImage; }
  wxTreeItemData *GetData() const { return m_data; }

  void SetText( const wxString &text, wxDC& dc );
  void SetImage(int image) { m_image = image; }
  void SetSelectedImage(int image) { m_selImage = image; }
  void SetData(wxTreeItemData *data) { m_data = data; }

  void SetHasPlus(bool has = TRUE) { m_hasPlus = has; }

  int GetX() const { return m_x; }
  int GetY() const { return m_y; }

  void SetHeight(int h) { m_height = h; }

  void SetX(int x) { m_x = x; }
  void SetY(int y) { m_y = y; }

  wxGenericTreeItem *GetParent() const { return m_parent; }

  // operations
  void Reset();

  // get count of all children (and grand children and ...) of this item
  size_t GetTotalNumberOfChildren() const;

  void Insert(wxGenericTreeItem *child, size_t index)
    { m_children.Insert(child, index); }

  void SetCross( int x, int y );
  void GetSize( int &x, int &y );

  // return the item at given position (or NULL if no item), onButton is TRUE
  // if the point belongs to the item's button, otherwise it lies on the
  // button's label
  wxGenericTreeItem *HitTest( const wxPoint& point, bool &onButton );

  void Expand() { m_isCollapsed = FALSE; }
  void Collapse() { m_isCollapsed = TRUE; }

  void SetHilight( bool set = TRUE ) { m_hasHilight = set; }

  // status inquiries
  bool HasChildren() const { return !m_children.IsEmpty(); }
  bool HasHilight()  const { return m_hasHilight; }
  bool IsExpanded()  const { return !m_isCollapsed; }
  bool HasPlus()     const { return m_hasPlus || HasChildren(); }

private:
  wxString            m_text;

  int                 m_image,
                      m_selImage;

  wxTreeItemData     *m_data;
  
  // @@ probably should use bitfields to save size
  bool                m_isCollapsed,
                      m_hasHilight,   // same as focused
                      m_hasPlus;      // used for item which doesn't have
                                      // children but still has a [+] button

  int                 m_x, m_y;
  long                m_height, m_width;
  int                 m_xCross, m_yCross;
  int                 m_level;
  wxArrayTreeItems    m_children;
  wxGenericTreeItem  *m_parent;
};

// =============================================================================
// implementation
// =============================================================================

// -----------------------------------------------------------------------------
// wxTreeEvent
// -----------------------------------------------------------------------------

IMPLEMENT_DYNAMIC_CLASS(wxTreeEvent, wxCommandEvent)

wxTreeEvent::wxTreeEvent( wxEventType commandType, int id )
           : wxCommandEvent( commandType, id )
{
  m_code = 0;
  m_itemOld = (wxGenericTreeItem *)NULL;
}

// -----------------------------------------------------------------------------
// wxGenericTreeItem
// -----------------------------------------------------------------------------

wxGenericTreeItem::wxGenericTreeItem(wxGenericTreeItem *parent,
                                     const wxString& text,
                                     wxDC& dc,
                                     int image, int selImage,
                                     wxTreeItemData *data)
                 : m_text(text)
{
  m_image = image;
  m_selImage = selImage;
  m_data = data;
  m_x = m_y = 0;
  m_xCross = m_yCross = 0;

  m_level = 0;

  m_isCollapsed = TRUE;
  m_hasHilight = FALSE;
  m_hasPlus = FALSE;

  m_parent = parent;

  dc.GetTextExtent( m_text, &m_width, &m_height );
}

wxGenericTreeItem::~wxGenericTreeItem()
{
  delete m_data;
  
  size_t count = m_children.Count();
  for ( size_t n = 0; n < count; n++ )
    delete m_children[n];
}

void wxGenericTreeItem::SetText( const wxString &text, wxDC& dc )
{
  m_text = text;

  dc.GetTextExtent( m_text, &m_width, &m_height );
}

void wxGenericTreeItem::Reset()
{
  m_text.Empty();
  m_image =
  m_selImage = -1;
  m_data = NULL;
  m_x = m_y =
  m_height = m_width = 0;
  m_xCross =
  m_yCross = 0;

  m_level = 0;

  m_children.Empty();
  m_isCollapsed = TRUE;

  m_parent = (wxGenericTreeItem *)NULL;
}

size_t wxGenericTreeItem::GetTotalNumberOfChildren() const
{
  size_t count = m_children.Count();
  size_t total = count;
  for ( size_t n = 0; n < count; n++ )
  {
    total += m_children[n]->GetTotalNumberOfChildren();
  }

  return total;
}

void wxGenericTreeItem::SetCross( int x, int y )
{
  m_xCross = x;
  m_yCross = y;
}

void wxGenericTreeItem::GetSize( int &x, int &y )
{
  // FIXME what does this all mean??
  if ( y < m_y ) y = m_y;
  int width = m_x +  m_width;
  if (width > x) x = width;

  if (IsExpanded())
  {
    size_t count = m_children.Count();
    for ( size_t n = 0; n < count; n++ )
    { 
      m_children[n]->GetSize( x, y );
    }
  }
}

wxGenericTreeItem *wxGenericTreeItem::HitTest( const wxPoint& point,
                                               bool &onButton )
{
  if ((point.y > m_y) && (point.y < m_y + m_height))
  {
    // FIXME why +5?
    if ((point.x > m_xCross-5) && (point.x < m_xCross+5) &&
        (point.y > m_yCross-5) && (point.y < m_yCross+5) &&
        (IsExpanded() || HasPlus()))
    {
      onButton = TRUE;
      return this;
    }

    if ((point.x > m_x) && (point.x < m_x+m_width))
    {
      onButton = FALSE;
      return this;
    }
  }
  else
  {
    if (!m_isCollapsed)
    {
      size_t count = m_children.Count();
      for ( size_t n = 0; n < count; n++ )
      {
        wxGenericTreeItem *res = m_children[n]->HitTest( point, onButton );
        if ( res != NULL )
          return res;
      }
    }
  }

  return NULL;
}

// -----------------------------------------------------------------------------
// wxTreeCtrl implementation
// -----------------------------------------------------------------------------

IMPLEMENT_DYNAMIC_CLASS(wxTreeCtrl, wxScrolledWindow)

BEGIN_EVENT_TABLE(wxTreeCtrl,wxScrolledWindow)
  EVT_PAINT          (wxTreeCtrl::OnPaint)
  EVT_MOUSE_EVENTS   (wxTreeCtrl::OnMouse)
  EVT_CHAR           (wxTreeCtrl::OnChar)
  EVT_SET_FOCUS      (wxTreeCtrl::OnSetFocus)
  EVT_KILL_FOCUS     (wxTreeCtrl::OnKillFocus)
END_EVENT_TABLE()

// -----------------------------------------------------------------------------
// construction/destruction
// -----------------------------------------------------------------------------
void wxTreeCtrl::Init()
{
  m_current =
  m_anchor = (wxGenericTreeItem *) NULL;
  m_hasFocus = FALSE;

  m_xScroll = 0;
  m_yScroll = 0;
  m_lineHeight = 10;
  m_indent = 15;

  m_hilightBrush = new wxBrush
    (
      wxSystemSettings::GetSystemColour(wxSYS_COLOUR_HIGHLIGHT),
      wxSOLID
    );

  m_imageListNormal =
  m_imageListState = (wxImageList *) NULL;
}

bool wxTreeCtrl::Create(wxWindow *parent, wxWindowID id,
                        const wxPoint& pos, const wxSize& size,
                        long style, const wxString& name )
{
  Init();

  wxScrolledWindow::Create( parent, id, pos, size, style, name );

  SetBackgroundColour( *wxWHITE );
  m_dottedPen = wxPen( *wxBLACK, 0, 0 );

  return TRUE;
}

wxTreeCtrl::~wxTreeCtrl()
{
  wxDELETE( m_hilightBrush );
  wxDELETE( m_anchor );
}

// -----------------------------------------------------------------------------
// accessors
// -----------------------------------------------------------------------------

size_t wxTreeCtrl::GetCount() const
{
  return m_anchor == NULL ? 0u : m_anchor->GetTotalNumberOfChildren();
}

void wxTreeCtrl::SetIndent(unsigned int indent)
{
  m_indent = indent;
  Refresh();
}

// -----------------------------------------------------------------------------
// functions to work with tree items
// -----------------------------------------------------------------------------

wxString wxTreeCtrl::GetItemText(const wxTreeItemId& item) const
{
  return item.m_pItem->GetText();
}

int wxTreeCtrl::GetItemImage(const wxTreeItemId& item) const
{
  return item.m_pItem->GetImage();
}

int wxTreeCtrl::GetItemSelectedImage(const wxTreeItemId& item) const
{
  return item.m_pItem->GetSelectedImage();
}

wxTreeItemData *wxTreeCtrl::GetItemData(const wxTreeItemId& item) const
{
  return item.m_pItem->GetData();
}

void wxTreeCtrl::SetItemText(const wxTreeItemId& item, const wxString& text)
{
  wxClientDC dc(this);
  item.m_pItem->SetText(text, dc);
}

void wxTreeCtrl::SetItemImage(const wxTreeItemId& item, int image)
{
  item.m_pItem->SetImage(image);
}

void wxTreeCtrl::SetItemSelectedImage(const wxTreeItemId& item, int image)
{
  item.m_pItem->SetSelectedImage(image);
}

void wxTreeCtrl::SetItemData(const wxTreeItemId& item, wxTreeItemData *data)
{
  item.m_pItem->SetData(data);
}

void wxTreeCtrl::SetItemHasChildren(const wxTreeItemId& item, bool has)
{
  item.m_pItem->SetHasPlus(has);
}

// -----------------------------------------------------------------------------
// item status inquiries
// -----------------------------------------------------------------------------

bool wxTreeCtrl::IsVisible(const wxTreeItemId& WXUNUSED(item)) const
{
  wxFAIL_MSG("not implemented");

  return TRUE;
}

bool wxTreeCtrl::ItemHasChildren(const wxTreeItemId& item) const
{
  return !item.m_pItem->GetChildren().IsEmpty();
}

bool wxTreeCtrl::IsExpanded(const wxTreeItemId& item) const
{
  return item.m_pItem->IsExpanded();
}

bool wxTreeCtrl::IsSelected(const wxTreeItemId& item) const
{
  return item.m_pItem->HasHilight();
}

// -----------------------------------------------------------------------------
// navigation
// -----------------------------------------------------------------------------

wxTreeItemId wxTreeCtrl::GetParent(const wxTreeItemId& item) const
{
  wxCHECK_MSG( item.IsOk(), NULL, "invalid tree item" );

  return item.m_pItem->GetParent();
}

wxTreeItemId wxTreeCtrl::GetFirstChild(const wxTreeItemId& item, long& cookie) const
{
  wxCHECK_MSG( item.IsOk(), NULL, "invalid tree item" );

  cookie = 0;
  return GetNextChild(item, cookie);
}

wxTreeItemId wxTreeCtrl::GetNextChild(const wxTreeItemId& item, long& cookie) const
{
  wxCHECK_MSG( item.IsOk(), NULL, "invalid tree item" );

  return item.m_pItem->GetChildren().Item(cookie++);
}

wxTreeItemId wxTreeCtrl::GetNextSibling(const wxTreeItemId& item) const
{
  wxCHECK_MSG( item.IsOk(), NULL, "invalid tree item" );

  wxGenericTreeItem *i = item.m_pItem;
  wxGenericTreeItem *parent = i->GetParent();
  if ( parent == NULL )
  {
    // root item doesn't have any siblings
    return NULL;
  }
  
  wxArrayTreeItems& siblings = parent->GetChildren();
  int index = siblings.Index(i);
  wxASSERT( index != NOT_FOUND ); // I'm not a child of my parent?

  size_t n = (size_t)(index + 1);
  return n == siblings.Count() ? (wxGenericTreeItem*)NULL : siblings[n];
}

wxTreeItemId wxTreeCtrl::GetPrevSibling(const wxTreeItemId& item) const
{
  wxCHECK_MSG( item.IsOk(), NULL, "invalid tree item" );

  wxGenericTreeItem *i = item.m_pItem;
  wxGenericTreeItem *parent = i->GetParent();
  if ( parent == NULL )
  {
    // root item doesn't have any siblings
    return NULL;
  }
  
  wxArrayTreeItems& siblings = parent->GetChildren();
  int index = siblings.Index(i);
  wxASSERT( index != NOT_FOUND ); // I'm not a child of my parent?

  return index == 0 ? (wxGenericTreeItem*)NULL : siblings[(size_t)(index - 1)];
}

wxTreeItemId wxTreeCtrl::GetFirstVisibleItem() const
{
  wxFAIL_MSG("not implemented");

  return NULL;
}

wxTreeItemId wxTreeCtrl::GetNextVisible(const wxTreeItemId& item) const
{
  wxCHECK_MSG( item.IsOk(), NULL, "invalid tree item" );

  wxFAIL_MSG("not implemented");

  return NULL;
}

wxTreeItemId wxTreeCtrl::GetPrevVisible(const wxTreeItemId& item) const
{
  wxCHECK_MSG( item.IsOk(), NULL, "invalid tree item" );

  wxFAIL_MSG("not implemented");

  return NULL;
}

// -----------------------------------------------------------------------------
// operations
// -----------------------------------------------------------------------------

wxTreeItemId wxTreeCtrl::DoInsertItem(const wxTreeItemId& parentId,
                                      size_t previous,
                                      const wxString& text,
                                      int image, int selImage,
                                      wxTreeItemData *data)
{
  wxGenericTreeItem *parent = parentId.m_pItem;
  if ( !parent )
  {
    // should we give a warning here?
    return AddRoot(text, image, selImage, data);
  }
  
  wxClientDC dc(this);
  wxGenericTreeItem *item = new wxGenericTreeItem(parent,
                                                  text, dc,
                                                  image, selImage,
                                                  data);

  if ( data != NULL )
  {
    data->m_pItem = item;
  }

  parent->Insert( item, previous );

  CalculatePositions();

  int cw, ch;
  GetClientSize( &cw, &ch );

  PrepareDC( dc );

  wxRectangle rect;
  rect.x = dc.LogicalToDeviceX( 0 ); 
  rect.y = 0;
  rect.width = 10000; // @@@ not very elegant...
  rect.height = ch;

  if ( previous != 0 )
  {
    rect.y = dc.LogicalToDeviceY( parent->GetChildren().Item(previous)->GetY() );
  }
  else // it's the 1st child
  {
    rect.y = dc.LogicalToDeviceY( parent->GetY() );
  }

  AdjustMyScrollbars();

  if ( rect.height > 0 )
    Refresh( FALSE, &rect );

  return item;
}

wxTreeItemId wxTreeCtrl::AddRoot(const wxString& text,
                                 int image, int selImage,
                                 wxTreeItemData *data)
{
  wxCHECK_MSG( !m_anchor, NULL, "tree can have only one root" );

  wxClientDC dc(this);
  m_anchor = new wxGenericTreeItem((wxGenericTreeItem *)NULL, text, dc,
                                   image, selImage, data);
  if ( data != NULL )
  {
    data->m_pItem = m_anchor;
  }

  AdjustMyScrollbars();
  Refresh();

  return m_anchor;
}

wxTreeItemId wxTreeCtrl::PrependItem(const wxTreeItemId& parent,
                                     const wxString& text,
                                     int image, int selImage,
                                     wxTreeItemData *data)
{
  return DoInsertItem(parent, 0u, text, image, selImage, data);
}

wxTreeItemId wxTreeCtrl::InsertItem(const wxTreeItemId& parentId,
                                    const wxTreeItemId& idPrevious,
                                    const wxString& text,
                                    int image, int selImage,
                                    wxTreeItemData *data)
{
  wxGenericTreeItem *parent = parentId.m_pItem;
  if ( !parent )
  {
    // should we give a warning here?
    return AddRoot(text, image, selImage, data);
  }

  int index = parent->GetChildren().Index(idPrevious.m_pItem);
  wxASSERT_MSG( index != NOT_FOUND,
                "previous item in wxTreeCtrl::InsertItem() is not a sibling" );
  return DoInsertItem(parentId, (size_t)index, text, image, selImage, data);
}

wxTreeItemId wxTreeCtrl::AppendItem(const wxTreeItemId& parentId,
                                    const wxString& text,
                                    int image, int selImage,
                                    wxTreeItemData *data)
{
  wxGenericTreeItem *parent = parentId.m_pItem;
  if ( !parent )
  {
    // should we give a warning here?
    return AddRoot(text, image, selImage, data);
  }

  return DoInsertItem(parent, parent->GetChildren().Count(), text,
                      image, selImage, data);
}

void wxTreeCtrl::Delete(const wxTreeItemId& itemId)
{
  wxGenericTreeItem *item = itemId.m_pItem;

  delete item;

  Refresh();
}

void wxTreeCtrl::DeleteAllItems()
{
  if ( m_anchor )
  {
    delete m_anchor;
    m_anchor = NULL;

    Refresh();
  }
}

void wxTreeCtrl::Expand(const wxTreeItemId& itemId)
{
  wxGenericTreeItem *item = itemId.m_pItem;

  if ( item->IsExpanded() )
    return;

  wxTreeEvent event( wxEVT_COMMAND_TREE_ITEM_EXPANDING, GetId() );
  event.m_item = item;
  event.SetEventObject( this );
  if ( ProcessEvent( event ) && event.m_code )
  {
    // cancelled by program
    return;
  }
  
  item->Expand();

  RefreshSubtree(item);

  event.SetEventType(wxEVT_COMMAND_TREE_ITEM_EXPANDED);
  ProcessEvent( event );
}

void wxTreeCtrl::Collapse(const wxTreeItemId& itemId)
{
  wxGenericTreeItem *item = itemId.m_pItem;

  if ( !item->IsExpanded() )
    return;

  wxTreeEvent event( wxEVT_COMMAND_TREE_ITEM_COLLAPSING, GetId() );
  event.m_item = item;
  event.SetEventObject( this );
  if ( ProcessEvent( event ) && event.m_code )
  {
    // cancelled by program
    return;
  }
  
  item->Collapse();

  wxArrayTreeItems& children = item->GetChildren();
  size_t count = children.Count();
  for ( size_t n = 0; n < count; n++ )
  {
    Collapse(children[n]);
  }

  CalculatePositions();

  RefreshSubtree(item);

  event.SetEventType(wxEVT_COMMAND_TREE_ITEM_COLLAPSED);
  ProcessEvent( event );
}

void wxTreeCtrl::CollapseAndReset(const wxTreeItemId& item)
{
  Collapse(item);
  Delete(item);
}

void wxTreeCtrl::Toggle(const wxTreeItemId& itemId)
{
  wxGenericTreeItem *item = itemId.m_pItem;

  if ( item->IsExpanded() )
    Collapse(itemId);
  else
    Expand(itemId);
}

void wxTreeCtrl::Unselect()
{
  if ( m_current )
  {
    m_current->SetHilight( FALSE );
    RefreshLine( m_current );
  }
}

void wxTreeCtrl::SelectItem(const wxTreeItemId& itemId)
{
  wxGenericTreeItem *item = itemId.m_pItem;

  if ( m_current != item )
  {
    wxTreeEvent event( wxEVT_COMMAND_TREE_SEL_CHANGING, GetId() );
    event.m_item = item;
    event.m_itemOld = m_current;
    event.SetEventObject( this );
    if ( ProcessEvent( event ) && event.WasVetoed() )
      return;

    if ( m_current )
    {
      m_current->SetHilight( FALSE );
      RefreshLine( m_current );
    }

    m_current = item;
    m_current->SetHilight( TRUE );
    RefreshLine( m_current );

    event.SetEventType(wxEVT_COMMAND_TREE_SEL_CHANGED);
    ProcessEvent( event );
  }
}

void wxTreeCtrl::EnsureVisible(const wxTreeItemId& WXUNUSED(item))
{
  wxFAIL_MSG("not implemented");
}

void wxTreeCtrl::ScrollTo(const wxTreeItemId& WXUNUSED(item))
{
  wxFAIL_MSG("not implemented");
}

wxTextCtrl *wxTreeCtrl::EditLabel( const wxTreeItemId& WXUNUSED(item),
                                   wxClassInfo* WXUNUSED(textCtrlClass) )
{
  wxFAIL_MSG("not implemented");

  return NULL;
}

wxTextCtrl *wxTreeCtrl::GetEditControl() const
{
  wxFAIL_MSG("not implemented");

  return NULL;
}

void wxTreeCtrl::EndEditLabel(const wxTreeItemId& WXUNUSED(item), bool WXUNUSED(discardChanges))
{
  wxFAIL_MSG("not implemented");
}

void wxTreeCtrl::SortChildren( const wxTreeItemId& WXUNUSED(item),
                               wxTreeItemCmpFunc *WXUNUSED(cmpFunction))
{
  wxFAIL_MSG("not implemented");
}

// -----------------------------------------------------------------------------
// images are not currently supported, but we still provide stubs for these
// functions
// -----------------------------------------------------------------------------
wxImageList *wxTreeCtrl::GetImageList() const
{
    return m_imageListNormal;
}

wxImageList *wxTreeCtrl::GetStateImageList() const
{
    return m_imageListState;
}

void wxTreeCtrl::SetImageList(wxImageList *imageList)
{
    m_imageListNormal = imageList;
}

void wxTreeCtrl::SetStateImageList(wxImageList *imageList)
{
    m_imageListState = imageList;
}

// -----------------------------------------------------------------------------
// helpers
// -----------------------------------------------------------------------------
void wxTreeCtrl::AdjustMyScrollbars()
{
  if (m_anchor)
  {
    int x = 0;
    int y = 0;
    m_anchor->GetSize( x, y );
    y += 2*m_lineHeight;
    int x_pos = GetScrollPos( wxHORIZONTAL );
    int y_pos = GetScrollPos( wxVERTICAL );
    SetScrollbars( 10, 10, x/10, y/10, x_pos, y_pos );
  }
  else
  {
    SetScrollbars( 0, 0, 0, 0 );
  }
}

void wxTreeCtrl::PaintLevel( wxGenericTreeItem *item, wxDC &dc, int level, int &y )
{
  int horizX = level*m_indent;

  item->SetX( horizX+33 );
  item->SetY( y-m_lineHeight/3 );
  item->SetHeight( m_lineHeight );

  item->SetCross( horizX+15, y );

  int oldY = y;

  int exposed_x = dc.LogicalToDeviceX( 0 );
  int exposed_y = dc.LogicalToDeviceY( item->GetY()-2 );
  
  if (IsExposed( exposed_x, exposed_y, 10000, m_lineHeight+4 ))  // 10000 = very much
  {
    int startX = horizX;
    int endX = horizX + 10;

    if (!item->HasChildren()) endX += 20;
    
    dc.DrawLine( startX, y, endX, y );

    if (item->HasPlus())
    {
      dc.DrawLine( horizX+20, y, horizX+30, y );
      dc.SetPen( *wxGREY_PEN );
      dc.DrawRectangle( horizX+10, y-4, 11, 9 );
      dc.SetPen( *wxBLACK_PEN );
      dc.DrawLine( horizX+13, y, horizX+18, y );

      if (!item->IsExpanded())
        dc.DrawLine( horizX+15, y-2, horizX+15, y+3 );
    }

    if (item->HasHilight())
    {
      dc.SetTextForeground( wxSystemSettings::GetSystemColour( wxSYS_COLOUR_HIGHLIGHTTEXT ) );
      dc.SetBrush( *m_hilightBrush );
      
      long text_w = 0;
      long text_h = 0;
      dc.GetTextExtent( item->GetText(), &text_w, &text_h );
      
      int image_h = 0;
      int image_w = 0;
      if (item->GetImage() != -1)
      {
        m_imageListNormal->GetSize( item->GetImage(), image_w, image_h );
        image_w += 4;
      }
      
      if (m_hasFocus)
        dc.SetPen( *wxBLACK_PEN );
      else
        dc.SetPen( *wxTRANSPARENT_PEN );
	
      dc.DrawRectangle( item->GetX()-2, item->GetY()-2, image_w+text_w+4, text_h+4 );
      
      if (item->GetImage() != -1)
      {
        dc.SetClippingRegion( item->GetX(), item->GetY(), image_w-2, text_h );
	m_imageListNormal->Draw( item->GetImage(), dc, item->GetX(), item->GetY()-1, wxIMAGELIST_DRAW_TRANSPARENT );
        dc.DestroyClippingRegion();
      }
      dc.DrawText( item->GetText(), image_w+item->GetX(), item->GetY() );

      dc.SetPen( *wxBLACK_PEN );
      dc.SetTextForeground( *wxBLACK );
      dc.SetBrush( *wxWHITE_BRUSH );
    }
    else
    {
      dc.SetBrush( *wxWHITE_BRUSH );
      dc.SetPen( *wxTRANSPARENT_PEN );
      
      long text_w = 0;
      long text_h = 0;
      dc.GetTextExtent( item->GetText(), &text_w, &text_h );
      
      int image_h = 0;
      int image_w = 0;
      if (item->GetImage() != -1)
      {
        m_imageListNormal->GetSize( item->GetImage(), image_w, image_h );
        image_w += 4;
      }
      
      dc.DrawRectangle( item->GetX()-2, item->GetY()-2, image_w+text_w+4, text_h+4 );
      
      if (item->GetImage() != -1)
      {
        dc.SetClippingRegion( item->GetX(), item->GetY(), image_w-2, text_h );
	m_imageListNormal->Draw( item->GetImage(), dc, item->GetX(), item->GetY()-1, wxIMAGELIST_DRAW_TRANSPARENT );
        dc.DestroyClippingRegion();
      }
      
      dc.DrawText( item->GetText(), image_w+item->GetX(), item->GetY() );
      dc.SetPen( *wxBLACK_PEN );
    }
  }

  if ( !item->IsExpanded() )
    return;

  int semiOldY = y;

  wxArrayTreeItems& children = item->GetChildren();
  size_t count = children.Count();
  for ( size_t n = 0; n < count; n++ )
  {
    y += m_lineHeight;
    semiOldY = y;

    PaintLevel( children[n], dc, level+1, y );
  }

  dc.DrawLine( horizX+15, oldY+5, horizX+15, semiOldY );
}

// -----------------------------------------------------------------------------
// wxWindows callbacks
// -----------------------------------------------------------------------------

void wxTreeCtrl::OnPaint( const wxPaintEvent &WXUNUSED(event) )
{
  if ( !m_anchor )
    return;

  wxPaintDC dc(this);
  PrepareDC( dc );

  dc.SetFont( wxSystemSettings::GetSystemFont( wxSYS_SYSTEM_FONT ) );

  dc.SetPen( m_dottedPen );
  m_lineHeight = (int)(dc.GetCharHeight() + 4);

  int y = m_lineHeight / 2 + 2;
  PaintLevel( m_anchor, dc, 0, y );
}

void wxTreeCtrl::OnSetFocus( const wxFocusEvent &WXUNUSED(event) )
{
  m_hasFocus = TRUE;
  if ( m_current )
    RefreshLine( m_current );
}

void wxTreeCtrl::OnKillFocus( const wxFocusEvent &WXUNUSED(event) )
{
  m_hasFocus = FALSE;
  if ( m_current )
    RefreshLine( m_current );
}

void wxTreeCtrl::OnChar( wxKeyEvent &event )
{
  // TODO process '+', '-' (expand/collapse branch) and cursor keys
  event.Skip();
}

void wxTreeCtrl::OnMouse( const wxMouseEvent &event )
{
  if ( !(event.LeftDown() || event.LeftDClick()) )
    return;

  if ( !m_anchor )
    return;

  wxClientDC dc(this);
  PrepareDC(dc);
  long x = dc.DeviceToLogicalX( (long)event.GetX() );
  long y = dc.DeviceToLogicalY( (long)event.GetY() );

  bool onButton = FALSE;
  wxGenericTreeItem *item = m_anchor->HitTest( wxPoint(x,y), onButton );
  if ( item == NULL )
    return;

  SelectItem(item);

  if ( event.LeftDClick() )
  {
    wxTreeEvent event( wxEVT_COMMAND_TREE_KEY_DOWN, GetId() );
    event.m_item = item;
    event.m_code = 0;
    event.SetEventObject( this );
    ProcessEvent( event );
  }

  if ( onButton )
  {
    Toggle( item );
  }
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
void wxTreeCtrl::CalculateLevel( wxGenericTreeItem *item,
                                 wxDC &dc,
                                 int level,
                                 int &y )
{
  int horizX = level*m_indent;

  item->SetX( horizX+33 );
  item->SetY( y-m_lineHeight/3-2 );
  item->SetHeight( m_lineHeight );

  if ( item->IsExpanded() )
    return;

  wxArrayTreeItems& children = item->GetChildren();
  size_t count = children.Count();
  for ( size_t n = 0; n < count; n++ )
  {
    y += m_lineHeight;
    CalculateLevel( children[n], dc, level+1, y );
  }
}

void wxTreeCtrl::CalculatePositions()
{
  if ( !m_anchor )
    return;

  wxClientDC dc(this);
  PrepareDC( dc );

  dc.SetFont( wxSystemSettings::GetSystemFont( wxSYS_SYSTEM_FONT ) );

  dc.SetPen( m_dottedPen );
  m_lineHeight = (int)(dc.GetCharHeight() + 4);

  int y = m_lineHeight / 2 + 2;
  CalculateLevel( m_anchor, dc, 0, y );
}

void wxTreeCtrl::RefreshSubtree(wxGenericTreeItem *item)
{
  wxClientDC dc(this);
  PrepareDC(dc);
  
  int cw = 0;
  int ch = 0;
  GetClientSize( &cw, &ch );
  
  wxRect rect;
  rect.x = dc.LogicalToDeviceX( 0 );
  rect.width = cw;
  rect.y = dc.LogicalToDeviceY( item->GetY() );
  rect.height = ch;

  Refresh( TRUE, &rect );

  AdjustMyScrollbars();
}

void wxTreeCtrl::RefreshLine( wxGenericTreeItem *item )
{
  wxClientDC dc(this);
  PrepareDC( dc );

  wxRect rect;
  rect.x = dc.LogicalToDeviceX( item->GetX() - 2 );
  rect.y = dc.LogicalToDeviceY( item->GetY() - 2 );
  rect.width = 1000;
  rect.height = dc.GetCharHeight() + 6;

  Refresh( TRUE, &rect );
}

