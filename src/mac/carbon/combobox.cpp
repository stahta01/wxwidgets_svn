/////////////////////////////////////////////////////////////////////////////
// Name:        src/mac/carbon/combobox.cpp
// Purpose:     wxComboBox class
// Author:      Stefan Csomor, Dan "Bud" Keith (composite combobox)
// Modified by:
// Created:     1998-01-01
// RCS-ID:      $Id$
// Copyright:   (c) Stefan Csomor
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#include "wx/wxprec.h"

#if wxUSE_COMBOBOX

#include "wx/combobox.h"

#ifndef WX_PRECOMP
    #include "wx/button.h"
    #include "wx/menu.h"
    #include "wx/containr.h"
    #include "wx/toplevel.h"
#endif

#include "wx/mac/uma.h"

WX_DELEGATE_TO_CONTROL_CONTAINER(wxComboBox, wxControl)

BEGIN_EVENT_TABLE(wxComboBox, wxControl)
    WX_EVENT_TABLE_CONTROL_CONTAINER(wxComboBox)
END_EVENT_TABLE()


static int nextPopUpMenuId = 1000 ;

MenuHandle NewUniqueMenu()
{
    MenuHandle handle = UMANewMenu(nextPopUpMenuId, wxString(wxT("Menu")), wxFont::GetDefaultEncoding() );
    nextPopUpMenuId++ ;

    return handle ;
}


// ----------------------------------------------------------------------------
// constants
// ----------------------------------------------------------------------------

// the margin between the text control and the choice
#if TARGET_API_MAC_OSX
// margin should be bigger on OS X due to blue highlight
// around text control.
static const wxCoord MARGIN = 4;
// this is the border a focus rect on OSX is needing
static const int    TEXTFOCUSBORDER = 3 ;
#else
static const wxCoord MARGIN = 2;
static const int    TEXTFOCUSBORDER = 0 ;
#endif


// ----------------------------------------------------------------------------
// wxComboBoxText: text control forwards events to combobox
// ----------------------------------------------------------------------------

class wxComboBoxText : public wxTextCtrl
{
public:
    wxComboBoxText( wxComboBox * cb )
        : wxTextCtrl( cb , 1 )
    {
        m_cb = cb;
        SetTriggerOnSetValue( false );
    }

protected:
    void OnChar( wxKeyEvent& event )
    {
        // Allows processing the tab key to go to the next control
        if (event.GetKeyCode() == WXK_TAB)
        {
            wxNavigationKeyEvent NavEvent;
            NavEvent.SetEventObject(this);
            NavEvent.SetDirection(true);
            NavEvent.SetWindowChange(false);

            // Get the parent of the combo and have it process the navigation?
            if (m_cb->GetParent()->GetEventHandler()->ProcessEvent(NavEvent))
                    return;
        }

        // send the event to the combobox class in case the user has bound EVT_CHAR
        wxKeyEvent kevt(event);
        kevt.SetEventObject(m_cb);
        if (m_cb->GetEventHandler()->ProcessEvent(kevt))
            // If the event was handled and not skipped then we're done
            return;

        if ( event.GetKeyCode() == WXK_RETURN )
        {
            wxCommandEvent event(wxEVT_COMMAND_TEXT_ENTER, m_cb->GetId());
            event.SetString( GetValue() );
            event.SetInt( m_cb->GetSelection() );
            event.SetEventObject( m_cb );

            // This will invoke the dialog default action,
            // such as the clicking the default button.
            if (!m_cb->GetEventHandler()->ProcessEvent( event ))
            {
                wxTopLevelWindow *tlw = wxDynamicCast(wxGetTopLevelParent(this), wxTopLevelWindow);
                if ( tlw && tlw->GetDefaultItem() )
                {
                    wxButton *def = wxDynamicCast(tlw->GetDefaultItem(), wxButton);
                    if ( def && def->IsEnabled() )
                    {
                        wxCommandEvent event( wxEVT_COMMAND_BUTTON_CLICKED, def->GetId() );
                        event.SetEventObject(def);
                        def->Command(event);
                    }
                }

                return;
            }
        }

        event.Skip();
    }

    void OnKeyUp( wxKeyEvent& event )
    {
        event.SetEventObject(m_cb);
        event.SetId(m_cb->GetId());
        if (! m_cb->GetEventHandler()->ProcessEvent(event))
            event.Skip();
    }

