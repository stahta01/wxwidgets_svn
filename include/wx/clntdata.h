/////////////////////////////////////////////////////////////////////////////
// Name:        wx/clntdata.h
// Purpose:     A mixin class for holding a wxClientData or void pointer
// Author:      Robin Dunn
// Modified by:
// Created:     9-Oct-2001
// RCS-ID:      $Id$
// Copyright:   (c) wxWindows team
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_CLNTDATAH__
#define _WX_CLNTDATAH__

#ifdef __GNUG__
    #pragma interface "event.h"
#endif

#include "wx/defs.h"

// ----------------------------------------------------------------------------

// what kind of client data do we have?
enum wxClientDataType
{
    wxClientData_None,    // we don't know yet because we don't have it at all
    wxClientData_Object,  // our client data is typed and we own it
    wxClientData_Void     // client data is untyped and we don't own it
};

class WXDLLEXPORT wxClientData
{
public:
    wxClientData() { }
    virtual ~wxClientData() { }
};

class WXDLLEXPORT wxStringClientData : public wxClientData
{
public:
    wxStringClientData() { }
    wxStringClientData( const wxString &data ) : m_data(data) { }
    void SetData( const wxString &data ) { m_data = data; }
    const wxString& GetData() const { return m_data; }

private:
    wxString  m_data;
};


class WXDLLEXPORT wxClientDataContainer
{
public:
    wxClientDataContainer();
    ~wxClientDataContainer();

        // each window may have associated client data: either a pointer to
        // wxClientData object in which case it is managed by the window (i.e.
        // it will delete the data when it's destroyed) or an untyped pointer
        // which won't be deleted by the window - but not both of them
    void SetClientObject( wxClientData *data ) { DoSetClientObject(data); }
    wxClientData *GetClientObject() const { return DoGetClientObject(); }

    void SetClientData( void *data ) { DoSetClientData(data); }
    void *GetClientData() const { return DoGetClientData(); }

protected:
    // user data associated with the window: either an object which will be
    // deleted by the window when it's deleted or some raw pointer which we do
    // nothing with - only one type of data can be used with the given window
    // (i.e. you cannot set the void data and then associate the window with
    // wxClientData or vice versa)
    union
    {
        wxClientData *m_clientObject;
        void         *m_clientData;
    };

    // client data accessors
    virtual void DoSetClientObject( wxClientData *data );
    virtual wxClientData *DoGetClientObject() const;

    virtual void DoSetClientData( void *data );
    virtual void *DoGetClientData() const;

    // what kind of data do we have?
    wxClientDataType m_clientDataType;

};

// ----------------------------------------------------------------------------
#endif

