///////////////////////////////////////////////////////////////////////////////
// Name:        listbox.cpp
// Purpose:     wxListBox
// Author:      Julian Smart
// Modified by:
// Created:     17/09/98
// RCS-ID:      $Id$
// Copyright:   (c) Julian Smart
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

#ifdef __GNUG__
    #pragma implementation "listbox.h"
#endif

#ifdef __VMS
#define XtParent XTPARENT
#define XtDisplay XTDISPLAY
#endif

# include "wx/listbox.h"
#include "wx/settings.h"
#include "wx/dynarray.h"
#include "wx/log.h"
#include "wx/utils.h"

#ifdef __VMS__
#pragma message disable nosimpint
#endif
#include <Xm/List.h>
#ifdef __VMS__
#pragma message enable nosimpint
#endif
#include "wx/motif/private.h"

    IMPLEMENT_DYNAMIC_CLASS(wxListBox, wxControl)

static void wxListBoxCallback(Widget w,
                              XtPointer clientData,
                              XmListCallbackStruct * cbs);

// ----------------------------------------------------------------------------
// wxSizeKeeper
// ----------------------------------------------------------------------------

// helper class to reduce code duplication
class wxSizeKeeper
{
    int m_x, m_y;
    wxWindow* m_w;
public:
    wxSizeKeeper( wxWindow* w )
        : m_w( w )
    {
        m_w->GetSize( &m_x, &m_y );
    }

    void Restore()
    {
        int x, y;

        m_w->GetSize( &x, &y );
        if( x != m_x || y != m_y )
            m_w->SetSize( -1, -1, m_x, m_y );
    }
};

// ============================================================================
// list box control implementation
// ============================================================================

// Listbox item
wxListBox::wxListBox()
{
    m_noItems = 0;
}

bool wxListBox::Create(wxWindow *parent, wxWindowID id,
                       const wxPoint& pos,
                       const wxSize& size,
                       int n, const wxString choices[],
                       long style,
                       const wxValidator& validator,
                       const wxString& name)
{
    if( !wxControl::CreateControl( parent, id, pos, size, style,
                                   validator, name ) )
        return FALSE;

    m_noItems = n;
    m_backgroundColour = * wxWHITE;

    Widget parentWidget = (Widget) parent->GetClientWidget();

    Arg args[3];
    int count = 0;
    XtSetArg( args[0], XmNlistSizePolicy, XmCONSTANT ); ++count;
    XtSetArg( args[1], XmNselectionPolicy,
              ( m_windowStyle & wxLB_MULTIPLE ) ? XmMULTIPLE_SELECT :
              ( m_windowStyle & wxLB_EXTENDED ) ? XmEXTENDED_SELECT :
                                                  XmBROWSE_SELECT );
    ++count;
    if( m_windowStyle & wxLB_ALWAYS_SB )
    {
        XtSetArg( args[2], XmNscrollBarDisplayPolicy, XmSTATIC );
        ++count;
    }

    Widget listWidget = XmCreateScrolledList(parentWidget,
                                             (char*)name.c_str(), args, count);

    m_mainWidget = (WXWidget) listWidget;

    Set(n, choices);

    XtManageChild (listWidget);

    long width = size.x;
    long height = size.y;
    if (width == -1)
        width = 150;
    if (height == -1)
        height = 80;

    XtAddCallback (listWidget,
                   XmNbrowseSelectionCallback,
                   (XtCallbackProc) wxListBoxCallback,
                   (XtPointer) this);
    XtAddCallback (listWidget,
                   XmNextendedSelectionCallback,
                   (XtCallbackProc) wxListBoxCallback,
                   (XtPointer) this);
    XtAddCallback (listWidget,
                   XmNmultipleSelectionCallback,
                   (XtCallbackProc) wxListBoxCallback,
                   (XtPointer) this);
    XtAddCallback (listWidget,
                   XmNdefaultActionCallback,
                   (XtCallbackProc) wxListBoxCallback,
                   (XtPointer) this);

    ChangeFont(FALSE);

    SetCanAddEventHandler(TRUE);
    AttachWidget (parent, m_mainWidget, (WXWidget) NULL,
                  pos.x, pos.y, width, height);

    ChangeBackgroundColour();

    return TRUE;
}

wxListBox::~wxListBox()
{
    if( HasClientObjectData() )
        m_clientDataDict.DestroyData();
}

