/////////////////////////////////////////////////////////////////////////////
// Name:        serbase.h
// Purpose:     Serialization plug-ins 
// Author:      Guilhem Lavaux
// Modified by:
// Created:     July 1998
// RCS-ID:      $Id$
// Copyright:   (c) Guilhem Lavaux
// Licence:     wxWindows license
/////////////////////////////////////////////////////////////////////////////
#ifndef _WX_WX_SERBASEH_H__
#define _WX_WX_SERBASEH_H__

#include <wx/setup.h>
#include <wx/dynlib.h>

#define WXSERIAL(classname) classname##_Serialize

#if wxUSE_SERIAL
class wxObject_Serialize : public wxObject {
  DECLARE_DYNAMIC_CLASS(wxObject_Serialize)
 public:
  wxObject_Serialize() {}
  virtual ~wxObject_Serialize() {}

  void SetObject(wxObject *obj) { m_object = obj; }
  wxObject *Object() { return m_object; }

 protected:
  wxObject *m_object;
};
#endif
  // wxUSE_SERIAL


#define DECLARE_SERIAL_CLASS(classname, parent) \
class WXSERIAL(classname) : public WXSERIAL(parent) { \
  DECLARE_DYNAMIC_CLASS(classname##_Serialize) \
 public: \
  WXSERIAL(classname)() { } \
  virtual ~WXSERIAL(classname)() { } \
\
  virtual void StoreObject(wxObjectOutputStream& stream); \
  virtual void LoadObject(wxObjectInputStream& stream); \
};

#define DECLARE_ALIAS_SERIAL_CLASS(classname, parent) \
class WXSERIAL(classname) : public WXSERIAL(parent) { \
  DECLARE_DYNAMIC_CLASS(classname##_Serialize) \
 public: \
  WXSERIAL(classname)() { } \
  virtual ~WXSERIAL(classname)() { } \
};

#define IMPLEMENT_SERIAL_CLASS(classname, parent) \
IMPLEMENT_DYNAMIC_CLASS(classname##_Serialize, parent##_Serialize)

#define IMPLEMENT_ALIAS_SERIAL_CLASS(classname, parent) \
IMPLEMENT_DYNAMIC_CLASS(classname##_Serialize, parent##_Serialize)

#if wxUSE_SERIAL
DECLARE_SERIAL_CLASS(wxList, wxObject)
DECLARE_SERIAL_CLASS(wxHashTable, wxObject)
#endif
 // wxUSE_SERIAL

#endif
