///////////////////////////////////////////////////////////////////////////////
// Name:        gtk/dataobj.h
// Purpose:     declaration of the wxDataObject
// Author:      Robert Roebling
// RCS-ID:      $Id$
// Copyright:   (c) 1998, 1999 Vadim Zeitlin, Robert Roebling
// Licence:     wxWindows license
///////////////////////////////////////////////////////////////////////////////

#ifndef _WX_GTK_DATAOBJ_H_
#define _WX_GTK_DATAOBJ_H_

#ifdef __GNUG__
    #pragma interface "dataobj.h"
#endif

// ----------------------------------------------------------------------------
// wxDataObject is the same as wxDataObjectBase under wxGTK
// ----------------------------------------------------------------------------

class wxDataObject : public wxDataObjectBase
{
public:
    wxDataObject();
#ifdef __DARWIN__
    virtual ~wxDataObject() { }
#endif
    
    virtual bool IsSupportedFormat( const wxDataFormat& format, Direction dir = Get ) const;
};

#endif // _WX_GTK_DATAOBJ_H_

