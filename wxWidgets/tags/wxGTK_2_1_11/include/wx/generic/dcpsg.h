/////////////////////////////////////////////////////////////////////////////
// Name:        dcps.h
// Purpose:     wxPostScriptDC class
// Author:      Julian Smart and others
// Modified by:
// RCS-ID:      $Id$
// Copyright:   (c) Julian Smart, Robert Roebling and Markus Holzem
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_DCPSG_H_
#define _WX_DCPSG_H_

#ifdef __GNUG__
#pragma interface
#endif

#include "wx/dc.h"

#if wxUSE_PRINTING_ARCHITECTURE

#if wxUSE_POSTSCRIPT

#include "wx/dialog.h"
#include "wx/module.h"
#include "wx/cmndata.h"

//-----------------------------------------------------------------------------
// classes
//-----------------------------------------------------------------------------

class wxPostScriptDC;

//-----------------------------------------------------------------------------
// wxPostScriptDC
//-----------------------------------------------------------------------------

class WXDLLEXPORT wxPostScriptDC: public wxDC
{
  DECLARE_DYNAMIC_CLASS(wxPostScriptDC)

public:

  wxPostScriptDC();

  // Deprecated constructor
  wxPostScriptDC(const wxString& output, bool interactive = TRUE, wxWindow *parent = (wxWindow *) NULL);

  // Recommended constructor
  wxPostScriptDC(const wxPrintData& printData);

  ~wxPostScriptDC();

  // Deprecated
  bool Create(const wxString& output, bool interactive = TRUE, wxWindow *parent = (wxWindow *) NULL);

  virtual bool Ok() const;

  // Deprecated: use wxGenericPrintDialog instead
  virtual bool PrinterDialog(wxWindow *parent = (wxWindow *) NULL);

  virtual void BeginDrawing() {}
  virtual void EndDrawing() {}

  void DoFloodFill(wxCoord x1, wxCoord y1, const wxColour &col, int style=wxFLOOD_SURFACE );
  bool DoGetPixel(wxCoord x1, wxCoord y1, wxColour *col) const;

  void DoDrawLine(wxCoord x1, wxCoord y1, wxCoord x2, wxCoord y2);
  void DoCrossHair(wxCoord x, wxCoord y) ;
  void DoDrawArc(wxCoord x1,wxCoord y1,wxCoord x2,wxCoord y2,wxCoord xc,wxCoord yc);
  void DoDrawEllipticArc(wxCoord x,wxCoord y,wxCoord w,wxCoord h,double sa,double ea);
  void DoDrawPoint(wxCoord x, wxCoord y);
  void DoDrawLines(int n, wxPoint points[], wxCoord xoffset = 0, wxCoord yoffset = 0);
  void DoDrawPolygon(int n, wxPoint points[], wxCoord xoffset = 0, wxCoord yoffset = 0, int fillStyle=wxODDEVEN_RULE);
  void DoDrawRectangle(wxCoord x, wxCoord y, wxCoord width, wxCoord height);
  void DoDrawRoundedRectangle(wxCoord x, wxCoord y, wxCoord width, wxCoord height, double radius = 20);
  void DoDrawEllipse(wxCoord x, wxCoord y, wxCoord width, wxCoord height);

  void DoDrawSpline(wxList *points);

  bool DoBlit(wxCoord xdest, wxCoord ydest, wxCoord width, wxCoord height,
            wxDC *source, wxCoord xsrc, wxCoord ysrc, int rop = wxCOPY, bool useMask = FALSE);
  bool CanDrawBitmap() const { return TRUE; }

  void DoDrawIcon( const wxIcon& icon, wxCoord x, wxCoord y );
  void DoDrawBitmap( const wxBitmap& bitmap, wxCoord x, wxCoord y, bool useMask=FALSE );

  void DoDrawText(const wxString& text, wxCoord x, wxCoord y );

  void Clear();
  void SetFont( const wxFont& font );
  void SetPen( const wxPen& pen );
  void SetBrush( const wxBrush& brush );
  void SetLogicalFunction( int function );
  void SetBackground( const wxBrush& brush );

