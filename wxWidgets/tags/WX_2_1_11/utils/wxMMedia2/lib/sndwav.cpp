// --------------------------------------------------------------------------
// Name: sndwav.cpp
// Purpose:
// Date: 08/11/1999
// Author: Guilhem Lavaux <lavaux@easynet.fr> (C) 1999
// CVSID: $Id$
// --------------------------------------------------------------------------
#ifdef __GNUG__
#pragma implementation "sndwav.cpp"
#endif

#include <wx/wxprec.h>

#include <wx/stream.h>
#include <wx/datstrm.h>
#include <wx/filefn.h>
#include <wx/mstream.h>

#include "sndbase.h"
#include "sndcodec.h"
#include "sndfile.h"
#include "sndpcm.h"
#include "sndg72x.h"
#include "sndwav.h"

#define BUILD_SIGNATURE(a,b,c,d) (((wxUint32)a) | (((wxUint32)b) << 8) | (((wxUint32)c) << 16)  | (((wxUint32)d) << 24)) 

#define RIFF_SIGNATURE BUILD_SIGNATURE('R','I','F','F')
#define WAVE_SIGNATURE BUILD_SIGNATURE('W','A','V','E')
#define FMT_SIGNATURE BUILD_SIGNATURE('f','m','t',' ')
#define DATA_SIGNATURE BUILD_SIGNATURE('d','a','t','a')

#define HEADER_SIZE 4+4 + 4+4+16 + 4+4
 // 4+4 => NAME + LEN
 // 16 => fmt size

wxSoundWave::wxSoundWave(wxInputStream& stream, wxSoundStream& io_sound)
  : wxSoundFileStream(stream, io_sound)
{
}

wxSoundWave::wxSoundWave(wxOutputStream& stream, wxSoundStream& io_sound)
  : wxSoundFileStream(stream, io_sound)
{
}

wxSoundWave::~wxSoundWave()
{
}

#define FAIL_WITH(condition, err) if (condition) { m_snderror = err; return FALSE; }

bool wxSoundWave::CanRead()
{
  wxUint32 len, signature1, signature2;
  m_snderror = wxSOUND_NOERR;

  FAIL_WITH(m_input->Read(&signature1, 4).LastRead() != 4, wxSOUND_INVSTRM);

  if (wxUINT32_SWAP_ON_BE(signature1) != RIFF_SIGNATURE) {
    m_input->Ungetch(&signature1, 4);
    return FALSE;
  }

  m_input->Read(&len, 4);
  FAIL_WITH(m_input->LastRead() != 4, wxSOUND_INVSTRM);

  FAIL_WITH(m_input->Read(&signature2, 4).LastRead() != 4, wxSOUND_INVSTRM);
  m_input->Ungetch(&signature2, 4);
  m_input->Ungetch(&len, 4);
  m_input->Ungetch(&signature1, 4);

  if (wxUINT32_SWAP_ON_BE(signature2) != WAVE_SIGNATURE)
    return FALSE;

  return TRUE;
}

bool wxSoundWave::HandleOutputPCM(wxDataInputStream& data, wxUint16 channels, 
                            wxUint32 sample_fq, wxUint32 byte_p_sec,
                            wxUint16 byte_p_spl, wxUint16 bits_p_spl)
{
  wxSoundFormatPcm sndformat;

  sndformat.SetSampleRate(sample_fq);
  sndformat.SetBPS(bits_p_spl);
  sndformat.SetChannels(channels);
  sndformat.Signed(TRUE);
  sndformat.SetOrder(wxLITTLE_ENDIAN);

  if (!SetSoundFormat(sndformat))
    return FALSE;

  return TRUE;
}

bool wxSoundWave::HandleOutputG721(wxDataInputStream& data, wxUint16 channels,
                             wxUint32 sample_fq, wxUint32 byte_p_sec,
                             wxUint16 byte_p_spl, wxUint16 bits_p_spl)
{
  wxSoundFormatG72X sndformat;

  sndformat.SetSampleRate(sample_fq);
  sndformat.SetG72XType(wxSOUND_G721);
 
  if (!SetSoundFormat(sndformat))
    return FALSE;

  return TRUE;
}

bool wxSoundWave::PrepareToPlay()
{
  wxUint32 signature, len;
  bool end_headers;

  if (!m_input) {
    m_snderror = wxSOUND_INVSTRM;
    return FALSE;
  }

  wxDataInputStream data(*m_input);
  data.BigEndianOrdered(FALSE);

  FAIL_WITH(m_input->Read(&signature, 4).LastRead() != 4, wxSOUND_INVSTRM);
  FAIL_WITH(wxUINT32_SWAP_ON_BE(signature) != RIFF_SIGNATURE, wxSOUND_INVSTRM);
     // "RIFF"

  len = data.Read32();
  FAIL_WITH(m_input->LastRead() != 4, wxSOUND_INVSTRM);
    // dummy len
 
  FAIL_WITH(m_input->Read(&signature, 4).LastRead() != 4, wxSOUND_INVSTRM);
  FAIL_WITH(wxUINT32_SWAP_ON_BE(signature) != WAVE_SIGNATURE, wxSOUND_INVSTRM);
     // "WAVE"
 
  end_headers = FALSE;
  while (!end_headers) {
    FAIL_WITH(m_input->Read(&signature, 4).LastRead() != 4, wxSOUND_INVSTRM);

    len = data.Read32();
    FAIL_WITH(m_input->LastRead() != 4, wxSOUND_INVSTRM);

    switch (wxUINT32_SWAP_ON_BE(signature)) {
    case FMT_SIGNATURE: {  // "fmt "
      wxUint16 format, channels, byte_p_spl, bits_p_spl;
      wxUint32 sample_fq, byte_p_sec;

      data >> format >> channels >> sample_fq 
           >> byte_p_sec >> byte_p_spl >> bits_p_spl;

      switch (format) {
      case 0x01:
        if (!HandleOutputPCM(data, channels, sample_fq,
                       byte_p_sec, byte_p_spl, bits_p_spl))
          return FALSE;
        break;
      case 0x40:
        if (!HandleOutputG721(data, channels, sample_fq,
                        byte_p_sec, byte_p_spl, bits_p_spl))
          return FALSE;
        break;
      default: 
        m_snderror = wxSOUND_NOCODEC;
        return FALSE;
      }
      break;
    }
    case DATA_SIGNATURE: // "data"
      end_headers = TRUE;
      break;
    default:
      m_input->SeekI(len, wxFromCurrent);
      break;
    }
  }
  return TRUE;
}

