/////////////////////////////////////////////////////////////////////////////
// Name:        notebook.cpp
// Purpose:
// Author:      Robert Roebling
// Id:          $Id$
// Copyright:   (c) 1998 Robert Roebling, Vadim Zeitlin
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifdef __GNUG__
#pragma implementation "notebook.h"
#endif

#include "wx/notebook.h"
#include "wx/panel.h"
#include "wx/utils.h"
#include "wx/imaglist.h"
#include "wx/intl.h"
#include "wx/log.h"

//-----------------------------------------------------------------------------
// wxNotebookPage
//-----------------------------------------------------------------------------

class wxNotebookPage: public wxObject
{
public:
  wxNotebookPage()
  {
    m_id = -1;
    m_text = "";
    m_image = -1;
    m_page = (GtkNotebookPage *) NULL;
    m_client = (wxWindow *) NULL;
    m_parent = (GtkNotebook *) NULL;
    m_box = (GtkWidget *) NULL;
    m_added = FALSE;
  }

  // mark page as "added' to the notebook, return FALSE if the page was
  // already added
  bool Add()
  {
    if ( WasAdded() )
      return FALSE;

    m_added = TRUE;
    return TRUE;
  }

  bool WasAdded() const { return m_added; }

  int                m_id;
  wxString           m_text;
  int                m_image;
  GtkNotebookPage   *m_page;
  GtkLabel          *m_label;
  wxWindow          *m_client;
  GtkNotebook       *m_parent;
  GtkWidget         *m_box;     // in which the label and image are packed

private:
  bool m_added;
};

//-----------------------------------------------------------------------------
// "switch_page"
//-----------------------------------------------------------------------------

static void gtk_notebook_page_change_callback(GtkNotebook *WXUNUSED(widget),
                                              GtkNotebookPage *WXUNUSED(page),
                                              gint nPage,
                                              gpointer data)
{
  wxNotebook *notebook = (wxNotebook *)data;

  int old = notebook->GetSelection();

  // TODO: emulate PAGE_CHANGING event

  wxNotebookEvent event( wxEVT_COMMAND_NOTEBOOK_PAGE_CHANGED,
                         notebook->GetId(),  nPage, old );
  event.SetEventObject( notebook );
  notebook->GetEventHandler()->ProcessEvent( event );
}

//-----------------------------------------------------------------------------
// "size_allocate"
//-----------------------------------------------------------------------------

static void gtk_page_size_callback( GtkWidget *WXUNUSED(widget), GtkAllocation* alloc, wxWindow *win )
{
  if ((win->m_x == alloc->x) &&
      (win->m_y == alloc->y) &&
      (win->m_width == alloc->width) &&
      (win->m_height == alloc->height))
  {
    return;
  }

  win->SetSize( alloc->x, alloc->y, alloc->width, alloc->height );
  
  if (win->GetAutoLayout()) win->Layout();
}

//-----------------------------------------------------------------------------
// InsertChild callback for wxNotebook
//-----------------------------------------------------------------------------

static void wxInsertChildInNotebook( wxNotebook* parent, wxWindow* child )
{
  wxNotebookPage *page = new wxNotebookPage();

  page->m_id = parent->GetPageCount();

  page->m_box = gtk_hbox_new (FALSE, 0);
  gtk_container_border_width(GTK_CONTAINER(page->m_box), 2);

  GtkNotebook *notebook = GTK_NOTEBOOK(parent->m_widget);

  page->m_client = child;
  gtk_notebook_append_page( notebook, child->m_widget, page->m_box );

  page->m_page = (GtkNotebookPage*) (g_list_last(notebook->children)->data);

  page->m_parent = notebook;

  gtk_signal_connect( GTK_OBJECT(child->m_widget), "size_allocate",
    GTK_SIGNAL_FUNC(gtk_page_size_callback), (gpointer)child );

  if (!page->m_page)
  {
     wxLogFatalError( "Notebook page creation error" );
     return;
  }

  parent->m_pages.Append( page );
}

//-----------------------------------------------------------------------------
// wxNotebook
//-----------------------------------------------------------------------------

IMPLEMENT_DYNAMIC_CLASS(wxNotebook,wxControl)

void wxNotebook::Init()
{
  m_imageList = (wxImageList *) NULL;
  m_pages.DeleteContents( TRUE );
  m_idHandler = 0;
}

wxNotebook::wxNotebook()
{
  Init();
}

wxNotebook::wxNotebook( wxWindow *parent, wxWindowID id,
      const wxPoint& pos, const wxSize& size,
      long style, const wxString& name )
{
  Init();
  Create( parent, id, pos, size, style, name );
}

wxNotebook::~wxNotebook()
{
  // don't generate change page events any more
  if (m_idHandler != 0)
    gtk_signal_disconnect(GTK_OBJECT(m_widget), m_idHandler);

  DeleteAllPages();
}