    void OnKeyDown( wxKeyEvent& event )
    {
        event.SetEventObject(m_cb);
        event.SetId(m_cb->GetId());
        if (! m_cb->GetEventHandler()->ProcessEvent(event))
            event.Skip();
    }

    void OnText( wxCommandEvent& event )
    {
        event.SetEventObject(m_cb);
        event.SetId(m_cb->GetId());
        if (! m_cb->GetEventHandler()->ProcessEvent(event))
            event.Skip();
    }

private:
    wxComboBox *m_cb;

    DECLARE_EVENT_TABLE()
};

BEGIN_EVENT_TABLE(wxComboBoxText, wxTextCtrl)
    EVT_KEY_DOWN(wxComboBoxText::OnKeyDown)
    EVT_CHAR(wxComboBoxText::OnChar)
    EVT_KEY_UP(wxComboBoxText::OnKeyUp)
    EVT_TEXT(wxID_ANY, wxComboBoxText::OnText)
END_EVENT_TABLE()

class wxComboBoxChoice : public wxChoice
{
public:
    wxComboBoxChoice( wxComboBox *cb, int style )
        : wxChoice( cb , 1 , wxDefaultPosition , wxDefaultSize , 0 , NULL , style & (wxCB_SORT) )
    {
        m_cb = cb;
    }

    int GetPopupWidth() const
    {
        switch ( GetWindowVariant() )
        {
            case wxWINDOW_VARIANT_NORMAL :
            case wxWINDOW_VARIANT_LARGE :
                return 24 ;

            default :
                return 21 ;
        }
    }

protected:
    void OnChoice( wxCommandEvent& e )
    {
        wxString    s = e.GetString();

        m_cb->DelegateChoice( s );
        wxCommandEvent event2(wxEVT_COMMAND_COMBOBOX_SELECTED, m_cb->GetId() );
        event2.SetInt(m_cb->GetSelection());
        event2.SetEventObject(m_cb);
        event2.SetString(m_cb->GetStringSelection());
        m_cb->ProcessCommand(event2);

        // For consistency with MSW and GTK, also send a text updated event
        // After all, the text is updated when a selection is made
        wxCommandEvent TextEvent( wxEVT_COMMAND_TEXT_UPDATED, m_cb->GetId() );
        TextEvent.SetString( m_cb->GetStringSelection() );
        TextEvent.SetEventObject( m_cb );
        m_cb->ProcessCommand( TextEvent );
    }

    virtual wxSize DoGetBestSize() const
    {
        wxSize sz = wxChoice::DoGetBestSize() ;
        if (! m_cb->HasFlag(wxCB_READONLY) )
            sz.x = GetPopupWidth() ;

        return sz ;
    }

private:
    wxComboBox *m_cb;

    friend class wxComboBox;

    DECLARE_EVENT_TABLE()
};

BEGIN_EVENT_TABLE(wxComboBoxChoice, wxChoice)
    EVT_CHOICE(wxID_ANY, wxComboBoxChoice::OnChoice)
END_EVENT_TABLE()

wxComboBox::~wxComboBox()
{
    // delete client objects
    FreeData();

    // delete the controls now, don't leave them alive even though they would
    // still be eventually deleted by our parent - but it will be too late, the
    // user code expects them to be gone now
    if (m_text != NULL)
    {
        delete m_text;
        m_text = NULL;
    }

    if (m_choice != NULL)
    {
        delete m_choice;
        m_choice = NULL;
    }
}

// ----------------------------------------------------------------------------
// geometry
// ----------------------------------------------------------------------------

wxSize wxComboBox::DoGetBestSize() const
{
    if (!m_choice && !m_text)
        return GetSize();

    wxSize size = m_choice->GetBestSize();

    if ( m_text != NULL )
    {
        wxSize  sizeText = m_text->GetBestSize();
        if (sizeText.y > size.y)
            size.y = sizeText.y;

        size.x = m_choice->GetPopupWidth() + sizeText.x + MARGIN;
        size.x += TEXTFOCUSBORDER ;
        size.y += 2 * TEXTFOCUSBORDER ;
    }
    else
    {
        // clipping is too tight
        size.y += 1 ;
    }

    return size;
}

