///////////////////////////////////////////////////////////////////////////////
// Name:        wx/qt/utils.h
// Purpose:     utility classes and/or functions
// Author:      Peter Most, Javier Torres
// Created:     15/05/10
// RCS-ID:      $Id$
// Copyright:   (c) Peter Most, Javier Torres
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

#ifndef _WX_QT_UTILS_H_
#define _WX_QT_UTILS_H_

#include "wx/mousestate.h"
#include <QtCore/Qt>

void wxQtFillMouseButtons( Qt::MouseButtons buttons, wxMouseState *state );

void wxMissingImplementation( const char fileName[], unsigned lineNumber,
    const char feature[] );

#define wxMISSING_IMPLEMENTATION( feature )\
    wxMissingImplementation( __FILE__, __LINE__, feature )

template< typename QtWidget, typename WxWindow >
    QtWidget *wxQtCreateWidget( WxWindow *window, wxWindow *parent )
    {
        QWidget *qtParent = NULL;
        if ( parent != NULL )
        {
            qtParent = parent->QtGetContainer();
            parent->AddChild( window );
        }
        return new QtWidget( window, qtParent );
    }

#endif // _WX_QT_UTILS_H_
