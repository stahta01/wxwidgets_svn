/////////////////////////////////////////////////////////////////////////////
// Name:        sizer.cpp
// Purpose:     provide new wxSizer class for layout
// Author:      Robert Roebling and Robin Dunn, contributions by
//              Dirk Holtwick, Ron Lee
// Modified by: Ron Lee
// Created:
// RCS-ID:      $Id$
// Copyright:   (c) Robin Dunn, Robert Roebling
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "sizer.h"
#endif

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#include "wx/sizer.h"
#include "wx/utils.h"
#include "wx/statbox.h"
#include "wx/notebook.h"
#include <wx/listimpl.cpp>

//---------------------------------------------------------------------------

IMPLEMENT_CLASS(wxSizerItem, wxObject)
IMPLEMENT_CLASS(wxSizer, wxObject)
IMPLEMENT_CLASS(wxGridSizer, wxSizer)
IMPLEMENT_CLASS(wxFlexGridSizer, wxGridSizer)
IMPLEMENT_CLASS(wxBoxSizer, wxSizer)
#if wxUSE_STATBOX
IMPLEMENT_CLASS(wxStaticBoxSizer, wxBoxSizer)
#endif
#if wxUSE_BOOKCTRL
IMPLEMENT_CLASS(wxBookCtrlSizer, wxSizer)
#if wxUSE_NOTEBOOK
IMPLEMENT_CLASS(wxNotebookSizer, wxBookCtrlSizer)
#endif // wxUSE_NOTEBOOK
#endif // wxUSE_BOOKCTRL

WX_DEFINE_EXPORTED_LIST( wxSizerItemList );

/*
	TODO PROPERTIES
	  sizeritem
	    object
		object_ref
		  minsize
		  option
		  flag
		  border
     spacer
		option
		flag
		borfder
	boxsizer
	   orient
    staticboxsizer
	   orient
	   label
	gridsizer
	   rows
	   cols
	   vgap
	   hgap
	flexgridsizer
	   rows
	   cols
	   vgap
	   hgap
	   growablerows
	   growablecols
    minsize
*/
//---------------------------------------------------------------------------
// wxSizerItem
//---------------------------------------------------------------------------

wxSizerItem::wxSizerItem( int width, int height, int proportion, int flag, int border, wxObject* userData )
    : m_window( NULL )
    , m_sizer( NULL )
    , m_size( wxSize( width, height ) ) // size is set directly
    , m_minSize( m_size )               // minimal size is the initial size
    , m_proportion( proportion )
    , m_border( border )
    , m_flag( flag )
    , m_show( true )
    , m_userData( userData )
{
    SetRatio( m_size );
}

wxSizerItem::wxSizerItem( wxWindow *window, int proportion, int flag, int border, wxObject* userData )
    : m_window( window )
    , m_sizer( NULL )
    , m_minSize( window->GetSize() )    // minimal size is the initial size
    , m_proportion( proportion )
    , m_border( border )
    , m_flag( flag )
    , m_show( true )
    , m_userData( userData )
{
    // aspect ratio calculated from initial size
    SetRatio( m_minSize );

    // m_size is calculated later
}

wxSizerItem::wxSizerItem( wxSizer *sizer, int proportion, int flag, int border, wxObject* userData )
    : m_window( NULL )
    , m_sizer( sizer )
    , m_proportion( proportion )
    , m_border( border )
    , m_flag( flag )
    , m_show( true )
    , m_ratio( 0.0 )
    , m_userData( userData )
{
    // m_minSize is calculated later
    // m_size is calculated later
}

wxSizerItem::wxSizerItem()
    : m_window( NULL )
    , m_sizer( NULL )
    , m_proportion( 0 )
    , m_border( 0 )
    , m_flag( 0 )
    , m_show( true )
    , m_ratio( 0.0 )
    , m_userData( NULL )
{
}

wxSizerItem::~wxSizerItem()
{
    delete m_userData;

    if ( m_window )
    {
        m_window->SetContainingSizer(NULL);
    }
    else // we must be a sizer
    {
        delete m_sizer;
    }
}


wxSize wxSizerItem::GetSize() const
{
    wxSize ret;
    if (IsSizer())
        ret = m_sizer->GetSize();
    else
    if (IsWindow())
        ret = m_window->GetSize();
    else ret = m_size;

    if (m_flag & wxWEST)
        ret.x += m_border;
    if (m_flag & wxEAST)
        ret.x += m_border;
    if (m_flag & wxNORTH)
        ret.y += m_border;
    if (m_flag & wxSOUTH)
        ret.y += m_border;

    return ret;
}

wxSize wxSizerItem::CalcMin()
{
    wxSize ret;
    if (IsSizer())
    {
        ret = m_sizer->GetMinSize();

        // if we have to preserve aspect ratio _AND_ this is
        // the first-time calculation, consider ret to be initial size
        if ((m_flag & wxSHAPED) && !m_ratio)
            SetRatio(ret);
    }
    else
    {
        if ( IsWindow() && (m_flag & wxADJUST_MINSIZE) )
        {
            // By user request, keep the minimal size for this item
            // in sync with the largest of BestSize and any user supplied
            // minimum size hint.  Useful in cases where the item is
            // changeable -- static text labels, etc.
            m_minSize = m_window->GetAdjustedBestSize();
        }

        ret = m_minSize;
    }

    if (m_flag & wxWEST)
        ret.x += m_border;
    if (m_flag & wxEAST)
        ret.x += m_border;
    if (m_flag & wxNORTH)
        ret.y += m_border;
    if (m_flag & wxSOUTH)
        ret.y += m_border;

    return ret;
}

