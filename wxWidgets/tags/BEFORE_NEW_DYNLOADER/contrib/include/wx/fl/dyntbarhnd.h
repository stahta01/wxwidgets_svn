/////////////////////////////////////////////////////////////////////////////
// Name:        No names yet.
// Purpose:     Contrib. demo
// Author:      Aleksandras Gluchovas
// Modified by:
// Created:     23/01/99
// RCS-ID:      $Id$
// Copyright:   (c) Aleksandras Gluchovas
// Licence:   	wxWindows license
/////////////////////////////////////////////////////////////////////////////

#ifndef __DYNTBARHND_G__
#define __DYNTBARHND_G__

#ifdef __GNUG__
    #pragma interface "dyntbarhnd.h"
#endif

#include "wx/fl/controlbar.h"
#include "wx/fl/dyntbar.h"

class cbDynToolBarDimHandler : public cbBarDimHandlerBase
{
	DECLARE_DYNAMIC_CLASS( cbDynToolBarDimHandler )
public:
	void OnChangeBarState(cbBarInfo* pBar, int newState );
	void OnResizeBar( cbBarInfo* pBar, const wxSize& given, wxSize& preferred );
};

#endif /* __DYNTBARHND_G__ */

