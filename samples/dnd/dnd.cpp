/////////////////////////////////////////////////////////////////////////////
// Name:        dnd.cpp
// Purpose:     Drag and drop sample
// Author:      Vadim Zeitlin
// Modified by:
// Created:     04/01/98
// RCS-ID:      $Id$
// Copyright:
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#if !wxUSE_DRAG_AND_DROP
    #error This sample requires drag and drop support in the library
#endif

// under Windows we also support data transfer of metafiles as an extra bonus,
// but they're not available under other platforms
#ifdef __WINDOWS__
    #define USE_METAFILES
#endif // Windows

#define USE_RESOURCES 0
#if !wxUSE_RESOURCES
#undef USE_RESOURCES
#define USE_RESOURCES 0
#endif

#include "wx/intl.h"
#include "wx/log.h"

#include "wx/dnd.h"
#include "wx/dirdlg.h"
#include "wx/filedlg.h"
#include "wx/image.h"
#include "wx/clipbrd.h"
#include "wx/colordlg.h"
#if USE_RESOURCES
    #include "wx/resource.h"
#else
    #include "wx/sizer.h"
#endif

#ifdef USE_METAFILES
    #include "wx/metafile.h"
#endif // Windows

#if defined(__WXGTK__) || defined(__WXX11__) || defined(__WXMOTIF__) || defined(__WXMAC__)
    #include "mondrian.xpm"

    #include "dnd_copy.xpm"
    #include "dnd_move.xpm"
    #include "dnd_none.xpm"
#endif

// ----------------------------------------------------------------------------
// Derive two simple classes which just put in the listbox the strings (text or
// file names) we drop on them
// ----------------------------------------------------------------------------

class DnDText : public wxTextDropTarget
{
public:
    DnDText(wxListBox *pOwner) { m_pOwner = pOwner; }

    virtual bool OnDropText(wxCoord x, wxCoord y, const wxString& text);

    // uncomment this if you want to always force Move to be the default
    // action, even under wxGTK where it is normally Copy
#if 0
    wxDragResult OnDragOver(wxCoord x, wxCoord y, wxDragResult def)
    {
        wxDragResult res = wxTextDropTarget::OnDragOver(x, y, def);
        return res == wxDragNone ? wxDragNone : wxDragMove;
    }
#endif // 0

private:
    wxListBox *m_pOwner;
};

class DnDFile : public wxFileDropTarget
{
public:
    DnDFile(wxListBox *pOwner) { m_pOwner = pOwner; }

    virtual bool OnDropFiles(wxCoord x, wxCoord y,
                             const wxArrayString& filenames);

private:
    wxListBox *m_pOwner;
};

// ----------------------------------------------------------------------------
// Define a custom dtop target accepting URLs
// ----------------------------------------------------------------------------

class URLDropTarget : public wxDropTarget
{
public:
    URLDropTarget() { SetDataObject(new wxURLDataObject); }

    void OnDropURL(wxCoord x, wxCoord y, const wxString& text)
    {
        // of course, a real program would do something more useful here...
        wxMessageBox(text, _T("wxDnD sample: got URL"),
                     wxICON_INFORMATION | wxOK);
    }

    // URLs can't be moved, only copied
    virtual wxDragResult OnDragOver(wxCoord WXUNUSED(x), wxCoord WXUNUSED(y),
                                    wxDragResult def)
        {
            return wxDragLink;  // At least IE 5.x needs wxDragLink, the
                                // other browsers on MSW seem okay with it too.
        }

    // translate this to calls to OnDropURL() just for convenience
    virtual wxDragResult OnData(wxCoord x, wxCoord y, wxDragResult def)
    {
        if ( !GetData() )
            return wxDragNone;

        OnDropURL(x, y, ((wxURLDataObject *)m_dataObject)->GetURL());

        return def;
    }
};

// ----------------------------------------------------------------------------
// Define a new application type
// ----------------------------------------------------------------------------

class DnDApp : public wxApp
{
public:
    virtual bool OnInit();
};

IMPLEMENT_APP(DnDApp);

// ----------------------------------------------------------------------------
// Define canvas class to show a bitmap
// ----------------------------------------------------------------------------

class DnDCanvasBitmap : public wxScrolledWindow
{
public:
    DnDCanvasBitmap(wxWindow *parent) : wxScrolledWindow(parent) { }

    void SetBitmap(const wxBitmap& bitmap)
    {
        m_bitmap = bitmap;

        SetScrollbars(10, 10,
                      m_bitmap.GetWidth() / 10, m_bitmap.GetHeight() / 10);

        Refresh();
    }

    void OnPaint(wxPaintEvent& event)
    {
        wxPaintDC dc(this);

        if ( m_bitmap.Ok() )
        {
            PrepareDC(dc);

            dc.DrawBitmap(m_bitmap, 0, 0);
        }
    }

private:
    wxBitmap m_bitmap;

    DECLARE_EVENT_TABLE()
};

#ifdef USE_METAFILES

// and the same thing fo metafiles
class DnDCanvasMetafile : public wxScrolledWindow
{
public:
    DnDCanvasMetafile(wxWindow *parent) : wxScrolledWindow(parent) { }

    void SetMetafile(const wxMetafile& metafile)
    {
        m_metafile = metafile;

        SetScrollbars(10, 10,
                      m_metafile.GetWidth() / 10, m_metafile.GetHeight() / 10);

        Refresh();
    }

    void OnPaint(wxPaintEvent& event)
    {
        wxPaintDC dc(this);

        if ( m_metafile.Ok() )
        {
            PrepareDC(dc);

            m_metafile.Play(&dc);
        }
    }

private:
    wxMetafile m_metafile;

    DECLARE_EVENT_TABLE()
};

#endif // USE_METAFILES

// ----------------------------------------------------------------------------
// Define a new frame type for the main frame
// ----------------------------------------------------------------------------

class DnDFrame : public wxFrame
{
public:
    DnDFrame(wxFrame *frame, char *title, int x, int y, int w, int h);
    ~DnDFrame();

    void OnPaint(wxPaintEvent& event);
    void OnSize(wxSizeEvent& event);
    void OnQuit (wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
    void OnDrag (wxCommandEvent& event);
    void OnNewFrame(wxCommandEvent& event);
    void OnHelp (wxCommandEvent& event);
    void OnLogClear(wxCommandEvent& event);

    void OnCopy(wxCommandEvent& event);
    void OnPaste(wxCommandEvent& event);

    void OnCopyBitmap(wxCommandEvent& event);
    void OnPasteBitmap(wxCommandEvent& event);

#ifdef USE_METAFILES
    void OnPasteMetafile(wxCommandEvent& event);
#endif // USE_METAFILES

    void OnCopyFiles(wxCommandEvent& event);

    void OnLeftDown(wxMouseEvent& event);
    void OnRightDown(wxMouseEvent& event);

    void OnUpdateUIPasteText(wxUpdateUIEvent& event);
    void OnUpdateUIPasteBitmap(wxUpdateUIEvent& event);

    DECLARE_EVENT_TABLE()

private:
    wxListBox  *m_ctrlFile,
               *m_ctrlText;
    wxTextCtrl *m_ctrlLog;

    wxLog *m_pLog,
          *m_pLogPrev;

    wxString  m_strText;
};

// ----------------------------------------------------------------------------
// A shape is an example of application-specific data which may be transported
// via drag-and-drop or clipboard: in our case, we have different geometric
// shapes, each one with its own colour and position
// ----------------------------------------------------------------------------

class DnDShape
{
public:
    enum Kind
    {
        None,
        Triangle,
        Rectangle,
        Ellipse
    };