void wxSizerItem::SetDimension( wxPoint pos, wxSize size )
{
    if (m_flag & wxSHAPED)
    {
        // adjust aspect ratio
        int rwidth = (int) (size.y * m_ratio);
        if (rwidth > size.x)
        {
            // fit horizontally
            int rheight = (int) (size.x / m_ratio);
            // add vertical space
            if (m_flag & wxALIGN_CENTER_VERTICAL)
                pos.y += (size.y - rheight) / 2;
            else if (m_flag & wxALIGN_BOTTOM)
                pos.y += (size.y - rheight);
            // use reduced dimensions
            size.y =rheight;
        }
        else if (rwidth < size.x)
        {
            // add horizontal space
            if (m_flag & wxALIGN_CENTER_HORIZONTAL)
                pos.x += (size.x - rwidth) / 2;
            else if (m_flag & wxALIGN_RIGHT)
                pos.x += (size.x - rwidth);
            size.x = rwidth;
        }
    }

    // This is what GetPosition() returns. Since we calculate
    // borders afterwards, GetPosition() will be the left/top
    // corner of the surrounding border.
    m_pos = pos;

    if (m_flag & wxWEST)
    {
        pos.x += m_border;
        size.x -= m_border;
    }
    if (m_flag & wxEAST)
    {
        size.x -= m_border;
    }
    if (m_flag & wxNORTH)
    {
        pos.y += m_border;
        size.y -= m_border;
    }
    if (m_flag & wxSOUTH)
    {
        size.y -= m_border;
    }

    if (IsSizer())
        m_sizer->SetDimension( pos.x, pos.y, size.x, size.y );

    if (IsWindow())
        m_window->SetSize( pos.x, pos.y, size.x, size.y, wxSIZE_ALLOW_MINUS_ONE );

    m_size = size;
}

void wxSizerItem::DeleteWindows()
{
    if (m_window)
         m_window->Destroy();

    if (m_sizer)
        m_sizer->DeleteWindows();
}

bool wxSizerItem::IsWindow() const
{
    return (m_window != NULL);
}

bool wxSizerItem::IsSizer() const
{
    return (m_sizer != NULL);
}

bool wxSizerItem::IsSpacer() const
{
    return (m_window == NULL) && (m_sizer == NULL);
}

void wxSizerItem::Show( bool show )
{
    m_show = show;

    if( IsWindow() )
        m_window->Show( show );
    else if( IsSizer() )
        m_sizer->ShowItems( show );

    // ... nothing else to do to hide/show spacers
}

void wxSizerItem::SetOption( int option )
{
    SetProportion( option );
}

int wxSizerItem::GetOption() const
{
    return GetProportion();
}


//---------------------------------------------------------------------------
// wxSizer
//---------------------------------------------------------------------------

wxSizer::wxSizer()
    : m_minSize( wxSize( 0, 0 ) )
{
}

wxSizer::~wxSizer()
{
    WX_CLEAR_LIST(wxSizerItemList, m_children);
}

void wxSizer::Add( wxWindow *window, int proportion, int flag, int border, wxObject* userData )
{
    m_children.Append( new wxSizerItem( window, proportion, flag, border, userData ) );
    window->SetContainingSizer( this );
}

void wxSizer::Add( wxSizer *sizer, int proportion, int flag, int border, wxObject* userData )
{
    m_children.Append( new wxSizerItem( sizer, proportion, flag, border, userData ) );
}

void wxSizer::Add( int width, int height, int proportion, int flag, int border, wxObject* userData )
{
    m_children.Append( new wxSizerItem( width, height, proportion, flag, border, userData ) );
}

void wxSizer::Add( wxSizerItem *item )
{
    m_children.Append( item );

    if( item->GetWindow() )
        item->GetWindow()->SetContainingSizer( this );
}

void wxSizer::Prepend( wxWindow *window, int proportion, int flag, int border, wxObject* userData )
{
    m_children.Insert( new wxSizerItem( window, proportion, flag, border, userData ) );
    window->SetContainingSizer( this );
}

void wxSizer::Prepend( wxSizer *sizer, int proportion, int flag, int border, wxObject* userData )
{
    m_children.Insert( new wxSizerItem( sizer, proportion, flag, border, userData ) );
}

void wxSizer::Prepend( int width, int height, int proportion, int flag, int border, wxObject* userData )
{
    m_children.Insert( new wxSizerItem( width, height, proportion, flag, border, userData ) );
}

void wxSizer::Prepend( wxSizerItem *item )
{
    m_children.Insert( item );

    if( item->GetWindow() )
        item->GetWindow()->SetContainingSizer( this );
}

void wxSizer::Insert( size_t index,
                      wxWindow *window,
                      int proportion,
                      int flag,
                      int border,
                      wxObject* userData )
{
    m_children.Insert( index,
                       new wxSizerItem( window, proportion, flag, border, userData ) );
    window->SetContainingSizer( this );
}

void wxSizer::Insert( size_t index,
                      wxSizer *sizer,
                      int proportion,
                      int flag,
                      int border,
                      wxObject* userData )
{
    m_children.Insert( index,
                       new wxSizerItem( sizer, proportion, flag, border, userData ) );
}

void wxSizer::Insert( size_t index,
                      int width,
                      int height,
                      int proportion,
                      int flag,
                      int border,
                      wxObject* userData )
{
    m_children.Insert( index,
                       new wxSizerItem( width, height, proportion, flag, border, userData ) );
}

void wxSizer::Insert( size_t index, wxSizerItem *item )
{
    m_children.Insert( index, item );

    if( item->GetWindow() )
        item->GetWindow()->SetContainingSizer( this );
}

bool wxSizer::Remove( wxWindow *window )
{
    return Detach( window );
}

