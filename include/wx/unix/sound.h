/////////////////////////////////////////////////////////////////////////////
// Name:        wave.h
// Purpose:     wxSound class
// Author:      Julian Smart, Vaclav Slavik
// Modified by:
// Created:     25/10/98
// RCS-ID:      $Id$
// Copyright:   (c) Julian Smart, Vaclav Slavik
// Licence:   	wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_SOUND_H_
#define _WX_SOUND_H_

#include "wx/defs.h"

#if wxUSE_WAVE

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "sound.h"
#endif

#include "wx/object.h"

// ----------------------------------------------------------------------------
// wxSound: simple audio playback class
// ----------------------------------------------------------------------------

class wxSoundBackend;
class wxSound;
class wxDynamicLibrary;

/// Sound data, as loaded from .wav file:
class wxSoundData
{
public:
    wxSoundData() : m_refCnt(1) {}
    void IncRef();
    void DecRef();
    
    // .wav header information:
    unsigned m_channels;       // num of channels (mono:1, stereo:2)
    unsigned m_samplingRate;
    unsigned m_bitsPerSample;  // if 8, then m_data contains unsigned 8bit
                               // samples (wxUint8), if 16 then signed 16bit
                               // (wxInt16)
    unsigned m_samples;        // length in samples:
    
    // wave data:
    size_t   m_dataBytes;
    wxUint8 *m_data;           // m_dataBytes bytes of data

private:
    ~wxSoundData();
    unsigned m_refCnt;
    wxUint8 *m_dataWithHeader; // ditto, but prefixed with .wav header
    friend class wxSound;
};


/// Simple sound class:
class wxSound : public wxSoundBase
{
public:
    wxSound();
    wxSound(const wxString& fileName, bool isResource = false);
    wxSound(int size, const wxByte* data);
    ~wxSound();

    // Create from resource or file
    bool Create(const wxString& fileName, bool isResource = false);
    // Create from data
    bool Create(int size, const wxByte* data);

    bool IsOk() const { return m_data != NULL; }
    
    // for internal use
    static void UnloadBackend();
    
protected:
    bool DoPlay(unsigned flags);

    static void EnsureBackend();
    void Free();
    bool LoadWAV(const wxUint8 *data, size_t length, bool copyData);
    
    static wxSoundBackend *ms_backend;
#if wxUSE_LIBSDL && wxUSE_PLUGINS
    // FIXME - temporary, until we have plugins architecture
    static wxDynamicLibrary *ms_backendSDL;
#endif

private:
    wxSoundData *m_data;
};


// ----------------------------------------------------------------------------
// wxSoundBackend: 
// ----------------------------------------------------------------------------

// This is interface to sound playing implementation. There are multiple
// sound architectures in use on Unix platforms and wxWindows can use several
// of them for playback, depending on their availability at runtime; hence
// the need for backends. This class is for use by wxWindows and people writing
// additional backends only, it is _not_ for use by applications! 

class wxSoundBackend
{
public:
    virtual ~wxSoundBackend() {}
    
    // Returns the name of the backend (e.g. "Open Sound System")
    virtual wxString GetName() const = 0;

    // Returns priority (higher priority backends are tried first)
    virtual int GetPriority() const = 0;

    // Checks if the backend's audio system is available and the backend can
    // be used for playback
    virtual bool IsAvailable() const = 0;

    // Returns true if the backend is capable of playing sound asynchronously.
    // If false, then wxWindows creates a playback thread and handles async
    // playback, otherwise it is left up to the backend (will usually be more
    // effective)
    virtual bool HasNativeAsyncPlayback() const = 0;

    // Plays the sound. flags are same flags as those passed to wxSound::Play
    virtual bool Play(wxSoundData *data, unsigned flags) = 0;
};


#endif // wxUSE_WAVE

#endif