wxSoundFormatBase *wxSoundWave::HandleInputPCM(wxDataOutputStream& data)
{
  wxUint16 format, channels, byte_p_spl, bits_p_spl;
  wxUint32 sample_fq, byte_p_sec;
  wxSoundFormatPcm *pcm;

  pcm = (wxSoundFormatPcm *)(m_sndformat->Clone());

  // Write block length
  data.Write32(16);

  sample_fq  = pcm->GetSampleRate();
  bits_p_spl = pcm->GetBPS();
  channels   = pcm->GetChannels();
  byte_p_spl = pcm->GetBPS() / 8;
  byte_p_sec = pcm->GetBytesFromTime(1);
  format     = 0x01;

  pcm->Signed(TRUE);
  pcm->SetOrder(wxLITTLE_ENDIAN);

  data << format << channels << sample_fq
       << byte_p_sec << byte_p_spl << bits_p_spl;

  return pcm;
}

wxSoundFormatBase *wxSoundWave::HandleInputG72X(wxDataOutputStream& data)
{
  wxUint16 format, channels, byte_p_spl, bits_p_spl;
  wxUint32 sample_fq, byte_p_sec;
  wxSoundFormatG72X *g72x;

  // Write block length
  data.Write32(16);

  g72x = (wxSoundFormatG72X *)(m_sndformat->Clone());
  if (g72x->GetG72XType() != wxSOUND_G721) {
    delete g72x;
    return NULL;
  } 

  sample_fq  = g72x->GetSampleRate();
  bits_p_spl = 4;
  channels   = 1;
  byte_p_spl = 0;
  byte_p_sec = g72x->GetBytesFromTime(1);
  format     = 0x40;
  data << format << channels << sample_fq
       << byte_p_sec << byte_p_spl << bits_p_spl;

  return g72x;
}

bool wxSoundWave::PrepareToRecord(unsigned long time)
{
#define WRITE_SIGNATURE(s,sig) \
signature = sig; \
signature = wxUINT32_SWAP_ON_BE(signature); \
FAIL_WITH(s->Write(&signature, 4).LastWrite() != 4, wxSOUND_INVSTRM);

  wxUint32 signature;
  wxMemoryOutputStream fmt_data;

  if (!m_output) {
    m_snderror = wxSOUND_INVSTRM;
    return FALSE;
  }

  wxDataOutputStream data(*m_output);
  wxDataOutputStream fmt_d_data(fmt_data);

  data.BigEndianOrdered(FALSE);
  fmt_d_data.BigEndianOrdered(FALSE);

  WRITE_SIGNATURE(m_output, RIFF_SIGNATURE);

  FAIL_WITH(m_output->LastWrite() != 4, wxSOUND_INVSTRM);

  WRITE_SIGNATURE((&fmt_data), WAVE_SIGNATURE);

  {
    wxSoundFormatBase *frmt;

    WRITE_SIGNATURE((&fmt_data), FMT_SIGNATURE);

    switch (m_sndformat->GetType()) {
    case wxSOUND_PCM:
      frmt = HandleInputPCM(fmt_d_data);
      break;
    case wxSOUND_G72X:
      frmt = HandleInputG72X(fmt_d_data);
      break;
    default:
      m_snderror = wxSOUND_NOCODEC;
      return FALSE;
    }

    FAIL_WITH(!frmt, wxSOUND_NOCODEC);

    if (!SetSoundFormat(*frmt)) {
      delete frmt;
      return FALSE;
    }

    delete frmt;
  }

  data << (fmt_data.GetSize() + 8 + m_sndformat->GetBytesFromTime(time));

  {
    char *out_buf;
    out_buf = new char[fmt_data.GetSize()];

    fmt_data.CopyTo(out_buf, fmt_data.GetSize());
    m_output->Write(out_buf, fmt_data.GetSize());

    delete[] out_buf;
  }

  WRITE_SIGNATURE(m_output, DATA_SIGNATURE);
  data.Write32(m_sndformat->GetBytesFromTime(time));
  return TRUE;
}

bool wxSoundWave::FinishRecording()
{
  // TODO: Update headers when we stop before the specified time (if possible)
  return TRUE;
}

wxUint32 wxSoundWave::GetData(void *buffer, wxUint32 len)
{
  return m_input->Read(buffer, len).LastRead();
}

wxUint32 wxSoundWave::PutData(const void *buffer, wxUint32 len)
{
  return m_output->Write(buffer, len).LastWrite();
}