    DnDShape(const wxPoint& pos,
             const wxSize& size,
             const wxColour& col)
        : m_pos(pos), m_size(size), m_col(col)
    {
    }

    // this is for debugging - lets us see when exactly an object is freed
    // (this may be later than you think if it's on the clipboard, for example)
    virtual ~DnDShape() { }

    // the functions used for drag-and-drop: they dump and restore a shape into
    // some bitwise-copiable data (might use streams too...)
    // ------------------------------------------------------------------------

    // restore from buffer
    static DnDShape *New(const void *buf);

    virtual size_t GetDataSize() const
    {
        return sizeof(ShapeDump);
    }

    virtual void GetDataHere(void *buf) const
    {
        ShapeDump& dump = *(ShapeDump *)buf;
        dump.x = m_pos.x;
        dump.y = m_pos.y;
        dump.w = m_size.x;
        dump.h = m_size.y;
        dump.r = m_col.Red();
        dump.g = m_col.Green();
        dump.b = m_col.Blue();
        dump.k = GetKind();
    }

    // accessors
    const wxPoint& GetPosition() const { return m_pos; }
    const wxColour& GetColour() const { return m_col; }
    const wxSize& GetSize() const { return m_size; }

    void Move(const wxPoint& pos) { m_pos = pos; }

    // to implement in derived classes
    virtual Kind GetKind() const = 0;

    virtual void Draw(wxDC& dc)
    {
        dc.SetPen(wxPen(m_col, 1, wxSOLID));
    }

protected:
    //get a point 1 up and 1 left, otherwise the mid-point of a triangle is on the line
    wxPoint GetCentre() const
         { return wxPoint(m_pos.x + m_size.x / 2 - 1, m_pos.y + m_size.y / 2 - 1); }
         
    struct ShapeDump
    {
        int x, y,       // position
            w, h,       // size
            r, g, b,    // colour
            k;          // kind
    };

    wxPoint  m_pos;
    wxSize   m_size;
    wxColour m_col;
};

class DnDTriangularShape : public DnDShape
{
public:
    DnDTriangularShape(const wxPoint& pos,
                       const wxSize& size,
                       const wxColour& col)
        : DnDShape(pos, size, col)
    {
        wxLogMessage(wxT("DnDTriangularShape is being created"));
    }

    virtual ~DnDTriangularShape()
    {
        wxLogMessage(wxT("DnDTriangularShape is being deleted"));
    }

    virtual Kind GetKind() const { return Triangle; }
    virtual void Draw(wxDC& dc)
    {
        DnDShape::Draw(dc);

        // well, it's a bit difficult to describe a triangle by position and
        // size, but we're not doing geometry here, do we? ;-)
        wxPoint p1(m_pos);
        wxPoint p2(m_pos.x + m_size.x, m_pos.y);
        wxPoint p3(m_pos.x, m_pos.y + m_size.y);

        dc.DrawLine(p1, p2);
        dc.DrawLine(p2, p3);
        dc.DrawLine(p3, p1);

        //works in multicolor modes; on GTK (at least) will fail in 16-bit color
        dc.SetBrush(wxBrush(m_col, wxSOLID));       
        dc.FloodFill(GetCentre(), m_col, wxFLOOD_BORDER);
    }
};

class DnDRectangularShape : public DnDShape
{
public:
    DnDRectangularShape(const wxPoint& pos,
                        const wxSize& size,
                        const wxColour& col)
        : DnDShape(pos, size, col)
    {
        wxLogMessage(wxT("DnDRectangularShape is being created"));
    }

    virtual ~DnDRectangularShape()
    {
        wxLogMessage(wxT("DnDRectangularShape is being deleted"));
    }

    virtual Kind GetKind() const { return Rectangle; }
    virtual void Draw(wxDC& dc)
    {
        DnDShape::Draw(dc);

        wxPoint p1(m_pos);
        wxPoint p2(p1.x + m_size.x, p1.y);
        wxPoint p3(p2.x, p2.y + m_size.y);
        wxPoint p4(p1.x, p3.y);

        dc.DrawLine(p1, p2);
        dc.DrawLine(p2, p3);
        dc.DrawLine(p3, p4);
        dc.DrawLine(p4, p1);

        dc.SetBrush(wxBrush(m_col, wxSOLID));
        dc.FloodFill(GetCentre(), m_col, wxFLOOD_BORDER);
    }
};

class DnDEllipticShape : public DnDShape
{
public:
    DnDEllipticShape(const wxPoint& pos,
                     const wxSize& size,
                     const wxColour& col)
        : DnDShape(pos, size, col)
    {
        wxLogMessage(wxT("DnDEllipticShape is being created"));
    }

    virtual ~DnDEllipticShape()
    {
        wxLogMessage(wxT("DnDEllipticShape is being deleted"));
    }

    virtual Kind GetKind() const { return Ellipse; }
    virtual void Draw(wxDC& dc)
    {
        DnDShape::Draw(dc);

        dc.DrawEllipse(m_pos, m_size);

        dc.SetBrush(wxBrush(m_col, wxSOLID));
        dc.FloodFill(GetCentre(), m_col, wxFLOOD_BORDER);
    }
};

// ----------------------------------------------------------------------------
// A wxDataObject specialisation for the application-specific data
// ----------------------------------------------------------------------------

static const wxChar *shapeFormatId = wxT("wxShape");

class DnDShapeDataObject : public wxDataObject
{
public:
    // ctor doesn't copy the pointer, so it shouldn't go away while this object
    // is alive
    DnDShapeDataObject(DnDShape *shape = (DnDShape *)NULL)
    {
        if ( shape )
        {
            // we need to copy the shape because the one we're handled may be
            // deleted while it's still on the clipboard (for example) - and we
            // reuse the serialisation methods here to copy it
            void *buf = malloc(shape->DnDShape::GetDataSize());
            shape->GetDataHere(buf);
            m_shape = DnDShape::New(buf);

            free(buf);
        }
        else
        {
            // nothing to copy
            m_shape = NULL;
        }

        // this string should uniquely identify our format, but is otherwise
        // arbitrary
        m_formatShape.SetId(shapeFormatId);

        // we don't draw the shape to a bitmap until it's really needed (i.e.
        // we're asked to do so)
        m_hasBitmap = FALSE;
#ifdef USE_METAFILES
        m_hasMetaFile = FALSE;
#endif // Windows
    }

    virtual ~DnDShapeDataObject() { delete m_shape; }

    // after a call to this function, the shape is owned by the caller and it
    // is responsible for deleting it!
    //
    // NB: a better solution would be to make DnDShapes ref counted and this
    //     is what should probably be done in a real life program, otherwise
    //     the ownership problems become too complicated really fast
    DnDShape *GetShape()
    {
        DnDShape *shape = m_shape;

        m_shape = (DnDShape *)NULL;
        m_hasBitmap = FALSE;
#ifdef USE_METAFILES
        m_hasMetaFile = FALSE;
#endif // Windows

        return shape;
    }

    // implement base class pure virtuals
    // ----------------------------------

    virtual wxDataFormat GetPreferredFormat(Direction WXUNUSED(dir)) const
    {
        return m_formatShape;
    }

    virtual size_t GetFormatCount(Direction dir) const
    {
        // our custom format is supported by both GetData() and SetData()
        size_t nFormats = 1;
        if ( dir == Get )
        {
            // but the bitmap format(s) are only supported for output
            nFormats += m_dobjBitmap.GetFormatCount(dir);

#ifdef USE_METAFILES
            nFormats += m_dobjMetaFile.GetFormatCount(dir);
#endif // Windows
        }

        return nFormats;
    }

