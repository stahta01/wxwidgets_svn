/////////////////////////////////////////////////////////////////////////////
// Name:        wx/generic/dataview.h
// Purpose:     wxDataViewCtrl generic implementation header
// Author:      Robert Roebling
// Id:          $Id$
// Copyright:   (c) 1998 Robert Roebling
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef __GENERICDATAVIEWCTRLH__
#define __GENERICDATAVIEWCTRLH__

#include "wx/defs.h"
#include "wx/object.h"
#include "wx/list.h"
#include "wx/control.h"
#include "wx/scrolwin.h"

// --------------------------------------------------------- 
// classes
// --------------------------------------------------------- 

class WXDLLIMPEXP_CORE wxDataViewCtrl;
class WXDLLIMPEXP_CORE wxDataViewMainWindow;
class WXDLLIMPEXP_CORE wxDataViewHeaderWindow;

// --------------------------------------------------------- 
// wxDataViewCell
// --------------------------------------------------------- 

class wxDataViewCell: public wxDataViewCellBase
{
public:
    wxDataViewCell( const wxString &varianttype, wxDataViewCellMode mode = wxDATAVIEW_CELL_INERT );
    ~wxDataViewCell();

    virtual bool Render( wxRect cell, wxDC *dc, int state ) = 0;
    virtual wxSize GetSize() = 0;
    
    virtual bool Activate( wxRect cell,
                           wxDataViewListModel *model, size_t col, size_t row )   
                           { return false; }
    
    virtual bool LeftClick( wxPoint cursor, wxRect cell, 
                           wxDataViewListModel *model, size_t col, size_t row )   
                           { return false; }
    virtual bool RightClick( wxPoint cursor, wxRect cell,
                           wxDataViewListModel *model, size_t col, size_t row )   
                           { return false; }
    virtual bool StartDrag( wxPoint cursor, wxRect cell, 
                           wxDataViewListModel *model, size_t col, size_t row )   
                           { return false; }
    
    // Create DC on request
    virtual wxDC *GetDC();
    
private:
    wxDC        *m_dc;
    
protected:
    DECLARE_DYNAMIC_CLASS_NO_COPY(wxDataViewCell)
};
    
// --------------------------------------------------------- 
// wxDataViewCustomCell
// --------------------------------------------------------- 

class wxDataViewCustomCell: public wxDataViewCell
{
public:
    wxDataViewCustomCell( const wxString &varianttype = wxT("string"), 
                          wxDataViewCellMode mode = wxDATAVIEW_CELL_INERT );
    
protected:
    DECLARE_DYNAMIC_CLASS_NO_COPY(wxDataViewCustomCell)
};
    
// --------------------------------------------------------- 
// wxDataViewTextCell
// --------------------------------------------------------- 

class wxDataViewTextCell: public wxDataViewCustomCell
{
public:
    wxDataViewTextCell( const wxString &varianttype = wxT("string"), 
                        wxDataViewCellMode mode = wxDATAVIEW_CELL_INERT );

    bool SetValue( const wxVariant &value );
    bool GetValue( wxVariant &value );
    
    bool Render( wxRect cell, wxDC *dc, int state );
    wxSize GetSize();
    
private:
    wxString m_text;
    
protected:
    DECLARE_DYNAMIC_CLASS_NO_COPY(wxDataViewTextCell)
};
    
// --------------------------------------------------------- 
// wxDataViewToggleCell
// --------------------------------------------------------- 

class wxDataViewToggleCell: public wxDataViewCustomCell
{
public:
    wxDataViewToggleCell( const wxString &varianttype = wxT("bool"), 
                        wxDataViewCellMode mode = wxDATAVIEW_CELL_INERT );

    bool SetValue( const wxVariant &value );
    bool GetValue( wxVariant &value );
    
    bool Render( wxRect cell, wxDC *dc, int state );
    bool Activate( wxRect cell, wxDataViewListModel *model, size_t col, size_t row );
    wxSize GetSize();
    
private:
    bool    m_toggle;
    
protected:
    DECLARE_DYNAMIC_CLASS_NO_COPY(wxDataViewToggleCell)
};
    
// --------------------------------------------------------- 
// wxDataViewProgressCell
// --------------------------------------------------------- 

