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

class wxComboBox : public wxControl, public wxComboBoxBase
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
    inline wxComboBox(wxWindow *parent, wxWindowID id,
           const wxString& value,
           const wxPoint& pos,
           const wxSize& size,
           const wxArrayString& choices,
           long style = 0,
           const wxValidator& validator = wxDefaultValidator,
           const wxString& name = wxComboBoxNameStr)
    {
        Create(parent, id, value, pos, size, choices, style, validator, name);
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
    bool Create(wxWindow *parent, wxWindowID id,
           const wxString& value,
           const wxPoint& pos,
           const wxSize& size,
           const wxArrayString& choices,
           long style = 0,
           const wxValidator& validator = wxDefaultValidator,
           const wxString& name = wxComboBoxNameStr);

    void Clear();
    void Delete( int n );

    virtual int FindString( const wxString &item ) const;
    int GetSelection() const;
    wxString GetString( int n ) const;
    wxString GetStringSelection() const;
    int GetCount() const;
    int Number() const { return GetCount(); }
    void SetSelection( int n );
    void Select( int n ) { return SetSelection( n ); }
    bool SetStringSelection( const wxString &string );
    void SetString(int n, const wxString &text);

    wxString GetValue() const;
    void SetValue(const wxString& value);

    void Copy();
    void Cut();
    void Paste();
    void SetInsertionPoint( long pos );
    void SetInsertionPointEnd() { SetInsertionPoint( -1 ); }
    long GetInsertionPoint() const;
    long GetLastPosition() const;
    void Remove(long from, long to) { Replace(from, to, wxEmptyString); }
    void Replace( long from, long to, const wxString& value );
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
    GtkWidget* GetConnectWidget();
    bool IsOwnGtkWindow( GdkWindow *window );
    void ApplyWidgetStyle();

    wxCONTROL_ITEMCONTAINER_CLIENTDATAOBJECT_RECAST

    static wxVisualAttributes
    GetClassDefaultAttributes(wxWindowVariant variant = wxWINDOW_VARIANT_NORMAL);
    
protected:
    virtual int DoAppend(const wxString& item);
    virtual int DoInsert(const wxString& item, int pos);

    virtual void DoSetItemClientData( int n, void* clientData );
    virtual void* DoGetItemClientData( int n ) const;
    virtual void DoSetItemClientObject( int n, wxClientData* clientData );
    virtual wxClientData* DoGetItemClientObject( int n ) const;

    virtual wxSize DoGetBestSize() const;

    // Widgets that use the style->base colour for the BG colour should
    // override this and return true.
    virtual bool UseGTKStyleBase() const { return true; }

private:
    DECLARE_DYNAMIC_CLASS_NO_COPY(wxComboBox)
    DECLARE_EVENT_TABLE()
};

#endif

#endif

  // __GTKCOMBOBOXH__