bool wxSizer::Remove( wxSizer *sizer )
{
    wxASSERT_MSG( sizer, _T("Removing NULL sizer") );

    wxSizerItemList::compatibility_iterator node = m_children.GetFirst();
    while (node)
    {
        wxSizerItem     *item = node->GetData();

        if (item->GetSizer() == sizer)
        {
            delete item;
            m_children.Erase( node );
            return true;
        }

        node = node->GetNext();
    }

    return false;
}

bool wxSizer::Remove( int index )
{
    wxCHECK_MSG( index >= 0 && (size_t)index < m_children.GetCount(),
                 false,
                 _T("Remove index is out of range") );

    wxSizerItemList::compatibility_iterator node = m_children.Item( index );

    wxCHECK_MSG( node, false, _T("Failed to find child node") );

    wxSizerItem *item = node->GetData();

    if( item->IsWindow() )
        item->GetWindow()->SetContainingSizer( NULL );

    delete item;
    m_children.Erase( node );
    return true;
}

bool wxSizer::Detach( wxSizer *sizer )
{
    wxASSERT_MSG( sizer, _T("Detaching NULL sizer") );

    wxSizerItemList::compatibility_iterator node = m_children.GetFirst();
    while (node)
    {
        wxSizerItem     *item = node->GetData();

        if (item->GetSizer() == sizer)
        {
            item->DetachSizer();
            delete item;
            m_children.Erase( node );
            return true;
        }
        node = node->GetNext();
    }

    return false;
}

bool wxSizer::Detach( wxWindow *window )
{
    wxASSERT_MSG( window, _T("Detaching NULL window") );

    wxSizerItemList::compatibility_iterator node = m_children.GetFirst();
    while (node)
    {
        wxSizerItem     *item = node->GetData();

        if (item->GetWindow() == window)
        {
            item->GetWindow()->SetContainingSizer( NULL );
            delete item;
            m_children.Erase( node );
            return true;
        }
        node = node->GetNext();
    }

    return false;
}

bool wxSizer::Detach( int index )
{
    wxCHECK_MSG( index >= 0 && (size_t)index < m_children.GetCount(),
                 false,
                 _T("Detach index is out of range") );

    wxSizerItemList::compatibility_iterator node = m_children.Item( index );

    wxCHECK_MSG( node, false, _T("Failed to find child node") );

    wxSizerItem *item = node->GetData();

    if( item->IsSizer() )
        item->DetachSizer();
    else if( item->IsWindow() )
        item->GetWindow()->SetContainingSizer( NULL );

    delete item;
    m_children.Erase( node );
    return true;
}

void wxSizer::Clear( bool delete_windows )
{
    // First clear the ContainingSizer pointers
    wxSizerItemList::compatibility_iterator node = m_children.GetFirst();
    while (node)
    {
        wxSizerItem     *item = node->GetData();

        if (item->IsWindow())
            item->GetWindow()->SetContainingSizer( NULL );
        node = node->GetNext();
    }

    // Destroy the windows if needed
    if (delete_windows)
        DeleteWindows();

    // Now empty the list
    WX_CLEAR_LIST(wxSizerItemList, m_children);
}

void wxSizer::DeleteWindows()
{
    wxSizerItemList::compatibility_iterator node = m_children.GetFirst();
    while (node)
    {
        wxSizerItem     *item = node->GetData();

        item->DeleteWindows();
        node = node->GetNext();
    }
}

wxSize wxSizer::Fit( wxWindow *window )
{
    wxSize size(window->IsTopLevel() ? FitSize(window)
                                     : GetMinWindowSize(window));

    window->SetSize( size );

    return size;
}

void wxSizer::FitInside( wxWindow *window )
{
    wxSize size;
    if (window->IsTopLevel())
        size = VirtualFitSize( window );
    else
        size = GetMinClientSize( window );

    window->SetVirtualSize( size );
}

void wxSizer::Layout()
{
    CalcMin();
    RecalcSizes();
}

void wxSizer::SetSizeHints( wxWindow *window )
{
    // Preserve the window's max size hints, but set the
    // lower bound according to the sizer calculations.

    wxSize size = Fit( window );

    window->SetSizeHints( size.x,
                          size.y,
                          window->GetMaxWidth(),
                          window->GetMaxHeight() );
}

void wxSizer::SetVirtualSizeHints( wxWindow *window )
{
    // Preserve the window's max size hints, but set the
    // lower bound according to the sizer calculations.

    FitInside( window );
    wxSize size( window->GetVirtualSize() );
    window->SetVirtualSizeHints( size.x,
                                 size.y,
                                 window->GetMaxWidth(),
                                 window->GetMaxHeight() );
}

wxSize wxSizer::GetMaxWindowSize( wxWindow *window ) const
{
    return window->GetMaxSize();
}

wxSize wxSizer::GetMinWindowSize( wxWindow *window )
{
    wxSize      minSize( GetMinSize() );
    wxSize      size( window->GetSize() );
    wxSize      client_size( window->GetClientSize() );

    return wxSize( minSize.x+size.x-client_size.x,
                   minSize.y+size.y-client_size.y );
}

// Return a window size that will fit within the screens dimensions
wxSize wxSizer::FitSize( wxWindow *window )
{
    wxSize size     = GetMinWindowSize( window );
    wxSize sizeMax  = GetMaxWindowSize( window );

    // Limit the size if sizeMax != wxDefaultSize

    if ( size.x > sizeMax.x && sizeMax.x != -1 )
        size.x = sizeMax.x;
    if ( size.y > sizeMax.y && sizeMax.y != -1 )
        size.y = sizeMax.y;

    return size;
}

