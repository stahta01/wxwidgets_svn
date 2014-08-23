/////////////////////////////////////////////////////////////////////////////
// Name:        wx/qt/toolbar.h
// Author:      Sean D'Epagnier
// Copyright:   (c) Sean D'Epagnier 2014
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_QT_DATAFORM_H_
#define _WX_QT_DATAFORM_H_

class WXDLLIMPEXP_CORE wxDataFormat
{
public:
    wxDataFormat();
    wxDataFormat( wxDataFormatId formatId );
    wxDataFormat(const wxString &id);
    wxDataFormat(const QString &id);
    wxDataFormat(const wxChar *id);

    void SetId( const wxChar *id );
    
    bool operator==(wxDataFormatId format) const;
    bool operator!=(wxDataFormatId format) const;
    bool operator==(const wxDataFormat& format) const;
    bool operator!=(const wxDataFormat& format) const;

    QString m_MimeType;
};

#endif // _WX_QT_DATAFORM_H_
