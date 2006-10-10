/////////////////////////////////////////////////////////////////////////////
// Name:        wx/generic/animate.h
// Purpose:     wxAnimation and wxAnimationCtrl
// Author:      Julian Smart and Guillermo Rodriguez Garcia
// Modified by: Francesco Montorsi
// Created:     13/8/99
// RCS-ID:      $Id$
// Copyright:   (c) Julian Smart and Guillermo Rodriguez Garcia
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_GENERIC_ANIMATEH__
#define _WX_GENERIC_ANIMATEH__

// ----------------------------------------------------------------------------
// wxAnimation
// ----------------------------------------------------------------------------

WX_DECLARE_LIST_WITH_DECL(wxAnimationDecoder, wxAnimationDecoderList, class WXDLLIMPEXP_ADV);

class WXDLLIMPEXP_ADV wxAnimation : public wxAnimationBase
{
public:
    bool operator == (const wxAnimation& anim) const
        { return m_refData == anim.m_refData; }
    bool operator != (const wxAnimation& anim) const
        { return m_refData != anim.m_refData; }

    bool IsOk() const
        { return m_refData != NULL; }

    size_t GetFrameCount() const;
    int GetDelay(size_t i) const;
    wxImage GetFrame(size_t i) const;
    wxSize GetSize() const;

    bool LoadFile(const wxString& filename, wxAnimationType type = wxANIMATION_TYPE_ANY);
    bool Load(wxInputStream &stream, wxAnimationType type = wxANIMATION_TYPE_ANY);

public:     // extended interface used by the generic implementation of wxAnimationCtrl

    wxPoint GetFramePosition(size_t frame) const;
    wxAnimationDisposal GetDisposalMethod(size_t frame) const;
    wxColour GetBackgroundColour() const;

protected:
    static wxAnimationDecoderList sm_handlers;

public:
    static inline wxAnimationDecoderList& GetHandlers() { return sm_handlers; }
    static void AddHandler(wxAnimationDecoder *handler);
    static void InsertHandler(wxAnimationDecoder *handler);
    static const wxAnimationDecoder *FindHandler( wxAnimationType animType );

    static void CleanUpHandlers();
    static void InitStandardHandlers();

    DECLARE_DYNAMIC_CLASS(wxAnimation)
};


// ----------------------------------------------------------------------------
// wxAnimationCtrl
// ----------------------------------------------------------------------------

class WXDLLIMPEXP_ADV wxAnimationCtrl: public wxAnimationCtrlBase
{
public:
    wxAnimationCtrl();
    wxAnimationCtrl(wxWindow *parent,
            wxWindowID id,
            const wxAnimation& anim = wxNullAnimation,
            const wxPoint& pos = wxDefaultPosition,
            const wxSize& size = wxDefaultSize,
            long style = wxAC_DEFAULT_STYLE,
            const wxString& name = wxAnimationCtrlNameStr)
    {
        Create(parent, id, anim, pos, size, style, name);
    }

    bool Create(wxWindow *parent, wxWindowID id,
                const wxAnimation& anim = wxNullAnimation,
                const wxPoint& pos = wxDefaultPosition,
                const wxSize& size = wxDefaultSize,
                long style = wxAC_DEFAULT_STYLE,
                const wxString& name = wxAnimationCtrlNameStr);

    ~wxAnimationCtrl();

public:
    virtual bool LoadFile(const wxString& filename, wxAnimationType type = wxANIMATION_TYPE_ANY);

    virtual void Stop();
    virtual bool Play()
        { return Play(true /* looped */); }
    virtual bool IsPlaying() const
        { return m_isPlaying; }

    void SetAnimation(const wxAnimation &animation);
    wxAnimation GetAnimation() const
        { return m_animation; }

public:     // event handlers

    void OnPaint(wxPaintEvent& event);
    void OnTimer(wxTimerEvent& event);
    void OnSize(wxSizeEvent& event);

public:     // extended API specific to this implementation of wxAnimateCtrl

    // Specify whether the animation's background colour is to be shown (the default),
    // or whether the window background should show through
    void SetUseWindowBackgroundColour(bool useWinBackground = true)
        { m_useWinBackgroundColour = useWinBackground; }
    bool IsUsingWindowBackgroundColour() const
        { return m_useWinBackgroundColour; }

    // This overload of Play() lets you specify if the animation must loop or not
    bool Play(bool looped);

    // Draw the current frame of the animation into given DC.
    // This is fast as current frame is always cached.
    void DrawCurrentFrame(wxDC& dc);

    // Returns a wxBitmap with the current frame drawn in it
    wxBitmap& GetBackingStore()
        { return m_backingStore; }

protected:      // internal utilities

    // resize this control to fit m_animation
    void FitToAnimation();

    // Draw the background; use this when e.g. previous frame had wxANIM_TOBACKGROUND disposal.
    void DisposeToBackground(wxDC& dc);

    void IncrementalUpdateBackingStore();
    void RebuildBackingStoreUpToFrame(size_t);
    void DrawFrame(wxDC &dc, size_t);

    virtual wxSize DoGetBestSize() const;

protected:
    size_t        m_currentFrame;     // Current frame
    bool          m_looped;           // Looped, or not
    wxTimer       m_timer;            // The timer
    wxAnimation   m_animation;        // The animation

    bool          m_isPlaying;        // Is the animation playing?
    bool          m_useWinBackgroundColour; // Use animation bg colour or window bg colour?

    wxBitmap      m_backingStore;     // The frames are drawn here and then blitted
                                      // on the screen

private:
    typedef wxAnimationCtrlBase base_type;
    DECLARE_DYNAMIC_CLASS(wxAnimationCtrl)
    DECLARE_EVENT_TABLE()
};

#endif // _WX_GENERIC_ANIMATEH__
