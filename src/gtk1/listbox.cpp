/////////////////////////////////////////////////////////////////////////////
// Name:        listbox.cpp
// Purpose:
// Author:      Robert Roebling
// Id:          $Id$
// Copyright:   (c) 1998 Robert Roebling
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////


#ifdef __GNUG__
#pragma implementation "listbox.h"
#endif

#include "wx/listbox.h"

#if wxUSE_LISTBOX

#include "wx/dynarray.h"
#include "wx/utils.h"
#include "wx/intl.h"
#include "wx/checklst.h"
#include "wx/settings.h"

#if wxUSE_TOOLTIPS
#include "wx/tooltip.h"
#endif

#if wxUSE_DRAG_AND_DROP
#include "wx/dnd.h"
#endif

#include "gdk/gdk.h"
#include "gtk/gtk.h"

//-----------------------------------------------------------------------------
// idle system
//-----------------------------------------------------------------------------

extern void wxapp_install_idle_handler();
extern bool g_isIdle;

//-------------------------------------------------------------------------
// conditional compilation
//-------------------------------------------------------------------------

#if (GTK_MINOR_VERSION > 0)
    #define NEW_GTK_SCROLL_CODE
#endif

//-----------------------------------------------------------------------------
// private functions
//-----------------------------------------------------------------------------

#if wxUSE_CHECKLISTBOX

#define CHECKBOX_STRING "[-] "

// checklistboxes have "[�] " prepended to their lables, this macro removes it
// (NB: 4 below is the length of CHECKBOX_STRING above)
//
// the argument to it is a "const char *" pointer
#define GET_REAL_LABEL(label) ((m_hasCheckBoxes)?(label)+4 : (label))

#else // !wxUSE_CHECKLISTBOX

#define GET_REAL_LABEL(label) (label)

#endif // wxUSE_CHECKLISTBOX

//-----------------------------------------------------------------------------
// data
//-----------------------------------------------------------------------------

extern bool       g_blockEventsOnDrag;
extern bool       g_blockEventsOnScroll;
extern wxCursor   g_globalCursor;

static bool       g_hasDoubleClicked = FALSE;

//-----------------------------------------------------------------------------
// "button_release_event"
//-----------------------------------------------------------------------------

/* we would normally emit a wxEVT_COMMAND_LISTBOX_DOUBLECLICKED event once
   a GDK_2BUTTON_PRESS occurs, but this has the particular problem of the
   listbox keeping the focus until it receives a GDK_BUTTON_RELEASE event.
   this can lead to race conditions so that we emit the dclick event
   after the GDK_BUTTON_RELEASE event after the GDK_2BUTTON_PRESS event */

static gint
gtk_listbox_button_release_callback( GtkWidget * WXUNUSED(widget),
                                     GdkEventButton * WXUNUSED(gdk_event),
                                     wxListBox *listbox )
{
    if (g_isIdle) wxapp_install_idle_handler();

    if (g_blockEventsOnDrag) return FALSE;
    if (g_blockEventsOnScroll) return FALSE;

    if (!listbox->m_hasVMT) return FALSE;

    if (!g_hasDoubleClicked) return FALSE;

    wxCommandEvent event( wxEVT_COMMAND_LISTBOX_DOUBLECLICKED, listbox->GetId() );
    event.SetEventObject( listbox );

    wxArrayInt aSelections;
    int n, count = listbox->GetSelections(aSelections);
    if ( count > 0 )
    {
        n = aSelections[0];
        if ( listbox->HasClientObjectData() )
            event.SetClientObject( listbox->GetClientObject(n) );
        else if ( listbox->HasClientUntypedData() )
            event.SetClientData( listbox->GetClientData(n) );
        event.SetString( listbox->GetString(n) );
    }
    else
    {
        n = -1;
    }

    event.m_commandInt = n;

    listbox->GetEventHandler()->ProcessEvent( event );

    return FALSE;
}

//-----------------------------------------------------------------------------
// "button_press_event"
//-----------------------------------------------------------------------------

