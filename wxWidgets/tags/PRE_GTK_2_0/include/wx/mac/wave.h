/////////////////////////////////////////////////////////////////////////////
// Name:        wave.h
// Purpose:     wxWave class (loads and plays short Windows .wav files).
//              Optional on non-Windows platforms.
// Author:      AUTHOR
// Modified by:
// Created:     ??/??/98
// RCS-ID:      $Id$
// Copyright:   (c) AUTHOR
// Licence:   	wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_WAVE_H_
#define _WX_WAVE_H_

#ifdef __GNUG__
#pragma interface "wave.h"
#endif

#include "wx/object.h"

class WXDLLEXPORT wxWave : public wxObject
{
public:
  wxWave();
  wxWave(const wxString& fileName, bool isResource = FALSE);
  wxWave(int size, const wxByte* data);
  ~wxWave();

public:
  bool  Create(const wxString& fileName, bool isResource = FALSE);
  bool  IsOk() const { return !m_sndname.IsEmpty(); }
  bool  Play(bool async = TRUE, bool looped = FALSE) const;

protected:
  bool  Free();

private:
  void* m_sndChan;
	
  wxString m_sndname;
  void* m_hSnd;
  int   m_waveLength;
  bool  m_isResource;
};

#endif
    // _WX_WAVE_H_
