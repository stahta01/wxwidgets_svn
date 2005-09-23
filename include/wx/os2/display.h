/////////////////////////////////////////////////////////////////////////////
// Name:        displayx11.h
// Purpose:     wxDisplay class for Unix/X11
// Author:      Brian Victor
// Modified by: 
// Created:     12/05/02
// RCS-ID:      $Id$
// Copyright:   (c) wxWidgets team
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_DISPLAY_H_
#define _WX_DISPLAY_H_

#if wxUSE_DISPLAY

class wxRect;
class wxString;
class wxDisplayUnixPriv;

class WXDLLEXPORT wxDisplay : public wxDisplayBase
{

  public:
    wxDisplay (size_t index = 0);
    ~wxDisplay();

    virtual wxRect            GetGeometry() const;
    virtual int               GetDepth() const;
    virtual wxString          GetName() const;
    virtual wxArrayVideoModes GetModes(const wxVideoMode& mode = wxDefaultVideoMode) const;
    virtual wxVideoMode       GetCurrentMode() const;

    virtual bool              ChangeMode(const wxVideoMode& mode = wxDefaultVideoMode);


  private:
    DECLARE_NO_COPY_CLASS(wxDisplay)
};

#endif // wxUSE_DISPLAY

#endif // _WX_DISPLAY_H_