static gint
gtk_listbox_button_press_callback( GtkWidget *widget,
                                   GdkEventButton *gdk_event,
                                   wxListBox *listbox )
{
    if (g_isIdle) wxapp_install_idle_handler();

    if (g_blockEventsOnDrag) return FALSE;
    if (g_blockEventsOnScroll) return FALSE;

    if (!listbox->m_hasVMT) return FALSE;

    int sel = listbox->GetIndex( widget );

#if wxUSE_CHECKLISTBOX
    if ((listbox->m_hasCheckBoxes) && (gdk_event->x < 15) && (gdk_event->type != GDK_2BUTTON_PRESS))
    {
        wxCheckListBox *clb = (wxCheckListBox *)listbox;

        clb->Check( sel, !clb->IsChecked(sel) );

        wxCommandEvent event( wxEVT_COMMAND_CHECKLISTBOX_TOGGLED, listbox->GetId() );
        event.SetEventObject( listbox );
        event.SetInt( sel );
        listbox->GetEventHandler()->ProcessEvent( event );
    }
#endif // wxUSE_CHECKLISTBOX

    /* emit wxEVT_COMMAND_LISTBOX_DOUBLECLICKED later */
    g_hasDoubleClicked = (gdk_event->type == GDK_2BUTTON_PRESS);

    return FALSE;
}

//-----------------------------------------------------------------------------
// "key_press_event"
//-----------------------------------------------------------------------------

#if wxUSE_CHECKLISTBOX
static gint
gtk_listbox_key_press_callback( GtkWidget *widget, GdkEventKey *gdk_event, wxListBox *listbox )
{
    if (g_isIdle) wxapp_install_idle_handler();

    if (g_blockEventsOnDrag) return FALSE;

    if (!listbox->m_hasVMT) return FALSE;

    if (gdk_event->keyval != ' ') return FALSE;

    int sel = listbox->GetIndex( widget );

    wxCheckListBox *clb = (wxCheckListBox *)listbox;

    clb->Check( sel, !clb->IsChecked(sel) );

    wxCommandEvent event( wxEVT_COMMAND_CHECKLISTBOX_TOGGLED, listbox->GetId() );
    event.SetEventObject( listbox );
    event.SetInt( sel );
    listbox->GetEventHandler()->ProcessEvent( event );

    return FALSE;
}
#endif // wxUSE_CHECKLISTBOX

//-----------------------------------------------------------------------------
// "select" and "deselect"
//-----------------------------------------------------------------------------

static void gtk_listitem_select_callback( GtkWidget *WXUNUSED(widget), wxListBox *listbox );

static void gtk_listitem_deselect_callback( GtkWidget *widget, wxListBox *listbox )
{
    gtk_listitem_select_callback( widget, listbox );
}

static void gtk_listitem_select_callback( GtkWidget *WXUNUSED(widget), wxListBox *listbox )
{
    if (g_isIdle) wxapp_install_idle_handler();

    if (!listbox->m_hasVMT) return;
    if (g_blockEventsOnDrag) return;

    wxCommandEvent event(wxEVT_COMMAND_LISTBOX_SELECTED, listbox->GetId() );
    event.SetEventObject( listbox );

    wxArrayInt aSelections;
    int n, count = listbox->GetSelections(aSelections);
    if ( count > 0 )
    {
        n = aSelections[0];
        if ( listbox->HasClientObjectData() )
            event.SetClientObject( listbox->GetClientObject(n) );
        else if ( listbox->HasClientUntypedData() )
            event.SetClientData( listbox->GetClientData(n) );
        event.SetString( listbox->GetString(n) );
    }
    else
    {
        n = -1;
    }

    event.m_commandInt = n;

    listbox->GetEventHandler()->ProcessEvent( event );
}

//-----------------------------------------------------------------------------
// wxListBox
//-----------------------------------------------------------------------------

IMPLEMENT_DYNAMIC_CLASS(wxListBox,wxControl)

// ----------------------------------------------------------------------------
// construction
// ----------------------------------------------------------------------------