void wxListBox::SetSelectionPolicy()
{
    Widget listBox = (Widget)m_mainWidget;
    Arg args[3];

    XtSetArg( args[0], XmNlistSizePolicy, XmCONSTANT );

    XtSetArg( args[1], XmNselectionPolicy,
              ( m_windowStyle & wxLB_MULTIPLE ) ? XmMULTIPLE_SELECT :
              ( m_windowStyle & wxLB_EXTENDED ) ? XmEXTENDED_SELECT :
                                                  XmBROWSE_SELECT );

    XtSetValues( listBox, args, 2 );
}

void wxListBox::DoSetFirstItem( int N )
{
    int count, length;

    if (N < 0)
        return;
    XtVaGetValues ((Widget) m_mainWidget,
                    XmNvisibleItemCount, &count,
                    XmNitemCount, &length,
                    NULL);
    if ((N + count) >= length)
        N = length - count;
    XmListSetPos ((Widget) m_mainWidget, N + 1);
}

void wxListBox::Delete(int N)
{
    wxSizeKeeper sk( this );
    Widget listBox = (Widget) m_mainWidget;

    bool managed = XtIsManaged(listBox);

    if (managed)
        XtUnmanageChild (listBox);

    XmListDeletePos (listBox, N + 1);

    if (managed)
        XtManageChild (listBox);

    sk.Restore();
    m_clientDataDict.Delete(N, HasClientObjectData());
    m_noItems --;
}

int wxListBox::DoAppend(const wxString& item)
{
    wxSizeKeeper sk( this );
    Widget listBox = (Widget) m_mainWidget;

    bool managed = XtIsManaged(listBox);

    if (managed)
        XtUnmanageChild (listBox);
    int n;
    XtVaGetValues (listBox, XmNitemCount, &n, NULL);
    wxXmString text( item );
    //  XmListAddItem(listBox, text, n + 1);
    XmListAddItemUnselected (listBox, text(), 0);

    // It seems that if the list is cleared, we must re-ask for
    // selection policy!!
    SetSelectionPolicy();

    if (managed)
        XtManageChild (listBox);

    sk.Restore();
    m_noItems ++;

    return GetCount() - 1;
}

void wxListBox::DoSetItems(const wxArrayString& items, void** clientData)
{
    wxSizeKeeper sk( this );
    Widget listBox = (Widget) m_mainWidget;

    if( HasClientObjectData() )
        m_clientDataDict.DestroyData();

    bool managed = XtIsManaged(listBox);

    if (managed)
        XtUnmanageChild (listBox);
    XmString *text = new XmString[items.GetCount()];
    size_t i;
    for (i = 0; i < items.GetCount(); ++i)
        text[i] = XmStringCreateSimple ((char*)items[i].c_str());

    if ( clientData )
        for (i = 0; i < items.GetCount(); ++i)
            m_clientDataDict.Set(i, (wxClientData*)clientData[i], FALSE);

    XmListAddItems (listBox, text, items.GetCount(), 0);
    for (i = 0; i < items.GetCount(); i++)
        XmStringFree (text[i]);
    delete[] text;

    // It seems that if the list is cleared, we must re-ask for
    // selection policy!!
    SetSelectionPolicy();

    if (managed)
        XtManageChild (listBox);

    sk.Restore();

    m_noItems = items.GetCount();
}

int wxDoFindStringInList(Widget w, const wxString& s)
{
    wxXmString str( s );
    int *positions = NULL;
    int no_positions = 0;
    bool success = XmListGetMatchPos (w, str(),
                                      &positions, &no_positions);

    if (success)
    {
        int pos = positions[0];
        if (positions)
            XtFree ((char *) positions);
        return pos - 1;
    }
    else
        return -1;
}

int wxListBox::FindString(const wxString& s) const
{
    return wxDoFindStringInList( (Widget)m_mainWidget, s );
}

void wxListBox::Clear()
{
    if (m_noItems <= 0)
        return;

    wxSizeKeeper sk( this );
    Widget listBox = (Widget) m_mainWidget;

    XmListDeleteAllItems (listBox);
    if( HasClientObjectData() )
        m_clientDataDict.DestroyData();

    sk.Restore();

    m_noItems = 0;
}

void wxListBox::SetSelection(int N, bool select)
{
    m_inSetValue = TRUE;
    if (select)
    {
#if 0
        if (m_windowStyle & wxLB_MULTIPLE)
        {
            int *selections = NULL;
            int n = GetSelections (&selections);

            // This hack is supposed to work, to make it possible
            // to select more than one item, but it DOESN'T under Motif 1.1.

            XtVaSetValues ((Widget) m_mainWidget,
                           XmNselectionPolicy, XmMULTIPLE_SELECT,
                           NULL);

            int i;
            for (i = 0; i < n; i++)
                XmListSelectPos ((Widget) m_mainWidget,
                                 selections[i] + 1, FALSE);

            XmListSelectPos ((Widget) m_mainWidget, N + 1, FALSE);

            XtVaSetValues ((Widget) m_mainWidget,
                           XmNselectionPolicy, XmEXTENDED_SELECT,
                           NULL);
        }
        else
#endif // 0
            XmListSelectPos ((Widget) m_mainWidget, N + 1, FALSE);

    }
    else
        XmListDeselectPos ((Widget) m_mainWidget, N + 1);

    m_inSetValue = FALSE;
}