bool wxNotebook::Create(wxWindow *parent, wxWindowID id,
      const wxPoint& pos, const wxSize& size,
      long style, const wxString& name )
{
  m_needParent = TRUE;
  m_insertCallback = (wxInsertChildFunction)wxInsertChildInNotebook;

  PreCreation( parent, id, pos, size, style, name );

  m_widget = gtk_notebook_new();

  gtk_notebook_set_scrollable( GTK_NOTEBOOK(m_widget), 1 );

  m_idHandler = gtk_signal_connect (
                  GTK_OBJECT(m_widget), "switch_page",
                  GTK_SIGNAL_FUNC(gtk_notebook_page_change_callback),
                  (gpointer)this );

  m_parent->AddChild( this );

  (m_parent->m_insertCallback)( m_parent, this );

  PostCreation();

  Show( TRUE );

  return TRUE;
}

int wxNotebook::GetSelection() const
{
  wxCHECK_MSG( m_widget != NULL, -1, "invalid notebook" );

  if (m_pages.Number() == 0) return -1;

  GtkNotebookPage *g_page = GTK_NOTEBOOK(m_widget)->cur_page;
  if (!g_page) return -1;

  wxNotebookPage *page = (wxNotebookPage *) NULL;

  wxNode *node = m_pages.First();
  while (node)
  {
    page = (wxNotebookPage*)node->Data();

    if ((page->m_page == g_page) || (page->m_page == (GtkNotebookPage*)NULL))
    {
        // page->m_page is NULL directly after gtk_notebook_append. gtk emits
        // "switch_page" then and we ask for GetSelection() in the handler for
        // "switch_page". otherwise m_page should never be NULL. all this
        // might also be wrong.
        break;
    }
    node = node->Next();
  }

  wxCHECK_MSG( node != NULL, -1, "wxNotebook: no selection?" );

  return page->m_id;
}

int wxNotebook::GetPageCount() const
{
  // count only the pages which were already added to the notebook for MSW
  // compatibility (and, in fact, this behaviour makes more sense anyhow
  // because only the added pages are shown)
  int n = 0;
  for ( wxNode *node = m_pages.First(); node; node = node->Next() )
  {
    wxNotebookPage *page = (wxNotebookPage*)node->Data();
    if ( page->WasAdded() )
        n++;
  }

  return n;
}

int wxNotebook::GetRowCount() const
{
  return 1;
}

wxString wxNotebook::GetPageText( int page ) const
{
  wxCHECK_MSG( m_widget != NULL, "", "invalid notebook" );

  wxNotebookPage* nb_page = GetNotebookPage(page);
  if (nb_page)
    return nb_page->m_text;
  else
    return "";
}

int wxNotebook::GetPageImage( int page ) const
{
  wxCHECK_MSG( m_widget != NULL, 0, "invalid notebook" );

  wxNotebookPage* nb_page = GetNotebookPage(page);
  if (nb_page)
    return nb_page->m_image;
  else
    return 0;
}

wxNotebookPage* wxNotebook::GetNotebookPage(int page) const
{
  wxCHECK_MSG( m_widget != NULL, (wxNotebookPage*)NULL, "invalid notebook" );

  wxNotebookPage *nb_page = (wxNotebookPage *) NULL;

  wxNode *node = m_pages.First();
  while (node)
  {
    nb_page = (wxNotebookPage*)node->Data();
    if (nb_page->m_id == page)
      return nb_page;
    node = node->Next();
  }

  wxLogDebug( "Notebook page %d not found!", page );

  return (wxNotebookPage *) NULL;
}

int wxNotebook::SetSelection( int page )
{
  wxCHECK_MSG( m_widget != NULL, -1, "invalid notebook" );

  int selOld = GetSelection();
  wxNotebookPage* nb_page = GetNotebookPage(page);

  if (!nb_page) return -1;

  int page_num = 0;
  GList *child = GTK_NOTEBOOK(m_widget)->children;
  while (child)
  {
    if (nb_page->m_page == (GtkNotebookPage*)child->data)  break;
    page_num++;
    child = child->next;
  }

  if (!child) return -1;

  gtk_notebook_set_page( GTK_NOTEBOOK(m_widget), page_num );

  return selOld;
}

void wxNotebook::AdvanceSelection( bool bForward )
{
  wxCHECK_RET( m_widget != NULL, "invalid notebook" );

  int sel = GetSelection();
  int max = GetPageCount();

  if (bForward)
    SetSelection( sel == max ? 0 : sel + 1 );
  else
    SetSelection( sel == 0 ? max : sel - 1 );
}

void wxNotebook::SetImageList( wxImageList* imageList )
{
  m_imageList = imageList;
}

bool wxNotebook::SetPageText( int page, const wxString &text )
{
  wxCHECK_MSG( m_widget != NULL, FALSE, "invalid notebook" );

  wxNotebookPage* nb_page = GetNotebookPage(page);

  if (!nb_page) return FALSE;

  nb_page->m_text = text;

  return TRUE;
}

bool wxNotebook::SetPageImage( int page, int image )
{
  wxNotebookPage* nb_page = GetNotebookPage(page);

  if (!nb_page) return FALSE;

  nb_page->m_image = image;

  return TRUE;
}

void wxNotebook::SetPageSize( const wxSize &WXUNUSED(size) )
{
  wxFAIL_MSG( "wxNotebook::SetPageSize not implemented" );
}