wxListBox::wxListBox()
{
    m_list = (GtkList *) NULL;
#if wxUSE_CHECKLISTBOX
    m_hasCheckBoxes = FALSE;
#endif // wxUSE_CHECKLISTBOX
}

bool wxListBox::Create( wxWindow *parent, wxWindowID id,
                        const wxPoint &pos, const wxSize &size,
                        int n, const wxString choices[],
                        long style, const wxValidator& validator,
                        const wxString &name )
{
    m_needParent = TRUE;
    m_acceptsFocus = TRUE;

    if (!PreCreation( parent, pos, size ) ||
        !CreateBase( parent, id, pos, size, style, validator, name ))
    {
        wxFAIL_MSG( wxT("wxListBox creation failed") );
        return FALSE;
    }

    m_widget = gtk_scrolled_window_new( (GtkAdjustment*) NULL, (GtkAdjustment*) NULL );
    if (style & wxLB_ALWAYS_SB)
    {
      gtk_scrolled_window_set_policy( GTK_SCROLLED_WINDOW(m_widget),
        GTK_POLICY_AUTOMATIC, GTK_POLICY_ALWAYS );
    }
    else
    {
      gtk_scrolled_window_set_policy( GTK_SCROLLED_WINDOW(m_widget),
        GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC );
    }

    m_list = GTK_LIST( gtk_list_new() );

    GtkSelectionMode mode = GTK_SELECTION_BROWSE;
    if (style & wxLB_MULTIPLE)
        mode = GTK_SELECTION_MULTIPLE;
    else if (style & wxLB_EXTENDED)
        mode = GTK_SELECTION_EXTENDED;

    gtk_list_set_selection_mode( GTK_LIST(m_list), mode );

#ifdef NEW_GTK_SCROLL_CODE
    gtk_scrolled_window_add_with_viewport( GTK_SCROLLED_WINDOW(m_widget), GTK_WIDGET(m_list) );
#else
    gtk_container_add( GTK_CONTAINER(m_widget), GTK_WIDGET(m_list) );
#endif

    /* make list scroll when moving the focus down using cursor keys */
    gtk_container_set_focus_vadjustment(
        GTK_CONTAINER(m_list),
        gtk_scrolled_window_get_vadjustment(
            GTK_SCROLLED_WINDOW(m_widget)));

    gtk_widget_show( GTK_WIDGET(m_list) );

    wxSize newSize = size;
    if (newSize.x == -1) newSize.x = 100;
    if (newSize.y == -1) newSize.y = 110;
    SetSize( newSize.x, newSize.y );

    if ( style & wxLB_SORT )
    {
        // this will change DoAppend() behaviour
        m_strings = new wxSortedArrayString;
    }

    for (int i = 0; i < n; i++)
    {
        DoAppend(choices[i]);

#if 0
        wxString str(choices[i]);

#if wxUSE_CHECKLISTBOX
        if (m_hasCheckBoxes)
        {
            str.Prepend(CHECKBOX_STRING);
        }
#endif // wxUSE_CHECKLISTBOX

        GtkWidget *list_item = gtk_list_item_new_with_label( str.mbc_str() );

        gtk_container_add( GTK_CONTAINER(m_list), list_item );

        gtk_signal_connect( GTK_OBJECT(list_item), "select",
          GTK_SIGNAL_FUNC(gtk_listitem_select_callback), (gpointer)this );

        if (style & wxLB_MULTIPLE)
            gtk_signal_connect( GTK_OBJECT(list_item), "deselect",
              GTK_SIGNAL_FUNC(gtk_listitem_deselect_callback), (gpointer)this );

        gtk_signal_connect( GTK_OBJECT(list_item),
                            "button_press_event",
                            (GtkSignalFunc)gtk_listbox_button_press_callback,
                            (gpointer) this );

        gtk_signal_connect_after( GTK_OBJECT(list_item),
                            "button_release_event",
                            (GtkSignalFunc)gtk_listbox_button_release_callback,
                            (gpointer) this );

#if wxUSE_CHECKLISTBOX
        if (m_hasCheckBoxes)
        {
            gtk_signal_connect( GTK_OBJECT(list_item),
                            "key_press_event",
                            (GtkSignalFunc)gtk_listbox_key_press_callback,
                            (gpointer)this );
        }
#endif // wxUSE_CHECKLISTBOX

        ConnectWidget( list_item );

        gtk_widget_show( list_item );
#endif // 0
    }

    m_parent->DoAddChild( this );

    PostCreation();

    SetBackgroundColour( wxSystemSettings::GetSystemColour( wxSYS_COLOUR_WINDOW ) );
    SetForegroundColour( parent->GetForegroundColour() );
    SetFont( parent->GetFont() );

    Show( TRUE );

    return TRUE;
}