bool wxListBox::IsSelected(int N) const
{
    // In Motif, no simple way to determine if the item is selected.
    wxArrayInt theSelections;
    int count = GetSelections (theSelections);
    if (count == 0)
        return FALSE;
    else
    {
        int j;
        for (j = 0; j < count; j++)
            if (theSelections[j] == N)
                return TRUE;
    }
    return FALSE;
}

void wxListBox::DoSetItemClientObject(int n, wxClientData* clientData)
{
    m_clientDataDict.Set(n, clientData, FALSE);
}

wxClientData* wxListBox::DoGetItemClientObject(int n) const
{
    return m_clientDataDict.Get(n);
}

void *wxListBox::DoGetItemClientData(int N) const
{
    return (void*)m_clientDataDict.Get(N);
}

void wxListBox::DoSetItemClientData(int N, void *Client_data)
{
    m_clientDataDict.Set(N, (wxClientData*)Client_data, FALSE);
}

// Return number of selections and an array of selected integers
int wxListBox::GetSelections(wxArrayInt& aSelections) const
{
    aSelections.Empty();

    Widget listBox = (Widget) m_mainWidget;
    int *posList = NULL;
    int posCnt = 0;
    bool flag = XmListGetSelectedPos (listBox, &posList, &posCnt);
    if (flag)
    {
        if (posCnt > 0)
        {
            aSelections.Alloc(posCnt);

            int i;
            for (i = 0; i < posCnt; i++)
                aSelections.Add(posList[i] - 1);

            XtFree ((char *) posList);
            return posCnt;
        }
        else
            return 0;
    }
    else
        return 0;
}

// Get single selection, for single choice list items
int wxDoGetSelectionInList(Widget listBox)
{
    int *posList = NULL;
    int posCnt = 0;
    bool flag = XmListGetSelectedPos (listBox, &posList, &posCnt);
    if (flag)
    {
        int id = -1;
        if (posCnt > 0)
            id = posList[0] - 1;
        XtFree ((char *) posList);
        return id;
    }
    else
        return -1;
}

int wxListBox::GetSelection() const
{
    return wxDoGetSelectionInList((Widget) m_mainWidget);
}

// Find string for position
wxString wxListBox::GetString(int N) const
{
    Widget listBox = (Widget) m_mainWidget;
    XmString *strlist;
    int n;
    XtVaGetValues (listBox, XmNitemCount, &n, XmNitems, &strlist, NULL);
    if (N <= n && N >= 0)
    {
        char *txt;
        if (XmStringGetLtoR (strlist[N], XmSTRING_DEFAULT_CHARSET, &txt))
        {
            wxString str(txt);
            XtFree (txt);
            return str;
        }
        else
            return wxEmptyString;
    }
    else
        return wxEmptyString;
}

void wxListBox::DoInsertItems(const wxArrayString& items, int pos)
{
    wxSizeKeeper sk( this );
    Widget listBox = (Widget) m_mainWidget;

    bool managed = XtIsManaged(listBox);

    if (managed)
        XtUnmanageChild(listBox);

    XmString *text = new XmString[items.GetCount()];
    size_t i;
    // Steve Hammes: Motif 1.1 compatibility
    // #if XmVersion > 1100
    // Corrected by Sergey Krasnov from Steve Hammes' code
#if XmVersion > 1001
    for (i = 0; i < items.GetCount(); i++)
        text[i] = XmStringCreateSimple((char*)items[i].c_str());
    XmListAddItemsUnselected(listBox, text, items.GetCount(), pos+1);
#else
    for (i = 0; i < items.GetCount(); i++)
    {
        text[i] = XmStringCreateSimple((char*)items[i].c_str());
        // Another Sergey correction
        XmListAddItemUnselected(listBox, text[i], pos+i+1);
    }
#endif
    for (i = 0; i < items.GetCount(); i++)
        XmStringFree(text[i]);
    delete[] text;

    // It seems that if the list is cleared, we must re-ask for
    // selection policy!!
    SetSelectionPolicy();

    if (managed)
        XtManageChild(listBox);

    sk.Restore();

    m_noItems += items.GetCount();
}