  void DoSetClippingRegion(wxCoord x, wxCoord y, wxCoord width, wxCoord height);
  void DestroyClippingRegion();

  void DoSetClippingRegionAsRegion( const wxRegion &WXUNUSED(clip) ) { }

  bool StartDoc(const wxString& message);
  void EndDoc();
  void StartPage();
  void EndPage();

  wxCoord GetCharHeight() const;
  wxCoord GetCharWidth() const;
  bool CanGetTextExtent() const { return TRUE; }
  void DoGetTextExtent(const wxString& string, wxCoord *x, wxCoord *y,
                     wxCoord *descent = (wxCoord *) NULL,
                     wxCoord *externalLeading = (wxCoord *) NULL,
                     wxFont *theFont = (wxFont *) NULL ) const;

  void DoGetSize(int* width, int* height) const;
  void DoGetSizeMM(int *width, int *height) const;

  // Resolution in pixels per logical inch
  wxSize GetPPI() const;

  void SetAxisOrientation( bool xLeftRight, bool yBottomUp );
  void SetDeviceOrigin( wxCoord x, wxCoord y );

  void SetBackgroundMode(int WXUNUSED(mode)) { }
  void SetPalette(const wxPalette& WXUNUSED(palette)) { }

  wxPrintData& GetPrintData() { return m_printData; }
  void SetPrintData(const wxPrintData& data) { m_printData = data; }

  virtual int GetDepth() const { return 24; }

protected:

  FILE*             m_pstream;    // PostScript output stream
  wxString          m_title;
  unsigned char     m_currentRed;
  unsigned char     m_currentGreen;
  unsigned char     m_currentBlue;
  int               m_pageNumber;
  bool              m_clipping;
  double            m_underlinePosition;
  double            m_underlineThickness;
  wxPrintData       m_printData;
};

// Deprecated: should use wxGenericPrintDialog instead.
#if 1
enum
{
    wxID_PRINTER_COMMAND = 1,
    wxID_PRINTER_OPTIONS,
    wxID_PRINTER_ORIENTATION,
    wxID_PRINTER_MODES,
    wxID_PRINTER_X_SCALE,
    wxID_PRINTER_Y_SCALE,
    wxID_PRINTER_X_TRANS,
    wxID_PRINTER_Y_TRANS
};

class WXDLLEXPORT wxPostScriptPrintDialog: public wxDialog
{
DECLARE_CLASS(wxPostScriptPrintDialog)
public:
    wxPostScriptPrintDialog (wxWindow *parent, const wxString& title,
          const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize,
          wxCoord style = wxDEFAULT_DIALOG_STYLE);

    virtual int ShowModal();
};
#endif // 1

// Print Orientation (Should also add Left, Right)
enum
{
  PS_PORTRAIT = 1,
  PS_LANDSCAPE = 2
};// ps_orientation = PS_PORTRAIT;

// Print Actions
enum
{
  PS_NONE,
  PS_PREVIEW,
  PS_FILE,
  PS_PRINTER
};// ps_action = PS_PREVIEW;

// PostScript printer settings
WXDLLEXPORT void wxSetPrinterCommand(const wxString& cmd);
WXDLLEXPORT void wxSetPrintPreviewCommand(const wxString& cmd);
WXDLLEXPORT void wxSetPrinterOptions(const wxString& flags);
WXDLLEXPORT void wxSetPrinterOrientation(int orientation);
WXDLLEXPORT void wxSetPrinterScaling(double x, double y);
WXDLLEXPORT void wxSetPrinterTranslation(wxCoord x, wxCoord y);
WXDLLEXPORT void wxSetPrinterMode(int mode);
WXDLLEXPORT void wxSetPrinterFile(const wxString& f);
WXDLLEXPORT void wxSetAFMPath(const wxString& f);