wxListBox::~wxListBox()
{
    Clear();
}

void wxListBox::DoInsertItems(const wxArrayString& items, int pos)
{
    wxCHECK_RET( m_list != NULL, wxT("invalid listbox") );

    // code elsewhere supposes we have as many items in m_clientData as items
    // in the listbox
    wxASSERT_MSG( m_clientData.GetCount() == (size_t)GetCount(),
                  wxT("bug in client data management") );

    GList *children = m_list->children;
    int length = g_list_length(children);
    wxCHECK_RET( pos <= length, wxT("invalid index in wxListBox::InsertItems") );

    // VZ: it seems that GTK 1.0.6 doesn't has a function to insert an item
    //     into a listbox at the given position, this is why we first delete
    //     all items after this position, then append these items and then
    //     reappend back the old ones.

    // VZ: notice that InsertItems knows nothing about sorting, so calling it
    //     from outside (and not from our own Append) is likely to break
    //     everything

    size_t nItems = items.GetCount();

    size_t n; // loop var

    // optimise for this trivial case
    if ( pos == length )
    {
        // no need to do anything complicated
        for ( n = 0; n < nItems; n++ )
        {
            AppendWithoutSorting(items[n]);

            m_clientData.Append((wxObject *)NULL);
        }

        wxASSERT_MSG( m_clientData.GetCount() == (size_t)GetCount(),
                      wxT("bug in client data management") );

        return;
    }

    // remove the old items
    wxArrayString deletedLabels;
#if wxUSE_CHECKLISTBOX
    wxArrayInt deletedChecks;
#endif

    GList *child = g_list_nth( children, pos );
    for ( n = 0; child != NULL; n++, child = child->next )
    {
        // save label
        GtkBin *bin = GTK_BIN( child->data );
        GtkLabel *label = GTK_LABEL( bin->child );

        wxString str(GET_REAL_LABEL(label->label),*wxConvCurrent);
        deletedLabels.Add(str);

#if wxUSE_CHECKLISTBOX
        // save check state
        if ( m_hasCheckBoxes )
        {
            deletedChecks.Add(((wxCheckListBox *)this)->IsChecked(pos + n));
        }
#endif // wxUSE_CHECKLISTBOX
    }

    size_t nDeletedCount = n;

    gtk_list_clear_items( m_list, pos, length );

    // now append the new items
    wxNode *node = m_clientData.Item(pos);
    for ( n = 0; n < nItems; n++ )
    {
        AppendWithoutSorting(items[n]);

        // make sure we have the correct number of items in this list
        m_clientData.Insert(node, (wxObject *)NULL);
    }

    // and append the old items too
    pos += nItems;  // now the indices are shifted
    for ( n = 0; n < nDeletedCount; n++ )
    {
        AppendWithoutSorting(deletedLabels[n]);

        // the data is already correct - the indices have been rearranged in
        // such manner that we now correspond to the same node as before

#if wxUSE_CHECKLISTBOX
        if ( m_hasCheckBoxes )
        {
            ((wxCheckListBox *)this)->Check(pos + n, (bool)deletedChecks[n]);
        }
#endif // wxUSE_CHECKLISTBOX
    }

    wxASSERT_MSG( m_clientData.GetCount() == (size_t)GetCount(),
                  wxT("bug in client data management") );
}

