/////////////////////////////////////////////////////////////////////////////
// Name:        wx/qt/statbmp.h
// Author:      Peter Most
// Id:          $Id$
// Copyright:   (c) Peter Most
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_QT_STATBMP_H_
#define _WX_QT_STATBMP_H_

class WXDLLIMPEXP_CORE wxStaticBitmap : public wxStaticBitmapBase
{
public:
    wxStaticBitmap();
    wxStaticBitmap( wxWindow *parent,
                    wxWindowID id,
                    const wxBitmap& label,
                    const wxPoint& pos = wxDefaultPosition,
                    const wxSize& size = wxDefaultSize,
                    long style = 0,
                    const wxString& name = wxStaticBitmapNameStr );
    bool Create( wxWindow *parent,
                 wxWindowID id,
                 const wxBitmap& label,
                 const wxPoint& pos = wxDefaultPosition,
                 const wxSize& size = wxDefaultSize,
                 long style = 0,
                 const wxString& name = wxStaticBitmapNameStr);

    virtual void SetIcon(const wxIcon& icon);
    virtual void SetBitmap(const wxBitmap& bitmap);
    virtual wxBitmap GetBitmap() const;
    virtual wxIcon GetIcon() const;

protected:

private:
    DECLARE_DYNAMIC_CLASS(wxStaticBitmap)
};

#endif // _WX_QT_STATBMP_H_