wxSize wxSizer::GetMaxClientSize( wxWindow *window ) const
{
    wxSize maxSize( window->GetMaxSize() );

    if( maxSize != wxDefaultSize )
    {
        wxSize size( window->GetSize() );
        wxSize client_size( window->GetClientSize() );

        return wxSize( maxSize.x + client_size.x - size.x,
                       maxSize.y + client_size.y - size.y );
    }
    else
        return wxDefaultSize;
}

wxSize wxSizer::GetMinClientSize( wxWindow *WXUNUSED(window) )
{
    return GetMinSize();  // Already returns client size.
}

wxSize wxSizer::VirtualFitSize( wxWindow *window )
{
    wxSize size     = GetMinClientSize( window );
    wxSize sizeMax  = GetMaxClientSize( window );

    // Limit the size if sizeMax != wxDefaultSize

    if ( size.x > sizeMax.x && sizeMax.x != -1 )
        size.x = sizeMax.x;
    if ( size.y > sizeMax.y && sizeMax.y != -1 )
        size.y = sizeMax.y;

    return size;
}

void wxSizer::SetDimension( int x, int y, int width, int height )
{
    m_position.x = x;
    m_position.y = y;
    m_size.x = width;
    m_size.y = height;
    Layout();
}

wxSize wxSizer::GetMinSize()
{
    wxSize ret( CalcMin() );
    if (ret.x < m_minSize.x) ret.x = m_minSize.x;
    if (ret.y < m_minSize.y) ret.y = m_minSize.y;
    return ret;
}

void wxSizer::DoSetMinSize( int width, int height )
{
    m_minSize.x = width;
    m_minSize.y = height;
}

bool wxSizer::DoSetItemMinSize( wxWindow *window, int width, int height )
{
    wxASSERT_MSG( window, _T("SetMinSize for NULL window") );

    // Is it our immediate child?

    wxSizerItemList::compatibility_iterator node = m_children.GetFirst();
    while (node)
    {
        wxSizerItem     *item = node->GetData();

        if (item->GetWindow() == window)
        {
            item->SetInitSize( width, height );
            return true;
        }
        node = node->GetNext();
    }

    // No?  Search any subsizers we own then

    node = m_children.GetFirst();
    while (node)
    {
        wxSizerItem     *item = node->GetData();

        if ( item->GetSizer() &&
             item->GetSizer()->DoSetItemMinSize( window, width, height ) )
        {
            // A child sizer found the requested windw, exit.
            return true;
        }
        node = node->GetNext();
    }

    return false;
}

bool wxSizer::DoSetItemMinSize( wxSizer *sizer, int width, int height )
{
    wxASSERT_MSG( sizer, _T("SetMinSize for NULL sizer") );

    // Is it our immediate child?

    wxSizerItemList::compatibility_iterator node = m_children.GetFirst();
    while (node)
    {
        wxSizerItem     *item = node->GetData();

        if (item->GetSizer() == sizer)
        {
            item->GetSizer()->DoSetMinSize( width, height );
            return true;
        }
        node = node->GetNext();
    }

    // No?  Search any subsizers we own then

    node = m_children.GetFirst();
    while (node)
    {
        wxSizerItem     *item = node->GetData();

        if ( item->GetSizer() &&
             item->GetSizer()->DoSetItemMinSize( sizer, width, height ) )
        {
            // A child found the requested sizer, exit.
            return true;
        }
        node = node->GetNext();
    }

    return false;
}

bool wxSizer::DoSetItemMinSize( size_t index, int width, int height )
{
    wxSizerItemList::compatibility_iterator node = m_children.Item( index );

    wxCHECK_MSG( node, false, _T("Failed to find child node") );

    wxSizerItem     *item = node->GetData();

    if (item->GetSizer())
    {
        // Sizers contains the minimal size in them, if not calculated ...
        item->GetSizer()->DoSetMinSize( width, height );
    }
    else
    {
        // ... but the minimal size of spacers and windows in stored in them
        item->SetInitSize( width, height );
    }

    return true;
}

void wxSizer::Show( wxWindow *window, bool show )
{
    wxASSERT_MSG( window, _T("Show for NULL window") );

    wxSizerItemList::compatibility_iterator node = m_children.GetFirst();
    while (node)
    {
        wxSizerItem     *item = node->GetData();

        if (item->GetWindow() == window)
        {
            item->Show( show );
            break;
        }
        node = node->GetNext();
    }
}

void wxSizer::Show( wxSizer *sizer, bool show )
{
    wxASSERT_MSG( sizer, _T("Show for NULL sizer") );

    wxSizerItemList::compatibility_iterator node = m_children.GetFirst();
    while (node)
    {
        wxSizerItem     *item = node->GetData();

        if (item->GetSizer() == sizer)
        {
            item->Show( show );
            break;
        }
        node = node->GetNext();
    }
}

void wxSizer::Show( size_t index, bool show )
{
    wxCHECK_RET( index < m_children.GetCount(),
                 _T("Show index is out of range") );

    m_children.Item( index )->GetData()->Show( show );
}

void wxSizer::ShowItems( bool show )
{
    wxSizerItemList::compatibility_iterator node = m_children.GetFirst();
    while (node)
    {
        node->GetData()->Show( show );
        node = node->GetNext();
    }
}

bool wxSizer::IsShown( wxWindow *window ) const
{
    wxSizerItemList::compatibility_iterator node = m_children.GetFirst();
    while (node)
    {
        wxSizerItem     *item = node->GetData();

        if (item->GetWindow() == window)
        {
            return item->IsShown();
        }
        node = node->GetNext();
    }

    wxFAIL_MSG( _T("IsShown failed to find sizer item") );

    return false;
}

bool wxSizer::IsShown( wxSizer *sizer ) const
{
    wxSizerItemList::compatibility_iterator node = m_children.GetFirst();
    while (node)
    {
        wxSizerItem     *item = node->GetData();

        if (item->GetSizer() == sizer)
        {
            return item->IsShown();
        }
        node = node->GetNext();
    }

    wxFAIL_MSG( _T("IsShown failed to find sizer item") );

    return false;
}

