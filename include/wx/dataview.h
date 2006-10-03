/////////////////////////////////////////////////////////////////////////////
// Name:        wx/dataview.h
// Purpose:     wxDataViewCtrl base classes
// Author:      Robert Roebling
// Modified by:
// Created:     08.01.06
// RCS-ID:      $Id$
// Copyright:   (c) Robert Roebling
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_DATAVIEW_H_BASE_
#define _WX_DATAVIEW_H_BASE_

#include "wx/defs.h"

#if wxUSE_DATAVIEWCTRL

#include "wx/control.h"
#include "wx/textctrl.h"
#include "wx/bitmap.h"
#include "wx/variant.h"


#if defined(__WXGTK20__)
    // for testing
    // #define wxUSE_GENERICDATAVIEWCTRL 1
#elif defined(__WXMAC__)
    #define wxUSE_GENERICDATAVIEWCTRL 1
#else
    #define wxUSE_GENERICDATAVIEWCTRL 1
#endif

// ----------------------------------------------------------------------------
// wxDataViewCtrl flags
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// wxDataViewCtrl globals
// ----------------------------------------------------------------------------

class WXDLLIMPEXP_ADV wxDataViewModel;
class WXDLLIMPEXP_ADV wxDataViewListModel;
class WXDLLIMPEXP_ADV wxDataViewCtrl;
class WXDLLIMPEXP_ADV wxDataViewColumn;
class WXDLLIMPEXP_ADV wxDataViewCell;

extern WXDLLIMPEXP_DATA_ADV(const wxChar) wxDataViewCtrlNameStr[];

// ---------------------------------------------------------
// wxDataViewModel
// ---------------------------------------------------------

class WXDLLIMPEXP_ADV wxDataViewModel: public wxObject
{
public:
    wxDataViewModel() { }
    virtual ~wxDataViewModel() { }

protected:
    DECLARE_DYNAMIC_CLASS_NO_COPY(wxDataViewModel)
};

// ---------------------------------------------------------
// wxDataViewListModelNotifier
// ---------------------------------------------------------


class WXDLLIMPEXP_ADV wxDataViewListModelNotifier: public wxObject
{
public:
    wxDataViewListModelNotifier() { }
    virtual ~wxDataViewListModelNotifier() { }

    virtual bool RowAppended() = 0;
    virtual bool RowPrepended() = 0;
    virtual bool RowInserted( unsigned int before ) = 0;
    virtual bool RowDeleted( unsigned int row ) = 0;
    virtual bool RowChanged( unsigned int row ) = 0;
    virtual bool ValueChanged( unsigned int col, unsigned int row ) = 0;
    virtual bool RowsReordered( unsigned int *new_order ) = 0;
    virtual bool Cleared() = 0;

    void SetOwner( wxDataViewListModel *owner ) { m_owner = owner; }
    wxDataViewListModel *GetOwner()             { return m_owner; }

private:
    wxDataViewListModel *m_owner;
};

// ---------------------------------------------------------
// wxDataViewListModel
// ---------------------------------------------------------

class WXDLLIMPEXP_ADV wxDataViewViewingColumn: public wxObject
{
public:
    wxDataViewViewingColumn( wxDataViewColumn *view_column, unsigned int model_column )
    {
        m_viewColumn = view_column;
        m_modelColumn = model_column;
    }

    wxDataViewColumn   *m_viewColumn;
    unsigned int        m_modelColumn;
};

class WXDLLIMPEXP_ADV wxDataViewListModel: public wxDataViewModel
{
public:
    wxDataViewListModel();
    virtual ~wxDataViewListModel();

    virtual unsigned int GetNumberOfRows() = 0;
    virtual unsigned int GetNumberOfCols() = 0;
    // return type as reported by wxVariant
    virtual wxString GetColType( unsigned int col ) = 0;
    // get value into a wxVariant
    virtual void GetValue( wxVariant &variant, unsigned int col, unsigned int row ) = 0;
    // set value, call ValueChanged() afterwards!
    virtual bool SetValue( wxVariant &variant, unsigned int col, unsigned int row ) = 0;

