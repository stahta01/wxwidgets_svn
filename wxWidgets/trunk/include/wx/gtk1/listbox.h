/////////////////////////////////////////////////////////////////////////////
// Name:        listbox.h
// Purpose:     wxListBox class declaration
// Author:      Robert Roebling
// Id:          $Id$
// Copyright:   (c) 1998 Robert Roebling
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////


#ifndef __GTKLISTBOXH__
#define __GTKLISTBOXH__

#ifdef __GNUG__
#pragma interface
#endif

#include "wx/defs.h"
#include "wx/object.h"
#include "wx/list.h"
#include "wx/control.h"

//-----------------------------------------------------------------------------
// classes
//-----------------------------------------------------------------------------

class wxListBox;
class wxArrayInt;

//-----------------------------------------------------------------------------
// global data
//-----------------------------------------------------------------------------

extern const char *wxListBoxNameStr;

//-----------------------------------------------------------------------------
// wxListBox
//-----------------------------------------------------------------------------

class wxListBox : public wxControl
{
DECLARE_DYNAMIC_CLASS(wxListBox)

public:
    wxListBox();
    wxListBox( wxWindow *parent, wxWindowID id,
            const wxPoint& pos = wxDefaultPosition,
            const wxSize& size = wxDefaultSize,
            int n = 0, const wxString choices[] = (const wxString *) NULL,
            long style = 0,
            const wxValidator& validator = wxDefaultValidator,
            const wxString& name = wxListBoxNameStr )
    {
        m_hasCheckBoxes = FALSE;
        Create(parent, id, pos, size, n, choices, style, validator, name);
    }
    virtual ~wxListBox();

    bool Create(wxWindow *parent, wxWindowID id,
                const wxPoint& pos = wxDefaultPosition,
                const wxSize& size = wxDefaultSize,
                int n = 0, const wxString choices[] = (const wxString *) NULL,
                long style = 0,
                const wxValidator& validator = wxDefaultValidator,
                const wxString& name = wxListBoxNameStr);

    void Append( const wxString &item );
    void Append( const wxString &item, void* clientData );
    void Append( const wxString &item, wxClientData* clientData );

    void InsertItems(int nItems, const wxString items[], int pos);

    void SetClientData( int n, void* clientData );
    void* GetClientData( int n );
    void SetClientObject( int n, wxClientData* clientData );
    wxClientData* GetClientObject( int n );

    void Clear();
    void Delete( int n );

    void Deselect( int n );
    int FindString( const wxString &item ) const;
    int GetSelection(void) const;
    int GetSelections( class wxArrayInt &) const;
    wxString GetString( int n ) const;
    wxString GetStringSelection(void) const;
    int Number();
    bool Selected( int n );
    void Set( int n, const wxString *choices );
    void SetFirstItem( int n );
    void SetFirstItem( const wxString &item );
    void SetSelection( int n, bool select = TRUE );
    void SetString( int n, const wxString &string );
    void SetStringSelection( const wxString &string, bool select = TRUE );

#if wxUSE_DRAG_AND_DROP
    void SetDropTarget( wxDropTarget *dropTarget );
#endif

    // implementation

    void AppendCommon( const wxString &item );
    int GetIndex( GtkWidget *item ) const;
    GtkWidget *GetConnectWidget();
    bool IsOwnGtkWindow( GdkWindow *window );
    void ApplyWidgetStyle();

#if wxUSE_TOOLTIPS
    void ApplyToolTip( GtkTooltips *tips, const wxChar *tip );
#endif // wxUSE_TOOLTIPS

    GtkList   *m_list;
    wxList     m_clientDataList;
    wxList     m_clientObjectList;
    bool       m_hasCheckBoxes;
};

#endif // __GTKLISTBOXH__
