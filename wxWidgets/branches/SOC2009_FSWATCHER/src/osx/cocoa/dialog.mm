/////////////////////////////////////////////////////////////////////////////
// Name:        src/osx/cocoa/dialog.cpp
// Purpose:     wxDialog class
// Author:      Stefan Csomor
// Modified by:
// Created:     1998-01-01
// RCS-ID:      $Id: dialog.cpp 54820 2008-07-29 20:04:11Z SC $
// Copyright:   (c) Stefan Csomor
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#include "wx/wxprec.h"

#include "wx/dialog.h"

#ifndef WX_PRECOMP
    #include "wx/app.h"
    #include "wx/utils.h"
    #include "wx/frame.h"
    #include "wx/settings.h"
#endif // WX_PRECOMP

#include "wx/osx/private.h"

extern wxList wxModalDialogs;

void wxDialog::DoShowModal()
{
    wxCHECK_RET( !IsModal(), wxT("DoShowModal() called twice") );

    wxModalDialogs.Append(this);

    SetFocus() ;
/*
    WindowGroupRef windowGroup;
    WindowGroupRef formerParentGroup;
    bool resetGroupParent = false;

    if ( GetParent() == NULL )
    {
        windowGroup = GetWindowGroup(windowRef) ;
        formerParentGroup = GetWindowGroupParent( windowGroup );
        SetWindowGroupParent( windowGroup, GetWindowGroupOfClass( kMovableModalWindowClass ) );
        resetGroupParent = true;
    }
*/
    NSWindow* theWindow = GetWXWindow();
    
    NSModalSession session = [NSApp beginModalSessionForWindow:theWindow];
    while (IsModal()) 
    {
        wxMacAutoreleasePool autoreleasepool;
        if ([NSApp runModalSession:session] != NSRunContinuesResponse)
            break;
        // TODO should we do some idle processing ?
    }
    [NSApp endModalSession:session];

/*
    if ( resetGroupParent )
    {
        SetWindowGroupParent( windowGroup , formerParentGroup );
    }
*/
}
