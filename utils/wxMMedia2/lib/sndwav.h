// --------------------------------------------------------------------------
// Name: sndwav.h
// Purpose:
// Date: 08/11/1999
// Author: Guilhem Lavaux <lavaux@easynet.fr> (C) 1999
// CVSID: $Id$
// --------------------------------------------------------------------------
#ifndef _WX_SNDWAV_H
#define _WX_SNDWAV_H

#ifdef __GNUG__
#pragma interface "sndwav.h"
#endif

#include <wx/defs.h>
#include <wx/stream.h>
#include <wx/datstrm.h>
#include "sndbase.h"
#include "sndcodec.h"
#include "sndfile.h"

//
// WAVE codec
//

class wxSoundWave: public wxSoundFileStream {
 public:
  wxSoundWave(wxInputStream& stream, wxSoundStream& io_sound);
  wxSoundWave(wxOutputStream& stream, wxSoundStream& io_sound);
  ~wxSoundWave();

  bool CanRead();

 protected:
  bool PrepareToPlay(); 
  bool PrepareToRecord(unsigned long time);
  bool FinishRecording();

  wxUint32 GetData(void *buffer, wxUint32 len);
  wxUint32 PutData(const void *buffer, wxUint32 len);

  bool HandlePCM(wxDataInputStream& data, wxUint16 channels,
                 wxUint32 sample_fq, wxUint32 byte_p_sec,
                 wxUint16 byte_p_spl, wxUint16 bits_p_spl);
  bool HandleG721(wxDataInputStream& data, wxUint16 channels,
                  wxUint32 sample_fq, wxUint32 byte_p_sec,
                  wxUint16 byte_p_spl, wxUint16 bits_p_spl);
};

#endif
