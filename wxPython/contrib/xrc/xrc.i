/////////////////////////////////////////////////////////////////////////////
// Name:        xrc.i
// Purpose:     Wrappers for the XML based Resource system
//
// Author:      Robin Dunn
//
// Created:     4-June-2001
// RCS-ID:      $Id$
// Copyright:   (c) 2001 by Total Control Software
// Licence:     wxWindows license
/////////////////////////////////////////////////////////////////////////////

%module xrc


%{
#include "wx/wxPython/wxPython.h"
#include "wx/wxPython/pyclasses.h"
#include "wx/wxPython/pyistream.h"

#include <wx/xml/xml.h>
#include <wx/xrc/xmlres.h>

%}

//---------------------------------------------------------------------------

%import core.i
%pythoncode { wx = _core }
%pythoncode { __docfilter__ = wx.__DocFilter(globals()) }


MAKE_CONST_WXSTRING_NOSWIG(EmptyString);
MAKE_CONST_WXSTRING2(UTF8String,   wxT("UTF-8"));
MAKE_CONST_WXSTRING2(StyleString,  wxT("style"));
MAKE_CONST_WXSTRING2(SizeString,   wxT("size"));
MAKE_CONST_WXSTRING2(PosString,    wxT("pos"));
MAKE_CONST_WXSTRING2(BitmapString, wxT("bitmap"));
MAKE_CONST_WXSTRING2(IconString,   wxT("icon"));
MAKE_CONST_WXSTRING2(FontString,   wxT("font"));


%include _xrc_rename.i


// Include all the files that make up this module
%include _xmlres.i
%include _xmlsub.i
%include _xml.i
%include _xmlhandler.i



//---------------------------------------------------------------------------

%init %{

    wxXmlInitResourceModule();
    wxXmlResource::Get()->InitAllHandlers();

%}


%pythoncode "_xrc_ex.py";


//---------------------------------------------------------------------------
