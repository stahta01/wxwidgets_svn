/////////////////////////////////////////////////////////////////////////////
// Name:        samples/docview/view.cpp
// Purpose:     View classes implementation
// Author:      Julian Smart
// Modified by: Vadim Zeitlin: merge with the MDI version and general cleanup
// Created:     04/01/98
// RCS-ID:      $Id$
// Copyright:   (c) 1998 Julian Smart
//              (c) 2008 Vadim Zeitlin
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#if !wxUSE_DOC_VIEW_ARCHITECTURE
    #error You must set wxUSE_DOC_VIEW_ARCHITECTURE to 1 in setup.h!
#endif

#include "docview.h"
#include "doc.h"
#include "view.h"

// ----------------------------------------------------------------------------
// DrawingView implementation
// ----------------------------------------------------------------------------

IMPLEMENT_DYNAMIC_CLASS(DrawingView, wxView)

BEGIN_EVENT_TABLE(DrawingView, wxView)
    EVT_MENU(wxID_CUT, DrawingView::OnCut)
END_EVENT_TABLE()

// What to do when a view is created. Creates actual
// windows for displaying the view.
bool DrawingView::OnCreate(wxDocument *doc, long WXUNUSED(flags) )
{
    MyApp& app = wxGetApp();
    if ( app.GetMode() != MyApp::Mode_Single )
    {
        // create a new window and canvas inside it
        m_frame = app.CreateChildFrame(doc, this, true);
        m_frame->SetTitle("Drawing View");

        m_canvas = new MyCanvas(this, m_frame);
        m_frame->Show(true);
    }
    else // single document mode
    {
        // reuse the existing window and canvas
        m_frame = wxStaticCast(app.GetTopWindow(), wxFrame);
        m_canvas = app.GetMainWindowCanvas();
        m_canvas->SetView(this);

        // Associate the appropriate frame with this view.
        SetFrame(m_frame);

        // Make sure the document manager knows that this is the
        // current view.
        Activate(true);

        // Initialize the edit menu Undo and Redo items
        doc->GetCommandProcessor()->SetEditMenu(app.GetMainWindowEditMenu());
        doc->GetCommandProcessor()->Initialize();
    }

    return true;
}

// Sneakily gets used for default print/preview as well as drawing on the
// screen.
void DrawingView::OnDraw(wxDC *dc)
{
    dc->SetPen(*wxBLACK_PEN);

    // simply draw all lines of all segments
    const DoodleSegments& segments = GetDocument()->GetSegments();
    for ( DoodleSegments::const_iterator i = segments.begin();
          i != segments.end();
          ++i )
    {
        const DoodleLines& lines = i->GetLines();
        for ( DoodleLines::const_iterator j = lines.begin(); 
              j != lines.end();
              ++j )
        {
            const DoodleLine& line = *j;

            dc->DrawLine(line.x1, line.y1, line.x2, line.y2);
        }
    }
}

DrawingDocument* DrawingView::GetDocument()
{
    return wxStaticCast(wxView::GetDocument(), DrawingDocument);
}

void DrawingView::OnUpdate(wxView* sender, wxObject* hint)
{
    wxView::OnUpdate(sender, hint);
    if ( m_canvas )
        m_canvas->Refresh();
}

// Clean up windows used for displaying the view.
bool DrawingView::OnClose(bool deleteWindow)
{
    if ( !wxView::OnClose(deleteWindow) )
        return false;

    Activate(false);

    // Clear the canvas in single-window mode in which it stays alive
    if ( wxGetApp().GetMode() == MyApp::Mode_Single )
    {
        m_canvas->ClearBackground();
        m_canvas->ResetView();
        m_canvas = NULL;

        if ( m_frame )
            m_frame->SetTitle(wxTheApp->GetAppDisplayName());
    }
    else // not single window mode
    {
        if ( deleteWindow )
            wxDELETE(m_frame);
    }

    SetFrame(NULL);

    return true;
}

void DrawingView::OnCut(wxCommandEvent& WXUNUSED(event) )
{
    DrawingDocument * const doc = GetDocument();

    doc->GetCommandProcessor()->Submit(new DrawingRemoveSegmentCommand(doc));
}

// ----------------------------------------------------------------------------
// TextEditView implementation
// ----------------------------------------------------------------------------

IMPLEMENT_DYNAMIC_CLASS(TextEditView, wxView)

BEGIN_EVENT_TABLE(TextEditView, wxView)
    EVT_MENU(wxID_COPY, TextEditView::OnCopy)
    EVT_MENU(wxID_PASTE, TextEditView::OnPaste)
    EVT_MENU(wxID_SELECTALL, TextEditView::OnSelectAll)
END_EVENT_TABLE()

bool TextEditView::OnCreate(wxDocument *doc, long WXUNUSED(flags))
{
    m_frame = wxGetApp().CreateChildFrame(doc, this, false);
    m_text = new wxTextCtrl(m_frame, wxID_ANY, "",
                            wxPoint(0, 0), m_frame->GetClientSize(),
                            wxTE_MULTILINE);

    m_frame->SetTitle("Text View");
    m_frame->Show(true);

    Activate(true);

    return true;
}