bool wxSizer::IsShown( size_t index ) const
{
    wxCHECK_MSG( index < m_children.GetCount(),
                 false,
                 _T("IsShown index is out of range") );

    return m_children.Item( index )->GetData()->IsShown();
}


//---------------------------------------------------------------------------
// wxGridSizer
//---------------------------------------------------------------------------

wxGridSizer::wxGridSizer( int rows, int cols, int vgap, int hgap )
    : m_rows( rows )
    , m_cols( cols )
    , m_vgap( vgap )
    , m_hgap( hgap )
{
    if (m_rows == 0 && m_cols == 0)
        m_rows = 1;
}

wxGridSizer::wxGridSizer( int cols, int vgap, int hgap )
    : m_rows( 0 )
    , m_cols( cols )
    , m_vgap( vgap )
    , m_hgap( hgap )
{
    if (m_rows == 0 && m_cols == 0)
        m_rows = 1;
}

int wxGridSizer::CalcRowsCols(int& nrows, int& ncols) const
{
    int nitems = m_children.GetCount();
    if ( nitems)
    {
        if ( m_cols )
        {
            ncols = m_cols;
            nrows = (nitems + m_cols - 1) / m_cols;
        }
        else if ( m_rows )
        {
            ncols = (nitems + m_rows - 1) / m_rows;
            nrows = m_rows;
        }
        else // 0 columns, 0 rows?
        {
            wxFAIL_MSG( _T("grid sizer must have either rows or columns fixed") );

            nrows = ncols = 0;
        }
    }

    return nitems;
}

void wxGridSizer::RecalcSizes()
{
    int nitems, nrows, ncols;
    if ( (nitems = CalcRowsCols(nrows, ncols)) == 0 )
        return;

    wxSize sz( GetSize() );
    wxPoint pt( GetPosition() );

    int w = (sz.x - (ncols - 1) * m_hgap) / ncols;
    int h = (sz.y - (nrows - 1) * m_vgap) / nrows;

    int x = pt.x;
    for (int c = 0; c < ncols; c++)
    {
        int y = pt.y;
        for (int r = 0; r < nrows; r++)
        {
            int i = r * ncols + c;
            if (i < nitems)
            {
                wxSizerItemList::compatibility_iterator node = m_children.Item( i );

                wxASSERT_MSG( node, _T("Failed to find SizerItemList node") );

                SetItemBounds( node->GetData(), x, y, w, h);
            }
            y = y + h + m_vgap;
        }
        x = x + w + m_hgap;
    }
}

wxSize wxGridSizer::CalcMin()
{
    int nrows, ncols;
    if ( CalcRowsCols(nrows, ncols) == 0 )
        return wxSize(10, 10);

    // Find the max width and height for any component
    int w = 0;
    int h = 0;

    wxSizerItemList::compatibility_iterator node = m_children.GetFirst();
    while (node)
    {
        wxSizerItem     *item = node->GetData();
        wxSize           sz( item->CalcMin() );

        w = wxMax( w, sz.x );
        h = wxMax( h, sz.y );

        node = node->GetNext();
    }

    return wxSize( ncols * w + (ncols-1) * m_hgap,
                   nrows * h + (nrows-1) * m_vgap );
}

void wxGridSizer::SetItemBounds( wxSizerItem *item, int x, int y, int w, int h )
{
    wxPoint pt( x,y );
    wxSize sz( item->CalcMin() );
    int flag = item->GetFlag();

    if ((flag & wxEXPAND) || (flag & wxSHAPED))
    {
       sz = wxSize(w, h);
    }
    else
    {
        if (flag & wxALIGN_CENTER_HORIZONTAL)
        {
            pt.x = x + (w - sz.x) / 2;
        }
        else if (flag & wxALIGN_RIGHT)
        {
            pt.x = x + (w - sz.x);
        }

        if (flag & wxALIGN_CENTER_VERTICAL)
        {
            pt.y = y + (h - sz.y) / 2;
        }
        else if (flag & wxALIGN_BOTTOM)
        {
            pt.y = y + (h - sz.y);
        }
    }

    item->SetDimension(pt, sz);
}

//---------------------------------------------------------------------------
// wxFlexGridSizer
//---------------------------------------------------------------------------

wxFlexGridSizer::wxFlexGridSizer( int rows, int cols, int vgap, int hgap )
               : wxGridSizer( rows, cols, vgap, hgap ),
                 m_flexDirection(wxBOTH),
                 m_growMode(wxFLEX_GROWMODE_SPECIFIED)
{
}

wxFlexGridSizer::wxFlexGridSizer( int cols, int vgap, int hgap )
               : wxGridSizer( cols, vgap, hgap ),
                 m_flexDirection(wxBOTH),
                 m_growMode(wxFLEX_GROWMODE_SPECIFIED)
{
}

wxFlexGridSizer::~wxFlexGridSizer()
{
}

void wxFlexGridSizer::RecalcSizes()
{
    int nitems, nrows, ncols;
    if ( (nitems = CalcRowsCols(nrows, ncols)) == 0 )
        return;

    wxPoint pt( GetPosition() );
    wxSize sz( GetSize() );
    wxSize minsz( CalcMin() );

    AdjustForGrowables(sz, minsz, nrows, ncols);

    sz = wxSize( pt.x + sz.x, pt.y + sz.y );

    int x = pt.x;
    for (int c = 0; c < ncols; c++)
    {
        int y = pt.y;
        for (int r = 0; r < nrows; r++)
        {
            int i = r * ncols + c;
            if (i < nitems)
            {
                wxSizerItemList::compatibility_iterator node = m_children.Item( i );

                wxASSERT_MSG( node, _T("Failed to find node") );

                int w = wxMax( 0, wxMin( m_colWidths[c], sz.x - x ) );
                int h = wxMax( 0, wxMin( m_rowHeights[r], sz.y - y ) );

                SetItemBounds( node->GetData(), x, y, w, h);
            }
            y = y + m_rowHeights[r] + m_vgap;
        }
        x = x + m_colWidths[c] + m_hgap;
    }
}