void wxNotebook::SetPadding( const wxSize &WXUNUSED(padding) )
{
  wxFAIL_MSG( "wxNotebook::SetPadding not implemented" );
}

bool wxNotebook::DeleteAllPages()
{
  wxCHECK_MSG( m_widget != NULL, FALSE, "invalid notebook" );

  wxNode *page_node = m_pages.First();
  while (page_node)
  {
    wxNotebookPage *page = (wxNotebookPage*)page_node->Data();

    DeletePage( page->m_id );

    page_node = m_pages.First();
  }

  return TRUE;
}

bool wxNotebook::DeletePage( int page )
{
  wxNotebookPage* nb_page = GetNotebookPage(page);
  if (!nb_page) return FALSE;

  int page_num = 0;
  GList *child = GTK_NOTEBOOK(m_widget)->children;
  while (child)
  {
    if (nb_page->m_page == (GtkNotebookPage*)child->data) break;
    page_num++;
    child = child->next;
  }

  wxCHECK_MSG( child != NULL, FALSE, "illegal notebook index" );

  delete nb_page->m_client;

  m_pages.DeleteObject( nb_page );

  return TRUE;
}

bool wxNotebook::RemovePage( int page )
{
  wxNotebookPage* nb_page = GetNotebookPage(page);
  if (!nb_page) return FALSE;

  int page_num = 0;
  GList *child = GTK_NOTEBOOK(m_widget)->children;
  while (child)
  {
    if (nb_page->m_page == (GtkNotebookPage*)child->data) break;
    page_num++;
    child = child->next;
  }

  wxCHECK_MSG( child != NULL, FALSE, "illegal notebook index" );

  gtk_notebook_remove_page( GTK_NOTEBOOK(m_widget), page_num );

  m_pages.DeleteObject( nb_page );

  return TRUE;
}

bool wxNotebook::AddPage(wxWindow* win, const wxString& text,
                         bool bSelect, int imageId)
{
  wxCHECK_MSG( m_widget != NULL, FALSE, "invalid notebook" );

  // we've created the notebook page in AddChild(). Now we just have to set
  // the caption for the page and set the others parameters.

  wxNotebookPage *page = (wxNotebookPage *) NULL;

  wxNode *node = m_pages.First();
  while (node)
  {
    page = (wxNotebookPage*)node->Data();
    if ( page->m_client == win ) break;
    node = node->Next();
  }

  wxCHECK_MSG( page != NULL, FALSE,
               "Can't add a page whose parent is not the notebook!" );

  wxCHECK_MSG( page->Add(), FALSE,
               "Can't add the same page twice to a notebook." );

  if (imageId != -1)
  {
    wxASSERT( m_imageList != NULL );

    const wxBitmap *bmp = m_imageList->GetBitmap(imageId);
    GdkPixmap *pixmap = bmp->GetPixmap();
    GdkBitmap *mask = (GdkBitmap*) NULL;
    if ( bmp->GetMask() )
    {
      mask = bmp->GetMask()->GetBitmap();
    }

    GtkWidget *pixmapwid = gtk_pixmap_new (pixmap, mask );

    gtk_box_pack_start(GTK_BOX(page->m_box), pixmapwid, FALSE, FALSE, 3);

    gtk_widget_show(pixmapwid);
  }

  // then set the attributes
  page->m_text = text;
  if (page->m_text.IsEmpty()) page->m_text = "";
  page->m_image = imageId;
  page->m_label = (GtkLabel *)gtk_label_new(page->m_text);
  gtk_box_pack_start( GTK_BOX(page->m_box), (GtkWidget *)page->m_label, FALSE, FALSE, 3);

  // @@@: what does this do? do we still need it?
  // gtk_misc_set_alignment(GTK_MISC(page->m_label), 0.0, 0.5);

  gtk_widget_show((GtkWidget *)page->m_label);

  if (bSelect) SetSelection(GetPageCount());

  return TRUE;
}

wxWindow *wxNotebook::GetPage( int page ) const
{
  wxCHECK_MSG( m_widget != NULL, (wxWindow*) NULL, "invalid notebook" );

  wxNotebookPage* nb_page = GetNotebookPage(page);
  if (!nb_page)
    return (wxWindow *) NULL;
  else
    return nb_page->m_client;
}

// override these 2 functions to do nothing: everything is done in OnSize
void wxNotebook::SetConstraintSizes( bool WXUNUSED(recurse) )
{
  // don't set the sizes of the pages - their correct size is not yet known
  wxControl::SetConstraintSizes(FALSE);
}

bool wxNotebook::DoPhase( int WXUNUSED(nPhase) )
{
  return TRUE;
}

void wxNotebook::ApplyWidgetStyle()
{
  SetWidgetStyle();
  gtk_widget_set_style( m_widget, m_widgetStyle );
}

//-----------------------------------------------------------------------------
// wxNotebookEvent
//-----------------------------------------------------------------------------

IMPLEMENT_DYNAMIC_CLASS(wxNotebookEvent, wxNotifyEvent)