    virtual void GetAllFormats(wxDataFormat *formats, Direction dir) const
    {
        formats[0] = m_formatShape;
        if ( dir == Get )
        {
            // in Get direction we additionally support bitmaps and metafiles
            // under Windows
            m_dobjBitmap.GetAllFormats(&formats[1], dir);

#ifdef USE_METAFILES
            // don't assume that m_dobjBitmap has only 1 format
            m_dobjMetaFile.GetAllFormats(&formats[1 +
                    m_dobjBitmap.GetFormatCount(dir)], dir);
#endif // Windows
        }
    }

    virtual size_t GetDataSize(const wxDataFormat& format) const
    {
        if ( format == m_formatShape )
        {
            return m_shape->GetDataSize();
        }
#ifdef USE_METAFILES
        else if ( m_dobjMetaFile.IsSupported(format) )
        {
            if ( !m_hasMetaFile )
                CreateMetaFile();

            return m_dobjMetaFile.GetDataSize(format);
        }
#endif // Windows
        else
        {
            wxASSERT_MSG( m_dobjBitmap.IsSupported(format),
                          wxT("unexpected format") );

            if ( !m_hasBitmap )
                CreateBitmap();

            return m_dobjBitmap.GetDataSize();
        }
    }

    virtual bool GetDataHere(const wxDataFormat& format, void *pBuf) const
    {
        if ( format == m_formatShape )
        {
            m_shape->GetDataHere(pBuf);

            return TRUE;
        }
#ifdef USE_METAFILES
        else if ( m_dobjMetaFile.IsSupported(format) )
        {
            if ( !m_hasMetaFile )
                CreateMetaFile();

            return m_dobjMetaFile.GetDataHere(format, pBuf);
        }
#endif // Windows
        else
        {
            wxASSERT_MSG( m_dobjBitmap.IsSupported(format),
                          wxT("unexpected format") );

            if ( !m_hasBitmap )
                CreateBitmap();

            return m_dobjBitmap.GetDataHere(pBuf);
        }
    }

    virtual bool SetData(const wxDataFormat& format,
                         size_t len, const void *buf)
    {
        wxCHECK_MSG( format == m_formatShape, FALSE,
                     wxT( "unsupported format") );

        delete m_shape;
        m_shape = DnDShape::New(buf);

        // the shape has changed
        m_hasBitmap = FALSE;

#ifdef USE_METAFILES
        m_hasMetaFile = FALSE;
#endif // Windows

        return TRUE;
    }

private:
    // creates a bitmap and assigns it to m_dobjBitmap (also sets m_hasBitmap)
    void CreateBitmap() const;
#ifdef USE_METAFILES
    void CreateMetaFile() const;
#endif // Windows

    wxDataFormat        m_formatShape;  // our custom format

    wxBitmapDataObject  m_dobjBitmap;   // it handles bitmaps
    bool                m_hasBitmap;    // true if m_dobjBitmap has valid bitmap

#ifdef USE_METAFILES
    wxMetaFileDataObject m_dobjMetaFile;// handles metafiles
    bool                 m_hasMetaFile; // true if we have valid metafile
#endif // Windows

    DnDShape           *m_shape;        // our data
};

// ----------------------------------------------------------------------------
// A dialog to edit shape properties
// ----------------------------------------------------------------------------

class DnDShapeDialog : public wxDialog
{
public:
    DnDShapeDialog(wxFrame *parent, DnDShape *shape);

    DnDShape *GetShape() const;

    virtual bool TransferDataToWindow();
    virtual bool TransferDataFromWindow();

    void OnColour(wxCommandEvent& event);

private:
    // input
    DnDShape *m_shape;

    // output
    DnDShape::Kind m_shapeKind;
    wxPoint  m_pos;
    wxSize   m_size;
    wxColour m_col;

    // controls
    wxRadioBox *m_radio;
    wxTextCtrl *m_textX,
               *m_textY,
               *m_textW,
               *m_textH;

    DECLARE_EVENT_TABLE()
};

// ----------------------------------------------------------------------------
// A frame for the shapes which can be drag-and-dropped between frames
// ----------------------------------------------------------------------------

class DnDShapeFrame : public wxFrame
{
public:
    DnDShapeFrame(wxFrame *parent);
    ~DnDShapeFrame();

    void SetShape(DnDShape *shape);

    // callbacks
    void OnNewShape(wxCommandEvent& event);
    void OnEditShape(wxCommandEvent& event);
    void OnClearShape(wxCommandEvent& event);

    void OnCopyShape(wxCommandEvent& event);
    void OnPasteShape(wxCommandEvent& event);

    void OnUpdateUICopy(wxUpdateUIEvent& event);
    void OnUpdateUIPaste(wxUpdateUIEvent& event);

    void OnDrag(wxMouseEvent& event);
    void OnPaint(wxPaintEvent& event);
    void OnDrop(wxCoord x, wxCoord y, DnDShape *shape);

private:
    DnDShape *m_shape;

    static DnDShapeFrame *ms_lastDropTarget;

    DECLARE_EVENT_TABLE()
};

// ----------------------------------------------------------------------------
// wxDropTarget derivation for DnDShapes
// ----------------------------------------------------------------------------

class DnDShapeDropTarget : public wxDropTarget
{
public:
    DnDShapeDropTarget(DnDShapeFrame *frame)
        : wxDropTarget(new DnDShapeDataObject)
    {
        m_frame = frame;
    }

    // override base class (pure) virtuals
    virtual wxDragResult OnEnter(wxCoord x, wxCoord y, wxDragResult def)
        { m_frame->SetStatusText("Mouse entered the frame"); return OnDragOver(x, y, def); }
    virtual void OnLeave()
        { m_frame->SetStatusText("Mouse left the frame"); }
    virtual wxDragResult OnData(wxCoord x, wxCoord y, wxDragResult def)
    {
        if ( !GetData() )
        {
            wxLogError(wxT("Failed to get drag and drop data"));

            return wxDragNone;
        }

        m_frame->OnDrop(x, y,
                        ((DnDShapeDataObject *)GetDataObject())->GetShape());

        return def;
    }

private:
    DnDShapeFrame *m_frame;
};

// ----------------------------------------------------------------------------
// functions prototypes
// ----------------------------------------------------------------------------

static void ShowBitmap(const wxBitmap& bitmap);

#ifdef USE_METAFILES
static void ShowMetaFile(const wxMetaFile& metafile);
#endif // USE_METAFILES

// ----------------------------------------------------------------------------
// IDs for the menu commands
// ----------------------------------------------------------------------------

enum
{
    Menu_Quit = 1,
    Menu_Drag,
    Menu_NewFrame,
    Menu_About = 101,
    Menu_Help,
    Menu_Clear,
    Menu_Copy,
    Menu_Paste,
    Menu_CopyBitmap,
    Menu_PasteBitmap,
    Menu_PasteMFile,
    Menu_CopyFiles,
    Menu_Shape_New = 500,
    Menu_Shape_Edit,
    Menu_Shape_Clear,
    Menu_ShapeClipboard_Copy,
    Menu_ShapeClipboard_Paste,
    Button_Colour = 1001
};

BEGIN_EVENT_TABLE(DnDFrame, wxFrame)
    EVT_MENU(Menu_Quit,       DnDFrame::OnQuit)
    EVT_MENU(Menu_About,      DnDFrame::OnAbout)
    EVT_MENU(Menu_Drag,       DnDFrame::OnDrag)
    EVT_MENU(Menu_NewFrame,   DnDFrame::OnNewFrame)
    EVT_MENU(Menu_Help,       DnDFrame::OnHelp)
    EVT_MENU(Menu_Clear,      DnDFrame::OnLogClear)
    EVT_MENU(Menu_Copy,       DnDFrame::OnCopy)
    EVT_MENU(Menu_Paste,      DnDFrame::OnPaste)
    EVT_MENU(Menu_CopyBitmap, DnDFrame::OnCopyBitmap)
    EVT_MENU(Menu_PasteBitmap,DnDFrame::OnPasteBitmap)
#ifdef USE_METAFILES
    EVT_MENU(Menu_PasteMFile, DnDFrame::OnPasteMetafile)
#endif // USE_METAFILES
    EVT_MENU(Menu_CopyFiles,  DnDFrame::OnCopyFiles)

