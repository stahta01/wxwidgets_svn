/////////////////////////////////////////////////////////////////////////////
// Name:        combobox.h
// Purpose:
// Author:      Robert Roebling
// Created:     01/02/97
// Id:          $Id$
// Copyright:   (c) 1998 Robert Roebling, Julian Smart and Markus Holzem
// Licence:   	wxWindows licence
/////////////////////////////////////////////////////////////////////////////


#ifndef __GTKCOMBOBOXH__
#define __GTKCOMBOBOXH__

#ifdef __GNUG__
#pragma interface "combobox.h"
#endif

#include "wx/defs.h"
#include "wx/object.h"
#include "wx/control.h"

//-----------------------------------------------------------------------------
// classes
//-----------------------------------------------------------------------------

class wxComboBox;

//-----------------------------------------------------------------------------
// global data
//-----------------------------------------------------------------------------

extern const char* wxComboBoxNameStr;
extern const char* wxEmptyString;

//-----------------------------------------------------------------------------
// wxComboBox
//-----------------------------------------------------------------------------

class wxComboBox: public wxControl
{
  DECLARE_DYNAMIC_CLASS(wxComboBox)

 public:
  inline wxComboBox(void) {}

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
  bool Create(wxWindow *parent, wxWindowID id,
           const wxString& value = wxEmptyString,
           const wxPoint& pos = wxDefaultPosition,
           const wxSize& size = wxDefaultSize,
           int n = 0, const wxString choices[] = (const wxString *) NULL,
           long style = 0,
           const wxValidator& validator = wxDefaultValidator,
           const wxString& name = wxComboBoxNameStr);

  void Clear(void);
  void Append( const wxString &item );
  void Append( const wxString &item, char* clientData );
  void Delete( int n );
  int FindString( const wxString &item );
  char* GetClientData( int n );
  void SetClientData( int n, char * clientData );
  int GetSelection(void) const;
  wxString GetString( int n ) const;
  wxString GetStringSelection(void) const;
  int Number(void) const;
  void SetSelection( int n );
  void SetStringSelection( const wxString &string );

  wxString GetValue(void) const ;
  void SetValue(const wxString& value);

  void Copy(void);
  void Cut(void);
  void Paste(void);
  void SetInsertionPoint(long pos);
  void SetInsertionPointEnd(void);
  long GetInsertionPoint(void) const ;
  long GetLastPosition(void) const ;
  void Replace(long from, long to, const wxString& value);
  void Remove(long from, long to);
  void SetSelection(long from, long to);
  void SetEditable(bool editable);
  
  void SetFont( const wxFont &font );
  void OnSize( wxSizeEvent &event );
  void SetBackgroundColour( const wxColour &colour );
  
  // implementation

    bool     m_alreadySent;
    
  private:
  
    wxList   m_clientData;

  GtkWidget* GetConnectWidget(void);
  bool IsOwnGtkWindow( GdkWindow *window );
  
  DECLARE_EVENT_TABLE()
};

#endif // __GTKCOMBOBOXH__