void wxComboBox::DoMoveWindow(int x, int y, int width, int height)
{
    wxControl::DoMoveWindow( x, y, width , height );

    if ( m_text == NULL )
    {
        // we might not be fully constructed yet, therefore watch out...
        if ( m_choice )
            m_choice->SetSize(0, 0 , width, -1);
    }
    else
    {
        wxCoord wText = width - m_choice->GetPopupWidth() - MARGIN;
        m_text->SetSize(TEXTFOCUSBORDER, TEXTFOCUSBORDER, wText, -1);

        // put it at an inset of 1 to have outer area shadows drawn as well
        m_choice->SetSize(TEXTFOCUSBORDER + wText + MARGIN - 1 , TEXTFOCUSBORDER, m_choice->GetPopupWidth() , -1);
    }
}

// ----------------------------------------------------------------------------
// operations forwarded to the subcontrols
// ----------------------------------------------------------------------------

bool wxComboBox::Enable(bool enable)
{
    if ( !wxControl::Enable(enable) )
        return false;

    if (m_text)
        m_text->Enable(enable);

    return true;
}

bool wxComboBox::Show(bool show)
{
    if ( !wxControl::Show(show) )
        return false;

    return true;
}

void wxComboBox::DelegateTextChanged( const wxString& value )
{
    SetStringSelection( value );
}

void wxComboBox::DelegateChoice( const wxString& value )
{
    SetStringSelection( value );
}

void wxComboBox::Init()
{
    WX_INIT_CONTROL_CONTAINER();
}

bool wxComboBox::Create(wxWindow *parent,
    wxWindowID id,
    const wxString& value,
    const wxPoint& pos,
    const wxSize& size,
    const wxArrayString& choices,
    long style,
    const wxValidator& validator,
    const wxString& name)
{
    wxCArrayString chs( choices );

    return Create( parent, id, value, pos, size, chs.GetCount(),
                   chs.GetStrings(), style, validator, name );
}

bool wxComboBox::Create(wxWindow *parent,
    wxWindowID id,
    const wxString& value,
    const wxPoint& pos,
    const wxSize& size,
    int n,
    const wxString choices[],
    long style,
    const wxValidator& validator,
    const wxString& name)
{
    if ( !wxControl::Create(parent, id, wxDefaultPosition, wxDefaultSize, style ,
                            validator, name) )
    {
        return false;
    }

    m_choice = new wxComboBoxChoice(this, style );
    wxSize csize = size;
    if ( style & wxCB_READONLY )
    {
        m_text = NULL;
    }
    else
    {
        m_text = new wxComboBoxText(this);
        if ( size.y == -1 )
        {
            csize.y = m_text->GetSize().y ;
            csize.y += 2 * TEXTFOCUSBORDER ;
        }
    }

    DoSetSize(pos.x, pos.y, csize.x, csize.y);

    for ( int i = 0 ; i < n ; i++ )
    {
        m_choice->DoAppend( choices[ i ] );
    }

    // Needed because it is a wxControlWithItems
    SetInitialSize(size);
    SetStringSelection(value);

    return true;
}

wxString wxComboBox::GetValue() const
{
    wxString        result;

    if ( m_text == NULL )
        result = m_choice->GetString( m_choice->GetSelection() );
    else
        result = m_text->GetValue();

    return result;
}

unsigned int wxComboBox::GetCount() const
{
    return m_choice->GetCount() ;
}

void wxComboBox::SetValue(const wxString& value)
{
    if ( HasFlag(wxCB_READONLY) )
        SetStringSelection( value ) ;
    else
        m_text->SetValue( value );
}

// Clipboard operations

void wxComboBox::Copy()
{
    if ( m_text != NULL )
        m_text->Copy();
}

void wxComboBox::Cut()
{
    if ( m_text != NULL )
        m_text->Cut();
}

void wxComboBox::Paste()
{
    if ( m_text != NULL )
        m_text->Paste();
}

void wxComboBox::SetEditable(bool editable)
{
    if ( ( m_text == NULL ) && editable )
    {
        m_text = new wxComboBoxText( this );
    }
    else if ( ( m_text != NULL ) && !editable )
    {
        delete m_text;
        m_text = NULL;
    }

    int currentX, currentY;
    GetPosition( &currentX, &currentY );

    int currentW, currentH;
    GetSize( &currentW, &currentH );

    DoMoveWindow( currentX, currentY, currentW, currentH );
}

void wxComboBox::SetInsertionPoint(long pos)
{
    if ( m_text )
        m_text->SetInsertionPoint(pos);
}

void wxComboBox::SetInsertionPointEnd()
{
    if ( m_text )
        m_text->SetInsertionPointEnd();
}

long wxComboBox::GetInsertionPoint() const
{
    if ( m_text )
        return m_text->GetInsertionPoint();
    return 0;
}

