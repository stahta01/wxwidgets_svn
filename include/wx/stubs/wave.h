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

class wxWave : public wxObject
{
public:
  wxWave();
  wxWave(const wxString& fileName, bool isResource = FALSE);
  ~wxWave();

public:
  bool  Create(const wxString& fileName, bool isResource = FALSE);
  bool  IsOk() const { return (m_waveData ? TRUE : FALSE); };
  bool  Play(bool async = TRUE, bool looped = FALSE) const;

protected:
  bool  Free();

private:
  byte* m_waveData;
  int   m_waveLength;
  bool  m_isResource;
};

#endif
    // _WX_WAVE_H_