    // delegated notifiers
    virtual bool RowAppended();
    virtual bool RowPrepended();
    virtual bool RowInserted( unsigned int before );
    virtual bool RowDeleted( unsigned int row );
    virtual bool RowChanged( unsigned int row );
    virtual bool ValueChanged( unsigned int col, unsigned int row );
    virtual bool RowsReordered( unsigned int *new_order );
    virtual bool Cleared();

    // Used internally
    virtual void AddViewingColumn( wxDataViewColumn *view_column, unsigned int model_column );
    virtual void RemoveViewingColumn( wxDataViewColumn *column );

    virtual void AddNotifier( wxDataViewListModelNotifier *notifier );
    virtual void RemoveNotifier( wxDataViewListModelNotifier *notifier );

    wxList                      m_notifiers;
    wxList                      m_viewingColumns;

protected:
    DECLARE_DYNAMIC_CLASS_NO_COPY(wxDataViewListModel)
};

// ---------------------------------------------------------
// wxDataViewSortedListModel
// ---------------------------------------------------------

typedef int (wxCALLBACK *wxDataViewListModelCompare)
    (unsigned int row1, unsigned int row2, unsigned int col, wxDataViewListModel* model );

WX_DEFINE_SORTED_USER_EXPORTED_ARRAY_SIZE_T(unsigned int, wxDataViewSortedIndexArray, WXDLLIMPEXP_ADV);

class WXDLLIMPEXP_ADV wxDataViewSortedListModel: public wxDataViewListModel
{
public:
    wxDataViewSortedListModel( wxDataViewListModel *child );
    virtual ~wxDataViewSortedListModel();

    virtual unsigned int GetNumberOfRows();
    virtual unsigned int GetNumberOfCols();
    // return type as reported by wxVariant
    virtual wxString GetColType( unsigned int col );
    // get value into a wxVariant
    virtual void GetValue( wxVariant &variant, unsigned int col, unsigned int row );
    // set value, call ValueChanged() afterwards!
    virtual bool SetValue( wxVariant &variant, unsigned int col, unsigned int row );

    // called from user
    virtual bool RowAppended();
    virtual bool RowPrepended();
    virtual bool RowInserted( unsigned int before );
    virtual bool RowDeleted( unsigned int row );
    virtual bool RowChanged( unsigned int row );
    virtual bool ValueChanged( unsigned int col, unsigned int row );
    virtual bool RowsReordered( unsigned int *new_order );
    virtual bool Cleared();

    // called if child's notifiers are called
    bool ChildRowAppended();
    bool ChildRowPrepended();
    bool ChildRowInserted( unsigned int before );
    bool ChildRowDeleted( unsigned int row );
    bool ChildRowChanged( unsigned int row );
    bool ChildValueChanged( unsigned int col, unsigned int row );
    bool ChildRowsReordered( unsigned int *new_order );
    bool ChildCleared();

    virtual void Resort();

private:
    wxDataViewListModel             *m_child;
    wxDataViewSortedIndexArray       m_array;
    wxDataViewListModelNotifier     *m_notifierOnChild;

protected:
    DECLARE_DYNAMIC_CLASS_NO_COPY(wxDataViewSortedListModel)
};

// ---------------------------------------------------------
// wxDataViewCellBase
// ---------------------------------------------------------

enum wxDataViewCellMode
{
    wxDATAVIEW_CELL_INERT,
    wxDATAVIEW_CELL_ACTIVATABLE,
    wxDATAVIEW_CELL_EDITABLE
};

enum wxDataViewCellRenderState
{
    wxDATAVIEW_CELL_SELECTED    = 1,
    wxDATAVIEW_CELL_PRELIT      = 2,
    wxDATAVIEW_CELL_INSENSITIVE = 4,
    wxDATAVIEW_CELL_FOCUSED     = 8
};

class WXDLLIMPEXP_ADV wxDataViewCellBase: public wxObject
{
public:
    wxDataViewCellBase( const wxString &varianttype, wxDataViewCellMode mode = wxDATAVIEW_CELL_INERT );

    virtual bool SetValue( const wxVariant& WXUNUSED(value) ) { return true; }
    virtual bool GetValue( wxVariant& WXUNUSED(value) )       { return true; }
    virtual bool Validate( wxVariant& WXUNUSED(value) )       { return true; }

