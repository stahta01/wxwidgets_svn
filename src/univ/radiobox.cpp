/////////////////////////////////////////////////////////////////////////////
// Name:        univ/radiobox.cpp
// Purpose:     wxRadioBox implementation
// Author:      Vadim Zeitlin
// Modified by:
// Created:     11.09.00
// RCS-ID:      $Id$
// Copyright:   (c) 2000 SciTech Software, Inc. (www.scitechsoft.com)
// Licence:     wxWindows license
/////////////////////////////////////////////////////////////////////////////

// ============================================================================
// declarations
// ============================================================================

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

#ifdef __GNUG__
    #pragma implementation "univradiobox.h"
#endif

#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#if wxUSE_RADIOBOX

#ifndef WX_PRECOMP
    #include "wx/dcclient.h"
    #include "wx/radiobox.h"
    #include "wx/radiobut.h"
    #include "wx/validate.h"
#endif

#include "wx/univ/theme.h"
#include "wx/univ/renderer.h"
#include "wx/univ/inphand.h"
#include "wx/univ/colschem.h"

// ----------------------------------------------------------------------------
// constants
// ----------------------------------------------------------------------------

static const int BUTTON_BORDER_X = 2;
static const int BUTTON_BORDER_Y = 4;

static const int BOX_BORDER_X = 2;
static const int BOX_BORDER_Y = 2;

// ----------------------------------------------------------------------------
// wxRadioBox event handler
// ----------------------------------------------------------------------------

class wxRadioHookHandler : public wxEvtHandler
{
public:
    wxRadioHookHandler(wxRadioBox *radio) { m_radio = radio; }

    virtual bool ProcessEvent(wxEvent& event)
    {
        // we intercept the command events from radio buttons
        if ( event.GetEventType() == wxEVT_COMMAND_RADIOBUTTON_SELECTED )
        {
            m_radio->OnRadioButton(event);
        }
        else if ( event.GetEventType() == wxEVT_KEY_DOWN )
        {
            if ( m_radio->OnKeyDown((wxKeyEvent &)event) )
            {
                return TRUE;
            }
        }

        // just pass it on
        return GetNextHandler()->ProcessEvent(event);
    }

private:
    wxRadioBox *m_radio;
};

// ============================================================================
// implementation
// ============================================================================

IMPLEMENT_DYNAMIC_CLASS(wxRadioBox, wxControl)

// ----------------------------------------------------------------------------
// wxRadioBox creation
// ----------------------------------------------------------------------------

void wxRadioBox::Init()
{
    m_selection = -1;
    m_majorDim = 0;
}

bool wxRadioBox::Create(wxWindow *parent,
                        wxWindowID id,
                        const wxString& title,
                        const wxPoint& pos,
                        const wxSize& size,
                        int n,
                        const wxString *choices,
                        int majorDim,
                        long style,
                        const wxValidator& val,
                        const wxString& name)
{
    // for compatibility with the other ports which don't handle (yet?)
    // wxRA_LEFTTORIGHT and wxRA_TOPTOBOTTOM flags, we add them ourselves if
    // not specified
    if ( !(style & (wxRA_LEFTTORIGHT | wxRA_TOPTOBOTTOM)) )
    {
        // horizontal radiobox use left to right layout
        if ( style & wxRA_HORIZONTAL )
        {
            style |= wxRA_LEFTTORIGHT;
        }
        else if ( style & wxRA_VERTICAL )
        {
            style |= wxRA_TOPTOBOTTOM;
        }
        else
        {
            wxFAIL_MSG( _T("you must specify wxRA_XXX style!") );

            // use default
            style = wxRA_HORIZONTAL | wxRA_LEFTTORIGHT;
        }
    }

    if ( !wxStaticBox::Create(parent, id, title, pos, size, style, name) )
        return FALSE;

#if wxUSE_VALIDATORS
    SetValidator(val);
#endif // wxUSE_VALIDATORS

    Append(n, choices);

    // majorDim default value is 0 which means make one row/column
    SetMajorDim(majorDim == 0 ? n : majorDim);

    if ( size == wxDefaultSize )
    {
        SetClientSize(DoGetBestClientSize());
    }

    // radiobox should already have selection so select at least one item
    SetSelection(0);

    return TRUE;
}