int wxListBox::DoAppend( const wxString& item )
{
    int index;
    if ( m_strings )
    {
        // need to determine the index
        index = m_strings->Add(item);

        InsertItems(1, &item, index);
    }
    else
    {
        // not sorted, just append
        AppendWithoutSorting(item);

        m_clientData.Append((wxObject *)NULL);

        index = GetCount() - 1;
    }

    return index;
}

void wxListBox::AppendWithoutSorting( const wxString &item )
{
    wxCHECK_RET( m_list != NULL, wxT("invalid listbox") );

    GtkWidget *list_item;

    wxString label(item);
#if wxUSE_CHECKLISTBOX
    if (m_hasCheckBoxes)
    {
        label.Prepend(CHECKBOX_STRING);
    }
#endif // wxUSE_CHECKLISTBOX

    list_item = gtk_list_item_new_with_label( label.mbc_str() );

    gtk_container_add( GTK_CONTAINER(m_list), list_item );

    gtk_signal_connect( GTK_OBJECT(list_item), "select",
      GTK_SIGNAL_FUNC(gtk_listitem_select_callback), (gpointer)this );

    if (HasFlag(wxLB_MULTIPLE))
        gtk_signal_connect( GTK_OBJECT(list_item), "deselect",
          GTK_SIGNAL_FUNC(gtk_listitem_deselect_callback), (gpointer)this );

    gtk_signal_connect( GTK_OBJECT(list_item),
                        "button_press_event",
                        (GtkSignalFunc)gtk_listbox_button_press_callback,
                        (gpointer) this );

    gtk_signal_connect_after( GTK_OBJECT(list_item),
                        "button_release_event",
                        (GtkSignalFunc)gtk_listbox_button_release_callback,
                        (gpointer) this );

#if wxUSE_CHECKLISTBOX
    if (m_hasCheckBoxes)
    {
       gtk_signal_connect( GTK_OBJECT(list_item),
                           "key_press_event",
                           (GtkSignalFunc)gtk_listbox_key_press_callback,
                           (gpointer)this );
    }
#endif // wxUSE_CHECKLISTBOX

    gtk_widget_show( list_item );

    ConnectWidget( list_item );

    if (GTK_WIDGET_REALIZED(m_widget))
    {
        gtk_widget_realize( list_item );
        gtk_widget_realize( GTK_BIN(list_item)->child );

        //if (m_widgetStyle) ApplyWidgetStyle();
        if (m_widgetStyle) {
            // Apply current widget style to the new list_item
            gtk_widget_set_style( GTK_WIDGET( list_item ), m_widgetStyle );
            GtkBin *bin = GTK_BIN( list_item );
            GtkWidget *label = GTK_WIDGET( bin->child );
            gtk_widget_set_style( label, m_widgetStyle );
        }

#if wxUSE_DRAG_AND_DROP
    #ifndef NEW_GTK_DND_CODE
        if (m_dropTarget) m_dropTarget->RegisterWidget( list_item );
    #endif
#endif

#if wxUSE_TOOLTIPS
        if (m_tooltip) m_tooltip->Apply( this );
#endif
    }
}

void wxListBox::DoSetItems( const wxArrayString& items,
                            void **clientData)
{
    Clear();

    DoInsertItems(items, 0);

    if ( clientData )
    {
        size_t count = items.GetCount();
        for ( size_t n = 0; n < count; n++ )
        {
            SetClientData(n, clientData[n]);
        }
    }
}

// ----------------------------------------------------------------------------
// client data
// ----------------------------------------------------------------------------

void wxListBox::DoSetItemClientData( int n, void* clientData )
{
    wxCHECK_RET( m_widget != NULL, wxT("invalid listbox control") );

    wxNode *node = m_clientData.Nth( n );
    wxCHECK_RET( node, wxT("invalid index in wxListBox::DoSetItemClientData") );

    node->SetData( (wxObject*) clientData );
}