    wxString GetVariantType()       { return m_variantType; }
    wxDataViewCellMode GetMode()    { return m_mode; }

    void SetOwner( wxDataViewColumn *owner )    { m_owner = owner; }
    wxDataViewColumn* GetOwner()                { return m_owner; }

protected:
    wxDataViewCellMode      m_mode;
    wxString                m_variantType;
    wxDataViewColumn       *m_owner;

protected:
    DECLARE_DYNAMIC_CLASS_NO_COPY(wxDataViewCellBase)
};

// ---------------------------------------------------------
// wxDataViewColumnBase
// ---------------------------------------------------------

enum wxDataViewColumnFlags
{
    wxDATAVIEW_COL_RESIZABLE  = 1,
    wxDATAVIEW_COL_SORTABLE   = 2,
    wxDATAVIEW_COL_HIDDEN     = 4
};

class WXDLLIMPEXP_ADV wxDataViewColumnBase: public wxObject
{
public:
    wxDataViewColumnBase( const wxString &title, wxDataViewCell *cell, unsigned int model_column,
        int width = 80, int flags = wxDATAVIEW_COL_RESIZABLE );
    virtual ~wxDataViewColumnBase();

    virtual void SetTitle( const wxString &title );
    virtual wxString GetTitle();

    wxDataViewCell* GetCell()               { return m_cell; }

    unsigned int GetModelColumn()                 { return m_model_column; }

    void SetOwner( wxDataViewCtrl *owner )  { m_owner = owner; }
    wxDataViewCtrl *GetOwner()              { return m_owner; }

    virtual int GetWidth() = 0;

private:
    wxDataViewCtrl          *m_ctrl;
    wxDataViewCell          *m_cell;
    int                      m_model_column;
    int                      m_flags;
    wxString                 m_title;
    wxDataViewCtrl          *m_owner;

protected:
    DECLARE_DYNAMIC_CLASS_NO_COPY(wxDataViewColumnBase)
};

// ---------------------------------------------------------
// wxDataViewCtrlBase
// ---------------------------------------------------------

#define wxDV_SINGLE                  0x0000     // for convenience
#define wxDV_MULTIPLE                0x0020     // can select multiple items

class WXDLLIMPEXP_ADV wxDataViewCtrlBase: public wxControl
{
public:
    wxDataViewCtrlBase();
    virtual ~wxDataViewCtrlBase();

    virtual bool AssociateModel( wxDataViewListModel *model );
    wxDataViewListModel* GetModel();

    virtual bool AppendTextColumn( const wxString &label, unsigned int model_column );
    virtual bool AppendToggleColumn( const wxString &label, unsigned int model_column );
    virtual bool AppendProgressColumn( const wxString &label, unsigned int model_column );
    virtual bool AppendDateColumn( const wxString &label, unsigned int model_column );
    virtual bool AppendColumn( wxDataViewColumn *col );
    virtual unsigned int GetNumberOfColumns();
    virtual bool DeleteColumn( unsigned int pos );
    virtual bool ClearColumns();
    virtual wxDataViewColumn* GetColumn( unsigned int pos );

    virtual void SetSelection( int row ) = 0; // -1 for unselect
    inline void ClearSelection() { SetSelection( -1 ); }
    virtual void SetSelectionRange( unsigned int from, unsigned int to ) = 0;
    virtual void SetSelections( const wxArrayInt& aSelections) = 0;
    
    virtual bool IsSelected( unsigned int row ) const = 0;
    virtual int GetSelection() const = 0;
    virtual int GetSelections(wxArrayInt& aSelections) const = 0;

private:
    wxDataViewListModel    *m_model;
    wxList                  m_cols;

protected:
    DECLARE_DYNAMIC_CLASS_NO_COPY(wxDataViewCtrlBase)
};

#if defined(wxUSE_GENERICDATAVIEWCTRL)
    #include "wx/generic/dataview.h"
#elif defined(__WXGTK20__)
    #include "wx/gtk/dataview.h"
#elif defined(__WXMAC__)
    // TODO
    // #include "wx/mac/dataview.h"
#else
    #include "wx/generic/dataview.h"
#endif

#endif // wxUSE_DATAVIEWCTRL

#endif
    // _WX_DATAVIEW_H_BASE_