wxSize wxFlexGridSizer::CalcMin()
{
    int     nrows,
            ncols;
    size_t  i, s;

    // Number of rows/columns can change as items are added or removed.
    if ( !CalcRowsCols(nrows, ncols) )
        return wxSize(10, 10);

    m_rowHeights.SetCount(nrows);
    m_colWidths.SetCount(ncols);

    // We have to recalcuate the sizes in case an item has wxADJUST_MINSIZE, has changed
    // minimum size since the previous layout, or has been hidden using wxSizer::Show().
    // If all the items in a row/column are hidden, the final dimension of the row/column
    // will be -1, indicating that the column itself is hidden.
    for( s = m_rowHeights.GetCount(), i = 0; i < s; ++i )
        m_rowHeights[ i ] = -1;
    for( s = m_colWidths.GetCount(), i = 0; i < s; ++i )
        m_colWidths[ i ] = -1;

    wxSizerItemList::compatibility_iterator node = m_children.GetFirst();

    i = 0;
    while (node)
    {
        wxSizerItem    *item = node->GetData();
        if ( item->IsShown() )
        {
            wxSize sz( item->CalcMin() );
            int row = i / ncols;
            int col = i % ncols;

            m_rowHeights[ row ] = wxMax( wxMax( 0, sz.y ), m_rowHeights[ row ] );
            m_colWidths[ col ] = wxMax( wxMax( 0, sz.x ), m_colWidths[ col ] );
        }

        node = node->GetNext();
        i++;
    }

    AdjustForFlexDirection();
    
    // Sum total minimum size, including gaps between rows/columns.
    // -1 is used as a magic number meaning empty column.
    int width = 0;
    for (int col = 0; col < ncols; col++)
        if ( m_colWidths[ col ] != -1 )
            width += m_colWidths[ col ] + ( col == ncols-1 ? 0 : m_hgap );

    int height = 0;
    for (int row = 0; row < nrows; row++)
        if ( m_rowHeights[ row ] != -1 )
            height += m_rowHeights[ row ] + ( row == nrows-1 ? 0 : m_vgap );

    return wxSize( width, height );
}

void wxFlexGridSizer::AdjustForFlexDirection()
{
    // the logic in CalcMin works when we resize flexibly in both directions
    // but maybe this is not the case
    if ( m_flexDirection != wxBOTH )
    {
        // select the array corresponding to the direction in which we do *not*
        // resize flexibly
        wxArrayInt& array = m_flexDirection == wxVERTICAL ? m_colWidths
                                                          : m_rowHeights;

        const int count = array.GetCount();

        // find the largest value in this array
        int n, largest = 0;
        for ( n = 0; n < count; ++n )
        {
            if ( array[n] > largest )
                largest = array[n];
        }

        // and now fill it with the largest value
        for ( n = 0; n < count; ++n )
        {
            array[n] = largest;
        }
    }
}    


void wxFlexGridSizer::AdjustForGrowables(const wxSize& sz, const wxSize& minsz,
                                         int nrows, int ncols)
{
    // what to do with the rows? by default, resize them proportionally
    if ( sz.y > minsz.y && ( (m_flexDirection & wxVERTICAL) || (m_growMode == wxFLEX_GROWMODE_SPECIFIED) ) )
    {
        int sum_proportions = 0;
        int growable_space = 0;
        int num = 0;
        size_t idx;
        for (idx = 0; idx < m_growableRows.GetCount(); idx++)
        {
            // Since the number of rows/columns can change as items are
            // inserted/deleted, we need to verify at runtime that the
            // requested growable rows/columns are still valid.
            if (m_growableRows[idx] >= nrows)
                continue;
            
            // If all items in a row/column are hidden, that row/column will
            // have a dimension of -1.  This causes the row/column to be
            // hidden completely.
            if (m_rowHeights[ m_growableRows[idx] ] == -1)
                continue;
            sum_proportions += m_growableRowsProportions[idx];
            growable_space += m_rowHeights[ m_growableRows[idx] ];
            num++;
        }

        if (num > 0)
        {
            for (idx = 0; idx < m_growableRows.GetCount(); idx++)
            {
                if (m_growableRows[idx] >= nrows )
                    continue;
                if (m_rowHeights[ m_growableRows[idx] ] == -1)
                    m_rowHeights[ m_growableRows[idx] ] = 0;
                else
                {
                    int delta = (sz.y - minsz.y);
                    if (sum_proportions == 0)
                        delta = (delta/num) + m_rowHeights[ m_growableRows[idx] ];
                    else
                        delta = ((delta+growable_space)*m_growableRowsProportions[idx]) / sum_proportions;
                    m_rowHeights[ m_growableRows[idx] ] = delta;
                }
            }
        }
    }
    else if ( (m_growMode == wxFLEX_GROWMODE_ALL) && (sz.y > minsz.y) )
    {
        // rounding problem?
        for ( int row = 0; row < nrows; ++row )
            m_rowHeights[ row ] = sz.y / nrows;
    }

    // the same logic as above but for the columns
    if ( sz.x > minsz.x && ( (m_flexDirection & wxHORIZONTAL) || (m_growMode == wxFLEX_GROWMODE_SPECIFIED) ) )
    {
        int sum_proportions = 0;
        int growable_space = 0;
        int num = 0;
        size_t idx;
        for (idx = 0; idx < m_growableCols.GetCount(); idx++)
        {
            // Since the number of rows/columns can change as items are
            // inserted/deleted, we need to verify at runtime that the
            // requested growable rows/columns are still valid.
            if (m_growableCols[idx] >= ncols)
                continue;
            
            // If all items in a row/column are hidden, that row/column will
            // have a dimension of -1.  This causes the column to be hidden
            // completely.
            if (m_colWidths[ m_growableCols[idx] ] == -1)
                continue;
            sum_proportions += m_growableColsProportions[idx];
            growable_space += m_colWidths[ m_growableCols[idx] ];
            num++;
        }

        if (num > 0)
        {
            for (idx = 0; idx < m_growableCols.GetCount(); idx++)
            {
                if (m_growableCols[idx] >= ncols )
                    continue;
                if (m_colWidths[ m_growableCols[idx] ] == -1)
                    m_colWidths[ m_growableCols[idx] ] = 0;
                else
                {
                    int delta = (sz.x - minsz.x);
                    if (sum_proportions == 0)
                        delta = (delta/num) + m_colWidths[ m_growableCols[idx] ];
                    else
                        delta = ((delta+growable_space)*m_growableColsProportions[idx])/sum_proportions;
                    m_colWidths[ m_growableCols[idx] ] = delta;
                }
            }
        }
    }
    else if ( (m_growMode == wxFLEX_GROWMODE_ALL) && (sz.x > minsz.x) )
    {
        for ( int col=0; col < ncols; ++col )
            m_colWidths[ col ] = sz.x / ncols;
    }
}