void* wxListBox::DoGetItemClientData( int n ) const
{
    wxCHECK_MSG( m_widget != NULL, NULL, wxT("invalid listbox control") );

    wxNode *node = m_clientData.Nth( n );
    wxCHECK_MSG( node, NULL, wxT("invalid index in wxListBox::DoGetItemClientData") );

    return node->Data();
}

void wxListBox::DoSetItemClientObject( int n, wxClientData* clientData )
{
    wxCHECK_RET( m_widget != NULL, wxT("invalid listbox control") );

    wxNode *node = m_clientData.Nth( n );
    wxCHECK_RET( node, wxT("invalid index in wxListBox::DoSetItemClientObject") );

    wxClientData *cd = (wxClientData*) node->Data();
    delete cd;

    node->SetData( (wxObject*) clientData );
}

wxClientData* wxListBox::DoGetItemClientObject( int n ) const
{
    wxCHECK_MSG( m_widget != NULL, (wxClientData*) NULL, wxT("invalid listbox control") );

    wxNode *node = m_clientData.Nth( n );
    wxCHECK_MSG( node, (wxClientData *)NULL,
                 wxT("invalid index in wxListBox::DoGetItemClientObject") );

    return (wxClientData*) node->Data();
}

void wxListBox::Clear()
{
    wxCHECK_RET( m_list != NULL, wxT("invalid listbox") );

    gtk_list_clear_items( m_list, 0, Number() );

    if ( HasClientObjectData() )
    {
        // destroy the data (due to Robert's idea of using wxList<wxObject>
        // and not wxList<wxClientData> we can't just say
        // m_clientList.DeleteContents(TRUE) - this would crash!
        wxNode *node = m_clientData.First();
        while ( node )
        {
            delete (wxClientData *)node->Data();
            node = node->Next();
        }
    }
    m_clientData.Clear();

    if ( m_strings )
        m_strings->Clear();
}

void wxListBox::Delete( int n )
{
    wxCHECK_RET( m_list != NULL, wxT("invalid listbox") );

    GList *child = g_list_nth( m_list->children, n );

    wxCHECK_RET( child, wxT("wrong listbox index") );

    GList *list = g_list_append( (GList*) NULL, child->data );
    gtk_list_remove_items( m_list, list );
    g_list_free( list );

    wxNode *node = m_clientData.Nth( n );
    if ( node )
    {
        if ( m_clientDataItemsType == ClientData_Object )
        {
            wxClientData *cd = (wxClientData*)node->Data();
            delete cd;
        }

        m_clientData.DeleteNode( node );
    }

    if ( m_strings )
        m_strings->Remove(n);
}

// ----------------------------------------------------------------------------
// string list access
// ----------------------------------------------------------------------------

void wxListBox::SetString( int n, const wxString &string )
{
    wxCHECK_RET( m_list != NULL, wxT("invalid listbox") );

    GList *child = g_list_nth( m_list->children, n );
    if (child)
    {
        GtkBin *bin = GTK_BIN( child->data );
        GtkLabel *label = GTK_LABEL( bin->child );

        wxString str;
#if wxUSE_CHECKLISTBOX
        if (m_hasCheckBoxes)
            str += CHECKBOX_STRING;
#endif // wxUSE_CHECKLISTBOX
        str += string;

        gtk_label_set( label, str.mbc_str() );
    }
    else
    {
        wxFAIL_MSG(wxT("wrong listbox index"));
    }
}

wxString wxListBox::GetString( int n ) const
{
    wxCHECK_MSG( m_list != NULL, wxT(""), wxT("invalid listbox") );

    GList *child = g_list_nth( m_list->children, n );
    if (child)
    {
        GtkBin *bin = GTK_BIN( child->data );
        GtkLabel *label = GTK_LABEL( bin->child );

        wxString str = wxString(GET_REAL_LABEL(label->label),*wxConvCurrent);

        return str;
    }

    wxFAIL_MSG(wxT("wrong listbox index"));

    return wxT("");
}

