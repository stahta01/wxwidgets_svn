// --------------------------------------------------------------------------
// Name: sndcodec.h
// Purpose:
// Date: 08/11/1999
// Author: Guilhem Lavaux <lavaux@easynet.fr> (C) 1999
// CVSID: $Id$
// License:    wxWindows license
// --------------------------------------------------------------------------
#ifndef _WX_SNDCODEC_H
#define _WX_SNDCODEC_H

#ifdef __GNUG__
#pragma interface "sndcodec.h"
#endif

#include "wx/defs.h"
#include "wx/mmedia/sndbase.h"

class WXDLLEXPORT wxSoundStreamCodec: public wxSoundStream {
 public:
  wxSoundStreamCodec(wxSoundStream& snd_io);
  ~wxSoundStreamCodec();

  bool StartProduction(int evt);
  bool StopProduction();

  wxUint32 GetBestSize() const;

 protected:
  wxSoundStream *m_sndio;
};

#endif