void wxFlexGridSizer::AddGrowableRow( size_t idx, int proportion )
{
    m_growableRows.Add( idx );
    m_growableRowsProportions.Add( proportion );
}

void wxFlexGridSizer::RemoveGrowableRow( size_t idx )
{
    m_growableRows.Remove( idx );
}

void wxFlexGridSizer::AddGrowableCol( size_t idx, int proportion )
{
    m_growableCols.Add( idx );
    m_growableColsProportions.Add( proportion );
}

void wxFlexGridSizer::RemoveGrowableCol( size_t idx )
{
    m_growableCols.Remove( idx );
}

//---------------------------------------------------------------------------
// wxBoxSizer
//---------------------------------------------------------------------------

wxBoxSizer::wxBoxSizer( int orient )
    : m_orient( orient )
{
}

void wxBoxSizer::RecalcSizes()
{
    if (m_children.GetCount() == 0)
        return;

    int delta = 0;
    if (m_stretchable)
    {
        if (m_orient == wxHORIZONTAL)
            delta = m_size.x - m_fixedWidth;
        else
            delta = m_size.y - m_fixedHeight;
    }

    wxPoint pt( m_position );

    wxSizerItemList::compatibility_iterator node = m_children.GetFirst();
    while (node)
    {
        wxSizerItem     *item = node->GetData();

        if (item->IsShown())
        {
            wxSize size( item->CalcMin() );

            if (m_orient == wxVERTICAL)
            {
                wxCoord height = size.y;
                if (item->GetProportion())
                {
                    // Because of at least one visible item has non-zero
                    // proportion then m_stretchable is not zero
                    height = (delta * item->GetProportion()) / m_stretchable;
                }

                wxPoint child_pos( pt );
                wxSize  child_size( wxSize( size.x, height) );

                if (item->GetFlag() & (wxEXPAND | wxSHAPED))
                    child_size.x = m_size.x;
                else if (item->GetFlag() & wxALIGN_RIGHT)
                    child_pos.x += m_size.x - size.x;
                else if (item->GetFlag() & (wxCENTER | wxALIGN_CENTER_HORIZONTAL))
                // XXX wxCENTER is added for backward compatibility;
                //     wxALIGN_CENTER should be used in new code
                    child_pos.x += (m_size.x - size.x) / 2;

                item->SetDimension( child_pos, child_size );

                pt.y += height;
            }
            else
            {
                wxCoord width = size.x;
                if (item->GetProportion())
                {
                    // Because of at least one visible item has non-zero
                    // proportion then m_stretchable is not zero
                    width = (delta * item->GetProportion()) / m_stretchable;
                }

                wxPoint child_pos( pt );
                wxSize  child_size( wxSize(width, size.y) );

                if (item->GetFlag() & (wxEXPAND | wxSHAPED))
                    child_size.y = m_size.y;
                else if (item->GetFlag() & wxALIGN_BOTTOM)
                    child_pos.y += m_size.y - size.y;
                else if (item->GetFlag() & (wxCENTER | wxALIGN_CENTER_VERTICAL))
                // XXX wxCENTER is added for backward compatibility;
                //     wxALIGN_CENTER should be used in new code
                    child_pos.y += (m_size.y - size.y) / 2;

                item->SetDimension( child_pos, child_size );

                pt.x += width;
            }
        }

        node = node->GetNext();
    }
}