wxRadioBox::~wxRadioBox()
{
    // remove the event handlers we pushed on them from all buttons and delete
    // the buttons themselves: this must be done as the user code expects them
    // to disappear now and not some time later when they will be deleted by
    // our (common) parent
    size_t count = m_buttons.GetCount();
    for ( size_t n = 0; n < count; n++ )
    {
        m_buttons[n]->PopEventHandler(TRUE /* delete it */);

        delete m_buttons[n];
    }
}

// ----------------------------------------------------------------------------
// wxRadioBox init
// ----------------------------------------------------------------------------

void wxRadioBox::SetMajorDim(int majorDim)
{
    wxCHECK_RET( majorDim != 0, _T("major radiobox dimension can't be 0") );

    m_majorDim = majorDim;

    int minorDim = (GetCount() + m_majorDim - 1) / m_majorDim;

    if ( GetWindowStyle() & wxRA_SPECIFY_COLS )
    {
        m_numCols = majorDim;
        m_numRows = minorDim;
    }
    else // wxRA_SPECIFY_ROWS
    {
        m_numCols = minorDim;
        m_numRows = majorDim;
    }
}

void wxRadioBox::Append(int count, const wxString *choices)
{
    if ( !count )
        return;

    wxWindow *parent = GetParent();
    m_buttons.Alloc(count);
    for ( int n = 0; n < count; n++ )
    {
        // make the first button in the box the start of new group by giving it
        // wxRB_GROUP style
        wxRadioButton *btn = new wxRadioButton(parent, -1, choices[n],
                                               wxDefaultPosition,
                                               wxDefaultSize,
                                               n == 0 ? wxRB_GROUP : 0);

        // we want to get the events from the buttons to translate it into
        btn->PushEventHandler(new wxRadioHookHandler(this));
        m_buttons.Add(btn);
    }
}

// ----------------------------------------------------------------------------
// selection
// ----------------------------------------------------------------------------

void wxRadioBox::SetSelection(int n)
{
    wxCHECK_RET( IsValid(n), _T("invalid index in wxRadioBox::SetSelection") );

    m_selection = n;

    wxRadioButton *btn = m_buttons[n];

    // the selected button is always focused in the radiobox
    btn->SetFocus();

    // this will also unselect the previously selected button in our group
    btn->SetValue(TRUE);
}

int wxRadioBox::GetSelection() const
{
    return m_selection;
}

void wxRadioBox::SendRadioEvent()
{
    wxCHECK_RET( m_selection != -1, _T("no active radio button") );

    wxCommandEvent event(wxEVT_COMMAND_RADIOBOX_SELECTED, GetId());
    InitCommandEvent(event);
    event.SetInt(m_selection);
    event.SetString(GetString(m_selection));

    Command(event);
}

void wxRadioBox::OnRadioButton(wxEvent& event)
{
    int n = m_buttons.Index((wxRadioButton *)event.GetEventObject());
    wxCHECK_RET( n != wxNOT_FOUND, _T("click from alien radio button") );

    m_selection = n;

    SendRadioEvent();
}

// ----------------------------------------------------------------------------
// methods forwarded to the buttons
// ----------------------------------------------------------------------------

wxString wxRadioBox::GetString(int n) const
{
    wxCHECK_MSG( IsValid(n), _T(""),
                 _T("invalid index in wxRadioBox::GetString") );

    return m_buttons[n]->GetLabel();
}

void wxRadioBox::SetString(int n, const wxString& label)
{
    wxCHECK_RET( IsValid(n), _T("invalid index in wxRadioBox::SetString") );

    m_buttons[n]->SetLabel(label);
}

void wxRadioBox::Enable(int n, bool enable)
{
    wxCHECK_RET( IsValid(n), _T("invalid index in wxRadioBox::Enable") );

    m_buttons[n]->Enable(enable);
}

