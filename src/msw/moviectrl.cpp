/////////////////////////////////////////////////////////////////////////////
// Name:        msw/moviectrl.cpp
// Purpose:     wxMovieCtrl MSW
// Author:      Ryan Norton <wxprojects@comcast.net>
// Modified by: 
// Created:     11/07/04
// RCS-ID:      $Id$
// Copyright:   (c) Ryan Norton
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

//#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
//#pragma implementation "moviectrl.h"
//#endif

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#define wxUSE_MOVIECTRL 1

#if wxUSE_MOVIECTRL

#include "wx/moviectrl.h"
#include "wx/msw/ole/oleutils.h" //for wxBasicString

#include <dshow.h>

IMPLEMENT_CLASS(wxMovieCtrl, wxControl);
IMPLEMENT_DYNAMIC_CLASS(wxMovieEvent, wxEvent); 
DEFINE_EVENT_TYPE(wxEVT_MOVIE_FINISHED); 

#define SAFE_RELEASE(x) { if (x) x->Release(); x = NULL; }

#define WM_GRAPHNOTIFY  WM_USER+13

#ifdef __WXDEBUG__
#define wxDSVERIFY(x) wxASSERT( SUCCEEDED ((x)) )
#else
#define wxDSVERIFY(x) (x)
#endif

//it's there someplace :)
extern "C" WXDLLIMPEXP_BASE HWND
wxCreateHiddenWindow(LPCTSTR *pclassname, LPCTSTR classname, WNDPROC wndproc);

bool wxMovieCtrl::Create(wxWindow* parent, wxWindowID id, const wxString& fileName, 
                         const wxString& label, const wxPoint& pos, const wxSize& size, 
                         long style, const wxString& name)
{
    //do some window stuff - ORDER IS IMPORTANT
    //base create
    if ( !wxControl::Create(parent, id, pos, size, wxNO_BORDER | wxCLIP_CHILDREN, wxDefaultValidator, name) )
        return false;

    //Set our background color to black by default
    SetBackgroundColour(*wxBLACK);

    if(!fileName.empty())
    {
        if (!Load(fileName))
            return false;

        SetLabel(label);

        if(!Play())
            return false;
    }
    else
        wxControl::SetLabel(label);

    return true;
}

bool wxMovieCtrl::Load(const wxString& fileName)
{
    if(m_bLoaded)
        Cleanup();                

    //cast helpers
    IGraphBuilder*& pGB = (IGraphBuilder*&) m_pGB;
    IMediaControl*& pMC = (IMediaControl*&) m_pMC;
    IMediaEventEx*& pME = (IMediaEventEx*&) m_pME;
    IVideoWindow*& pVW  = (IVideoWindow*&)  m_pVW;
    IBasicAudio*& pBA   = (IBasicAudio*&)   m_pBA;
    IBasicVideo*& pBV   = (IBasicVideo*&)   m_pBV;
    IMediaSeeking*& pMS = (IMediaSeeking*&) m_pMS;

    //create our filter graph
    CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER,
                      IID_IGraphBuilder, (void**)&pGB);

    //load the graph & render 
    if (FAILED(pGB->RenderFile(fileName.wc_str(wxConvLocal), NULL)))
    {
        wxFAIL_MSG("Could not load movie!");
        return false;
    }

    //get the interfaces, all of them
    wxDSVERIFY( pGB->QueryInterface(IID_IMediaControl, (void**)&pMC) );
    wxDSVERIFY( pGB->QueryInterface(IID_IMediaEventEx, (void**)&pME) );
    wxDSVERIFY( pGB->QueryInterface(IID_IMediaSeeking, (void**)&pMS) );
    wxDSVERIFY( pGB->QueryInterface(IID_IVideoWindow, (void**)&pVW) );
    wxDSVERIFY( pGB->QueryInterface(IID_IBasicAudio, (void**)&pBA) );
    wxDSVERIFY( pGB->QueryInterface(IID_IBasicVideo, (void**)&pBV) );

    //get the _actual_ size of the movie & remember it
    long nX, nY, nSX, nSY;
    if (FAILED(pVW->GetWindowPosition(&nX,&nY,&nSX,&nSY)))
        m_bVideo = false;
    else 
    {
        m_bVideo = true;
  
        this->Connect( wxID_ANY,
        wxEVT_SIZE,
        (wxObjectEventFunction) (wxEventFunction) (wxSizeEventFunction) &wxMovieCtrl::OnSize );
    }

    m_bestSize.x = nSX;
    m_bestSize.y = nSY;

    if (m_bVideo)
    {
        wxDSVERIFY( pVW->put_Owner((OAHWND)this->GetHandle()) );
        wxDSVERIFY( pVW->put_WindowStyle(WS_CHILD | WS_CLIPSIBLINGS) );
    //    wxDSVERIFY( pME->SetNotifyWindow((OAHWND)this->GetHandle(), WM_GRAPHNOTIFY, 0) );
        wxDSVERIFY( pVW->put_Visible(OATRUE) ); //OATRUE actually == -1 :)
    }

    //set the time format
    wxDSVERIFY( pMS->SetTimeFormat(&TIME_FORMAT_MEDIA_TIME) );

    m_bLoaded = true;
    return true;
}