class wxDataViewProgressCell: public wxDataViewCustomCell
{
public:
    wxDataViewProgressCell( const wxString &label = wxEmptyString, 
                            const wxString &varianttype = wxT("long"), 
                            wxDataViewCellMode mode = wxDATAVIEW_CELL_INERT );
    ~wxDataViewProgressCell();
    
    bool SetValue( const wxVariant &value );
    
    virtual bool Render( wxRect cell, wxDC *dc, int state );
    virtual wxSize GetSize();
    
private:
    wxString    m_label;
    int         m_value;
    
protected:
    DECLARE_DYNAMIC_CLASS_NO_COPY(wxDataViewProgressCell)
};
    
// --------------------------------------------------------- 
// wxDataViewDateCell
// --------------------------------------------------------- 

class wxDataViewDateCell: public wxDataViewCustomCell
{
public:
    wxDataViewDateCell( const wxString &varianttype = wxT("datetime"), 
                        wxDataViewCellMode mode = wxDATAVIEW_CELL_ACTIVATABLE );
    
    bool SetValue( const wxVariant &value );
    
    virtual bool Render( wxRect cell, wxDC *dc, int state );
    virtual wxSize GetSize();
    virtual bool Activate( wxRect cell,
                           wxDataViewListModel *model, size_t col, size_t row );
    
private:
    wxDateTime    m_date;
    
protected:
    DECLARE_DYNAMIC_CLASS_NO_COPY(wxDataViewDateCell)
};
    
// --------------------------------------------------------- 
// wxDataViewColumn
// --------------------------------------------------------- 

class WXDLLIMPEXP_CORE wxDataViewColumn: public wxDataViewColumnBase
{
public:
    wxDataViewColumn( const wxString &title, wxDataViewCell *cell, size_t model_column, int flags = 0 );
    virtual ~wxDataViewColumn();

    virtual void SetTitle( const wxString &title );
    
    void SetWidth( int width ) { m_width = width; }
    int GetWidth() { return m_width; }
    
private:
    int     m_width;

protected:
    DECLARE_DYNAMIC_CLASS_NO_COPY(wxDataViewColumn)
};

// --------------------------------------------------------- 
// wxDataViewCtrl
// --------------------------------------------------------- 

class WXDLLIMPEXP_CORE wxDataViewCtrl: public wxDataViewCtrlBase, 
                                       public wxScrollHelperNative
{
public:
    wxDataViewCtrl() : wxScrollHelperNative(this)
    {
        Init();
    }
    
    wxDataViewCtrl( wxWindow *parent, wxWindowID id,
           const wxPoint& pos = wxDefaultPosition,
           const wxSize& size = wxDefaultSize, long style = 0,
           const wxValidator& validator = wxDefaultValidator ) 
             : wxScrollHelperNative(this)
    {
        Create(parent, id, pos, size, style, validator );
    }

    virtual ~wxDataViewCtrl();

    void Init();

    bool Create(wxWindow *parent, wxWindowID id,
           const wxPoint& pos = wxDefaultPosition,
           const wxSize& size = wxDefaultSize, long style = 0,
           const wxValidator& validator = wxDefaultValidator );

    virtual bool AssociateModel( wxDataViewListModel *model );
    virtual bool AppendColumn( wxDataViewColumn *col );
    
private:
    friend class wxDataViewMainWindow;
    friend class wxDataViewHeaderWindow;
    wxDataViewListModelNotifier *m_notifier;
    wxDataViewMainWindow        *m_clientArea;
    wxDataViewHeaderWindow      *m_headerArea;
    
private:
    void OnSize( wxSizeEvent &event );
    
    // we need to return a special WM_GETDLGCODE value to process just the
    // arrows but let the other navigation characters through
#ifdef __WXMSW__
    virtual WXLRESULT MSWWindowProc(WXUINT nMsg, WXWPARAM wParam, WXLPARAM lParam);
#endif // __WXMSW__

    WX_FORWARD_TO_SCROLL_HELPER()

private:
    DECLARE_DYNAMIC_CLASS(wxDataViewCtrl)
    DECLARE_NO_COPY_CLASS(wxDataViewCtrl)
    DECLARE_EVENT_TABLE()
};


#endif // __GENERICDATAVIEWCTRLH__
