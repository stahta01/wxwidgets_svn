/////////////////////////////////////////////////////////////////////////////
// Name:        combobox.h
// Purpose:
// Author:      Robert Roebling
// Created:     01/02/97
// Id:          $Id$
// Copyright:   (c) 1998 Robert Roebling
// Licence:   	wxWindows licence
/////////////////////////////////////////////////////////////////////////////


#ifndef __GTKCOMBOBOXH__
#define __GTKCOMBOBOXH__

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "combobox.h"
#endif

#include "wx/defs.h"

#if wxUSE_COMBOBOX

#include "wx/object.h"
#include "wx/control.h"

//-----------------------------------------------------------------------------
// classes
//-----------------------------------------------------------------------------

class wxComboBox;

//-----------------------------------------------------------------------------
// global data
//-----------------------------------------------------------------------------

extern const wxChar* wxComboBoxNameStr;
extern const wxChar* wxEmptyString;

//-----------------------------------------------------------------------------
// wxComboBox
//-----------------------------------------------------------------------------

class wxComboBox : public wxControl
{
public:
    inline wxComboBox() {}
    inline wxComboBox(wxWindow *parent, wxWindowID id,
           const wxString& value = wxEmptyString,
           const wxPoint& pos = wxDefaultPosition,
           const wxSize& size = wxDefaultSize,
           int n = 0, const wxString choices[] = (const wxString *) NULL,
           long style = 0,
           const wxValidator& validator = wxDefaultValidator,
           const wxString& name = wxComboBoxNameStr)
    {
        Create(parent, id, value, pos, size, n, choices, style, validator, name);
    }
    ~wxComboBox();
    bool Create(wxWindow *parent, wxWindowID id,
           const wxString& value = wxEmptyString,
           const wxPoint& pos = wxDefaultPosition,
           const wxSize& size = wxDefaultSize,
           int n = 0, const wxString choices[] = (const wxString *) NULL,
           long style = 0,
           const wxValidator& validator = wxDefaultValidator,
           const wxString& name = wxComboBoxNameStr);

    int Append( const wxString &item );
    int Append( const wxString &item, void* clientData );
    int Append( const wxString &item, wxClientData* clientData );

    int Insert( const wxString &item, int pos );
    int Insert( const wxString &item, int pos, void* clientData );
    int Insert( const wxString &item, int pos, wxClientData* clientData );

    void SetClientData( int n, void* clientData );
    void* GetClientData( int n ) const;
    void SetClientObject( int n, wxClientData* clientData );
    wxClientData* GetClientObject( int n ) const;

    void SetClientObject( wxClientData *data )  { wxControl::SetClientObject( data ); }
    wxClientData *GetClientObject() const       { return wxControl::GetClientObject(); }
    void SetClientData( void *data )            { wxControl::SetClientData( data ); }
    void *GetClientData() const                 { return wxControl::GetClientData(); }

    void Clear();
    void Delete( int n );

    int FindString( const wxString &item );
    int GetSelection() const;
    wxString GetString( int n ) const;
    wxString GetStringSelection() const;
    int GetCount() const { return Number(); }
    int Number() const;
    void SetSelection( int n );
    void SetStringSelection( const wxString &string );
    void SetString(int n, const wxString &text);

    wxString GetValue() const;
    void SetValue(const wxString& value);

    void Copy();
    void Cut();
    void Paste();
    void SetInsertionPoint( long pos );
    void SetInsertionPointEnd();
    long GetInsertionPoint() const;
    long GetLastPosition() const;
    void Replace( long from, long to, const wxString& value );
    void Remove( long from, long to );
    void SetSelection( long from, long to );
    void SetEditable( bool editable );

    // implementation

    virtual void SetFocus();

    void OnSize( wxSizeEvent &event );
    void OnChar( wxKeyEvent &event );

    bool     m_alreadySent;
    wxList   m_clientDataList;
    wxList   m_clientObjectList;
    int      m_prevSelection;

    void DisableEvents();
    void EnableEvents();
    int AppendCommon( const wxString &item );
    int InsertCommon( const wxString &item, int pos );
    GtkWidget* GetConnectWidget();
    bool IsOwnGtkWindow( GdkWindow *window );
    void ApplyWidgetStyle();

protected:
    virtual wxSize DoGetBestSize() const;

private:
    DECLARE_DYNAMIC_CLASS(wxComboBox)
    DECLARE_EVENT_TABLE()
};

#endif

#endif

  // __GTKCOMBOBOXH__
