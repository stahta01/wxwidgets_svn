/////////////////////////////////////////////////////////////////////////////
// Name:        msw/radiobut.cpp
// Purpose:     wxRadioButton
// Author:      Julian Smart
// Modified by:
// Created:     04/01/98
// RCS-ID:      $Id$
// Copyright:   (c) Julian Smart
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// ============================================================================
// declarations
// ============================================================================

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
    #pragma implementation "radiobut.h"
#endif

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#if wxUSE_RADIOBTN

#ifndef WX_PRECOMP
    #include "wx/radiobut.h"
    #include "wx/settings.h"
    #include "wx/dcscreen.h"
#endif

#include "wx/msw/private.h"

// ============================================================================
// wxRadioButton implementation
// ============================================================================

// ----------------------------------------------------------------------------
// wxRadioButton creation
// ----------------------------------------------------------------------------


#if wxUSE_EXTENDED_RTTI
WX_DEFINE_FLAGS( wxRadioButtonStyle )

wxBEGIN_FLAGS( wxRadioButtonStyle )
    // new style border flags, we put them first to
    // use them for streaming out
    wxFLAGS_MEMBER(wxBORDER_SIMPLE)
    wxFLAGS_MEMBER(wxBORDER_SUNKEN)
    wxFLAGS_MEMBER(wxBORDER_DOUBLE)
    wxFLAGS_MEMBER(wxBORDER_RAISED)
    wxFLAGS_MEMBER(wxBORDER_STATIC)
    wxFLAGS_MEMBER(wxBORDER_NONE)
    
    // old style border flags
    wxFLAGS_MEMBER(wxSIMPLE_BORDER)
    wxFLAGS_MEMBER(wxSUNKEN_BORDER)
    wxFLAGS_MEMBER(wxDOUBLE_BORDER)
    wxFLAGS_MEMBER(wxRAISED_BORDER)
    wxFLAGS_MEMBER(wxSTATIC_BORDER)
    wxFLAGS_MEMBER(wxNO_BORDER)

    // standard window styles
    wxFLAGS_MEMBER(wxTAB_TRAVERSAL)
    wxFLAGS_MEMBER(wxCLIP_CHILDREN)
    wxFLAGS_MEMBER(wxTRANSPARENT_WINDOW)
    wxFLAGS_MEMBER(wxWANTS_CHARS)
    wxFLAGS_MEMBER(wxNO_FULL_REPAINT_ON_RESIZE)
    wxFLAGS_MEMBER(wxALWAYS_SHOW_SB )
    wxFLAGS_MEMBER(wxVSCROLL)
    wxFLAGS_MEMBER(wxHSCROLL)

    wxFLAGS_MEMBER(wxRB_GROUP)

wxEND_FLAGS( wxRadioButtonStyle )

IMPLEMENT_DYNAMIC_CLASS_XTI(wxRadioButton, wxControl,"wx/radiobut.h")

wxBEGIN_PROPERTIES_TABLE(wxRadioButton)
    wxEVENT_PROPERTY( Click , wxEVT_COMMAND_RADIOBUTTON_SELECTED , wxCommandEvent )
    wxPROPERTY( Font , wxFont , SetFont , GetFont  , , 0 /*flags*/ , wxT("Helpstring") , wxT("group"))
    wxPROPERTY( Label,wxString, SetLabel, GetLabel, wxString(), 0 /*flags*/ , wxT("Helpstring") , wxT("group") )
    wxPROPERTY( Value ,bool, SetValue, GetValue,, 0 /*flags*/ , wxT("Helpstring") , wxT("group") )
    wxPROPERTY_FLAGS( WindowStyle , wxRadioButtonStyle , long , SetWindowStyleFlag , GetWindowStyleFlag , , 0 /*flags*/ , wxT("Helpstring") , wxT("group")) // style
wxEND_PROPERTIES_TABLE()

wxBEGIN_HANDLERS_TABLE(wxRadioButton)
wxEND_HANDLERS_TABLE()

wxCONSTRUCTOR_6( wxRadioButton , wxWindow* , Parent , wxWindowID , Id , wxString , Label , wxPoint , Position , wxSize , Size , long , WindowStyle ) 

#else
IMPLEMENT_DYNAMIC_CLASS(wxRadioButton, wxControl)
#endif


void wxRadioButton::Init()
{
    m_isChecked = false;
}

bool wxRadioButton::Create(wxWindow *parent,
                           wxWindowID id,
                           const wxString& label,
                           const wxPoint& pos,
                           const wxSize& size,
                           long style,
                           const wxValidator& validator,
                           const wxString& name)
{
    if ( !CreateControl(parent, id, pos, size, style, validator, name) )
        return false;

    long msStyle = WS_TABSTOP;
    if ( HasFlag(wxRB_GROUP) )
        msStyle |= WS_GROUP;

    /*
       wxRB_SINGLE is a temporary workaround for the following problem: if you
       have 2 radiobuttons in the same group but which are not consecutive in
       the dialog, Windows can enter an infinite loop! The simplest way to
       reproduce it is to create radio button, then a panel and then another
       radio button: then checking the last button hangs the app.

       Ideally, we'd detect (and avoid) such situation automatically but for
       now, as I don't know how to do it, just allow the user to create
       BS_RADIOBUTTON buttons for such situations.
     */
    msStyle |= HasFlag(wxRB_SINGLE) ? BS_RADIOBUTTON : BS_AUTORADIOBUTTON;

    if ( HasFlag(wxCLIP_SIBLINGS) )
        msStyle |= WS_CLIPSIBLINGS;

    if ( !MSWCreateControl(_T("BUTTON"), msStyle, pos, size, label, 0) )
        return false;

    // for compatibility with wxGTK, the first radio button in a group is
    // always checked (this makes sense anyhow as you need to ensure that at
    // least one button in the group is checked and this is the simlpest way to
    // do it)
    if ( HasFlag(wxRB_GROUP) )
        SetValue(true);

    return true;
}

