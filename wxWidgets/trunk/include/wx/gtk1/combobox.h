/////////////////////////////////////////////////////////////////////////////
// Name:        combobox.h
// Purpose:
// Author:      Robert Roebling
// Created:     01/02/97
// Id:
// Copyright:   (c) 1998 Robert Roebling, Julian Smart and Markus Holzem
// Licence:   	wxWindows licence
/////////////////////////////////////////////////////////////////////////////


#ifndef __GTKCOMBOBOXH__
#define __GTKCOMBOBOXH__

#ifdef __GNUG__
#pragma interface
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

  inline wxComboBox(wxWindow *parent, const wxWindowID id,
           const wxString& value = wxEmptyString,
           const wxPoint& pos = wxDefaultPosition,
           const wxSize& size = wxDefaultSize,
           const int n = 0, const wxString choices[] = NULL,
           const long style = 0,
           const wxString& name = wxComboBoxNameStr)
  {
    Create(parent, id, value, pos, size, n, choices, style, name);
  }

  bool Create(wxWindow *parent, const wxWindowID id,
           const wxString& value = wxEmptyString,
           const wxPoint& pos = wxDefaultPosition,
           const wxSize& size = wxDefaultSize,
           const int n = 0, const wxString choices[] = NULL,
           const long style = 0,
           const wxString& name = wxComboBoxNameStr);

  // List functions
  void Clear(void);
  void Append( const wxString &item );
  void Append( const wxString &item, char* clientData );
  void Delete( const int n );
  int FindString( const wxString &item );
  char* GetClientData( const int n );
  void SetClientData( const int n, char * clientData );
  int GetSelection(void) const;
  wxString GetString( const int n ) const;
  wxString GetStringSelection(void) const;
  int Number(void) const;
  void SetSelection( const int n );
  
  // Text field functions
  wxString GetValue(void) const ;
  void SetValue(const wxString& value);

  // Clipboard operations
  void Copy(void);
  void Cut(void);
  void Paste(void);
  void SetInsertionPoint(const long pos);
  void SetInsertionPointEnd(void);
  long GetInsertionPoint(void) const ;
  long GetLastPosition(void) const ;
  void Replace(const long from, const long to, const wxString& value);
  void Remove(const long from, const long to);
  void SetSelection(const long from, const long to);
  void SetEditable(const bool editable);
 
  private:
  
  wxList m_clientData;
   
};

#endif // __GTKCOMBOBOXH__