void wxRadioBox::Show(int n, bool show)
{
    wxCHECK_RET( IsValid(n), _T("invalid index in wxRadioBox::Show") );

    m_buttons[n]->Show(show);
}

// ----------------------------------------------------------------------------
// methods forwarded to the static box
// ----------------------------------------------------------------------------

bool wxRadioBox::Enable(bool enable)
{
    return wxStaticBox::Enable(enable);
}

bool wxRadioBox::Show(bool show)
{
    return wxStaticBox::Show(show);
}

wxString wxRadioBox::GetLabel() const
{
    return wxStaticBox::GetLabel();
}

void wxRadioBox::SetLabel(const wxString& label)
{
    wxStaticBox::SetLabel(label);
}

// ----------------------------------------------------------------------------
// buttons positioning
// ----------------------------------------------------------------------------

wxSize wxRadioBox::GetMaxButtonSize() const
{
    int widthMax, heightMax, width, height;
    widthMax = heightMax = 0;

    int count = GetCount();
    for ( int n = 0; n < count; n++ )
    {
        m_buttons[n]->GetBestSize(&width, &height);

        if ( width > widthMax )
            widthMax = width;
        if ( height > heightMax )
            heightMax = height;
    }

    return wxSize(widthMax + BUTTON_BORDER_X, heightMax + BUTTON_BORDER_Y);
}

wxSize wxRadioBox::DoGetBestClientSize() const
{
    wxSize sizeBtn = GetMaxButtonSize();

    sizeBtn.x *= m_numCols;
    sizeBtn.y *= m_numRows;

    // add a border around all buttons
    sizeBtn.x += 2*BOX_BORDER_X;
    sizeBtn.y += 2*BOX_BORDER_Y;

    // account for the area taken by static box
    wxRect rect = GetBorderGeometry();
    sizeBtn.x += rect.x + rect.width;
    sizeBtn.y += rect.y + rect.height;

    return sizeBtn;
}

void wxRadioBox::DoMoveWindow(int x0, int y0, int width, int height)
{
    wxStaticBox::DoMoveWindow(x0, y0, width, height);

    wxSize sizeBtn = GetMaxButtonSize();
    wxPoint ptOrigin = GetBoxAreaOrigin();

    x0 += ptOrigin.x + BOX_BORDER_X;
    y0 += ptOrigin.y + BOX_BORDER_Y;

    int x = x0,
        y = y0;

    int count = GetCount();
    for ( int n = 0; n < count; n++ )
    {
        m_buttons[n]->SetSize(x, y, sizeBtn.x, sizeBtn.y);

        if ( GetWindowStyle() & wxRA_TOPTOBOTTOM )
        {
            // from top to bottom
            if ( (n + 1) % m_numRows )
            {
                // continue in this column
                y += sizeBtn.y;
            }
            else
            {
                // start a new column
                x += sizeBtn.x;
                y = y0;
            }
        }
        else // wxRA_LEFTTORIGHT: mirror the code above
        {
            // from left to right
            if ( (n + 1) % m_numCols )
            {
                // continue in this row
                x += sizeBtn.x;
            }
            else
            {
                // start a new row
                y += sizeBtn.y;
                x = x0;
            }
        }
    }
}

// ----------------------------------------------------------------------------
// keyboard navigation
// ----------------------------------------------------------------------------

bool wxRadioBox::OnKeyDown(wxKeyEvent& event)
{
    wxDirection dir;
    switch ( event.GetKeyCode() )
    {
        case WXK_UP:
            dir = wxUP;
            break;

        case WXK_LEFT:
            dir = wxLEFT;
            break;

        case WXK_DOWN:
            dir = wxDOWN;
            break;

        case WXK_RIGHT:
            dir = wxRIGHT;
            break;

        default:
            return FALSE;
    }

    int selOld = GetSelection();
    int selNew = GetNextItem(selOld, dir, GetWindowStyle());
    if ( selNew != selOld )
    {
        SetSelection(selNew);

        // emulate the button click
        SendRadioEvent();
    }

    return TRUE;
}

#endif // wxUSE_RADIOBOX

