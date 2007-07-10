///////////////////////////////////////////////////////////////////////////////
// Name:        src/msw/checklst.cpp
// Purpose:     implementation of wxCheckListBox class
// Author:      Vadim Zeitlin
// Modified by:
// Created:     16.11.97
// RCS-ID:      $Id$
// Copyright:   (c) 1998 Vadim Zeitlin <zeitlin@dptmaths.ens-cachan.fr>
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

// ============================================================================
// declarations
// ============================================================================

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#if wxUSE_CHECKLISTBOX && wxUSE_OWNER_DRAWN

#include "wx/checklst.h"

#ifndef WX_PRECOMP
    #include "wx/msw/wrapcctl.h"
    #include "wx/object.h"
    #include "wx/colour.h"
    #include "wx/font.h"
    #include "wx/bitmap.h"
    #include "wx/window.h"
    #include "wx/listbox.h"
    #include "wx/dcmemory.h"
    #include "wx/settings.h"
    #include "wx/log.h"
#endif

#include "wx/ownerdrw.h"

#include <windowsx.h>

#include "wx/renderer.h"
#include "wx/msw/private.h"

// ----------------------------------------------------------------------------
// private functions
// ----------------------------------------------------------------------------

// get item (converted to right type)
#define GetItem(n)    ((wxCheckListBoxItem *)(GetItem(n)))

// ============================================================================
// implementation
// ============================================================================

// ----------------------------------------------------------------------------
// declaration and implementation of wxCheckListBoxItem class
// ----------------------------------------------------------------------------

class wxCheckListBoxItem : public wxOwnerDrawn
{
friend class WXDLLEXPORT wxCheckListBox;
public:
    // ctor
    wxCheckListBoxItem(wxCheckListBox *pParent, size_t nIndex);

    // drawing functions
    virtual bool OnDrawItem(wxDC& dc, const wxRect& rc, wxODAction act, wxODStatus stat);

    // simple accessors and operations
    bool IsChecked() const { return m_bChecked; }

    void Check(bool bCheck);
    void Toggle() { Check(!IsChecked()); }

    void SendEvent();

private:
    bool            m_bChecked;
    wxCheckListBox *m_pParent;
    size_t          m_nIndex;

    DECLARE_NO_COPY_CLASS(wxCheckListBoxItem)
};

wxCheckListBoxItem::wxCheckListBoxItem(wxCheckListBox *pParent, size_t nIndex)
                  : wxOwnerDrawn(wxEmptyString, true)   // checkable
{
    m_bChecked = false;
    m_pParent  = pParent;
    m_nIndex   = nIndex;

    // we don't initialize m_nCheckHeight/Width vars because it's
    // done in OnMeasure while they are used only in OnDraw and we
    // know that there will always be OnMeasure before OnDraw

    SetMarginWidth(::GetSystemMetrics(SM_CXMENUCHECK) - 2);

    SetBackgroundColour(pParent->GetBackgroundColour());
}

bool wxCheckListBoxItem::OnDrawItem(wxDC& dc, const wxRect& rc,
                                    wxODAction act, wxODStatus stat)
{
    // first draw the label
    if ( IsChecked() )
        stat = (wxOwnerDrawn::wxODStatus)(stat | wxOwnerDrawn::wxODChecked);

    if ( !wxOwnerDrawn::OnDrawItem(dc, rc, act, stat) )
        return false;

    // now draw the check mark part
    HDC hdc = GetHdcOf(dc);

    int nBmpWidth  = ::GetSystemMetrics(SM_CXMENUCHECK),
        nBmpHeight = ::GetSystemMetrics(SM_CYMENUCHECK);


    // first create bitmap in a memory DC
    MemoryHDC hdcMem(hdc);
    CompatibleBitmap hBmpCheck(hdc, nBmpWidth, nBmpHeight);

    // then draw a check mark into it
    {
        SelectInHDC selBmp(hdcMem, hBmpCheck);

        int flags = wxCONTROL_FLAT;
        if ( IsChecked() )
            flags |= wxCONTROL_CHECKED;

        wxDCTemp dcMem(hdcMem);
        wxRendererNative::Get().DrawCheckBox(
                m_pParent, dcMem, wxRect(0, 0, nBmpWidth, nBmpHeight), flags);
    } // select hBmpCheck out of hdcMem

    // shift check mark 2 pixel to the right and bottom, looks better like this
    int x = rc.GetX() + 2,
        y = rc.GetY() + 2;

    // finally draw bitmap to screen: uses image list functions to blend
    // the bitmap with the background colour (better for the selected items)
    HIMAGELIST himl = ImageList_Create(nBmpWidth, nBmpHeight,
                                       ILC_COLOR32 | ILC_MASK, 1, 1);
    ImageList_Add(himl, hBmpCheck, NULL);

    UINT fStyle = stat & wxOwnerDrawn::wxODSelected ? ILD_SELECTED : ILD_NORMAL;
    ImageList_Draw(himl, 0, hdc, x, y, fStyle);

    ImageList_Destroy(himl);

    return true;
}

