/////////////////////////////////////////////////////////////////////////////
// Name:        src/common/dcbufcmn.cpp
// Purpose:     Buffered DC implementation
// Author:      Ron Lee, Jaakko Salli
// Modified by:
// Created:     Sep-20-2006
// RCS-ID:      $Id$
// Copyright:   (c) wxWidgets team
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// ============================================================================
// declarations
// ============================================================================

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#include "wx/dcbuffer.h"

#ifndef WX_PRECOMP
    #include "wx/module.h"
#endif

// ============================================================================
// implementation
// ============================================================================

// ----------------------------------------------------------------------------
// wxSharedDCBufferManager: helper class maintaining backing store bitmap
// ----------------------------------------------------------------------------

class wxSharedDCBufferManager : public wxModule
{
public:
    wxSharedDCBufferManager() { }

    virtual bool OnInit() { return true; }
    virtual void OnExit() { wxDELETE(ms_buffer); }

    static wxBitmap* GetBuffer(int w, int h)
    {
        if ( !ms_buffer ||
                w > ms_buffer->GetWidth() ||
                    h > ms_buffer->GetHeight() )
        {
            delete ms_buffer;
            ms_buffer = new wxBitmap(w, h);
        }

        // return a copy of the static instance
        return ms_buffer;
    }

private:
    static wxBitmap *ms_buffer;

    DECLARE_DYNAMIC_CLASS(wxSharedDCBufferManager)
};

wxBitmap* wxSharedDCBufferManager::ms_buffer = NULL;

IMPLEMENT_DYNAMIC_CLASS(wxSharedDCBufferManager, wxModule)

// ============================================================================
// wxBufferedDC
// ============================================================================

void wxBufferedDC::UseBuffer(wxCoord w, wxCoord h)
{
    if ( !m_buffer || !m_buffer->IsOk() )
    {
        if ( w == -1 || h == -1 )
            m_dc->GetSize(&w, &h);

        m_buffer = wxSharedDCBufferManager::GetBuffer(w, h);
    }

    SelectObject(*m_buffer);
}

