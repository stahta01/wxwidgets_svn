/////////////////////////////////////////////////////////////////////////////
// Name:        dialoged.cpp
// Purpose:     Main Dialog Editor implementation file
// Author:      Julian Smart
// Modified by:
// Created:     04/01/98
// RCS-ID:      $Id$
// Copyright:   (c) Julian Smart
// Licence:   	wxWindows license
/////////////////////////////////////////////////////////////////////////////

#ifdef __GNUG__
#pragma implementation "dialoged.h"
#endif

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "dialoged.h"

// A macro needed for some compilers (AIX) that need 'main' to be defined
// in the application itself.
IMPLEMENT_WXWIN_MAIN

IMPLEMENT_APP(MyApp)

MyApp::MyApp(void)
{
}

wxResourceManager theResourceManager;

bool MyApp::OnInit(void)
{
  theResourceManager.Initialize();
  theResourceManager.ShowResourceEditor(TRUE);
  
  if (argc > 1)
    theResourceManager.Load(argv[1]);

  SetTopWindow(theResourceManager.GetEditorFrame());

  return TRUE;
}

