/////////////////////////////////////////////////////////////////////////////
// Name:        src/gtk/dcmemory.cpp
// Purpose:
// Author:      Robert Roebling
// RCS-ID:      $Id: dcmemory.cpp 61724 2009-08-21 10:41:26Z VZ $
// Copyright:   (c) 1998 Robert Roebling
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#include "wx/dcmemory.h"
#include "wx/gtk/dcmemory_gtk3.h"
#include "wx/graphics.h"

#include "wx/gtk/private.h"

//-----------------------------------------------------------------------------
// wxMemoryDCImpl
//-----------------------------------------------------------------------------

IMPLEMENT_ABSTRACT_CLASS(wxMemoryDCImpl,wxPaintDCImpl)


wxMemoryDCImpl::wxMemoryDCImpl( wxMemoryDC *owner )
  : wxPaintDCImpl( owner )
{
    Init();
}

wxMemoryDCImpl::wxMemoryDCImpl( wxMemoryDC *owner, wxBitmap& bitmap )
  : wxPaintDCImpl( owner )
{
    Init();
    DoSelect(bitmap);
}

wxMemoryDCImpl::wxMemoryDCImpl( wxMemoryDC *owner, wxDC * WXUNUSED(dc) )
  : wxPaintDCImpl( owner )
{
    Init();
}

void wxMemoryDCImpl::Init()
{
    m_ok = true;
    SetBackground(*wxWHITE_BRUSH);
    SetBrush(*wxWHITE_BRUSH);
    SetPen(*wxBLACK_PEN);
    SetFont(*wxNORMAL_FONT);
    m_ok = false;
}

wxMemoryDCImpl::~wxMemoryDCImpl()
{
    if ( m_selected.IsOk() )
    {
        // m_selected.EndRawAccess() ;
        wxDELETE(m_graphicContext);
    }
}

void wxMemoryDCImpl::DoSelect( const wxBitmap& bitmap )
{
    if ( m_selected.IsOk() )
    {
        // m_selected.EndRawAccess() ;
        wxDELETE(m_graphicContext);
    }

    m_selected = bitmap;

    if (m_selected.IsOk())
    {
        // if ( m_selected.GetDepth() != 1 )
        //     m_selected.UseAlpha() ;

        // m_selected.BeginRawAccess() ;
        bitmap.GetPixmap();
        m_width = bitmap.GetWidth();
        m_height = bitmap.GetHeight();
        // CGColorSpaceRef genericColorSpace  = wxMacGetGenericRGBColorSpace();
        // CGContextRef bmCtx = (CGContextRef) m_selected.GetHBITMAP();

        // if ( bmCtx )
        // {
        //     CGContextSetFillColorSpace( bmCtx, genericColorSpace );
        //     CGContextSetStrokeColorSpace( bmCtx, genericColorSpace );
        //     SetGraphicsContext( wxGraphicsContext::CreateFromNative( bmCtx ) );
        // }
        m_ok = (m_graphicContext != NULL) ;
    }
    else
    {
        m_ok = false;
    }
}

void wxMemoryDCImpl::DoGetSize( int *width, int *height ) const
{
    if (m_selected.IsOk())
    {
        if (width)
            (*width) = m_selected.GetWidth();
        if (height)
            (*height) = m_selected.GetHeight();
    }
    else
    {
        if (width)
            (*width) = 0;
        if (height)
            (*height) = 0;
    }
}