void wxMovieCtrl::SetLabel(const wxString& label)
{
    wxControl::SetLabel(label);

    IVideoWindow*& pVW  = (IVideoWindow*&)  m_pVW;
    
    //wxBasicString will have a null string on an
    //empty wxString - gotta love those workarounds!!
    if(!label.empty() && m_bVideo)
    {
        wxBasicString theBasicString(label.mb_str());
        wxDSVERIFY( pVW->put_Caption(theBasicString.Get()) );
    }
}

bool wxMovieCtrl::Play()
{
    return SUCCEEDED( ((IMediaControl*&)m_pMC)->Run() );
}

bool wxMovieCtrl::Pause()
{
    return SUCCEEDED( ((IMediaControl*&)m_pMC)->Pause() );
}

bool wxMovieCtrl::Stop()
{
    return SUCCEEDED( ((IMediaControl*&)m_pMC)->Stop() );
}

#if wxUSE_DATETIME

bool wxMovieCtrl::Seek(const wxTimeSpan& where)
{
    //DS uses 100 nanos - so we need a 10 mult
    LONGLONG pos = ((size_t)where.GetMilliseconds().ToLong()) * 10;

    return SUCCEEDED( ((IMediaSeeking*&)m_pMS)->SetPositions(
                                &pos, 
                                AM_SEEKING_AbsolutePositioning,
                                NULL, 
                                AM_SEEKING_NoPositioning
                                    ) );
}

wxTimeSpan wxMovieCtrl::Tell()
{
    LONGLONG outCur, outStop;
    wxDSVERIFY( ((IMediaSeeking*&)m_pMS)->GetPositions(&outCur, &outStop) );

    return outCur;
}

wxTimeSpan wxMovieCtrl::Length()
{
    LONGLONG outDuration;
    wxDSVERIFY( ((IMediaSeeking*&)m_pMS)->GetDuration(&outDuration) );

    return outDuration;
}

#endif // wxUSE_DATETIME

wxMovieCtrlState wxMovieCtrl::GetState()
{
    HRESULT hr;
    OAFilterState theState;
    hr = ((IMediaControl*&)m_pMC)->GetState(INFINITE, &theState);
    
    wxASSERT( SUCCEEDED(hr) );

    //MSW state is the same as ours
    //State_Stopped   = 0,
    //State_Paused    = State_Stopped + 1,
    //State_Running   = State_Paused + 1

    return (wxMovieCtrlState) theState;
}

double wxMovieCtrl::GetPlaybackRate()
{
    double dRate;
    wxDSVERIFY( ((IMediaSeeking*&)m_pMS)->GetRate(&dRate) );
    return dRate;
}

bool wxMovieCtrl::SetPlaybackRate(double dRate)
{
    return SUCCEEDED( ((IMediaSeeking*&)m_pMS)->SetRate(dRate) );
}