    EVT_UPDATE_UI(Menu_Paste,       DnDFrame::OnUpdateUIPasteText)
    EVT_UPDATE_UI(Menu_PasteBitmap, DnDFrame::OnUpdateUIPasteBitmap)

    EVT_LEFT_DOWN(            DnDFrame::OnLeftDown)
    EVT_RIGHT_DOWN(           DnDFrame::OnRightDown)
    EVT_PAINT(                DnDFrame::OnPaint)
    EVT_SIZE(                 DnDFrame::OnSize)
END_EVENT_TABLE()

BEGIN_EVENT_TABLE(DnDShapeFrame, wxFrame)
    EVT_MENU(Menu_Shape_New,    DnDShapeFrame::OnNewShape)
    EVT_MENU(Menu_Shape_Edit,   DnDShapeFrame::OnEditShape)
    EVT_MENU(Menu_Shape_Clear,  DnDShapeFrame::OnClearShape)

    EVT_MENU(Menu_ShapeClipboard_Copy,  DnDShapeFrame::OnCopyShape)
    EVT_MENU(Menu_ShapeClipboard_Paste, DnDShapeFrame::OnPasteShape)

    EVT_UPDATE_UI(Menu_ShapeClipboard_Copy,  DnDShapeFrame::OnUpdateUICopy)
    EVT_UPDATE_UI(Menu_ShapeClipboard_Paste, DnDShapeFrame::OnUpdateUIPaste)

    EVT_LEFT_DOWN(DnDShapeFrame::OnDrag)

