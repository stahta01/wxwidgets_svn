/////////////////////////////////////////////////////////////////////////////
// Name:        objstrm.h
// Purpose:     wxObjectStream classes
// Author:      Guilhem Lavaux
// Modified by:
// Created:     16/07/98
// RCS-ID:      $Id$
// Copyright:   (c) 1998 Guilhem Lavaux
// Licence:     wxWindows license
/////////////////////////////////////////////////////////////////////////////
#ifndef __WXOBJSTRM_H__
#define __WXOBJSTRM_H__

#ifdef __GNUG__
#pragma interface
#endif

#include "wx/defs.h"
#include "wx/object.h"
#include "wx/string.h"
#include "wx/stream.h"

class wxObjectStreamInfo : public wxObject {
 public:
  wxString object_name;
  int n_children;
  wxList children;
  wxObject *parent;
  wxObject *object;
};

class wxObjectOutputStream : public wxFilterOutputStream {
 public:
  wxObjectOutputStream(wxOutputStream& s);

  void AddChildren(wxObject *obj);
  bool SaveObject(wxObject& obj);

  bool FirstStage() const { return m_stage == 0; }

  wxString GetObjectName(wxObject *obj);

 protected:
  void WriteObjectDef(wxObjectStreamInfo& info);
  void ProcessObjectDef(wxObjectStreamInfo *info);
  void ProcessObjectData(wxObjectStreamInfo *info);

 protected:
  int m_stage;
  bool m_saving;
  wxObjectStreamInfo *m_current_info;
};

class wxObjectInputStream : public wxFilterInputStream {
 public:
  wxObjectInputStream(wxInputStream& s);

  wxObject *GetChild(int no) const;
  wxObject *GetParent() const { return m_current_info->parent; }
  wxObject *LoadObject();

  wxObject *SolveName(const wxString& objName) const;

 protected:
  bool ReadObjectDef(wxObjectStreamInfo *info);
  wxObjectStreamInfo *ProcessObjectDef(wxObjectStreamInfo *info);
  void ProcessObjectData(wxObjectStreamInfo *info);

 protected:
  wxObjectStreamInfo *m_current_info;
  wxList m_solver;
};

#endif