// ----------------------------------------------------------------------------
// wxRadioButton functions
// ----------------------------------------------------------------------------

void wxRadioButton::SetValue(bool value)
{
    // BST_CHECKED is defined as 1, BST_UNCHECKED as 0, so we can just pass
    // value as is (we don't use BST_XXX here as they're not defined for Win16)
    (void)::SendMessage(GetHwnd(), BM_SETCHECK, (WPARAM)value, 0L);

    m_isChecked = value;

    // if we set the value of one radio button we also must clear all the other
    // buttons in the same group: Windows doesn't do it automatically
    if ( m_isChecked )
    {
        const wxWindowList& siblings = GetParent()->GetChildren();
        wxWindowList::compatibility_iterator nodeThis = siblings.Find(this);
        wxCHECK_RET( nodeThis, _T("radio button not a child of its parent?") );

        // if it's not the first item of the group ...
        if ( !HasFlag(wxRB_GROUP) )
        {
            // ... turn off all radio buttons before it
            for ( wxWindowList::compatibility_iterator nodeBefore = nodeThis->GetPrevious();
                  nodeBefore;
                  nodeBefore = nodeBefore->GetPrevious() )
            {
                wxRadioButton *btn = wxDynamicCast(nodeBefore->GetData(),
                                                   wxRadioButton);
                if ( !btn )
                {
                    // the radio buttons in a group must be consecutive, so
                    // there are no more of them
                    break;
                }

                btn->SetValue(false);

                if ( btn->HasFlag(wxRB_GROUP) )
                {
                    // even if there are other radio buttons before this one,
                    // they're not in the same group with us
                    break;
                }
            }
        }

        // ... and also turn off all buttons after this one
        for ( wxWindowList::compatibility_iterator nodeAfter = nodeThis->GetNext();
              nodeAfter;
              nodeAfter = nodeAfter->GetNext() )
        {
            wxRadioButton *btn = wxDynamicCast(nodeAfter->GetData(),
                                               wxRadioButton);

            if ( !btn || btn->HasFlag(wxRB_GROUP) )
            {
                // no more buttons or the first button of the next group
                break;
            }

            btn->SetValue(false);
        }
    }
}

bool wxRadioButton::GetValue() const
{
    wxASSERT_MSG( m_isChecked ==
                    (::SendMessage(GetHwnd(), BM_GETCHECK, 0, 0L) != 0),
                  _T("wxRadioButton::m_isChecked is out of sync?") );

    return m_isChecked;
}

// ----------------------------------------------------------------------------
// wxRadioButton event processing
// ----------------------------------------------------------------------------

void wxRadioButton::Command (wxCommandEvent& event)
{
    SetValue(event.m_commandInt != 0);
    ProcessCommand(event);
}

bool wxRadioButton::MSWCommand(WXUINT param, WXWORD WXUNUSED(id))
{
    if ( param != BN_CLICKED )
        return false;

    if ( !m_isChecked )
    {
        // we have to do this for BS_RADIOBUTTON anyhow and, strangely enough,
        // sometimes this is needed even for BS_AUTORADIOBUTTON (when we
        // receive focus the button gets BN_CLICKED but stays unchecked!)
        SetValue(true);

        wxCommandEvent event(wxEVT_COMMAND_RADIOBUTTON_SELECTED, GetId());
        event.SetEventObject( this );
        event.SetInt(true); // always checked

        ProcessCommand(event);
    }

    return true;
}

// ----------------------------------------------------------------------------
// wxRadioButton geometry
// ----------------------------------------------------------------------------

wxSize wxRadioButton::DoGetBestSize() const
{
    static int s_radioSize = 0;

    if ( !s_radioSize )
    {
        wxScreenDC dc;
        dc.SetFont(wxSystemSettings::GetFont(wxSYS_DEFAULT_GUI_FONT));

        s_radioSize = dc.GetCharHeight();
    }

    wxString str = GetLabel();

    int wRadio, hRadio;
    if ( !str.empty() )
    {
        GetTextExtent(str, &wRadio, &hRadio);
        wRadio += s_radioSize + GetCharWidth();

        if ( hRadio < s_radioSize )
            hRadio = s_radioSize;
    }
    else
    {
        wRadio = s_radioSize;
        hRadio = s_radioSize;
    }

    return wxSize(wRadio, hRadio);
}

#endif // wxUSE_RADIOBTN