WXLRESULT wxMovieCtrl::MSWWindowProc(WXUINT nMsg, WXWPARAM wParam, WXLPARAM lParam)
{
    //cast helpers
//    IMediaControl*& pMC = (IMediaControl*&) m_pMC;
    IMediaEventEx*& pME = (IMediaEventEx*&) m_pME;
//    IMediaSeeking*& pMS = (IMediaSeeking*&) m_pMS;

    if (nMsg == WM_GRAPHNOTIFY)
    {
        LONG evCode, evParam1, evParam2;
        HRESULT hr=S_OK;

        // Process all queued events
        while(SUCCEEDED(pME->GetEvent(&evCode, (LONG_PTR *) &evParam1,
                                       (LONG_PTR *) &evParam2, 0) 
                       ) 
             )
        {
            // Free memory associated with callback, since we're not using it
            hr = pME->FreeEventParams(evCode, evParam1, evParam2);

            // If this is the end of the clip, notify handler
            if(EC_COMPLETE == evCode)
            {
                wxMovieEvent theEvent(wxEVT_MOVIE_FINISHED, this->GetId());
                GetParent()->ProcessEvent(theEvent);
/*
                LONGLONG pos=0;

                // Reset to first frame of movie
                hr = pMS->SetPositions(&pos, AM_SEEKING_AbsolutePositioning ,
                                       NULL, AM_SEEKING_NoPositioning);
                if (FAILED(hr))
                {
                    hr = pMC->Stop();
                    // for filters that don't support seeking do a rewind
                    if (FAILED(hr))
                    {
                        wxFAIL_MSG(
                            wxString::Format(TEXT("Failed(0x%08lx) to stop media clip!\r\n"), hr)
                                    );
                        break;
                    }

                    hr = hr = pMC->Run();

                    if (FAILED(hr))
                    {
                        wxFAIL_MSG(
                        wxString::Format(TEXT("Failed(0x%08lx) to reset media clip!\r\n"), hr)
                                    );
                        break;
                    }
                }
*/
            }
        }    
        return wxControl::MSWDefWindowProc(nMsg, wParam, lParam);
    }
    //pass the event to our parent
    return wxControl::MSWWindowProc(nMsg, wParam, lParam);
}

void wxMovieCtrl::Cleanup()
{
    if(m_bVideo)
        this->Disconnect( wxID_ANY,
        wxEVT_SIZE,
        (wxObjectEventFunction) (wxEventFunction) (wxSizeEventFunction) &wxMovieCtrl::OnSize );

    //cast helpers
    IGraphBuilder*& pGB = (IGraphBuilder*&) m_pGB;
    IMediaControl*& pMC = (IMediaControl*&) m_pMC;
    IMediaEventEx*& pME = (IMediaEventEx*&) m_pME;
    IVideoWindow*& pVW  = (IVideoWindow*&)  m_pVW;
    IBasicAudio*& pBA   = (IBasicAudio*&)   m_pBA;
    IBasicVideo*& pBV   = (IBasicVideo*&)   m_pBV;
    IMediaSeeking*& pMS = (IMediaSeeking*&) m_pMS;

    // Hide then disown the window
    if(pVW)
    {
        pVW->put_Visible(OAFALSE); //OSFALSE == 0
        pVW->put_Owner(NULL);
    }

    // Release and zero DirectShow interfaces
    SAFE_RELEASE(pME);
    SAFE_RELEASE(pMS);
    SAFE_RELEASE(pMC);
    SAFE_RELEASE(pBA);
    SAFE_RELEASE(pBV);
    SAFE_RELEASE(pVW);
    SAFE_RELEASE(pGB);
}

wxMovieCtrl::~wxMovieCtrl()
{
    if (m_bLoaded)
        Cleanup();
}

wxSize wxMovieCtrl::DoGetBestSize() const
{
    return m_bestSize;
}

void wxMovieCtrl::OnSize(wxSizeEvent& evt)
{
    IVideoWindow*& pVW  = (IVideoWindow*&)  m_pVW;
    wxDSVERIFY( pVW->SetWindowPosition(0, 0, evt.GetSize().GetWidth(), evt.GetSize().GetHeight()) );

    evt.Skip();
}

#endif //wxUSE_MOVIECTRL