wxSize wxBoxSizer::CalcMin()
{
    if (m_children.GetCount() == 0)
        return wxSize(10,10);

    m_stretchable = 0;
    m_minWidth = 0;
    m_minHeight = 0;
    m_fixedWidth = 0;
    m_fixedHeight = 0;

    wxSizerItemList::compatibility_iterator node = m_children.GetFirst();
    while (node)
    {
        wxSizerItem *item = node->GetData();

        if (item->IsShown() && item->GetProportion() != 0)
            m_stretchable += item->GetProportion();

        node = node->GetNext();
    }

    // Total minimum size (width or height) of sizer
    int maxMinSize = 0;

    node = m_children.GetFirst();
    while (node)
    {
        wxSizerItem *item = node->GetData();

        if (item->IsShown() && item->GetProportion() != 0)
        {
            int stretch = item->GetProportion();
            wxSize size( item->CalcMin() );
            int minSize;
            
            // Integer division rounded up is (a + b - 1) / b
            // Round up needed in order to guarantee that all
            // all items will have size not less then their min size
            if (m_orient == wxHORIZONTAL)
                minSize = ( size.x*m_stretchable + stretch - 1)/stretch;
            else
                minSize = ( size.y*m_stretchable + stretch - 1)/stretch;
            
            if (minSize > maxMinSize)
                maxMinSize = minSize;
        }
        node = node->GetNext();
    }

    // Calculate overall minimum size
    node = m_children.GetFirst();
    while (node)
    {
        wxSizerItem *item = node->GetData();

        if (item->IsShown())
        {
            wxSize size( item->CalcMin() );
            if (item->GetProportion() != 0)
            {
                if (m_orient == wxHORIZONTAL)
                    size.x = (maxMinSize*item->GetProportion())/m_stretchable;
                else
                    size.y = (maxMinSize*item->GetProportion())/m_stretchable;
            }
            else
            {
                if (m_orient == wxVERTICAL)
                {
                    m_fixedHeight += size.y;
                    m_fixedWidth = wxMax( m_fixedWidth, size.x );
                }
                else
                {
                    m_fixedWidth += size.x;
                    m_fixedHeight = wxMax( m_fixedHeight, size.y );
                }
            }

            if (m_orient == wxHORIZONTAL)
            {
                m_minWidth += size.x;
                m_minHeight = wxMax( m_minHeight, size.y );
            }
            else
            {
                m_minHeight += size.y;
                m_minWidth = wxMax( m_minWidth, size.x );
            }
        }
        node = node->GetNext();
    }

    return wxSize( m_minWidth, m_minHeight );
}

//---------------------------------------------------------------------------
// wxStaticBoxSizer
//---------------------------------------------------------------------------

#if wxUSE_STATBOX

wxStaticBoxSizer::wxStaticBoxSizer( wxStaticBox *box, int orient )
    : wxBoxSizer( orient )
    , m_staticBox( box )
{
    wxASSERT_MSG( box, wxT("wxStaticBoxSizer needs a static box") );
}

static void GetStaticBoxBorders( wxStaticBox *box,
                                 int *borderTop,
                                 int *borderOther)
{
    // this has to be done platform by platform as there is no way to
    // guess the thickness of a wxStaticBox border
#ifdef __WXCOCOA__
    box->GetBordersForSizer(borderTop,borderOther);
#else // __WXCOCOA__
#ifdef __WXGTK__
    if ( box->GetLabel().IsEmpty() )
        *borderTop = 5;
    else
#endif // __WXGTK__
        *borderTop = box->GetCharHeight();

    *borderOther = 5;
#endif // __WXCOCOA__
}

void wxStaticBoxSizer::RecalcSizes()
{
    int top_border, other_border;
    GetStaticBoxBorders(m_staticBox, &top_border, &other_border);

    m_staticBox->SetSize( m_position.x, m_position.y, m_size.x, m_size.y );

    wxPoint old_pos( m_position );
    m_position.x += other_border;
    m_position.y += top_border;
    wxSize old_size( m_size );
    m_size.x -= 2*other_border;
    m_size.y -= top_border + other_border;

    wxBoxSizer::RecalcSizes();

    m_position = old_pos;
    m_size = old_size;
}

wxSize wxStaticBoxSizer::CalcMin()
{
    int top_border, other_border;
    GetStaticBoxBorders(m_staticBox, &top_border, &other_border);

    wxSize ret( wxBoxSizer::CalcMin() );
    ret.x += 2*other_border;
    ret.y += other_border + top_border;

    return ret;
}

#endif // wxUSE_STATBOX

// ----------------------------------------------------------------------------
// wxNotebookSizer
// ----------------------------------------------------------------------------

#if wxUSE_BOOKCTRL

wxBookCtrlSizer::wxBookCtrlSizer(wxBookCtrl *bookctrl)
               : m_bookctrl(bookctrl)
{
    wxASSERT_MSG( bookctrl, wxT("wxBookCtrlSizer needs a control") );
}

void wxBookCtrlSizer::RecalcSizes()
{
    m_bookctrl->SetSize( m_position.x, m_position.y, m_size.x, m_size.y );
}

wxSize wxBookCtrlSizer::CalcMin()
{
    wxSize sizeBorder = m_bookctrl->CalcSizeFromPage(wxSize(0, 0));

    sizeBorder.x += 5;
    sizeBorder.y += 5;

    if ( m_bookctrl->GetPageCount() == 0 )
    {
        return wxSize(sizeBorder.x + 10, sizeBorder.y + 10);
    }

    int maxX = 0;
    int maxY = 0;

    wxWindowList::compatibility_iterator
        node = m_bookctrl->GetChildren().GetFirst();
    while (node)
    {
        wxWindow *item = node->GetData();
        wxSizer *itemsizer = item->GetSizer();

        if (itemsizer)
        {
            wxSize subsize( itemsizer->CalcMin() );

            if (subsize.x > maxX)
                maxX = subsize.x;
            if (subsize.y > maxY)
                maxY = subsize.y;
        }

        node = node->GetNext();
    }

    return wxSize( maxX, maxY ) + sizeBorder;
}


#if wxUSE_NOTEBOOK

wxNotebookSizer::wxNotebookSizer(wxNotebook *nb)
    : wxBookCtrlSizer(nb)
{
}

#endif // wxUSE_NOTEBOOOK
#endif // wxUSE_BOOKCTRL

