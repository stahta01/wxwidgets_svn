/////////////////////////////////////////////////////////////////////////////
// Name:        glcanvas.h
// Purpose:     wxGLCanvas, for using OpenGL/Mesa with wxWindows and GTK
// Author:      Robert Roebling
// Modified by:
// Created:     17/8/98
// RCS-ID:      $Id$
// Copyright:   (c) Robert Roebling
// Licence:   	wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifdef __GNUG__
#pragma interface "glcanvas.h"
#endif

#ifndef _WX_GLCANVAS_H_
#define _WX_GLCANVAS_H_

#include "wx/defs.h"
#include "wx/scrolwin.h"

extern "C" {
#include "GL/gl.h"
#include "GL/glx.h"
#include "GL/glu.h"
}

//---------------------------------------------------------------------------
// classes
//---------------------------------------------------------------------------

class wxGLContext;
class wxGLCanvas;

//---------------------------------------------------------------------------
// wxGLContext
//---------------------------------------------------------------------------


class wxGLContext: public wxObject
{
  DECLARE_CLASS(wxGLContext)
   
  public:
  
    wxGLContext( bool isRGB, wxWindow *win, const wxPalette& palette = wxNullPalette );
    ~wxGLContext();

    void SetCurrent();
    void SetColour(const char *colour);
    void SwapBuffers();

    void SetupPixelFormat();
    void SetupPalette(const wxPalette& palette);
    wxPalette CreateDefaultPalette();

    inline wxPalette* GetPalette() const { return (wxPalette*) & m_palette; }
    inline wxWindow* GetWindow() const { return m_window; }
    inline GtkWidget* GetWidget() const { return m_widget; }
    inline GLXContext GetContext() const { return m_glContext; }

 public:

   GLXContext       m_glContext;

   GtkWidget       *m_widget;
   wxPalette        m_palette;
   wxWindow*        m_window;
};

//---------------------------------------------------------------------------
// wxGLContext
//---------------------------------------------------------------------------

class wxGLCanvas: public wxScrolledWindow
{
   DECLARE_CLASS(wxGLCanvas)
   
 public:
   wxGLCanvas( wxWindow *parent, wxWindowID id = -1, 
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize, 
	long style = 0, const wxString& name = "GLCanvas", 
	int *attribList = (int*) NULL, 
	const wxPalette& palette = wxNullPalette );
	
   bool Create( wxWindow *parent, wxWindowID id = -1, 
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize, 
	long style = 0, const wxString& name = "GLCanvas", 
	int *attribList = (int*) NULL,
	const wxPalette& palette = wxNullPalette );
	
   ~wxGLCanvas();

   void SetCurrent();
   void SetColour(const char *colour);
   void SwapBuffers();

   void OnSize(wxSizeEvent& event);

   inline wxGLContext* GetContext() const { return m_glContext; } 

 // implementation
  
    virtual void SetSize( int x, int y, int width, int height,
      int sizeFlags = wxSIZE_AUTO );
    virtual void SetSize( int width, int height );
    
    virtual GtkWidget *GetConnectWidget();
    bool IsOwnGtkWindow( GdkWindow *window );
  
    wxGLContext      *m_glContext; 
    GtkWidget        *m_glWidget;

  DECLARE_EVENT_TABLE()
};

#endif