// Get current values
WXDLLEXPORT wxString wxGetPrinterCommand();
WXDLLEXPORT wxString wxGetPrintPreviewCommand();
WXDLLEXPORT wxString wxGetPrinterOptions();
WXDLLEXPORT int wxGetPrinterOrientation();
WXDLLEXPORT void wxGetPrinterScaling(double* x, double* y);
WXDLLEXPORT void wxGetPrinterTranslation(wxCoord *x, wxCoord *y);
WXDLLEXPORT int wxGetPrinterMode();
WXDLLEXPORT wxString wxGetPrinterFile();
WXDLLEXPORT wxString wxGetAFMPath();

/*
 * PostScript print setup information.
 * This is now obsolete, but retained for a while for compatibility
 */

class WXDLLEXPORT wxPrintSetupData: public wxObject
{
public:
    wxPrintSetupData();
    ~wxPrintSetupData();

    void SetPrinterCommand(const wxString& cmd) { m_printerCommand = cmd; };
    void SetPaperName(const wxString& paper) { m_paperName = paper; };
    void SetPrintPreviewCommand(const wxString& cmd) { m_previewCommand = cmd; };
    void SetPrinterOptions(const wxString& flags) { m_printerFlags = flags; };
    void SetPrinterFile(const wxString& f) { m_printerFile = f; };
    void SetPrinterOrientation(int orient) { m_printerOrient = orient; };
    void SetPrinterScaling(double x, double y) { m_printerScaleX = x; m_printerScaleY = y; };
    void SetPrinterTranslation(wxCoord x, wxCoord y) { m_printerTranslateX = x; m_printerTranslateY = y; };
    // 1 = Preview, 2 = print to file, 3 = send to printer
    void SetPrinterMode(int mode) { m_printerMode = mode; };
    void SetAFMPath(const wxString& f) { m_afmPath = f; };
    void SetColour(bool col) { m_printColour = col; };

    // Get current values
    wxString GetPrinterCommand() const { return m_printerCommand; } ;
    wxString GetPrintPreviewCommand() const { return m_previewCommand; } ;
    wxString GetPrinterOptions() const { return m_printerFlags; };
    wxString GetPrinterFile() const { return m_printerFile; };
    wxString GetPaperName() const { return m_paperName; }
    int GetPrinterOrientation() const { return m_printerOrient; };
    void GetPrinterScaling(double* x, double* y) const { *x = m_printerScaleX; *y = m_printerScaleY; };
    void GetPrinterTranslation(wxCoord *x, wxCoord *y) const { *x = m_printerTranslateX; *y = m_printerTranslateY; };
    int GetPrinterMode() const { return m_printerMode; };
    wxString GetAFMPath() const { return m_afmPath; };
    bool GetColour() const { return m_printColour; };

    void operator=(wxPrintSetupData& data);

    // Initialize from a wxPrintData object (wxPrintData should now be used instead of wxPrintSetupData).
    // There is also an operator for initializing a wxPrintData from a wxPrintSetupData.
    void operator=(const wxPrintData& data);

#ifndef __WIN16__
    void GetPrinterTranslation(long *x, long *y) const
        { *x = m_printerTranslateX; *y = m_printerTranslateY; }
#endif // !Win16

public:
    wxString        m_printerCommand;
    wxString        m_previewCommand;
    wxString        m_printerFlags;
    wxString        m_printerFile;
    int             m_printerOrient;
    double          m_printerScaleX;
    double          m_printerScaleY;
    wxCoord         m_printerTranslateX;
    wxCoord         m_printerTranslateY;
    // 1 = Preview, 2 = print to file, 3 = send to printer
    int             m_printerMode;
    wxString        m_afmPath;
    // A name in the paper database (see paper.h)
    wxString        m_paperName;
    bool            m_printColour;

    DECLARE_DYNAMIC_CLASS(wxPrintSetupData)
};

WXDLLEXPORT_DATA(extern wxPrintSetupData*) wxThePrintSetupData;
WXDLLEXPORT extern void wxInitializePrintSetupData(bool init = TRUE);

#endif
    // wxUSE_POSTSCRIPT

#endif
    // wxUSE_PRINTING_ARCHITECTURE

#endif
        // _WX_DCPSG_H_