// change the state of the item and redraw it
void wxCheckListBoxItem::Check(bool check)
{
    m_bChecked = check;

    // index may be changed because new items were added/deleted
    if ( m_pParent->GetItemIndex(this) != (int)m_nIndex )
    {
        // update it
        int index = m_pParent->GetItemIndex(this);

        wxASSERT_MSG( index != wxNOT_FOUND, wxT("what does this item do here?") );

        m_nIndex = (size_t)index;
    }

    HWND hwndListbox = (HWND)m_pParent->GetHWND();

    RECT rcUpdate;

    if ( ::SendMessage(hwndListbox, LB_GETITEMRECT,
                       m_nIndex, (LPARAM)&rcUpdate) == LB_ERR )
    {
        wxLogDebug(wxT("LB_GETITEMRECT failed"));
    }

    ::InvalidateRect(hwndListbox, &rcUpdate, FALSE);
}

// send an "item checked" event
void wxCheckListBoxItem::SendEvent()
{
    wxCommandEvent event(wxEVT_COMMAND_CHECKLISTBOX_TOGGLED, m_pParent->GetId());
    event.SetInt(m_nIndex);
    event.SetEventObject(m_pParent);
    m_pParent->ProcessCommand(event);
}

// ----------------------------------------------------------------------------
// implementation of wxCheckListBox class
// ----------------------------------------------------------------------------

// define event table
// ------------------
BEGIN_EVENT_TABLE(wxCheckListBox, wxListBox)
  EVT_KEY_DOWN(wxCheckListBox::OnKeyDown)
  EVT_LEFT_DOWN(wxCheckListBox::OnLeftClick)
END_EVENT_TABLE()

// control creation
// ----------------

// def ctor: use Create() to really create the control
wxCheckListBox::wxCheckListBox()
{
}

// ctor which creates the associated control
wxCheckListBox::wxCheckListBox(wxWindow *parent, wxWindowID id,
                               const wxPoint& pos, const wxSize& size,
                               int nStrings, const wxString choices[],
                               long style, const wxValidator& val,
                               const wxString& name)
{
    Create(parent, id, pos, size, nStrings, choices, style, val, name);
}

wxCheckListBox::wxCheckListBox(wxWindow *parent, wxWindowID id,
                               const wxPoint& pos, const wxSize& size,
                               const wxArrayString& choices,
                               long style, const wxValidator& val,
                               const wxString& name)
{
    Create(parent, id, pos, size, choices, style, val, name);
}

bool wxCheckListBox::Create(wxWindow *parent, wxWindowID id,
                            const wxPoint& pos, const wxSize& size,
                            int n, const wxString choices[],
                            long style,
                            const wxValidator& validator, const wxString& name)
{
    return wxListBox::Create(parent, id, pos, size, n, choices,
                             style | wxLB_OWNERDRAW, validator, name);
}

bool wxCheckListBox::Create(wxWindow *parent, wxWindowID id,
                            const wxPoint& pos, const wxSize& size,
                            const wxArrayString& choices,
                            long style,
                            const wxValidator& validator, const wxString& name)
{
    return wxListBox::Create(parent, id, pos, size, choices,
                             style | wxLB_OWNERDRAW, validator, name);
}

// misc overloaded methods
// -----------------------

void wxCheckListBox::Delete(unsigned int n)
{
    wxCHECK_RET( IsValid(n),
                 wxT("invalid index in wxListBox::Delete") );

    wxListBox::Delete(n);

    // free memory
    delete m_aItems[n];

    m_aItems.RemoveAt(n);
}