void TextEditView::OnDraw(wxDC *WXUNUSED(dc))
{
    // nothing to do here, wxTextCtrl draws itself
}

bool TextEditView::OnClose(bool deleteWindow)
{
    if ( !wxView::OnClose(deleteWindow) )
        return false;

    Activate(false);

    if ( wxGetApp().GetMode() == MyApp::Mode_Single )
    {
        m_text->Clear();
    }
    else // not single window mode
    {
        if ( deleteWindow )
            wxDELETE(m_frame);
    }

    return true;
}

// ----------------------------------------------------------------------------
// MyCanvas implementation
// ----------------------------------------------------------------------------

BEGIN_EVENT_TABLE(MyCanvas, wxScrolledWindow)
    EVT_MOUSE_EVENTS(MyCanvas::OnMouseEvent)
END_EVENT_TABLE()

// Define a constructor for my canvas
MyCanvas::MyCanvas(wxView *view, wxWindow *parent)
    : wxScrolledWindow(parent, wxID_ANY, wxPoint(0, 0), parent->GetClientSize())
{
    m_view = view;
    m_currentSegment = NULL;
    m_lastMousePos = wxDefaultPosition;

    SetCursor(wxCursor(wxCURSOR_PENCIL));

    // this is completely arbitrary and is done just for illustration purposes
    SetVirtualSize(1000, 1000);
    SetScrollRate(20, 20);

    SetBackgroundColour(*wxWHITE);
}

MyCanvas::~MyCanvas()
{
    delete m_currentSegment;
}

// Define the repainting behaviour
void MyCanvas::OnDraw(wxDC& dc)
{
    if ( m_view )
        m_view->OnDraw(& dc);
}

// This implements a tiny doodling program. Drag the mouse using the left
// button.
void MyCanvas::OnMouseEvent(wxMouseEvent& event)
{
    if ( !m_view )
        return;

    wxClientDC dc(this);
    PrepareDC(dc);

    dc.SetPen(*wxBLACK_PEN);

    const wxPoint pt(event.GetLogicalPosition(dc));

    // is this the end of the current segment?
    if ( m_currentSegment && event.LeftUp() )
    {
        if ( !m_currentSegment->IsEmpty() )
        {
            // We've got a valid segment on mouse left up, so store it.
            DrawingDocument * const
                doc = wxStaticCast(m_view->GetDocument(), DrawingDocument);

            doc->GetCommandProcessor()->Submit(
                new DrawingAddSegmentCommand(doc, *m_currentSegment));

            doc->Modify(true);
        }

        wxDELETE(m_currentSegment);
    }

    // is this the start of a new segment?
    if ( m_lastMousePos != wxDefaultPosition && event.Dragging() )
    {
        if ( !m_currentSegment )
            m_currentSegment = new DoodleSegment;

        m_currentSegment->AddLine(m_lastMousePos, pt);

        dc.DrawLine(m_lastMousePos, pt);
    }

    m_lastMousePos = pt;
}

// ----------------------------------------------------------------------------
// ImageCanvas implementation
// ----------------------------------------------------------------------------

// Define a constructor for my canvas
ImageCanvas::ImageCanvas(wxView* view, wxWindow* parent)
    : wxScrolledWindow(parent, wxID_ANY, wxPoint(0, 0), parent->GetClientSize())
{
    SetScrollRate( 10, 10 );

    m_view = view;
}

// Define the repainting behaviour
void ImageCanvas::OnDraw(wxDC& dc)
{
    if ( m_view )
        m_view->OnDraw(& dc);
}

// ----------------------------------------------------------------------------
// ImageView implementation
// ----------------------------------------------------------------------------

IMPLEMENT_DYNAMIC_CLASS(ImageView, wxView)

ImageDocument* ImageView::GetDocument()
{
    return wxStaticCast(wxView::GetDocument(), ImageDocument);
}

bool ImageView::OnCreate(wxDocument* doc, long WXUNUSED(flags))
{
    m_frame = wxGetApp().CreateChildFrame(doc, this, false);
    m_frame->SetTitle("Image View");
    m_canvas = new ImageCanvas(this, m_frame);
    m_frame->Show(true);
    Activate(true);
    return true;
}

void ImageView::OnUpdate(wxView* sender, wxObject* hint)
{
    wxView::OnUpdate(sender, hint);
    wxImage image = GetDocument()->GetImage();
    if ( image.IsOk() )
    {
        m_canvas->SetVirtualSize(image.GetWidth(), image.GetHeight());
    }
}

void ImageView::OnDraw(wxDC* dc)
{
    wxImage image = GetDocument()->GetImage();
    if ( image.IsOk() )
    {
        dc->DrawBitmap(wxBitmap(image), 0, 0);
    }
}

bool ImageView::OnClose(bool deleteWindow)
{
    if ( !wxView::OnClose(deleteWindow) )
        return false;

    Activate(false);

    if ( wxGetApp().GetMode() == MyApp::Mode_Single )
    {
        GetDocument()->DeleteContents();
    }
    else // not single window mode
    {
        if ( deleteWindow )
            wxDELETE(m_frame);
    }
    return true;
}