    EVT_PAINT(DnDShapeFrame::OnPaint)
END_EVENT_TABLE()

BEGIN_EVENT_TABLE(DnDShapeDialog, wxDialog)
    EVT_BUTTON(Button_Colour, DnDShapeDialog::OnColour)
END_EVENT_TABLE()

BEGIN_EVENT_TABLE(DnDCanvasBitmap, wxScrolledWindow)
    EVT_PAINT(DnDCanvasBitmap::OnPaint)
END_EVENT_TABLE()

#ifdef USE_METAFILES
BEGIN_EVENT_TABLE(DnDCanvasMetafile, wxScrolledWindow)
    EVT_PAINT(DnDCanvasMetafile::OnPaint)
END_EVENT_TABLE()
#endif // USE_METAFILES

// ============================================================================
// implementation
// ============================================================================

// `Main program' equivalent, creating windows and returning main app frame
bool DnDApp::OnInit()
{
#if USE_RESOURCES
    // load our ressources
    wxPathList pathList;
    pathList.Add(".");
#ifdef __WXMSW__
    pathList.Add("./Debug");
    pathList.Add("./Release");
#endif // wxMSW

    wxString path = pathList.FindValidPath("dnd.wxr");
    if ( !path )
    {
        wxLogError(wxT("Can't find the resource file dnd.wxr in the current ")
                   wxT("directory, aborting."));

        return FALSE;
    }

    wxDefaultResourceTable->ParseResourceFile(path);
#endif

    // switch on trace messages
#if defined(__WXGTK__)
    wxLog::AddTraceMask(_T("clipboard"));
#elif defined(__WXMSW__)
    wxLog::AddTraceMask(wxTRACE_OleCalls);
#endif

#if wxUSE_LIBPNG
    wxImage::AddHandler( new wxPNGHandler );
#endif

    // under X we usually want to use the primary selection by default (which
    // is shared with other apps)
    wxTheClipboard->UsePrimarySelection();

    // create the main frame window
    DnDFrame *frame = new DnDFrame((wxFrame  *) NULL,
                                   "Drag-and-Drop/Clipboard wxWindows Sample",
                                   10, 100, 650, 340);

    // activate it
    frame->Show(TRUE);

    SetTopWindow(frame);

    return TRUE;
}

DnDFrame::DnDFrame(wxFrame *frame, char *title, int x, int y, int w, int h)
        : wxFrame(frame, -1, title, wxPoint(x, y), wxSize(w, h)),
          m_strText("wxWindows drag & drop works :-)")

{
    // frame icon and status bar
    SetIcon(wxICON(mondrian));

    CreateStatusBar();

    // construct menu
    wxMenu *file_menu = new wxMenu;
    file_menu->Append(Menu_Drag, "&Test drag...");
    file_menu->AppendSeparator();
    file_menu->Append(Menu_NewFrame, "&New frame\tCtrl-N");
    file_menu->AppendSeparator();
    file_menu->Append(Menu_Quit, "E&xit\tCtrl-Q");

    wxMenu *log_menu = new wxMenu;
    log_menu->Append(Menu_Clear, "Clear\tCtrl-L");

    wxMenu *help_menu = new wxMenu;
    help_menu->Append(Menu_Help, "&Help...");
    help_menu->AppendSeparator();
    help_menu->Append(Menu_About, "&About");

    wxMenu *clip_menu = new wxMenu;
    clip_menu->Append(Menu_Copy, "&Copy text\tCtrl-C");
    clip_menu->Append(Menu_Paste, "&Paste text\tCtrl-V");
    clip_menu->AppendSeparator();
    clip_menu->Append(Menu_CopyBitmap, "Copy &bitmap\tCtrl-Shift-C");
    clip_menu->Append(Menu_PasteBitmap, "Paste b&itmap\tCtrl-Shift-V");
#ifdef USE_METAFILES
    clip_menu->AppendSeparator();
    clip_menu->Append(Menu_PasteMFile, "Paste &metafile\tCtrl-M");
#endif // USE_METAFILES
    clip_menu->AppendSeparator();
    clip_menu->Append(Menu_CopyFiles, "Copy &files\tCtrl-F");

    wxMenuBar *menu_bar = new wxMenuBar;
    menu_bar->Append(file_menu, "&File");
    menu_bar->Append(log_menu,  "&Log");
    menu_bar->Append(clip_menu, "&Clipboard");
    menu_bar->Append(help_menu, "&Help");

    SetMenuBar(menu_bar);

    // make a panel with 3 subwindows
    wxPoint pos(0, 0);
    wxSize  size(400, 200);

    wxString strFile("Drop files here!"), strText("Drop text on me");

    m_ctrlFile  = new wxListBox(this, -1, pos, size, 1, &strFile,
                                wxLB_HSCROLL | wxLB_ALWAYS_SB );
    m_ctrlText  = new wxListBox(this, -1, pos, size, 1, &strText,
                                wxLB_HSCROLL | wxLB_ALWAYS_SB );

    m_ctrlLog   = new wxTextCtrl(this, -1, "", pos, size,
                                 wxTE_MULTILINE | wxTE_READONLY |
                                 wxSUNKEN_BORDER );

    // redirect log messages to the text window
    m_pLog = new wxLogTextCtrl(m_ctrlLog);
    m_pLogPrev = wxLog::SetActiveTarget(m_pLog);

    // associate drop targets with the controls
    m_ctrlFile->SetDropTarget(new DnDFile(m_ctrlFile));
    m_ctrlText->SetDropTarget(new DnDText(m_ctrlText));
    m_ctrlLog->SetDropTarget(new URLDropTarget);

    wxLayoutConstraints *c;

    // Top-left listbox
    c = new wxLayoutConstraints;
    c->left.SameAs(this, wxLeft);
    c->top.SameAs(this, wxTop);
    c->right.PercentOf(this, wxRight, 50);
    c->height.PercentOf(this, wxHeight, 30);
    m_ctrlFile->SetConstraints(c);

    // Top-right listbox
    c = new wxLayoutConstraints;
    c->left.SameAs    (m_ctrlFile, wxRight);
    c->top.SameAs     (this, wxTop);
    c->right.SameAs   (this, wxRight);
    c->height.PercentOf(this, wxHeight, 30);
    m_ctrlText->SetConstraints(c);

    // Lower text control
    c = new wxLayoutConstraints;
    c->left.SameAs    (this, wxLeft);
    c->right.SameAs   (this, wxRight);
    c->height.PercentOf(this, wxHeight, 50);
    c->top.SameAs(m_ctrlText, wxBottom);
    m_ctrlLog->SetConstraints(c);

    SetAutoLayout(TRUE);
}

void DnDFrame::OnQuit(wxCommandEvent& WXUNUSED(event))
{
    Close(TRUE);
}

void DnDFrame::OnSize(wxSizeEvent& event)
{
    Refresh();

    event.Skip();
}

void DnDFrame::OnPaint(wxPaintEvent& WXUNUSED(event))
{
    int w = 0;
    int h = 0;
    GetClientSize( &w, &h );

    wxPaintDC dc(this);
    // dc.Clear(); -- this kills wxGTK
    dc.SetFont( wxFont( 24, wxDECORATIVE, wxNORMAL, wxNORMAL, FALSE, "charter" ) );
    dc.DrawText( "Drag text from here!", 100, h-50 );
}

void DnDFrame::OnUpdateUIPasteText(wxUpdateUIEvent& event)
{
#ifdef __WXDEBUG__
    // too many trace messages if we don't do it - this function is called
    // very often
    wxLogNull nolog;
#endif

    event.Enable( wxTheClipboard->IsSupported(wxDF_TEXT) );
}

void DnDFrame::OnUpdateUIPasteBitmap(wxUpdateUIEvent& event)
{
#ifdef __WXDEBUG__
    // too many trace messages if we don't do it - this function is called
    // very often
    wxLogNull nolog;
#endif

    event.Enable( wxTheClipboard->IsSupported(wxDF_BITMAP) );
}

void DnDFrame::OnNewFrame(wxCommandEvent& WXUNUSED(event))
{
    (new DnDShapeFrame(this))->Show(TRUE);

    wxLogStatus(this, wxT("Double click the new frame to select a shape for it"));
}

void DnDFrame::OnDrag(wxCommandEvent& WXUNUSED(event))
{
    wxString strText = wxGetTextFromUser
        (
         "After you enter text in this dialog, press any mouse\n"
         "button in the bottom (empty) part of the frame and \n"
         "drag it anywhere - you will be in fact dragging the\n"
         "text object containing this text",
         "Please enter some text", m_strText, this
        );

    m_strText = strText;
}

void DnDFrame::OnAbout(wxCommandEvent& WXUNUSED(event))
{
    wxMessageBox("Drag-&-Drop Demo\n"
                 "Please see \"Help|Help...\" for details\n"
                 "Copyright (c) 1998 Vadim Zeitlin",
                 "About wxDnD",
                 wxICON_INFORMATION | wxOK,
                 this);
}

void DnDFrame::OnHelp(wxCommandEvent& /* event */)
{
    wxMessageDialog dialog(this,
            "This small program demonstrates drag & drop support in wxWindows. The program window\n"
            "consists of 3 parts: the bottom pane is for debug messages, so that you can see what's\n"
            "going on inside. The top part is split into 2 listboxes, the left one accepts files\n"
            "and the right one accepts text.\n"
            "\n"
            "To test wxDropTarget: open wordpad (write.exe), select some text in it and drag it to\n"
            "the right listbox (you'll notice the usual visual feedback, i.e. the cursor will change).\n"
            "Also, try dragging some files (you can select several at once) from Windows Explorer (or \n"
            "File Manager) to the left pane. Hold down Ctrl/Shift keys when you drop text (doesn't \n"
            "work with files) and see what changes.\n"
            "\n"
            "To test wxDropSource: just press any mouse button on the empty zone of the window and drag\n"
            "it to wordpad or any other droptarget accepting text (and of course you can just drag it\n"
            "to the right pane). Due to a lot of trace messages, the cursor might take some time to \n"
            "change, don't release the mouse button until it does. You can change the string being\n"
            "dragged in in \"File|Test drag...\" dialog.\n"
            "\n"
            "\n"
            "Please send all questions/bug reports/suggestions &c to \n"
            "Vadim Zeitlin <zeitlin@dptmaths.ens-cachan.fr>",
            "wxDnD Help");

    dialog.ShowModal();
}

void DnDFrame::OnLogClear(wxCommandEvent& /* event */ )
{
    m_ctrlLog->Clear();
    m_ctrlText->Clear();
    m_ctrlFile->Clear();
}

void DnDFrame::OnLeftDown(wxMouseEvent &WXUNUSED(event) )
{
    if ( !m_strText.IsEmpty() )
    {
        // start drag operation
        wxTextDataObject textData(m_strText);
/*
        wxFileDataObject textData;
        textData.AddFile( "/file1.txt" );
        textData.AddFile( "/file2.txt" );
*/
        wxDropSource source(textData, this,
                            wxDROP_ICON(dnd_copy),
                            wxDROP_ICON(dnd_move),
                            wxDROP_ICON(dnd_none));

        const char *pc;

        switch ( source.DoDragDrop(TRUE) )
        {
            case wxDragError:   pc = "Error!";    break;
            case wxDragNone:    pc = "Nothing";   break;
            case wxDragCopy:    pc = "Copied";    break;
            case wxDragMove:    pc = "Moved";     break;
            case wxDragCancel:  pc = "Cancelled"; break;
            default:            pc = "Huh?";      break;
        }

        SetStatusText(wxString("Drag result: ") + pc);
    }
}

void DnDFrame::OnRightDown(wxMouseEvent &event )
{
    wxMenu menu("Dnd sample menu");

    menu.Append(Menu_Drag, "&Test drag...");
    menu.AppendSeparator();
    menu.Append(Menu_About, "&About");

    PopupMenu( &menu, event.GetX(), event.GetY() );
}

DnDFrame::~DnDFrame()
{
    if ( m_pLog != NULL ) {
        if ( wxLog::SetActiveTarget(m_pLogPrev) == m_pLog )
            delete m_pLog;
    }
}

// ---------------------------------------------------------------------------
// bitmap clipboard
// ---------------------------------------------------------------------------

void DnDFrame::OnCopyBitmap(wxCommandEvent& WXUNUSED(event))
{
    // PNG support is not always compiled in under Windows, so use BMP there
#ifdef __WXMSW__
    wxFileDialog dialog(this, "Open a BMP file", "", "", "BMP files (*.bmp)|*.bmp", 0);
#else
    wxFileDialog dialog(this, "Open a PNG file", "", "", "PNG files (*.png)|*.png", 0);
#endif

    if (dialog.ShowModal() != wxID_OK)
    {
        wxLogMessage( _T("Aborted file open") );
        return;
    }

    if (dialog.GetPath().IsEmpty())
    {
        wxLogMessage( _T("Returned empty string.") );
        return;
    }

    if (!wxFileExists(dialog.GetPath()))
    {
        wxLogMessage( _T("File doesn't exist.") );
        return;
    }

    wxImage image;
    image.LoadFile( dialog.GetPath(),
#ifdef __WXMSW__
                    wxBITMAP_TYPE_BMP
#else
                    wxBITMAP_TYPE_PNG
#endif
                  );
    if (!image.Ok())
    {
        wxLogError( _T("Invalid image file...") );
        return;
    }

    wxLogStatus( _T("Decoding image file...") );
    wxYield();

    wxBitmap bitmap( image );

    if ( !wxTheClipboard->Open() )
    {
        wxLogError(_T("Can't open clipboard."));

        return;
    }

    wxLogMessage( _T("Creating wxBitmapDataObject...") );
    wxYield();

    if ( !wxTheClipboard->AddData(new wxBitmapDataObject(bitmap)) )
    {
        wxLogError(_T("Can't copy image to the clipboard."));
    }
    else
    {
        wxLogMessage(_T("Image has been put on the clipboard.") );
        wxLogMessage(_T("You can paste it now and look at it.") );
    }

    wxTheClipboard->Close();
}

void DnDFrame::OnPasteBitmap(wxCommandEvent& WXUNUSED(event))
{
    if ( !wxTheClipboard->Open() )
    {
        wxLogError(_T("Can't open clipboard."));

        return;
    }

    if ( !wxTheClipboard->IsSupported(wxDF_BITMAP) )
    {
        wxLogWarning(_T("No bitmap on clipboard"));

        wxTheClipboard->Close();
        return;
    }

    wxBitmapDataObject data;
    if ( !wxTheClipboard->GetData(data) )
    {
        wxLogError(_T("Can't paste bitmap from the clipboard"));
    }
    else
    {
        const wxBitmap& bmp = data.GetBitmap();

        wxLogMessage(_T("Bitmap %dx%d pasted from the clipboard"),
                     bmp.GetWidth(), bmp.GetHeight());
        ShowBitmap(bmp);
    }

    wxTheClipboard->Close();
}

#ifdef USE_METAFILES

void DnDFrame::OnPasteMetafile(wxCommandEvent& WXUNUSED(event))
{
    if ( !wxTheClipboard->Open() )
    {
        wxLogError(_T("Can't open clipboard."));

        return;
    }

    if ( !wxTheClipboard->IsSupported(wxDF_METAFILE) )
    {
        wxLogWarning(_T("No metafile on clipboard"));
    }
    else
    {
        wxMetaFileDataObject data;
        if ( !wxTheClipboard->GetData(data) )
        {
            wxLogError(_T("Can't paste metafile from the clipboard"));
        }
        else
        {
            const wxMetaFile& mf = data.GetMetafile();

            wxLogMessage(_T("Metafile %dx%d pasted from the clipboard"),
                         mf.GetWidth(), mf.GetHeight());

            ShowMetaFile(mf);
        }
    }

    wxTheClipboard->Close();
}

#endif // USE_METAFILES

// ----------------------------------------------------------------------------
// file clipboard
// ----------------------------------------------------------------------------

void DnDFrame::OnCopyFiles(wxCommandEvent& WXUNUSED(event))
{
#ifdef __WXMSW__
    wxFileDialog dialog(this, "Select a file to copy", "", "",
                         "All files (*.*)|*.*", 0);

    wxArrayString filenames;
    while ( dialog.ShowModal() == wxID_OK )
    {
        filenames.Add(dialog.GetPath());
    }

    if ( !filenames.IsEmpty() )
    {
        wxFileDataObject *dobj = new wxFileDataObject;
        size_t count = filenames.GetCount();
        for ( size_t n = 0; n < count; n++ )
        {
            dobj->AddFile(filenames[n]);
        }

        wxClipboardLocker locker;
        if ( !locker )
        {
            wxLogError(wxT("Can't open clipboard"));
        }
        else
        {
            if ( !wxTheClipboard->AddData(dobj) )
            {
                wxLogError(wxT("Can't copy file(s) to the clipboard"));
            }
            else
            {
                wxLogStatus(this, wxT("%d file%s copied to the clipboard"),
                            count, count == 1 ? wxT("") : wxT("s"));
            }
        }
    }
    else
    {
        wxLogStatus(this, wxT("Aborted"));
    }
#else // !MSW
    wxLogError(wxT("Sorry, not implemented"));
#endif // MSW/!MSW
}

// ---------------------------------------------------------------------------
// text clipboard
// ---------------------------------------------------------------------------

void DnDFrame::OnCopy(wxCommandEvent& WXUNUSED(event))
{
    if ( !wxTheClipboard->Open() )
    {
        wxLogError(_T("Can't open clipboard."));

        return;
    }

    if ( !wxTheClipboard->AddData(new wxTextDataObject(m_strText)) )
    {
        wxLogError(_T("Can't copy data to the clipboard"));
    }
    else
    {
        wxLogMessage(_T("Text '%s' put on the clipboard"), m_strText.c_str());
    }

    wxTheClipboard->Close();
}

void DnDFrame::OnPaste(wxCommandEvent& WXUNUSED(event))
{
    if ( !wxTheClipboard->Open() )
    {
        wxLogError(_T("Can't open clipboard."));

        return;
    }

    if ( !wxTheClipboard->IsSupported(wxDF_TEXT) )
    {
        wxLogWarning(_T("No text data on clipboard"));

        wxTheClipboard->Close();
        return;
    }

    wxTextDataObject text;
    if ( !wxTheClipboard->GetData(text) )
    {
        wxLogError(_T("Can't paste data from the clipboard"));
    }
    else
    {
        wxLogMessage(_T("Text '%s' pasted from the clipboard"),
                     text.GetText().c_str());
    }

    wxTheClipboard->Close();
}

// ----------------------------------------------------------------------------
// Notifications called by the base class
// ----------------------------------------------------------------------------

bool DnDText::OnDropText(wxCoord, wxCoord, const wxString& text)
{
    m_pOwner->Append(text);

    return TRUE;
}

bool DnDFile::OnDropFiles(wxCoord, wxCoord, const wxArrayString& filenames)
{
    size_t nFiles = filenames.GetCount();
    wxString str;
    str.Printf( _T("%d files dropped"), nFiles);
    m_pOwner->Append(str);
    for ( size_t n = 0; n < nFiles; n++ ) {
        m_pOwner->Append(filenames[n]);
    }

    return TRUE;
}

// ----------------------------------------------------------------------------
// DnDShapeDialog
// ----------------------------------------------------------------------------

DnDShapeDialog::DnDShapeDialog(wxFrame *parent, DnDShape *shape)
#if !USE_RESOURCES
  :wxDialog( parent, 6001, wxT("Choose Shape"), wxPoint( 10, 10 ),
             wxSize( 40, 40 ),
             wxRAISED_BORDER|wxCAPTION|wxTHICK_FRAME|wxSYSTEM_MENU )
#endif
{
    m_shape = shape;
#if USE_RESOURCES
    LoadFromResource(parent, "dialogShape");

    m_textX = (wxTextCtrl *)wxFindWindowByName("textX", this);
    m_textY = (wxTextCtrl *)wxFindWindowByName("textY", this);
    m_textW = (wxTextCtrl *)wxFindWindowByName("textW", this);
    m_textH = (wxTextCtrl *)wxFindWindowByName("textH", this);

    m_radio = (wxRadioBox *)wxFindWindowByName("radio", this);
#else
    wxBoxSizer* topSizer = new wxBoxSizer( wxVERTICAL );

    // radio box
    wxBoxSizer* shapesSizer = new wxBoxSizer( wxHORIZONTAL );
    const wxString choices[] = { wxT("None"), wxT("Triangle"),
                                 wxT("Rectangle"), wxT("Ellipse") };

    m_radio = new wxRadioBox( this, -1, wxT("&Shape"),
                              wxDefaultPosition, wxDefaultSize, 4, choices, 4,
                              wxRA_SPECIFY_COLS );
    shapesSizer->Add( m_radio, 0, wxGROW|wxALL, 5 );
    topSizer->Add( shapesSizer, 0, wxALL, 2 );

    // attributes
    wxStaticBox* box = new wxStaticBox( this, -1, wxT("&Attributes") );
    wxStaticBoxSizer* attrSizer = new wxStaticBoxSizer( box, wxHORIZONTAL );
    wxFlexGridSizer* xywhSizer = new wxFlexGridSizer( 4, 2 );

    wxStaticText* st;

    st = new wxStaticText( this, -1, wxT("Position &X:") );
    m_textX = new wxTextCtrl( this, -1, wxEmptyString, wxDefaultPosition,
                              wxSize( 30, 20 ) );
    xywhSizer->Add( st, 1, wxGROW|wxALL, 2 );
    xywhSizer->Add( m_textX, 1, wxGROW|wxALL, 2 );

    st = new wxStaticText( this, -1, wxT("Size &width:") );
    m_textW = new wxTextCtrl( this, -1, wxEmptyString, wxDefaultPosition,
                              wxSize( 30, 20 ) );
    xywhSizer->Add( st, 1, wxGROW|wxALL, 2 );
    xywhSizer->Add( m_textW, 1, wxGROW|wxALL, 2 );

    st = new wxStaticText( this, -1, wxT("&Y:") );
    m_textY = new wxTextCtrl( this, -1, wxEmptyString, wxDefaultPosition,
                              wxSize( 30, 20 ) );
    xywhSizer->Add( st, 1, wxALL|wxALIGN_RIGHT, 2 );
    xywhSizer->Add( m_textY, 1, wxGROW|wxALL, 2 );

    st = new wxStaticText( this, -1, wxT("&height:") );
    m_textH = new wxTextCtrl( this, -1, wxEmptyString, wxDefaultPosition,
                              wxSize( 30, 20 ) );
    xywhSizer->Add( st, 1, wxALL|wxALIGN_RIGHT, 2 );
    xywhSizer->Add( m_textH, 1, wxGROW|wxALL, 2 );

    wxButton* col = new wxButton( this, Button_Colour, wxT("&Colour...") );
    attrSizer->Add( xywhSizer, 1, wxGROW );
    attrSizer->Add( col, 0, wxALL|wxALIGN_CENTRE_VERTICAL, 2 );
    topSizer->Add( attrSizer, 0, wxGROW|wxALL, 5 );

    // buttons
    wxBoxSizer* buttonSizer = new wxBoxSizer( wxHORIZONTAL );
    wxButton* bt;
    bt = new wxButton( this, wxID_OK, wxT("Ok") );
    buttonSizer->Add( bt, 0, wxALL, 2 );
    bt = new wxButton( this, wxID_CANCEL, wxT("Cancel") );
    buttonSizer->Add( bt, 0, wxALL, 2 );
    topSizer->Add( buttonSizer, 0, wxALL|wxALIGN_RIGHT, 2 );

    SetAutoLayout( TRUE );
    SetSizer( topSizer );
    topSizer->Fit( this );
#endif
}

DnDShape *DnDShapeDialog::GetShape() const
{
    switch ( m_shapeKind )
    {
        default:
        case DnDShape::None:      return NULL;
        case DnDShape::Triangle:  return new DnDTriangularShape(m_pos, m_size, m_col);
        case DnDShape::Rectangle: return new DnDRectangularShape(m_pos, m_size, m_col);
        case DnDShape::Ellipse:   return new DnDEllipticShape(m_pos, m_size, m_col);
    }
}

bool DnDShapeDialog::TransferDataToWindow()
{

    if ( m_shape )
    {
        m_radio->SetSelection(m_shape->GetKind());
        m_pos = m_shape->GetPosition();
        m_size = m_shape->GetSize();
        m_col = m_shape->GetColour();
    }
    else
    {
        m_radio->SetSelection(DnDShape::None);
        m_pos = wxPoint(1, 1);
        m_size = wxSize(100, 100);
    }

    m_textX->SetValue(wxString() << m_pos.x);
    m_textY->SetValue(wxString() << m_pos.y);
    m_textW->SetValue(wxString() << m_size.x);
    m_textH->SetValue(wxString() << m_size.y);

    return TRUE;
}

bool DnDShapeDialog::TransferDataFromWindow()
{
    m_shapeKind = (DnDShape::Kind)m_radio->GetSelection();

    m_pos.x = wxAtoi(m_textX->GetValue());
    m_pos.y = wxAtoi(m_textY->GetValue());
    m_size.x = wxAtoi(m_textW->GetValue());
    m_size.y = wxAtoi(m_textH->GetValue());

    if ( !m_pos.x || !m_pos.y || !m_size.x || !m_size.y )
    {
        wxMessageBox("All sizes and positions should be non null!",
                     "Invalid shape", wxICON_HAND | wxOK, this);

        return FALSE;
    }

    return TRUE;
}

void DnDShapeDialog::OnColour(wxCommandEvent& WXUNUSED(event))
{
    wxColourData data;
    data.SetChooseFull(TRUE);
    for (int i = 0; i < 16; i++)
    {
        wxColour colour(i*16, i*16, i*16);
        data.SetCustomColour(i, colour);
    }

    wxColourDialog dialog(this, &data);
    if ( dialog.ShowModal() == wxID_OK )
    {
        m_col = dialog.GetColourData().GetColour();
    }
}

// ----------------------------------------------------------------------------
// DnDShapeFrame
// ----------------------------------------------------------------------------

DnDShapeFrame *DnDShapeFrame::ms_lastDropTarget = NULL;

DnDShapeFrame::DnDShapeFrame(wxFrame *parent)
             : wxFrame(parent, -1, "Shape Frame",
                       wxDefaultPosition, wxSize(250, 150))
{
    CreateStatusBar();

    wxMenu *menuShape = new wxMenu;
    menuShape->Append(Menu_Shape_New, "&New default shape\tCtrl-S");
    menuShape->Append(Menu_Shape_Edit, "&Edit shape\tCtrl-E");
    menuShape->AppendSeparator();
    menuShape->Append(Menu_Shape_Clear, "&Clear shape\tCtrl-L");

    wxMenu *menuClipboard = new wxMenu;
    menuClipboard->Append(Menu_ShapeClipboard_Copy, "&Copy\tCtrl-C");
    menuClipboard->Append(Menu_ShapeClipboard_Paste, "&Paste\tCtrl-V");

    wxMenuBar *menubar = new wxMenuBar;
    menubar->Append(menuShape, "&Shape");
    menubar->Append(menuClipboard, "&Clipboard");

    SetMenuBar(menubar);

    SetStatusText("Press Ctrl-S to create a new shape");

    SetDropTarget(new DnDShapeDropTarget(this));

    m_shape = NULL;

    SetBackgroundColour(*wxWHITE);
}

DnDShapeFrame::~DnDShapeFrame()
{
    if (m_shape)
        delete m_shape;
}

void DnDShapeFrame::SetShape(DnDShape *shape)
{
    if (m_shape)
        delete m_shape;
    m_shape = shape;
    Refresh();
}

// callbacks
void DnDShapeFrame::OnDrag(wxMouseEvent& event)
{
    if ( !m_shape )
    {
        event.Skip();

        return;
    }

    // start drag operation
    DnDShapeDataObject shapeData(m_shape);
    wxDropSource source(shapeData, this);

    const char *pc = NULL;
    switch ( source.DoDragDrop(TRUE) )
    {
        default:
        case wxDragError:
            wxLogError(wxT("An error occured during drag and drop operation"));
            break;

        case wxDragNone:
            SetStatusText("Nothing happened");
            break;

        case wxDragCopy:
            pc = "copied";
            break;

        case wxDragMove:
            pc = "moved";
            if ( ms_lastDropTarget != this )
            {
                // don't delete the shape if we dropped it on ourselves!
                SetShape(NULL);
            }
            break;

        case wxDragCancel:
            SetStatusText("Drag and drop operation cancelled");
            break;
    }

    if ( pc )
    {
        SetStatusText(wxString("Shape successfully ") + pc);
    }
    //else: status text already set
}

void DnDShapeFrame::OnDrop(wxCoord x, wxCoord y, DnDShape *shape)
{
    ms_lastDropTarget = this;

    wxPoint pt(x, y);

    wxString s;
    s.Printf(wxT("Shape dropped at (%ld, %ld)"), pt.x, pt.y);
    SetStatusText(s);

    shape->Move(pt);
    SetShape(shape);
}

void DnDShapeFrame::OnEditShape(wxCommandEvent& event)
{
    DnDShapeDialog dlg(this, m_shape);
    if ( dlg.ShowModal() == wxID_OK )
    {
        SetShape(dlg.GetShape());

        if ( m_shape )
        {
            SetStatusText("You can now drag the shape to another frame");
        }
    }
}

void DnDShapeFrame::OnNewShape(wxCommandEvent& event)
{
    SetShape(new DnDEllipticShape(wxPoint(10, 10), wxSize(80, 60), *wxRED));

    SetStatusText("You can now drag the shape to another frame");
}

void DnDShapeFrame::OnClearShape(wxCommandEvent& event)
{
    SetShape(NULL);
}

void DnDShapeFrame::OnCopyShape(wxCommandEvent& event)
{
    if ( m_shape )
    {
        wxClipboardLocker clipLocker;
        if ( !clipLocker )
        {
            wxLogError(wxT("Can't open the clipboard"));

            return;
        }

        wxTheClipboard->AddData(new DnDShapeDataObject(m_shape));
    }
}

void DnDShapeFrame::OnPasteShape(wxCommandEvent& event)
{
    wxClipboardLocker clipLocker;
    if ( !clipLocker )
    {
        wxLogError(wxT("Can't open the clipboard"));

        return;
    }

    DnDShapeDataObject shapeDataObject(NULL);
    if ( wxTheClipboard->GetData(shapeDataObject) )
    {
        SetShape(shapeDataObject.GetShape());
    }
    else
    {
        wxLogStatus(wxT("No shape on the clipboard"));
    }
}

void DnDShapeFrame::OnUpdateUICopy(wxUpdateUIEvent& event)
{
    event.Enable( m_shape != NULL );
}

void DnDShapeFrame::OnUpdateUIPaste(wxUpdateUIEvent& event)
{
    event.Enable( wxTheClipboard->IsSupported(wxDataFormat(shapeFormatId)) );
}

void DnDShapeFrame::OnPaint(wxPaintEvent& event)
{
    if ( m_shape )
    {
        wxPaintDC dc(this);

        m_shape->Draw(dc);
    }
    else
    {
        event.Skip();
    }
}

// ----------------------------------------------------------------------------
// DnDShape
// ----------------------------------------------------------------------------

DnDShape *DnDShape::New(const void *buf)
{
    const ShapeDump& dump = *(const ShapeDump *)buf;
    switch ( dump.k )
    {
        case Triangle:
            return new DnDTriangularShape(wxPoint(dump.x, dump.y),
                                          wxSize(dump.w, dump.h),
                                          wxColour(dump.r, dump.g, dump.b));

        case Rectangle:
            return new DnDRectangularShape(wxPoint(dump.x, dump.y),
                                           wxSize(dump.w, dump.h),
                                           wxColour(dump.r, dump.g, dump.b));

        case Ellipse:
            return new DnDEllipticShape(wxPoint(dump.x, dump.y),
                                        wxSize(dump.w, dump.h),
                                        wxColour(dump.r, dump.g, dump.b));

        default:
            wxFAIL_MSG(wxT("invalid shape!"));
            return NULL;
    }
}

// ----------------------------------------------------------------------------
// DnDShapeDataObject
// ----------------------------------------------------------------------------

#ifdef USE_METAFILES

void DnDShapeDataObject::CreateMetaFile() const
{
    wxPoint pos = m_shape->GetPosition();
    wxSize size = m_shape->GetSize();

    wxMetaFileDC dcMF(wxEmptyString, pos.x + size.x, pos.y + size.y);

    m_shape->Draw(dcMF);

    wxMetafile *mf = dcMF.Close();

    DnDShapeDataObject *self = (DnDShapeDataObject *)this; // const_cast
    self->m_dobjMetaFile.SetMetafile(*mf);
    self->m_hasMetaFile = TRUE;

    delete mf;
}

#endif // Windows

void DnDShapeDataObject::CreateBitmap() const
{
    wxPoint pos = m_shape->GetPosition();
    wxSize size = m_shape->GetSize();
    int x = pos.x + size.x,
        y = pos.y + size.y;
    wxBitmap bitmap(x, y);
    wxMemoryDC dc;
    dc.SelectObject(bitmap);
    dc.SetBrush(wxBrush(wxT("white"), wxSOLID));
    dc.Clear();
    m_shape->Draw(dc);
    dc.SelectObject(wxNullBitmap);

    DnDShapeDataObject *self = (DnDShapeDataObject *)this; // const_cast
    self->m_dobjBitmap.SetBitmap(bitmap);
    self->m_hasBitmap = TRUE;
}

// ----------------------------------------------------------------------------
// global functions
// ----------------------------------------------------------------------------

static void ShowBitmap(const wxBitmap& bitmap)
{
    wxFrame *frame = new wxFrame(NULL, -1, _T("Bitmap view"));
    frame->CreateStatusBar();
    DnDCanvasBitmap *canvas = new DnDCanvasBitmap(frame);
    canvas->SetBitmap(bitmap);

    int w = bitmap.GetWidth(),
        h = bitmap.GetHeight();
    frame->SetStatusText(wxString::Format(_T("%dx%d"), w, h));

    frame->SetClientSize(w > 100 ? 100 : w, h > 100 ? 100 : h);
    frame->Show(TRUE);
}

#ifdef USE_METAFILES

static void ShowMetaFile(const wxMetaFile& metafile)
{
    wxFrame *frame = new wxFrame(NULL, -1, _T("Metafile view"));
    frame->CreateStatusBar();
    DnDCanvasMetafile *canvas = new DnDCanvasMetafile(frame);
    canvas->SetMetafile(metafile);

    wxSize size = metafile.GetSize();
    frame->SetStatusText(wxString::Format(_T("%dx%d"), size.x, size.y));

    frame->SetClientSize(size.x > 100 ? 100 : size.x,
                         size.y > 100 ? 100 : size.y);
    frame->Show();
}

#endif // USE_METAFILES