int wxListBox::GetCount() const
{
    wxCHECK_MSG( m_list != NULL, -1, wxT("invalid listbox") );

    GList *child = m_list->children;
    int count = 0;
    while (child)
    {
        count++;
        child = child->next;
    }

    return count;
}

int wxListBox::FindString( const wxString &item ) const
{
    wxCHECK_MSG( m_list != NULL, -1, wxT("invalid listbox") );

    GList *child = m_list->children;
    int count = 0;
    while (child)
    {
        GtkBin *bin = GTK_BIN( child->data );
        GtkLabel *label = GTK_LABEL( bin->child );

        wxString str = wxString(GET_REAL_LABEL(label->label),*wxConvCurrent);

        if (str == item)
            return count;

        count++;
        child = child->next;
    }

    // it's not an error if the string is not found -> no wxCHECK

    return wxNOT_FOUND;
}

// ----------------------------------------------------------------------------
// selection
// ----------------------------------------------------------------------------

int wxListBox::GetSelection() const
{
    wxCHECK_MSG( m_list != NULL, -1, wxT("invalid listbox") );

    GList *child = m_list->children;
    int count = 0;
    while (child)
    {
        if (GTK_WIDGET(child->data)->state == GTK_STATE_SELECTED) return count;
        count++;
        child = child->next;
    }
    return -1;
}

int wxListBox::GetSelections( wxArrayInt& aSelections ) const
{
    wxCHECK_MSG( m_list != NULL, -1, wxT("invalid listbox") );

    // get the number of selected items first
    GList *child = m_list->children;
    int count = 0;
    for (child = m_list->children; child != NULL; child = child->next)
    {
        if (GTK_WIDGET(child->data)->state == GTK_STATE_SELECTED)
            count++;
    }

    aSelections.Empty();

    if (count > 0)
    {
        // now fill the list
        aSelections.Alloc(count); // optimization attempt
        int i = 0;
        for (child = m_list->children; child != NULL; child = child->next, i++)
        {
            if (GTK_WIDGET(child->data)->state == GTK_STATE_SELECTED)
                 aSelections.Add(i);
        }
    }

    return count;
}

bool wxListBox::IsSelected( int n ) const
{
    wxCHECK_MSG( m_list != NULL, FALSE, wxT("invalid listbox") );

    GList *target = g_list_nth( m_list->children, n );
    if (target)
    {
        GList *child = m_list->selection;
        while (child)
        {
            if (child->data == target->data) return TRUE;
            child = child->next;
        }
    }

    wxFAIL_MSG(wxT("wrong listbox index"));

    return FALSE;
}

void wxListBox::SetSelection( int n, bool select )
{
    wxCHECK_RET( m_list != NULL, wxT("invalid listbox") );

    DisableEvents();

    if (select)
        gtk_list_select_item( m_list, n );
    else
        gtk_list_unselect_item( m_list, n );

    EnableEvents();
}

void wxListBox::DoSetFirstItem( int WXUNUSED(n) )
{
    wxFAIL_MSG(wxT("wxListBox::SetFirstItem not implemented"));
}

// ----------------------------------------------------------------------------
// helpers
// ----------------------------------------------------------------------------

int wxListBox::GetIndex( GtkWidget *item ) const
{
    if (item)
    {
        GList *child = m_list->children;
        int count = 0;
        while (child)
        {
            if (GTK_WIDGET(child->data) == item) return count;
            count++;
            child = child->next;
        }
    }
    return -1;
}

#if wxUSE_TOOLTIPS
void wxListBox::ApplyToolTip( GtkTooltips *tips, const wxChar *tip )
{
    GList *child = m_list->children;
    while (child)
    {
        gtk_tooltips_set_tip( tips, GTK_WIDGET( child->data ), wxConvCurrent->cWX2MB(tip), (gchar*) NULL );
        child = child->next;
    }
}
#endif // wxUSE_TOOLTIPS