void wxListBox::SetString(int N, const wxString& s)
{
    wxSizeKeeper sk( this );
    Widget listBox = (Widget) m_mainWidget;

    wxXmString text( s );

    // delete the item and add it again.
    // FIXME isn't there a way to change it in place?
    XmListDeletePos (listBox, N+1);
    XmListAddItem (listBox, text(), N+1);

    sk.Restore();
}

void wxListBox::Command (wxCommandEvent & event)
{
    if (event.m_extraLong)
        SetSelection (event.m_commandInt);
    else
    {
        Deselect (event.m_commandInt);
        return;
    }
    ProcessCommand (event);
}

void wxListBoxCallback (Widget WXUNUSED(w), XtPointer clientData,
                        XmListCallbackStruct * cbs)
{
    wxListBox *item = (wxListBox *) clientData;

    if (item->InSetValue())
        return;

    wxEventType evtType;

    if( cbs->reason == XmCR_DEFAULT_ACTION )
        evtType = wxEVT_COMMAND_LISTBOX_DOUBLECLICKED;
    else
        evtType = wxEVT_COMMAND_LISTBOX_SELECTED;

    int n = cbs->item_position - 1;
    wxCommandEvent event (evtType, item->GetId());
    if ( item->HasClientObjectData() )
        event.SetClientObject( item->GetClientObject(n) );
    else if ( item->HasClientUntypedData() )
        event.SetClientData( item->GetClientData(n) );
    event.m_commandInt = n;
    event.m_extraLong = TRUE;
    event.SetEventObject(item);
    event.SetString( item->GetString( n ) );

    int x = -1;
    if( NULL != cbs->event && cbs->event->type == ButtonRelease )
    {
        XButtonEvent* evt = (XButtonEvent*)cbs->event;

        x = evt->x;
    }

    switch (cbs->reason)
    {
    case XmCR_MULTIPLE_SELECT:
    case XmCR_BROWSE_SELECT:
#if wxUSE_CHECKLISTBOX
        item->DoToggleItem( n, x );
#endif
    case XmCR_DEFAULT_ACTION:
        item->GetEventHandler()->ProcessEvent(event);
        break;
    case XmCR_EXTENDED_SELECT:
        switch (cbs->selection_type)
        {
        case XmINITIAL:
        case XmADDITION:
        case XmMODIFICATION:
            item->DoToggleItem( n, x );
            item->GetEventHandler()->ProcessEvent(event);
            break;
        }
        break;
    }
}

WXWidget wxListBox::GetTopWidget() const
{
    return (WXWidget) XtParent( (Widget) m_mainWidget );
}

void wxListBox::ChangeBackgroundColour()
{
    wxWindow::ChangeBackgroundColour();

    Widget parent = XtParent ((Widget) m_mainWidget);
    Widget hsb, vsb;

    XtVaGetValues (parent,
        XmNhorizontalScrollBar, &hsb,
        XmNverticalScrollBar, &vsb,
        NULL);

   /* TODO: should scrollbars be affected? Should probably have separate
    * function to change them (by default, taken from wxSystemSettings)
    */
    wxColour backgroundColour = wxSystemSettings::GetColour(wxSYS_COLOUR_3DFACE);
    DoChangeBackgroundColour((WXWidget) hsb, backgroundColour, TRUE);
    DoChangeBackgroundColour((WXWidget) vsb, backgroundColour, TRUE);

    XtVaSetValues (hsb,
        XmNtroughColor, backgroundColour.AllocColour(XtDisplay(hsb)),
        NULL);
    XtVaSetValues (vsb,
        XmNtroughColor, backgroundColour.AllocColour(XtDisplay(vsb)),
        NULL);

    DoChangeBackgroundColour((WXWidget) parent, m_backgroundColour, TRUE);
}

void wxListBox::ChangeForegroundColour()
{
    wxWindow::ChangeForegroundColour();

    Widget parent = XtParent ((Widget) m_mainWidget);
    Widget hsb, vsb;

    XtVaGetValues(parent,
                  XmNhorizontalScrollBar, &hsb,
                  XmNverticalScrollBar, &vsb,
                  NULL);

    /* TODO: should scrollbars be affected? Should probably have separate
             function to change them (by default, taken from wxSystemSettings)

        DoChangeForegroundColour((WXWidget) hsb, m_foregroundColour);
        DoChangeForegroundColour((WXWidget) vsb, m_foregroundColour);
        DoChangeForegroundColour((WXWidget) parent, m_foregroundColour);
    */
}

int wxListBox::GetCount() const
{
    return m_noItems;
}