wxTextPos wxComboBox::GetLastPosition() const
{
    if ( m_text )
        return m_text->GetLastPosition();
    return 0;
}

void wxComboBox::Replace(long from, long to, const wxString& value)
{
    if ( m_text )
        m_text->Replace(from,to,value);
}

void wxComboBox::Remove(long from, long to)
{
    if ( m_text )
        m_text->Remove(from,to);
}

void wxComboBox::SetSelection(long from, long to)
{
    if ( m_text )
        m_text->SetSelection(from,to);
}

int wxComboBox::DoAppend(const wxString& item)
{
    return m_choice->DoAppend( item ) ;
}

int wxComboBox::DoInsert(const wxString& item, unsigned int pos)
{
    return m_choice->DoInsert( item , pos ) ;
}

void wxComboBox::DoSetItemClientData(unsigned int n, void* clientData)
{
    return m_choice->DoSetItemClientData( n , clientData ) ;
}

void* wxComboBox::DoGetItemClientData(unsigned int n) const
{
    return m_choice->DoGetItemClientData( n ) ;
}

void wxComboBox::DoSetItemClientObject(unsigned int n, wxClientData* clientData)
{
    return m_choice->DoSetItemClientObject(n, clientData);
}

wxClientData* wxComboBox::DoGetItemClientObject(unsigned int n) const
{
    return m_choice->DoGetItemClientObject( n ) ;
}

void wxComboBox::FreeData()
{
    if ( HasClientObjectData() )
    {
        unsigned int count = GetCount();
        for ( unsigned int n = 0; n < count; n++ )
        {
            SetClientObject( n, NULL );
        }
    }
}

void wxComboBox::Delete(unsigned int n)
{
    // force client object deletion
    if( HasClientObjectData() )
        SetClientObject( n, NULL );
    m_choice->Delete( n );
}

void wxComboBox::Clear()
{
    FreeData();
    m_choice->Clear();
}

int wxComboBox::GetSelection() const
{
    return m_choice->GetSelection();
}

void wxComboBox::SetSelection(int n)
{
    m_choice->SetSelection( n );

    if ( m_text != NULL )
        m_text->SetValue(n != wxNOT_FOUND ? GetString(n) : wxString(wxEmptyString));
}

int wxComboBox::FindString(const wxString& s, bool bCase) const
{
    return m_choice->FindString( s, bCase );
}

wxString wxComboBox::GetString(unsigned int n) const
{
    return m_choice->GetString( n );
}

wxString wxComboBox::GetStringSelection() const
{
    int sel = GetSelection();
    if (sel != wxNOT_FOUND)
        return wxString(this->GetString((unsigned int)sel));
    else
        return wxEmptyString;
}

void wxComboBox::SetString(unsigned int n, const wxString& s)
{
    m_choice->SetString( n , s );
}

bool wxComboBox::IsEditable() const
{
    return m_text != NULL && !HasFlag(wxCB_READONLY);
}

void wxComboBox::Undo()
{
    if (m_text != NULL)
        m_text->Undo();
}

void wxComboBox::Redo()
{
    if (m_text != NULL)
        m_text->Redo();
}

void wxComboBox::SelectAll()
{
    if (m_text != NULL)
        m_text->SelectAll();
}

bool wxComboBox::CanCopy() const
{
    if (m_text != NULL)
        return m_text->CanCopy();
    else
        return false;
}

bool wxComboBox::CanCut() const
{
    if (m_text != NULL)
        return m_text->CanCut();
    else
        return false;
}

bool wxComboBox::CanPaste() const
{
    if (m_text != NULL)
        return m_text->CanPaste();
    else
        return false;
}

bool wxComboBox::CanUndo() const
{
    if (m_text != NULL)
        return m_text->CanUndo();
    else
        return false;
}

bool wxComboBox::CanRedo() const
{
    if (m_text != NULL)
        return m_text->CanRedo();
    else
        return false;
}

wxInt32 wxComboBox::MacControlHit( WXEVENTHANDLERREF WXUNUSED(handler) , WXEVENTREF WXUNUSED(event) )
{
/*
    For consistency with other platforms, clicking in the text area does not constitute a selection
    wxCommandEvent event(wxEVT_COMMAND_COMBOBOX_SELECTED, m_windowId );
    event.SetInt(GetSelection());
    event.SetEventObject(this);
    event.SetString(GetStringSelection());
    ProcessCommand(event);
*/

    return noErr ;
}

#endif // wxUSE_COMBOBOX