bool wxCheckListBox::SetFont( const wxFont &font )
{
    unsigned int i;
    for ( i = 0; i < m_aItems.GetCount(); i++ )
        m_aItems[i]->SetFont(font);

    wxListBox::SetFont(font);

    return true;
}

// create/retrieve item
// --------------------

// create a check list box item
wxOwnerDrawn *wxCheckListBox::CreateLboxItem(size_t nIndex)
{
  wxCheckListBoxItem *pItem = new wxCheckListBoxItem(this, nIndex);
  return pItem;
}

// return item size
// ----------------
bool wxCheckListBox::MSWOnMeasure(WXMEASUREITEMSTRUCT *item)
{
  if ( wxListBox::MSWOnMeasure(item) ) {
    MEASUREITEMSTRUCT *pStruct = (MEASUREITEMSTRUCT *)item;

    // add place for the check mark
    pStruct->itemWidth  += wxOwnerDrawn::GetDefaultMarginWidth();
    pStruct->itemHeight += 1;

    // save item height
    m_nItemHeight = pStruct->itemHeight;

    return true;
  }

  return false;
}

// check items
// -----------

bool wxCheckListBox::IsChecked(unsigned int uiIndex) const
{
    wxCHECK_MSG( IsValid(uiIndex), false, _T("bad wxCheckListBox index") );

    return GetItem(uiIndex)->IsChecked();
}

void wxCheckListBox::Check(unsigned int uiIndex, bool bCheck)
{
    wxCHECK_RET( IsValid(uiIndex), _T("bad wxCheckListBox index") );

    GetItem(uiIndex)->Check(bCheck);
}

// process events
// --------------

void wxCheckListBox::OnKeyDown(wxKeyEvent& event)
{
    // what do we do?
    enum
    {
        None,
        Toggle,
        Set,
        Clear
    } oper;

    switch ( event.GetKeyCode() )
    {
        case WXK_SPACE:
            oper = Toggle;
            break;

        case WXK_NUMPAD_ADD:
        case '+':
            oper = Set;
            break;

        case WXK_NUMPAD_SUBTRACT:
        case '-':
            oper = Clear;
            break;

        default:
            oper = None;
    }

    if ( oper != None )
    {
        wxArrayInt selections;
        int count = 0;
        if ( HasMultipleSelection() )
        {
            count = GetSelections(selections);
        }
        else
        {
            int sel = GetSelection();
            if (sel != -1)
            {
                count = 1;
                selections.Add(sel);
            }
        }

        for ( int i = 0; i < count; i++ )
        {
            wxCheckListBoxItem *item = GetItem(selections[i]);
            if ( !item )
            {
                wxFAIL_MSG( _T("no wxCheckListBoxItem?") );
                continue;
            }

            switch ( oper )
            {
                case Toggle:
                    item->Toggle();
                    break;

                case Set:
                case Clear:
                    item->Check( oper == Set );
                    break;

                default:
                    wxFAIL_MSG( _T("what should this key do?") );
            }

            // we should send an event as this has been done by the user and
            // not by the program
            item->SendEvent();
        }
    }
    else // nothing to do
    {
        event.Skip();
    }
}

void wxCheckListBox::OnLeftClick(wxMouseEvent& event)
{
    // clicking on the item selects it, clicking on the checkmark toggles
    if ( event.GetX() <= wxOwnerDrawn::GetDefaultMarginWidth() )
    {
        int nItem = HitTest(event.GetX(), event.GetY());

        if ( nItem != wxNOT_FOUND )
        {
            wxCheckListBoxItem *item = GetItem(nItem);
            item->Toggle();
            item->SendEvent();
        }
        //else: it's not an error, just click outside of client zone
    }
    else
    {
        // implement default behaviour: clicking on the item selects it
        event.Skip();
    }
}

int wxCheckListBox::DoHitTestItem(wxCoord x, wxCoord y) const
{
    int nItem = (int)::SendMessage
                             (
                              (HWND)GetHWND(),
                              LB_ITEMFROMPOINT,
                              0,
                              MAKELPARAM(x, y)
                             );

    return nItem >= (int)m_noItems ? wxNOT_FOUND : nItem;
}


wxSize wxCheckListBox::DoGetBestSize() const
{
    wxSize best = wxListBox::DoGetBestSize();
    best.x += wxOwnerDrawn::GetDefaultMarginWidth();  // add room for the checkbox
    CacheBestSize(best);
    return best;
}

#endif