#if wxUSE_DRAG_AND_DROP
void wxListBox::SetDropTarget( wxDropTarget *dropTarget )
{
    wxCHECK_RET( m_list != NULL, wxT("invalid listbox") );

#ifndef NEW_GTK_DND_CODE
    if (m_dropTarget)
    {
        GList *child = m_list->children;
        while (child)
        {
            m_dropTarget->UnregisterWidget( GTK_WIDGET( child->data ) );
            child = child->next;
        }
    }
#endif

    wxWindow::SetDropTarget( dropTarget );

#ifndef NEW_GTK_DND_CODE
    if (m_dropTarget)
    {
        GList *child = m_list->children;
        while (child)
        {
            m_dropTarget->RegisterWidget( GTK_WIDGET( child->data ) );
            child = child->next;
        }
    }
#endif
}
#endif

void wxListBox::DisableEvents()
{
    GList *child = m_list->children;
    while (child)
    {
        gtk_signal_disconnect_by_func( GTK_OBJECT(child->data),
          GTK_SIGNAL_FUNC(gtk_listitem_select_callback), (gpointer)this );

        if (HasFlag(wxLB_MULTIPLE))
            gtk_signal_disconnect_by_func( GTK_OBJECT(child->data),
              GTK_SIGNAL_FUNC(gtk_listitem_deselect_callback), (gpointer)this );

        child = child->next;
    }
}

void wxListBox::EnableEvents()
{
    GList *child = m_list->children;
    while (child)
    {
        gtk_signal_connect( GTK_OBJECT(child->data), "select",
          GTK_SIGNAL_FUNC(gtk_listitem_select_callback), (gpointer)this );

        if (HasFlag(wxLB_MULTIPLE))
            gtk_signal_connect( GTK_OBJECT(child->data), "deselect",
              GTK_SIGNAL_FUNC(gtk_listitem_deselect_callback), (gpointer)this );

        child = child->next;
    }
}

GtkWidget *wxListBox::GetConnectWidget()
{
    return GTK_WIDGET(m_list);
}

bool wxListBox::IsOwnGtkWindow( GdkWindow *window )
{
    if (wxWindow::IsOwnGtkWindow( window )) return TRUE;

    GList *child = m_list->children;
    while (child)
    {
        GtkWidget *bin = GTK_WIDGET( child->data );
        if (bin->window == window) return TRUE;
        child = child->next;
    }

    return FALSE;
}

void wxListBox::ApplyWidgetStyle()
{
    SetWidgetStyle();

    if (m_backgroundColour.Ok())
    {
        GdkWindow *window = GTK_WIDGET(m_list)->window;
        if ( window )
        {
            m_backgroundColour.CalcPixel( gdk_window_get_colormap( window ) );
            gdk_window_set_background( window, m_backgroundColour.GetColor() );
            gdk_window_clear( window );
        }
    }

    GList *child = m_list->children;
    while (child)
    {
        gtk_widget_set_style( GTK_WIDGET(child->data), m_widgetStyle );

        GtkBin *bin = GTK_BIN( child->data );
        GtkWidget *label = GTK_WIDGET( bin->child );
        gtk_widget_set_style( label, m_widgetStyle );

        child = child->next;
    }
}

void wxListBox::OnInternalIdle()
{
    wxCursor cursor = m_cursor;
    if (g_globalCursor.Ok()) cursor = g_globalCursor;

    if (GTK_WIDGET(m_list)->window && cursor.Ok())
    {
        /* I now set the cursor the anew in every OnInternalIdle call
           as setting the cursor in a parent window also effects the
           windows above so that checking for the current cursor is
           not possible. */
           
        gdk_window_set_cursor( GTK_WIDGET(m_list)->window, cursor.GetCursor() );

        GList *child = m_list->children;
        while (child)
        {
            GtkBin *bin = GTK_BIN( child->data );
            GtkWidget *label = GTK_WIDGET( bin->child );
            
            if (!label->window)
                break;
            else
                gdk_window_set_cursor( label->window, cursor.GetCursor() );

            child = child->next;
        }
    }

    UpdateWindowUI();
}

#endif
