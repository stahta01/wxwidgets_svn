/////////////////////////////////////////////////////////////////////////////
// Name:        dcpsg.cpp
// Purpose:     Generic wxPostScriptDC implementation
// Author:      Julian Smart, Robert Roebling, Markus Holzhem
// Modified by:
// Created:     04/01/98
// RCS-ID:      $Id$
// Copyright:   (c) Julian Smart and Markus Holzem
// Licence:   	wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifdef __GNUG__
#pragma implementation
#pragma interface
#endif

#include "wx/defs.h"

#if wxUSE_POSTSCRIPT

#include "wx/dcmemory.h"
#include "wx/utils.h"
#include "wx/intl.h"
#include "wx/filedlg.h"
#include "wx/app.h"
#include "wx/msgdlg.h"
#include "wx/image.h"
#include "wx/log.h"
#include "wx/generic/dcpsg.h"
#include "wx/generic/prntdlgg.h"
#include "wx/button.h"
#include "wx/stattext.h"
#include "wx/radiobox.h"
#include "wx/textctrl.h"

#include <math.h>

#ifdef __WXGTK__
#include "gdk/gdk.h"
#include "gtk/gtk.h"
#endif

#ifdef __WXMSW__

#ifdef DrawText
#undef DrawText
#endif

#ifdef StartDoc
#undef StartDoc
#endif

#ifdef GetCharWidth
#undef GetCharWidth
#endif

#ifdef FindWindow
#undef FindWindow
#endif

#endif

//-----------------------------------------------------------------------------
// start and end of document/page
//-----------------------------------------------------------------------------

static const char *wxPostScriptHeaderEllipse = "\
/ellipsedict 8 dict def\n\
ellipsedict /mtrx matrix put\n\
/ellipse {\n\
	ellipsedict begin\n\
	/endangle exch def\n\
	/startangle exch def\n\
	/yrad exch def\n\
	/xrad exch def\n\
	/y exch def\n\
	/x exch def\n\
	/savematrix mtrx currentmatrix def\n\
	x y translate\n\
	xrad yrad scale\n\
	0 0 1 startangle endangle arc\n\
	savematrix setmatrix\n\
	end\n\
	} def\n\
";

static const char *wxPostScriptHeaderEllipticArc= "\
/ellipticarcdict 8 dict def\n\
ellipticarcdict /mtrx matrix put\n\
/ellipticarc\n\
{ ellipticarcdict begin\n\
  /do_fill exch def\n\
  /endangle exch def\n\
  /startangle exch def\n\
  /yrad exch def\n\
  /xrad exch def \n\
  /y exch def\n\
  /x exch def\n\
  /savematrix mtrx currentmatrix def\n\
  x y translate\n\
  xrad yrad scale\n\
  do_fill { 0 0 moveto } if\n\
  0 0 1 startangle endangle arc\n\
  savematrix setmatrix\n\
  do_fill { fill }{ stroke } ifelse\n\
  end\n\
} def\n";

static const char *wxPostScriptHeaderSpline = "\
/DrawSplineSection {\n\
	/y3 exch def\n\
	/x3 exch def\n\
	/y2 exch def\n\
	/x2 exch def\n\
	/y1 exch def\n\
	/x1 exch def\n\
	/xa x1 x2 x1 sub 0.666667 mul add def\n\
	/ya y1 y2 y1 sub 0.666667 mul add def\n\
	/xb x3 x2 x3 sub 0.666667 mul add def\n\
	/yb y3 y2 y3 sub 0.666667 mul add def\n\
	x1 y1 lineto\n\
	xa ya xb yb x3 y3 curveto\n\
	} def\n\
";

static const char *wxPostScriptHeaderColourImage = "\
% define 'colorimage' if it isn't defined\n\
%   ('colortogray' and 'mergeprocs' come from xwd2ps\n\
%     via xgrab)\n\
/colorimage where   % do we know about 'colorimage'?\n\
  { pop }           % yes: pop off the 'dict' returned\n\
  {                 % no:  define one\n\
    /colortogray {  % define an RGB->I function\n\
      /rgbdata exch store    % call input 'rgbdata'\n\
      rgbdata length 3 idiv\n\
      /npixls exch store\n\
      /rgbindx 0 store\n\
      0 1 npixls 1 sub {\n\
        grays exch\n\
        rgbdata rgbindx       get 20 mul    % Red\n\
        rgbdata rgbindx 1 add get 32 mul    % Green\n\
        rgbdata rgbindx 2 add get 12 mul    % Blue\n\
        add add 64 idiv      % I = .5G + .31R + .18B\n\
        put\n\
        /rgbindx rgbindx 3 add store\n\
      } for\n\
      grays 0 npixls getinterval\n\
    } bind def\n\
\n\
    % Utility procedure for colorimage operator.\n\
    % This procedure takes two procedures off the\n\
    % stack and merges them into a single procedure.\n\
\n\
    /mergeprocs { % def\n\
      dup length\n\
      3 -1 roll\n\
      dup\n\
      length\n\
      dup\n\
      5 1 roll\n\
      3 -1 roll\n\
      add\n\
      array cvx\n\
      dup\n\
      3 -1 roll\n\
      0 exch\n\
      putinterval\n\
      dup\n\
      4 2 roll\n\
      putinterval\n\
    } bind def\n\
\n\
    /colorimage { % def\n\
      pop pop     % remove 'false 3' operands\n\
      {colortogray} mergeprocs\n\
      image\n\
    } bind def\n\
  } ifelse          % end of 'false' case\n\
";

static char wxPostScriptHeaderReencodeISO1[] =
    "\n/reencodeISO {\n"
"dup dup findfont dup length dict begin\n"
"{ 1 index /FID ne { def }{ pop pop } ifelse } forall\n"
"/Encoding ISOLatin1Encoding def\n"
"currentdict end definefont\n"
"} def\n"
"/ISOLatin1Encoding [\n"
"/.notdef/.notdef/.notdef/.notdef/.notdef/.notdef/.notdef/.notdef\n"
"/.notdef/.notdef/.notdef/.notdef/.notdef/.notdef/.notdef/.notdef\n"
"/.notdef/.notdef/.notdef/.notdef/.notdef/.notdef/.notdef/.notdef\n"
"/.notdef/.notdef/.notdef/.notdef/.notdef/.notdef/.notdef/.notdef\n"
"/space/exclam/quotedbl/numbersign/dollar/percent/ampersand/quoteright\n"
"/parenleft/parenright/asterisk/plus/comma/minus/period/slash\n"
"/zero/one/two/three/four/five/six/seven/eight/nine/colon/semicolon\n"
"/less/equal/greater/question/at/A/B/C/D/E/F/G/H/I/J/K/L/M/N\n"
"/O/P/Q/R/S/T/U/V/W/X/Y/Z/bracketleft/backslash/bracketright\n"
"/asciicircum/underscore/quoteleft/a/b/c/d/e/f/g/h/i/j/k/l/m\n"
"/n/o/p/q/r/s/t/u/v/w/x/y/z/braceleft/bar/braceright/asciitilde\n"
"/.notdef/.notdef/.notdef/.notdef/.notdef/.notdef/.notdef/.notdef\n"
"/.notdef/.notdef/.notdef/.notdef/.notdef/.notdef/.notdef/.notdef\n"
"/.notdef/dotlessi/grave/acute/circumflex/tilde/macron/breve\n"
"/dotaccent/dieresis/.notdef/ring/cedilla/.notdef/hungarumlaut\n";

static char wxPostScriptHeaderReencodeISO2[] =
"/ogonek/caron/space/exclamdown/cent/sterling/currency/yen/brokenbar\n"
"/section/dieresis/copyright/ordfeminine/guillemotleft/logicalnot\n"
"/hyphen/registered/macron/degree/plusminus/twosuperior/threesuperior\n"
"/acute/mu/paragraph/periodcentered/cedilla/onesuperior/ordmasculine\n"
"/guillemotright/onequarter/onehalf/threequarters/questiondown\n"
"/Agrave/Aacute/Acircumflex/Atilde/Adieresis/Aring/AE/Ccedilla\n"
"/Egrave/Eacute/Ecircumflex/Edieresis/Igrave/Iacute/Icircumflex\n"
"/Idieresis/Eth/Ntilde/Ograve/Oacute/Ocircumflex/Otilde/Odieresis\n"
"/multiply/Oslash/Ugrave/Uacute/Ucircumflex/Udieresis/Yacute\n"
"/Thorn/germandbls/agrave/aacute/acircumflex/atilde/adieresis\n"
"/aring/ae/ccedilla/egrave/eacute/ecircumflex/edieresis/igrave\n"
"/iacute/icircumflex/idieresis/eth/ntilde/ograve/oacute/ocircumflex\n"
"/otilde/odieresis/divide/oslash/ugrave/uacute/ucircumflex/udieresis\n"
"/yacute/thorn/ydieresis\n"
        "] def\n\n";

//-------------------------------------------------------------------------------
// wxPostScriptDC
//-------------------------------------------------------------------------------

wxPostScriptDC::wxPostScriptDC ()
{
    m_pstream = (ofstream*) NULL;
    
    m_currentRed = 0;
    m_currentGreen = 0;
    m_currentBlue = 0;
    
    m_pageNumber = 0;
    
    m_clipping = FALSE;
    
    m_underlinePosition = 0.0;
    m_underlineThickness = 0.0;

    m_signX =  1;  // default x-axis left to right
    m_signY = -1;  // default y-axis bottom up -> top down
}

wxPostScriptDC::wxPostScriptDC (const wxString& file, bool interactive, wxWindow *parent)
{
    m_pstream = (ofstream*) NULL;
    
    m_currentRed = 0;
    m_currentGreen = 0;
    m_currentBlue = 0;
    
    m_pageNumber = 0;
    
    m_clipping = FALSE;
    
    m_underlinePosition = 0.0;
    m_underlineThickness = 0.0;

    m_signX =  1;  // default x-axis left to right
    m_signY = -1;  // default y-axis bottom up -> top down

    Create(file, interactive, parent);
}

bool wxPostScriptDC::Create(const wxString& file, bool interactive, wxWindow *parent)
{
    m_isInteractive = interactive;

    m_title = "";
    m_filename = file;

#ifdef __WXMSW__
    // Can only send to file in Windows
    wxThePrintSetupData->SetPrinterMode(PS_FILE);
#endif

    if (m_isInteractive)
    {
        if ((m_ok = PrinterDialog (parent) ) == FALSE) return FALSE;
    }
    else
    {
        m_ok = TRUE;
    }
    
    return m_ok;
}

wxPostScriptDC::~wxPostScriptDC ()
{
    if (m_pstream) delete m_pstream;
}

bool wxPostScriptDC::Ok() const
{
  return m_ok;
}
  
bool wxPostScriptDC::PrinterDialog(wxWindow *parent)
{
    wxPostScriptPrintDialog dialog( parent, _("Printer Settings"), wxPoint(150, 150), wxSize(400, 400), 
                                    wxDEFAULT_DIALOG_STYLE | wxDIALOG_MODAL );
    m_ok = (dialog.ShowModal () == wxID_OK);

    if (!m_ok) return FALSE;

    if ((m_filename == "") && 
        (wxThePrintSetupData->GetPrinterMode() == PS_PREVIEW  || 
         wxThePrintSetupData->GetPrinterMode() == PS_PRINTER))
    {
// steve, 05.09.94
#ifdef __VMS__
      wxThePrintSetupData->SetPrinterFile("preview");
#else
      // For PS_PRINTER action this depends on a Unix-style print spooler
      // since the wx_printer_file can be destroyed during a session
      // @@@ TODO: a Windows-style answer for non-Unix
      char userId[256];
      wxGetUserId (userId, sizeof (userId) / sizeof (char));
      char tmp[256];
      strcpy (tmp, "/tmp/preview_");
      strcat (tmp, userId);
      wxThePrintSetupData->SetPrinterFile(tmp);
#endif
      char tmp2[256];
      strcpy(tmp2, wxThePrintSetupData->GetPrinterFile());
      strcat (tmp2, ".ps");
      wxThePrintSetupData->SetPrinterFile(tmp2);
      m_filename = tmp2;
    }
    else if ((m_filename == "") && (wxThePrintSetupData->GetPrinterMode() == PS_FILE))
    {
      char *file = wxSaveFileSelector (_("PostScript"), "ps");
      if (!file)
      {
        m_ok = FALSE;
        return FALSE;
      }
      wxThePrintSetupData->SetPrinterFile(file);
      m_filename = file;
      m_ok = TRUE;
    }

    return m_ok;
}

void wxPostScriptDC::SetClippingRegion (long x, long y, long w, long h)
{
    wxCHECK_RET( m_ok && m_pstream, "invalid postscript dc" );
    
    if (m_clipping) return;

    wxDC::SetClippingRegion( x, y, w, h );
    
    m_clipping = TRUE;
    *m_pstream << "gsave\n" 
               << "newpath\n" 
	       << XLOG2DEV(x)   << " " << YLOG2DEV(y)   << " moveto\n"
	       << XLOG2DEV(x+w) << " " << YLOG2DEV(y)   << " lineto\n"
	       << XLOG2DEV(x+w) << " " << YLOG2DEV(y+h) << " lineto\n"
	       << XLOG2DEV(x)   << " " << YLOG2DEV(y+h) << " lineto\n"
               << "closepath clip newpath\n";
}

void wxPostScriptDC::SetClippingRegion( const wxRegion &WXUNUSED(region) )
{
}

void wxPostScriptDC::DestroyClippingRegion()
{
    wxCHECK_RET( m_ok && m_pstream, "invalid postscript dc" );
    
    wxDC::DestroyClippingRegion();
    
    if (m_clipping)
    {
        m_clipping = FALSE;
        *m_pstream << "grestore\n";
    }
}

void wxPostScriptDC::Clear()
{
    wxFAIL_MSG( "wxPostScriptDC::Clear not implemented." );
}

void wxPostScriptDC::FloodFill (long WXUNUSED(x), long WXUNUSED(y), const wxColour &WXUNUSED(col), int WXUNUSED(style))
{
    wxFAIL_MSG( "wxPostScriptDC::FloodFill not implemented." );
}

bool wxPostScriptDC::GetPixel (long WXUNUSED(x), long WXUNUSED(y), wxColour * WXUNUSED(col)) const
{
    wxFAIL_MSG( "wxPostScriptDC::GetPixel not implemented." );
    return FALSE;
}

void wxPostScriptDC::CrossHair (long WXUNUSED(x), long WXUNUSED(y))
{
    wxFAIL_MSG( "wxPostScriptDC::CrossHair not implemented." );
}

void wxPostScriptDC::DrawLine (long x1, long y1, long x2, long y2)
{
    wxCHECK_RET( m_ok && m_pstream, "invalid postscript dc" );
    
    if  (m_pen.GetStyle() == wxTRANSPARENT) return;
    
    SetPen( m_pen );
    
    *m_pstream << "newpath\n"
	       <<  XLOG2DEV(x1) << " " <<  YLOG2DEV (y1) << " moveto\n"
	       <<  XLOG2DEV(x2) << " " <<  YLOG2DEV (y2) << " lineto\n"
	       << "stroke\n";
		   
    CalcBoundingBox( x1, y1 );
    CalcBoundingBox( x2, y2 );
}

#define RAD2DEG 57.29577951308

void wxPostScriptDC::DrawArc (long x1, long y1, long x2, long y2, long xc, long yc)
{
    wxCHECK_RET( m_ok && m_pstream, "invalid postscript dc" );
    
    long dx = x1 - xc;
    long dy = y1 - yc;
    long radius = (long) sqrt(dx*dx+dy*dy);
    double alpha1, alpha2;

    if (x1 == x2 && y1 == y2) 
    {
	alpha1 = 0.0;
	alpha2 = 360.0;
    } else if (radius == 0.0) 
    {
	alpha1 = alpha2 = 0.0;
    } else 
    {
	alpha1 = (x1 - xc == 0) ?
	    (y1 - yc < 0) ? 90.0 : -90.0 :
	    -atan2(double(y1-yc), double(x1-xc)) * RAD2DEG;
	alpha2 = (x2 - xc == 0) ?
	    (y2 - yc < 0) ? 90.0 : -90.0 :
	    -atan2(double(y2-yc), double(x2-xc)) * RAD2DEG;
    }
    while (alpha1 <= 0)   alpha1 += 360;
    while (alpha2 <= 0)   alpha2 += 360; // adjust angles to be between
    while (alpha1 > 360)  alpha1 -= 360; // 0 and 360 degree
    while (alpha2 > 360)  alpha2 -= 360;

    if (m_brush.GetStyle() != wxTRANSPARENT) 
    {
	SetBrush( m_brush );
	*m_pstream << "newpath\n"
	         << XLOG2DEV(xc)        << " "
		 << YLOG2DEV(yc)        << " "
	         << XLOG2DEVREL(radius) << " "
		 << YLOG2DEVREL(radius) << " "
		 << alpha1               << " "
		 << alpha2               << " ellipse\n"
	         << XLOG2DEV(xc)        << " "
		 << YLOG2DEV(yc)        << " lineto\n"
		 << "closepath\n"
	         << "fill\n";
    }
    
    if (m_pen.GetStyle() != wxTRANSPARENT) 
    {
	SetPen( m_pen );
	*m_pstream << "newpath\n"
	         << XLOG2DEV(xc)        << " "
		 << YLOG2DEV(yc)        << " "
	         << XLOG2DEVREL(radius) << " "
		 << YLOG2DEVREL(radius) << " "
		 << alpha1               << " "
		 << alpha2               << " ellipse\n"
	         << "stroke\n";
    }
    
    CalcBoundingBox( xc-radius, yc-radius );
    CalcBoundingBox( xc+radius, yc+radius );
}

void wxPostScriptDC::DrawEllipticArc(long x,long y,long w,long h,double sa,double ea)
{
    wxCHECK_RET( m_ok && m_pstream, "invalid postscript dc" );
    
    if (sa>=360 || sa<=-360) sa=sa-int(sa/360)*360;
    if (ea>=360 || ea<=-360) ea=ea-int(ea/360)*360;
    if (sa<0) sa+=360;
    if (ea<0) ea+=360;
    
    if (sa==ea)
    {
        DrawEllipse(x,y,w,h);
        return;
    }

    if (m_brush.GetStyle () != wxTRANSPARENT)
    {
        SetBrush( m_brush );

        *m_pstream << "newpath\n" 
	           << XLOG2DEV(x+w/2) << " " << YLOG2DEV(y+h/2) << " " 
		   << XLOG2DEVREL(w/2) << " " << YLOG2DEVREL(h/2) << " " 
		   << int(sa) <<" "<< int(ea) << " true ellipticarc\n";

        CalcBoundingBox( x ,y );
        CalcBoundingBox( x+w, y+h );
    }
    
    if (m_pen.GetStyle () != wxTRANSPARENT)
    {
        SetPen( m_pen );

        *m_pstream << "newpath\n" 
	           << XLOG2DEV(x+w/2) << " " << YLOG2DEV(y+h/2) << " " 
		   << XLOG2DEVREL(w/2) << " " << XLOG2DEVREL(h/2) << " " 
		   << int(sa) <<" "<< int(ea) << " false ellipticarc\n";

        CalcBoundingBox( x, y );
        CalcBoundingBox( x+w, y+h );
    }
}

void wxPostScriptDC::DrawPoint (long x, long y)
{
    wxCHECK_RET( m_ok && m_pstream, "invalid postscript dc" );
    
    if (m_pen.GetStyle() == wxTRANSPARENT) return;
    
    SetPen (m_pen);
    
    *m_pstream << "newpath\n"
	     << XLOG2DEV(x)   << " " << YLOG2DEV (y) << " moveto\n"
	     << XLOG2DEV(x+1) << " " << YLOG2DEV (y) << " lineto\n"
	     << "stroke\n";
    
    CalcBoundingBox( x, y );
}

void wxPostScriptDC::DrawPolygon (int n, wxPoint points[], long xoffset, long yoffset, int WXUNUSED(fillStyle))
{
    wxCHECK_RET( m_ok && m_pstream, "invalid postscript dc" );
    
    if (n <= 0) return;
    
    if (m_brush.GetStyle () != wxTRANSPARENT)
    {
	SetBrush( m_brush );
	  
	*m_pstream << "newpath\n";

	long xx = XLOG2DEV(points[0].x + xoffset);
	long yy = YLOG2DEV(points[0].y + yoffset);
	*m_pstream << xx << " " << yy << " moveto\n";
	CalcBoundingBox( points[0].x + xoffset, points[0].y + yoffset );

	for (int i = 1; i < n; i++)
	{
	    xx = XLOG2DEV(points[i].x + xoffset);
	    yy = YLOG2DEV(points[i].y + yoffset);
	    *m_pstream << xx << " " << yy << " lineto\n";
	    CalcBoundingBox( points[i].x + xoffset, points[i].y + yoffset);
	}
	*m_pstream << "fill\n";
    }

    if (m_pen.GetStyle () != wxTRANSPARENT)
    {
	SetPen( m_pen );
	
	*m_pstream << "newpath\n";

	long xx = XLOG2DEV(points[0].x + xoffset);
	long yy = YLOG2DEV(points[0].y + yoffset);
	*m_pstream << xx << " " << yy << " moveto\n";
	CalcBoundingBox( points[0].x + xoffset, points[0].y + yoffset );

	for (int i = 1; i < n; i++)
	{
	    xx = XLOG2DEV(points[i].x + xoffset);
	    yy = YLOG2DEV(points[i].y + yoffset);
	    *m_pstream << xx << " " << yy << " lineto\n";
	    CalcBoundingBox( points[i].x + xoffset, points[i].y + yoffset);
	}

	*m_pstream << "stroke\n";
    }
}

void wxPostScriptDC::DrawLines (int n, wxPoint points[], long xoffset, long yoffset)
{
    wxCHECK_RET( m_ok && m_pstream, "invalid postscript dc" );
    
    if (m_pen.GetStyle() == wxTRANSPARENT) return;
    if (n <= 0) return;
  
    SetPen (m_pen);
    
    for (int i=0; i<n ; ++i)
    {
	CalcBoundingBox( XLOG2DEV(points[i].x+xoffset), YLOG2DEV(points[i].y+yoffset));
    }

    *m_pstream << "newpath\n"
               << XLOG2DEV(points[0].x+xoffset) << " "
	       << YLOG2DEV(points[0].y+yoffset) << " moveto\n";
	     
    for (int i = 1; i < n; i++)	
    {
	*m_pstream << XLOG2DEV(points[i].x+xoffset) << " "
		   << YLOG2DEV(points[i].y+yoffset) << " lineto\n";
    }
    
    *m_pstream << "stroke\n";
}

void wxPostScriptDC::DrawRectangle (long x, long y, long width, long height)
{
    wxCHECK_RET( m_ok && m_pstream, "invalid postscript dc" );
    
    if (m_brush.GetStyle () != wxTRANSPARENT)
    {
        SetBrush( m_brush );

        *m_pstream << "newpath\n"
                   << XLOG2DEV(x) << " " << YLOG2DEV(y) << " moveto\n"
                   << XLOG2DEV(x + width) << " " << YLOG2DEV(y) << " lineto\n"
                   << XLOG2DEV(x + width) << " " << YLOG2DEV(y + height) << " lineto\n"
                   << XLOG2DEV(x) << " " << YLOG2DEV(y + height) << " lineto\n"
                   << "closepath\n"
                   << "fill\n";

        CalcBoundingBox( x, y );
        CalcBoundingBox( x + width, y + height );
    }
    
    if (m_pen.GetStyle () != wxTRANSPARENT)
    {
        SetPen (m_pen);

        *m_pstream << "newpath\n"
                   << XLOG2DEV(x) << " " << YLOG2DEV(y) << " moveto\n"
                   << XLOG2DEV(x + width) << " " << YLOG2DEV(y) << " lineto\n"
                   << XLOG2DEV(x + width) << " " << YLOG2DEV(y + height) << " lineto\n"
                   << XLOG2DEV(x) << " " << YLOG2DEV(y + height) << " lineto\n"
                   << "closepath\n"
                   << "stroke\n";

        CalcBoundingBox( x, y );
        CalcBoundingBox( x + width, y + height );
    }
}

void wxPostScriptDC::DrawRoundedRectangle (long x, long y, long width, long height, double radius)
{
    wxCHECK_RET( m_ok && m_pstream, "invalid postscript dc" );
    
    if (radius < 0.0)
    {
        // Now, a negative radius is interpreted to mean
        // 'the proportion of the smallest X or Y dimension'
        double smallest = 0.0;
        if (width < height)
	    smallest = width;
        else
	    smallest = height;
        radius =  (-radius * smallest);
    }
    
    long rad = (long) radius;

    if (m_brush.GetStyle () != wxTRANSPARENT)
    {
        SetBrush( m_brush );
	
        // Draw rectangle anticlockwise
        *m_pstream << "newpath\n"
                   << XLOG2DEV(x + rad) << " " << YLOG2DEV(y + rad) << " " << XLOG2DEVREL(rad) << " 90 180 arc\n"
                   << XLOG2DEV(x) << " " << YLOG2DEV(y + rad) << " moveto\n"
                   << XLOG2DEV(x + rad) << " " << YLOG2DEV(y + height - rad) << " " << XLOG2DEVREL(rad) << " 180 270 arc\n"
                   << XLOG2DEV(x + width - rad) << " " << YLOG2DEV(y + height) << " lineto\n"
                   << XLOG2DEV(x + width - rad) << " " << YLOG2DEV(y + height - rad) << " " << XLOG2DEVREL(rad) << " 270 0 arc\n"
                   << XLOG2DEV(x + width) << " " << YLOG2DEV(y + rad) << " lineto\n"
                   << XLOG2DEV(x + width - rad) << " " << YLOG2DEV(y + rad) << " " << XLOG2DEVREL(rad) << " 0 90 arc\n"
                   << XLOG2DEV(x + rad) << " " << YLOG2DEV(y) << " lineto\n"
                   << "closepath\n"
                   << "fill\n";

        CalcBoundingBox( x, y );
        CalcBoundingBox( x + width, y + height );
    }
    
    if (m_pen.GetStyle () != wxTRANSPARENT)
    {
        SetPen (m_pen);
	
        // Draw rectangle anticlockwise
        *m_pstream << "newpath\n"
                   << XLOG2DEV(x + rad) << " " << YLOG2DEV(y + rad) << " " << XLOG2DEVREL(rad) << " 90 180 arc\n"
                   << XLOG2DEV(x) << " " << YLOG2DEV(y + rad) << " moveto\n"
                   << XLOG2DEV(x + rad) << " " << YLOG2DEV(y + height - rad) << " " << XLOG2DEVREL(rad) << " 180 270 arc\n"
                   << XLOG2DEV(x + width - rad) << " " << YLOG2DEV(y + height) << " lineto\n"
                   << XLOG2DEV(x + width - rad) << " " << YLOG2DEV(y + height - rad) << " " << XLOG2DEVREL(rad) << " 270 0 arc\n"
                   << XLOG2DEV(x + width) << " " << YLOG2DEV(y + rad) << " lineto\n"
                   << XLOG2DEV(x + width - rad) << " " << YLOG2DEV(y + rad) << " " << XLOG2DEVREL(rad) << " 0 90 arc\n"
                   << XLOG2DEV(x + rad) << " " << YLOG2DEV(y) << " lineto\n"
                   << "closepath\n"
                   << "stroke\n";

        CalcBoundingBox( x, y );
        CalcBoundingBox( x + width, y + height );
    }
}

void wxPostScriptDC::DrawEllipse (long x, long y, long width, long height)
{
    wxCHECK_RET( m_ok && m_pstream, "invalid postscript dc" );
    
    if (m_brush.GetStyle () != wxTRANSPARENT)
    {
        SetBrush (m_brush);

        *m_pstream << "newpath\n" 
                   << XLOG2DEV(x + width / 2) << " " << YLOG2DEV(y + height / 2) << " " 
                   << XLOG2DEV(width / 2) << " " << YLOG2DEVREL(height / 2) << " 0 360 ellipse\n" 
                   << "fill\n";

        CalcBoundingBox( x - width, y - height );
        CalcBoundingBox( x + width, y + height );
    }
    
    if (m_pen.Ok() && m_pen.GetStyle () != wxTRANSPARENT)
    {
        SetPen (m_pen);

        *m_pstream << "newpath\n" 
                   << XLOG2DEV(x + width / 2) << " " << YLOG2DEV(y + height / 2) << " " 
                   << XLOG2DEV(width / 2) << " " << YLOG2DEVREL(height / 2) << " 0 360 ellipse\n" 
                   << "stroke\n";

        CalcBoundingBox( x - width, y - height );
        CalcBoundingBox( x + width, y + height );
    }
}

void wxPostScriptDC::DrawIcon (const wxIcon& icon, long x, long y)
{
    DrawBitmap( icon, x, y, TRUE );
}

void wxPostScriptDC::DrawBitmap( const wxBitmap& bitmap, long x, long y, bool WXUNUSED(useMask) )
{
    wxCHECK_RET( m_ok && m_pstream, "invalid postscript dc" );

    if (!bitmap.Ok()) return;
    
    wxImage image( bitmap );
    
    if (!image.Ok()) return;
    
    int ww = XLOG2DEVREL(image.GetWidth());
    int hh = YLOG2DEVREL(image.GetHeight());
    
    image = image.Scale( ww, hh );
    
    if (!image.Ok()) return;
    
    int xx = XLOG2DEV(x);
    int yy = YLOG2DEV(y + bitmap.GetHeight());
    
    *m_pstream << "/origstate save def\n"
               << "20 dict begin\n"
               << "/pix " << ww << " string def\n"
               << "/grays " << ww << " string def\n"
               << "/npixels 0 def\n"
               << "/rgbindx 0 def\n"
               << xx << " " << yy << " translate\n"
               << ww << " " << hh << " scale\n"
               << ww << " " << hh << " 8\n"
               << "[" << ww << " 0 0 " << (-hh) << " 0 " << hh << "]\n"
               << "{currentfile pix readhexstring pop}\n"
               << "false 3 colorimage\n";
  
    for (int j = 0; j < hh; j++)
    {
        for (int i = 0; i < ww; i++)
        {
	    char buffer[5];
	    buffer[2] = 0;
	    wxDecToHex( image.GetRed(i,j), buffer );
            *m_pstream << buffer;
	    wxDecToHex( image.GetGreen(i,j), buffer );
            *m_pstream << buffer;
	    wxDecToHex( image.GetBlue(i,j), buffer );
            *m_pstream << buffer;
	}
	*m_pstream << "\n";
    }

    *m_pstream << "end\n";
    *m_pstream << "origstate restore\n";
    
}

void wxPostScriptDC::SetFont (const wxFont& font)
{
    wxCHECK_RET( m_ok && m_pstream, "invalid postscript dc" );
    
    if (!font.Ok())  return;
    
    m_font = font;

#ifdef __WXGTK__    
    char *name = wxTheFontNameDirectory->GetPostScriptName( m_font.GetFamily(),
							    m_font.GetWeight(),
							    m_font.GetStyle() );
    if (!name) name = "Times-Roman";

    *m_pstream << "/" << name << " reencodeISO def\n"
	       << "/" << name << " findfont\n"
	       << YLOG2DEVREL(font.GetPointSize())
	       << " scalefont setfont\n";
#else
  char buf[100];
  const char *name;
  const char *style = "";
  int Style = m_font.GetStyle ();
  int Weight = m_font.GetWeight ();

  switch (m_font.GetFamily ())
    {
    case wxTELETYPE:
    case wxMODERN:
      name = "/Courier";
      break;
    case wxSWISS:
      name = "/Helvetica";
      break;
    case wxROMAN:
//      name = "/Times-Roman";
      name = "/Times"; // Altered by EDZ
      break;
    case wxSCRIPT:
      name = "/Zapf-Chancery-MediumItalic";
      Style  = wxNORMAL;
      Weight = wxNORMAL;
      break;
    default:
    case wxDEFAULT: // Sans Serif Font
      name = "/LucidaSans";
    }

  if (Style == wxNORMAL && (Weight == wxNORMAL || Weight == wxLIGHT))
    {
      if (m_font.GetFamily () == wxROMAN)
	style = "-Roman";
      else
	style = "";
    }
  else if (Style == wxNORMAL && Weight == wxBOLD)
    style = "-Bold";

  else if (Style == wxITALIC && (Weight == wxNORMAL || Weight == wxLIGHT))
    {
      if (m_font.GetFamily () == wxROMAN)
	style = "-Italic";
      else
	style = "-Oblique";
    }
  else if (Style == wxITALIC && Weight == wxBOLD)
    {
      if (m_font.GetFamily () == wxROMAN)
	style = "-BoldItalic";
      else
	style = "-BoldOblique";
    }
  else if (Style == wxSLANT && (Weight == wxNORMAL || Weight == wxLIGHT))
    {
      if (m_font.GetFamily () == wxROMAN)
	style = "-Italic";
      else
	style = "-Oblique";
    }
  else if (Style == wxSLANT && Weight == wxBOLD)
    {
      if (m_font.GetFamily () == wxROMAN)
	style = "-BoldItalic";
      else
	style = "-BoldOblique";
    }
  else
    style = "";

  strcpy (buf, name);
  strcat (buf, style);
  *m_pstream << buf << " findfont\n";
  //  *m_pstream << (m_font.GetPointSize() * m_scaleFactor) << " scalefont setfont\n";
  // No scale factor in this implementation?
  *m_pstream << (m_font.GetPointSize()) << " scalefont setfont\n";
#endif
}

void wxPostScriptDC::SetPen( const wxPen& pen )
{
    wxCHECK_RET( m_ok && m_pstream, "invalid postscript dc" );
    
    if (!pen.Ok()) return;
    
    int oldStyle = m_pen.GetStyle();

    m_pen = pen;

    *m_pstream << XLOG2DEVREL(m_pen.GetWidth()) << " setlinewidth\n";

/*
     Line style - WRONG: 2nd arg is OFFSET
     
     Here, I'm afraid you do not conceive meaning of parameters of 'setdash'
     operator correctly. You should look-up this in the Red Book: the 2nd parame-
     ter is not number of values in the array of the first one, but an offset
     into this description of the pattern. I mean a real *offset* not index
     into array. I.e. If the command is [3 4] 1 setdash   is used, then there
     will be first black line *2* units long, then space 4 units, then the
     pattern of *3* units black, 4 units space will be repeated.
*/

    static const char *dotted = "[2 5] 2";
    static const char *short_dashed = "[4 4] 2";
    static const char *long_dashed = "[4 8] 2";
    static const char *dotted_dashed = "[6 6 2 6] 4";

    const char *psdash = (char *) NULL;
    switch (m_pen.GetStyle ())
    {
        case wxDOT:           psdash = dotted;         break;
        case wxSHORT_DASH:    psdash = short_dashed;   break;
        case wxLONG_DASH:     psdash = long_dashed;    break;
        case wxDOT_DASH:      psdash = dotted_dashed;  break;
        case wxSOLID:
        case wxTRANSPARENT:
        default:              psdash = "[] 0";         break;
    }
    
    if (oldStyle != m_pen.GetStyle())
    {
        *m_pstream << psdash << " setdash\n";
    }

    // Line colour
    unsigned char red = m_pen.GetColour().Red();
    unsigned char blue = m_pen.GetColour().Blue();
    unsigned char green = m_pen.GetColour().Green();

    if (!m_colour)
    {
        // Anything not white is black
        if (!(red == (unsigned char) 255 && blue == (unsigned char) 255
	    && green == (unsigned char) 255))
	{
	    red = (unsigned char) 0;
	    green = (unsigned char) 0;
	    blue = (unsigned char) 0;
	}
	
	// setgray here ?
    }

    if (!(red == m_currentRed && green == m_currentGreen && blue == m_currentBlue))
    {
       long redPS = (long) (((int) red) / 255.0);
       long bluePS = (long) (((int) blue) / 255.0);
       long greenPS = (long) (((int) green) / 255.0);

       *m_pstream << redPS << " " << greenPS << " " << bluePS << " setrgbcolor\n";

       m_currentRed = red;
       m_currentBlue = blue;
       m_currentGreen = green;
    }
}

void wxPostScriptDC::SetBrush( const wxBrush& brush )
{
    wxCHECK_RET( m_ok && m_pstream, "invalid postscript dc" );
    
    if (!brush.Ok()) return;
    
    m_brush = brush;

    // Brush colour
    unsigned char red = m_brush.GetColour ().Red ();
    unsigned char blue = m_brush.GetColour ().Blue ();
    unsigned char green = m_brush.GetColour ().Green ();

    if (!m_colour)
    {
        // Anything not black is white
        if (!(red == (unsigned char) 0 && blue == (unsigned char) 0
	    && green == (unsigned char) 0))
	{
	    red = (unsigned char) 255;
	    green = (unsigned char) 255;
	    blue = (unsigned char) 255;
	}
	
	// setgray here ?
    }

    if (!(red == m_currentRed && green == m_currentGreen && blue == m_currentBlue))
    {
       long redPS = (long) (((int) red) / 255.0);
       long bluePS = (long) (((int) blue) / 255.0);
       long greenPS = (long) (((int) green) / 255.0);
       *m_pstream << redPS << " " << greenPS << " " << bluePS << " setrgbcolor\n";
       m_currentRed = red;
       m_currentBlue = blue;
       m_currentGreen = green;
    }
}

void wxPostScriptDC::DrawText( const wxString& text, long x, long y, bool WXUNUSED(use16bit) )
{
    wxCHECK_RET( m_ok && m_pstream, "invalid postscript dc" );
    
    SetFont( m_font );

    if (m_textForegroundColour.Ok ())
    {
        unsigned char red = m_textForegroundColour.Red ();
        unsigned char blue = m_textForegroundColour.Blue ();
        unsigned char green = m_textForegroundColour.Green ();

        if (!m_colour)
	{
	  // Anything not white is black
	  if (!(red == (unsigned char) 255 && blue == (unsigned char) 255
		&& green == (unsigned char) 255))
	    {
	      red = (unsigned char) 0;
	      green = (unsigned char) 0;
	      blue = (unsigned char) 0;
	    }
	}
	
	// maybe setgray here ?
	
        if (!(red == m_currentRed && green == m_currentGreen && blue == m_currentBlue))
        {
            long redPS = (long) (((int) red) / 255.0);
            long bluePS = (long) (((int) blue) / 255.0);
            long greenPS = (long) (((int) green) / 255.0);
            *m_pstream << redPS << " " << greenPS << " " << bluePS << " setrgbcolor\n";

            m_currentRed = red;
            m_currentBlue = blue;
            m_currentGreen = green;
        }
    }

    int size = m_font.GetPointSize();

    long by = y + (long)floor( float(size) * 2.0 / 3.0 ); // approximate baseline
    *m_pstream << XLOG2DEV(x) << " " << YLOG2DEV(by) << " moveto\n";
    
    *m_pstream << "(";
    int len = strlen ((char *)(const char *)text);
    int i;
    for (i = 0; i < len; i++)
    {
        int c = (unsigned char) text[i];
        if ( c == ')' || c == '(' || c == '\\')
        {
            *m_pstream << "\\" << (char) c;
        }
        else if ( c >= 128 )
        {
            // Cope with character codes > 127
            char tmp[5];
            sprintf(tmp, "\\%o", c);
            *m_pstream << tmp;
        }
        else
            *m_pstream << (char) c;
    }

    *m_pstream << ")" << " show\n";

    if (m_font.GetUnderlined())
    {
        long uy = (long)(y + size - m_underlinePosition);
        long w, h;
        GetTextExtent(text, &w, &h);
      
        *m_pstream << "gsave " << XLOG2DEV(x) << " " << YLOG2DEV(uy)
                   << " moveto\n"
                   << (long)m_underlineThickness << " setlinewidth "
                   << XLOG2DEV(x + w) << " " << YLOG2DEV(uy)
                   << " lineto stroke grestore\n";
    }
    
    CalcBoundingBox( x, y );
    CalcBoundingBox( x + size * text.Length() * 2/3 , y );
}


void wxPostScriptDC::SetBackground (const wxBrush& brush)
{
    m_backgroundBrush = brush;
}

void wxPostScriptDC::SetLogicalFunction (int WXUNUSED(function))
{
    wxFAIL_MSG( "wxPostScriptDC::SetLogicalFunction not implemented." );
}

void wxPostScriptDC::DrawSpline( wxList *points )
{
    wxCHECK_RET( m_ok && m_pstream, "invalid postscript dc" );
    
    SetPen( m_pen );

    double a, b, c, d, x1, y1, x2, y2, x3, y3;
    wxPoint *p, *q;

    wxNode *node = points->First();
    p = (wxPoint *)node->Data();
    x1 = p->x; 
    y1 = p->y;

    node = node->Next();
    p = (wxPoint *)node->Data();
    c = p->x; 
    d = p->y;
    x3 = a = (double)(x1 + c) / 2;
    y3 = b = (double)(y1 + d) / 2;

    *m_pstream  << "newpath " 
                << XLOG2DEV((long)x1) << " " << YLOG2DEV((long)y1) << " moveto " 
		<< XLOG2DEV((long)x3) << " " << YLOG2DEV((long)y3) << " lineto\n";
		
    CalcBoundingBox( (long)x1, (long)y1 );
    CalcBoundingBox( (long)x3, (long)y3 );

    while ((node = node->Next()) != NULL)
    {
        q = (wxPoint *)node->Data();

	x1 = x3; 
	y1 = y3;
	x2 = c;  
	y2 = d;
	c = q->x; 
	d = q->y;
        x3 = (double)(x2 + c) / 2;
        y3 = (double)(y2 + d) / 2;
        *m_pstream << XLOG2DEV((long)x1) << " " << YLOG2DEV((long)y1) << " " 
	           << XLOG2DEV((long)x2) << " " << YLOG2DEV((long)y2) << " "
                   << XLOG2DEV((long)x3) << " " << YLOG2DEV((long)y3) << " DrawSplineSection\n";

        CalcBoundingBox( (long)x1, (long)y1 );
        CalcBoundingBox( (long)x3, (long)y3 );
    }
    
/*
     At this point, (x2,y2) and (c,d) are the position of the
     next-to-last and last point respectively, in the point list
*/

    *m_pstream << XLOG2DEV((long)c) << " " << YLOG2DEV((long)d) << " lineto stroke\n";
}

long wxPostScriptDC::GetCharWidth ()
{
    // Chris Breeze: reasonable approximation using wxMODERN/Courier
    return (long) (GetCharHeight() * 72.0 / 120.0);
}


void wxPostScriptDC::SetAxisOrientation( bool xLeftRight, bool yBottomUp )
{
    wxCHECK_RET( m_ok && m_pstream, "invalid postscript dc" );
    
    m_signX = (xLeftRight ? 1 : -1);
    m_signY = (yBottomUp  ? 1 : -1);
    
    ComputeScaleAndOrigin();
}

void wxPostScriptDC::SetDeviceOrigin( long x, long y )
{
    wxCHECK_RET( m_ok && m_pstream, "invalid postscript dc" );
    
    int h = 0;
    int w = 0;
    GetSize( &w, &h );
  
    wxDC::SetDeviceOrigin( x, h-y );
}

void wxPostScriptDC::GetSize(int* width, int* height) const
{
    const char *paperType = wxThePrintSetupData->GetPaperName();
    
    if (!paperType) paperType = _("A4 210 x 297 mm");

    wxPrintPaperType *paper = wxThePrintPaperDatabase->FindPaperType(paperType);
    
    if (!paper) paper = wxThePrintPaperDatabase->FindPaperType(_("A4 210 x 297 mm"));
    
    if (paper)
    {
        if (width) *width = paper->widthPixels;
        if (height) *height = paper->heightPixels;
    }
    else
    {
        if (width) *width = 595;
        if (height) *height = 842;
    }
}

bool wxPostScriptDC::StartDoc (const wxString& message)
{
    wxCHECK_MSG( m_ok, FALSE, "invalid postscript dc" );
    
    if (m_filename == "")
    {
        m_filename = wxGetTempFileName("ps");
        wxThePrintSetupData->SetPrinterFile((char *)(const char *)m_filename);
        m_ok = TRUE;
    }
    else
    {
	wxThePrintSetupData->SetPrinterFile((char *)(const char *)m_filename);
    }

    m_pstream = new ofstream (wxThePrintSetupData->GetPrinterFile());
      
    if (!m_pstream || !m_pstream->good())
    {
	wxMessageBox (_("Cannot open file!"), _("Error"), wxOK);
	m_ok = FALSE;
	return FALSE;
    }
    
    m_ok = TRUE;

    SetBrush( *wxBLACK_BRUSH );
    SetPen( *wxBLACK_PEN );
    SetBackground( *wxWHITE_BRUSH );
    SetTextForeground( *wxBLACK );

    // set origin according to paper size
    SetDeviceOrigin( 0,0 );
    
    wxPageNumber = 1;
    m_pageNumber = 1;
    m_title = message;
    return TRUE;
}

void wxPostScriptDC::EndDoc ()
{
    wxCHECK_RET( m_ok && m_pstream, "invalid postscript dc" );
    
    if (m_clipping)
    {
        m_clipping = FALSE;
        *m_pstream << "grestore\n";
    }

    if (m_pstream)
    {
        delete m_pstream;
        m_pstream = (ofstream *) NULL;
    }

    char *header_file = wxGetTempFileName("ps");
    
    m_pstream = new ofstream( header_file );

    *m_pstream << "%!PS-Adobe-2.0\n";	/* PostScript magic strings */
    *m_pstream << "%%Title: " << (const char *) m_title << "\n";
    *m_pstream << "%%Creator: " << wxTheApp->argv[0] << "\n";
    *m_pstream << "%%CreationDate: " << wxNow() << "\n";

    char userID[256];
    if ( wxGetEmailAddress(userID, sizeof(userID)) )
    {
        *m_pstream << "%%For: " << (char *)userID;
        char userName[245];
        if (wxGetUserName(userName, sizeof(userName)))
            *m_pstream << " (" << (char *)userName << ")";
        *m_pstream << "\n";
    }
    else if ( wxGetUserName(userID, sizeof(userID)) )
    {
        *m_pstream << "%%For: " << (char *)userID << "\n";
    }

    // THE FOLLOWING HAS BEEN CONTRIBUTED BY Andy Fyfe <andy@hyperparallel.com>

    long wx_printer_translate_x, wx_printer_translate_y;
    double wx_printer_scale_x, wx_printer_scale_y;
    wxThePrintSetupData->GetPrinterTranslation(&wx_printer_translate_x, &wx_printer_translate_y);
    wxThePrintSetupData->GetPrinterScaling(&wx_printer_scale_x, &wx_printer_scale_y);

    if (wxThePrintSetupData->GetPrinterOrientation() == PS_LANDSCAPE)
    {
	*m_pstream << "%%Orientation: Landscape\n";
    }
    else
    {
        *m_pstream << "%%Orientation: Portrait\n";
    }

    // Compute the bounding box.  Note that it is in the default user
    // coordinate system, thus we have to convert the values.
    long llx = (long) ((XLOG2DEV(m_minX)+wx_printer_translate_x)*wx_printer_scale_x);
    long lly = (long) ((YLOG2DEV(m_minY)+wx_printer_translate_y)*wx_printer_scale_y);
    long urx = (long) ((XLOG2DEV(m_maxX)+wx_printer_translate_x)*wx_printer_scale_x);
    long ury = (long) ((YLOG2DEV(m_maxY)+wx_printer_translate_y)*wx_printer_scale_y);

    // If we're landscape, our sense of "x" and "y" is reversed.
    if (wxThePrintSetupData->GetPrinterOrientation() == PS_LANDSCAPE)
    {
        long tmp;
        tmp = llx; llx = lly; lly = tmp;
        tmp = urx; urx = ury; ury = tmp;

        // We need either the two lines that follow, or we need to subtract
        // min_x from real_translate_y, which is commented out below.
        llx = llx - (long)(m_minX*wx_printer_scale_y);
        urx = urx - (long)(m_minX*wx_printer_scale_y);
    }

    // The Adobe specifications call for integers; we round as to make
    // the bounding larger.
    *m_pstream << "%%BoundingBox: "
               << floor((double)llx) << " " << floor((double)lly) << " "
               << ceil((double)urx)  << " " << ceil((double)ury)  << "\n";
    *m_pstream << "%%Pages: " << (wxPageNumber - 1) << "\n";
    *m_pstream << "%%EndComments\n\n";

    // To check the correctness of the bounding box, postscript commands
    // to draw a box corresponding to the bounding box are generated below.
    // But since we typically don't want to print such a box, the postscript
    // commands are generated within comments.  These lines appear before any
    // adjustment of scale, rotation, or translation, and hence are in the
    // default user coordinates.
    *m_pstream << "% newpath\n";
    *m_pstream << "% " << llx << " " << lly << " moveto\n";
    *m_pstream << "% " << urx << " " << lly << " lineto\n";
    *m_pstream << "% " << urx << " " << ury << " lineto\n";
    *m_pstream << "% " << llx << " " << ury << " lineto closepath stroke\n";

    *m_pstream << "%%BeginProlog\n";
    *m_pstream << wxPostScriptHeaderEllipse;
    *m_pstream << wxPostScriptHeaderEllipticArc;
    *m_pstream << wxPostScriptHeaderColourImage;
    *m_pstream << wxPostScriptHeaderReencodeISO1;
    *m_pstream << wxPostScriptHeaderReencodeISO2;

    if (wxPostScriptHeaderSpline)
        *m_pstream << wxPostScriptHeaderSpline;
    *m_pstream << "%%EndProlog\n";

    delete m_pstream;
    m_pstream = (ofstream *) NULL;

    char *tmp_file = wxGetTempFileName("ps");

    // Paste header Before wx_printer_file
    wxConcatFiles (header_file, wxThePrintSetupData->GetPrinterFile(), tmp_file);
    wxRemoveFile (header_file);
    wxRemoveFile (wxThePrintSetupData->GetPrinterFile());
    wxRenameFile(tmp_file, wxThePrintSetupData->GetPrinterFile());

#if defined(__X__) || defined(__WXGTK__)
  if (m_ok)
    {
      switch (wxThePrintSetupData->GetPrinterMode()) {
	case PS_PREVIEW:
	{
          char *argv[3];
          argv[0] = wxThePrintSetupData->GetPrintPreviewCommand();
          argv[1] = wxThePrintSetupData->GetPrinterFile();
          argv[2] = (char *) NULL;
	  wxExecute (argv, TRUE);
          wxRemoveFile(wxThePrintSetupData->GetPrinterFile());
	}
	break;

	case PS_PRINTER:
	{
          char *argv[4];
          int argc = 0;
          argv[argc++] = wxThePrintSetupData->GetPrinterCommand();

          // !SM! If we simply assign to argv[1] here, if printer options
          // are blank, we get an annoying and confusing message from lpr.
          char * opts = wxThePrintSetupData->GetPrinterOptions();
          if (opts && *opts)
              argv[argc++] = opts;

          argv[argc++] = wxThePrintSetupData->GetPrinterFile();
          argv[argc++] = (char *) NULL;
	  wxExecute (argv, TRUE);
          wxRemoveFile(wxThePrintSetupData->GetPrinterFile());
	}
	break;

	case PS_FILE:
	  break;
	}
    }
#endif
}

void wxPostScriptDC::StartPage ()
{
    wxCHECK_RET( m_ok && m_pstream, "invalid postscript dc" );
    
    *m_pstream << "%%Page: " << (wxPageNumber++) << "\n";
    
//  *m_pstream << "matrix currentmatrix\n";

    // Added by Chris Breeze

	// Each page starts with an "initgraphics" which resets the
	// transformation and so we need to reset the origin
	// (and rotate the page for landscape printing)
	
/*
	m_scaleFactor = 1.0;
	m_logicalOriginX = 0;
	m_logicalOriginY = 0;
*/

	// Output scaling
	long translate_x, translate_y;
	double scale_x, scale_y;
	wxThePrintSetupData->GetPrinterTranslation(&translate_x, &translate_y);
	wxThePrintSetupData->GetPrinterScaling(&scale_x, &scale_y);

	if (wxThePrintSetupData->GetPrinterOrientation() == PS_LANDSCAPE)
	{
		translate_y -= m_maxY;
		*m_pstream << "90 rotate\n";
	}

	*m_pstream << scale_x << " " << scale_y << " scale\n";
	*m_pstream << translate_x << " " << translate_y << " translate\n";
}

void wxPostScriptDC::EndPage ()
{
    wxCHECK_RET( m_ok && m_pstream, "invalid postscript dc" );
    
    *m_pstream << "showpage\n";
}

bool wxPostScriptDC::Blit( long WXUNUSED(xdest), long WXUNUSED(ydest), 
                           long WXUNUSED(fwidth), long WXUNUSED(fheight),
                           wxDC *WXUNUSED(source), 
			   long WXUNUSED(xsrc), long WXUNUSED(ysrc), 
			   int WXUNUSED(rop), bool WXUNUSED(useMask) )
{
    wxCHECK_MSG( m_ok && m_pstream, FALSE, "invalid postscript dc" );
    
    wxFAIL_MSG( "wxPostScriptDC::Blit no yet implemented." );
    
    return TRUE;
}

long wxPostScriptDC::GetCharHeight ()
{
    if (m_font.Ok())
        return  m_font.GetPointSize();
    else
        return 12;
}

void wxPostScriptDC::GetTextExtent (const wxString& string, long *x, long *y,
	       long *descent, long *externalLeading, wxFont *theFont,
				    bool WXUNUSED(use16))
{
//  if (!m_pstream) return;
  
  wxFont *fontToUse = theFont;
  
  if (!fontToUse) fontToUse = (wxFont*) &m_font;

#if !USE_AFM_FOR_POSTSCRIPT
  // Provide a VERY rough estimate (avoid using it)
	// Chris Breeze 5/11/97: produces accurate results for mono-spaced
	// font such as Courier (aka wxMODERN)
	int height = 12;
	if (fontToUse)
	{
		height = fontToUse->GetPointSize();
	}
	*x = strlen (string) * height * 72 / 120;
	*y = (long) (height * 1.32);	// allow for descender

  if (descent)
    *descent = 0;
  if (externalLeading)
    *externalLeading = 0;
#else
  // +++++ start of contributed code +++++

  // ************************************************************
  // method for calculating string widths in postscript:
  // read in the AFM (adobe font metrics) file for the
  // actual font, parse it and extract the character widths
  // and also the descender. this may be improved, but for now
  // it works well. the AFM file is only read in if the
  // font is changed. this may be chached in the future.
  // calls to GetTextExtent with the font unchanged are rather
  // efficient!!!
  //
  // for each font and style used there is an AFM file necessary.
  // currently i have only files for the roman font family.
  // i try to get files for the other ones!
  //
  // CAVE: the size of the string is currently always calculated
  //       in 'points' (1/72 of an inch). this should later on be
  //       changed to depend on the mapping mode.
  // CAVE: the path to the AFM files must be set before calling this
  //       function. this is usually done by a call like the following:
  //       wxSetAFMPath("d:\\wxw161\\afm\\");
  //
  // example:
  //
  //    wxPostScriptDC dc(NULL, TRUE);
  //    if (dc.Ok()){
  //      wxSetAFMPath("d:\\wxw161\\afm\\");
  //      dc.StartDoc("Test");
  //      dc.StartPage();
  //      long w,h;
  //      dc.SetFont(new wxFont(10, wxROMAN, wxNORMAL, wxNORMAL));
  //      dc.GetTextExtent("Hallo",&w,&h);
  //      dc.EndPage();
  //      dc.EndDoc();
  //    }
  //
  // by steve (stefan.hammes@urz.uni-heidelberg.de)
  // created: 10.09.94
  // updated: 14.05.95

  assert(fontToUse && "void wxPostScriptDC::GetTextExtent: no font defined");
  assert(x && "void wxPostScriptDC::GetTextExtent: x == NULL");
  assert(y && "void wxPostScriptDC::GetTextExtent: y == NULL");

  // these static vars are for storing the state between calls
  static int lastFamily= INT_MIN;
  static int lastSize= INT_MIN;
  static int lastStyle= INT_MIN;
  static int lastWeight= INT_MIN;
  static int lastDescender = INT_MIN;
  static int lastWidths[256]; // widths of the characters

  // get actual parameters
  const int Family = fontToUse->GetFamily();
  const int Size =   fontToUse->GetPointSize();
  const int Style =  fontToUse->GetStyle();
  const int Weight = fontToUse->GetWeight();

  // if we have another font, read the font-metrics
  if(Family!=lastFamily||Size!=lastSize||Style!=lastStyle||Weight!=lastWeight){
    // store actual values
    lastFamily = Family;
    lastSize =   Size;
    lastStyle =  Style;
    lastWeight = Weight;

    // read in new font metrics **************************************

    // 1. construct filename ******************************************
    /* MATTHEW: [2] Use wxTheFontNameDirectory */
    const char *name;

	// Julian - we'll need to do this a different way now we've removed the
	// font directory system. Must find Stefan's original code.

    name = wxTheFontNameDirectory->GetAFMName(Family, Weight, Style);
    if (!name)
      name = "unknown";

    // get the directory of the AFM files
    char afmName[256];
    afmName[0] = 0;
    if (wxGetAFMPath())
      strcpy(afmName,wxGetAFMPath());

    // 2. open and process the file **********************************

    // a short explanation of the AFM format:
    // we have for each character a line, which gives its size
    // e.g.:
    //
    //   C 63 ; WX 444 ; N question ; B 49 -14 395 676 ;
    //
    // that means, we have a character with ascii code 63, and width
    // (444/1000 * fontSize) points.
    // the other data is ignored for now!
    //
    // when the font has changed, we read in the right AFM file and store the
    // character widths in an array, which is processed below (see point 3.).

        // new elements JC Sun Aug 25 23:21:44 MET DST 1996


    strcat(afmName,name);
    strcat(afmName,".afm");
    FILE *afmFile = fopen(afmName,"r");
    if(afmFile==NULL){
      wxLogDebug("GetTextExtent: can't open AFM file '%s'\n",afmName);
      wxLogDebug("               using approximate values\n");
      int i;
      for (i=0; i<256; i++) lastWidths[i] = 500; // an approximate value
      lastDescender = -150; // dito.
    }else{
      int i;
      // init the widths array
      for(i=0; i<256; i++) lastWidths[i]= INT_MIN;
      // some variables for holding parts of a line
      char cString[10],semiString[10],WXString[10],descString[20];
      char upString[30], utString[30], encString[50];
      char line[256];
      int ascii,cWidth;
      // read in the file and parse it
      while(fgets(line,sizeof(line),afmFile)!=NULL){
        // A.) check for descender definition
        if(strncmp(line,"Descender",9)==0){
          if((sscanf(line,"%s%d",descString,&lastDescender)!=2)
	     || (strcmp(descString,"Descender")!=0)) {
	    wxLogDebug("AFM-file '%s': line '%s' has error (bad descender)\n",
		       afmName,line);
          }
        }
            // JC 1.) check for UnderlinePosition
        else if(strncmp(line,"UnderlinePosition",17)==0){
          if((sscanf(line,"%s%lf",upString,&UnderlinePosition)!=2)
	     || (strcmp(upString,"UnderlinePosition")!=0)) {
	    wxLogDebug("AFM-file '%s': line '%s' has error (bad UnderlinePosition)\n",
		       afmName,line);
          }
        }
	// JC 2.) check for UnderlineThickness
        else if(strncmp(line,"UnderlineThickness",18)==0){
           if((sscanf(line,"%s%lf",utString,&UnderlineThickness)!=2)
	     || (strcmp(utString,"UnderlineThickness")!=0)) {
	    wxLogDebug("AFM-file '%s': line '%s' has error (bad UnderlineThickness)\n",
		       afmName,line);
          }
        }
	// JC 3.) check for EncodingScheme
        else if(strncmp(line,"EncodingScheme",14)==0){
          if((sscanf(line,"%s%s",utString,encString)!=2)
	     || (strcmp(utString,"EncodingScheme")!=0)) {
	    wxLogDebug("AFM-file '%s': line '%s' has error (bad EncodingScheme)\n",
		       afmName,line);
          }
          else if (strncmp(encString, "AdobeStandardEncoding", 21))
          {
	    wxLogDebug("AFM-file '%s': line '%s' has error (unsupported EncodingScheme %s)\n",
		       afmName,line, encString);
          }
        }
            // B.) check for char-width
        else if(strncmp(line,"C ",2)==0){
          if(sscanf(line,"%s%d%s%s%d",
              cString,&ascii,semiString,WXString,&cWidth)!=5){
             wxLogDebug("AFM-file '%s': line '%s' has an error (bad character width)\n",afmName,line);
          }
          if(strcmp(cString,"C")!=0 || strcmp(semiString,";")!=0 ||
             strcmp(WXString,"WX")!=0){
             wxLogDebug("AFM-file '%s': line '%s' has a format error\n",afmName,line);
          }
          //printf("            char '%c'=%d has width '%d'\n",ascii,ascii,cWidth);
          if(ascii>=0 && ascii<256){
            lastWidths[ascii] = cWidth; // store width
          }else{
	    /* MATTHEW: this happens a lot; don't print an error */
            // wxLogDebug("AFM-file '%s': ASCII value %d out of range\n",afmName,ascii);
          }
        }
        // C.) ignore other entries.
      }
      fclose(afmFile);
    }
        // hack to compute correct values for german 'Umlaute'
        // the correct way would be to map the character names
        // like 'adieresis' to corresp. positions of ISOEnc and read
        // these values from AFM files, too. Maybe later ...
    lastWidths[196] = lastWidths['A'];  // �
    lastWidths[228] = lastWidths['a'];  // �
    lastWidths[214] = lastWidths['O'];  // �
    lastWidths[246] = lastWidths['o'];  // �
    lastWidths[220] = lastWidths['U'];  // �
    lastWidths[252] = lastWidths['u'];  // �
    lastWidths[223] = lastWidths[251];  // �
  }

      // JC: calculate UnderlineThickness/UnderlinePosition
  m_underlinePosition = m_underlinePosition * fontToUse->GetPointSize() / 1000.0f;
  m_underlineThickness = m_underlineThickness * fontToUse->GetPointSize() / 1000.0f * m_scaleFactor;

  // 3. now the font metrics are read in, calc size *******************
  // this is done by adding the widths of the characters in the
  // string. they are given in 1/1000 of the size!

  long widthSum=0;
  long height=Size; // by default
  unsigned char *p;
  for(p=(unsigned char *)(const char *)string; *p; p++){
    if(lastWidths[*p]== INT_MIN){
      wxLogDebug("GetTextExtent: undefined width for character '%c' (%d)\n",
                 *p,*p);
      widthSum += (long)(lastWidths[' ']/1000.0F * Size); // assume space
    }else{
      widthSum += (long)((lastWidths[*p]/1000.0F)*Size);
    }
  }
  // add descender to height (it is usually a negative value)
  if(lastDescender!=INT_MIN){
    height += (long)(((-lastDescender)/1000.0F) * Size); /* MATTHEW: forgot scale */
  }

  // return size values
  *x = widthSum;
  *y = height;

  // return other parameters
  if (descent){
    if(lastDescender!=INT_MIN){
      *descent = (long)(((-lastDescender)/1000.0F) * Size); /* MATTHEW: forgot scale */
    }else{
      *descent = 0;
    }
  }

  // currently no idea how to calculate this!
  // if (externalLeading) *externalLeading = 0;
  if (externalLeading)
    *externalLeading = 0;

  // ----- end of contributed code -----
#endif
}

void wxPostScriptDC::GetSizeMM(long *width, long *height) const
{
    const char *paperType = wxThePrintSetupData->GetPaperName();
  
    if (!paperType) paperType = _("A4 210 x 297 mm");

    wxPrintPaperType *paper = wxThePrintPaperDatabase->FindPaperType(paperType);
    
    if (!paper) paper = wxThePrintPaperDatabase->FindPaperType(_("A4 210 x 297 mm"));
    
    if (paper)
    {
        if (width) *width = paper->widthMM;
        if (height) *height = paper->heightMM;
    }
    else
    {
       if (width) *width = 210;
       if (height) *height = 297;
    }
}

// Determine the Default Postscript Previewer
// available on the platform
#if defined(__SUN__) && defined(__XVIEW__)
// OpenWindow/NeWS's Postscript Previewer
# define PS_VIEWER_PROG "pageview"
#elif defined(__VMS__)
#define PS_VIEWER_PROG "view/format=ps/select=x_display"
#elif defined(__SGI__)
// SGI's Display Postscript Previewer
//# define PS_VIEWER_PROG "dps"
# define PS_VIEWER_PROG "xpsview"
#elif defined(__X__) || defined(__WXGTK__)
// Front-end to ghostscript
# define PS_VIEWER_PROG "ghostview"
#else
// Windows ghostscript/ghostview
# define PS_VIEWER_PROG NULL
#endif

wxPrintSetupData *wxThePrintSetupData = (wxPrintSetupData *) NULL;

#if !USE_SHARED_LIBRARY
IMPLEMENT_DYNAMIC_CLASS(wxPostScriptModule, wxModule)
IMPLEMENT_DYNAMIC_CLASS(wxPostScriptDC, wxDC)
IMPLEMENT_DYNAMIC_CLASS(wxPrintSetupData, wxObject)
IMPLEMENT_DYNAMIC_CLASS(wxPrintPaperType, wxObject)
#endif

// Redundant now I think
#if 1
IMPLEMENT_CLASS(wxPostScriptPrintDialog, wxDialog)

wxPostScriptPrintDialog::wxPostScriptPrintDialog (wxWindow *parent, const wxString& title,
		    const wxPoint& pos, const wxSize& size, long style):
   wxDialog(parent, -1, title, pos, size, style)
{
   wxBeginBusyCursor();

   char buf[100];
   int yPos = 40;
   wxString
      *orientation = new wxString[2],
      *print_modes = new wxString[3];
   int features;
   long wx_printer_translate_x, wx_printer_translate_y;
   double wx_printer_scale_x, wx_printer_scale_y;

   orientation[0] = _("Portrait");
   orientation[1] = _("Landscape");

   print_modes[0] = _("Send to Printer");
   print_modes[1] = _("Print to File");
   print_modes[2] = _("Preview Only");


  
   wxButton *okBut = new wxButton (this, wxID_OK, _("OK"), wxPoint(5, 5));
   (void) new wxButton (this, wxID_CANCEL, _("Cancel"), wxPoint(40, 5));
   okBut->SetDefault();


#if defined(__WXGTK__) || defined (__WXMOTIF__)
   (void) new wxStaticText( this, -1, _("Printer Command: "),
                            wxPoint(5, yPos) );
   (void) new wxTextCtrl( this, wxID_PRINTER_COMMAND, wxThePrintSetupData->GetPrinterCommand(),
                          wxPoint(100, yPos), wxSize(100, -1) );

   (void) new wxStaticText( this, -1, _("Printer Options: "),
                            wxPoint(210, yPos) );
   (void) new wxTextCtrl( this, wxID_PRINTER_OPTIONS, wxThePrintSetupData->GetPrinterOptions(),
                          wxPoint(305, yPos), wxSize(150, -1) );

   yPos += 40;
#endif


   wxRadioBox *radio0 = new wxRadioBox(this, wxID_PRINTER_ORIENTATION, "Orientation: ", wxPoint(5, yPos), wxSize(-1,-1),
                                       2,orientation,2,wxRA_SPECIFY_ROWS);
   radio0->SetSelection((int)wxThePrintSetupData->GetPrinterOrientation() - 1);

  // @@@ Configuration hook
   if (wxThePrintSetupData->GetPrintPreviewCommand() == NULL)
      wxThePrintSetupData->SetPrintPreviewCommand(PS_VIEWER_PROG);

   wxGetResource ("wxWindows", "PSView", &wxThePrintSetupData->previewCommand);

   features = (wxThePrintSetupData->GetPrintPreviewCommand() &&
               *wxThePrintSetupData->GetPrintPreviewCommand()) ? 3 : 2; 
  
   wxRadioBox *radio1 = new wxRadioBox(this, wxID_PRINTER_MODES, _("PostScript:"),
                                       wxPoint(150, yPos),
                                       wxSize(-1,-1), features,
                                       print_modes, features, wxRA_SPECIFY_ROWS); 

#ifdef __WXMSW__
   radio1->Enable(0, FALSE);
   if (wxThePrintSetupData->GetPrintPreviewCommand() && *wxThePrintSetupData->GetPrintPreviewCommand())
      radio1->Enable(2, FALSE);
#endif

   radio1->SetSelection((int)wxThePrintSetupData->GetPrinterMode());
   wxThePrintSetupData->GetPrinterTranslation(&wx_printer_translate_x, &wx_printer_translate_y);
   wxThePrintSetupData->GetPrinterScaling(&wx_printer_scale_x, &wx_printer_scale_y);

   sprintf (buf, "%.2f", wx_printer_scale_x);

   yPos += 90;
   (void) new wxStaticText(this, -1, _("X Scaling"), wxPoint(5, yPos));
   /* wxTextCtrl *text1 = */ (void) new wxTextCtrl(this, wxID_PRINTER_X_SCALE, buf, wxPoint(100, yPos), wxSize(100, -1));

   sprintf (buf, "%.2f", wx_printer_scale_y);
   (void) new wxStaticText(this, -1, _("Y Scaling"), wxPoint(220, yPos));
   /* wxTextCtrl *text2 = */ (void) new wxTextCtrl(this, wxID_PRINTER_Y_SCALE, buf, wxPoint(320, yPos), wxSize(100, -1));

   yPos += 25;

   (void) new wxStaticText(this, -1, _("X Translation"), wxPoint(5, yPos));
   sprintf (buf, "%.2ld", wx_printer_translate_x);
   /* wxTextCtrl *text3 = */ (void) new wxTextCtrl(this, wxID_PRINTER_X_TRANS, buf, wxPoint(100, yPos), wxSize(100, -1));

   (void) new wxStaticText(this, -1, _("Y Translation"), wxPoint(220, yPos));
   sprintf (buf, "%.2ld", wx_printer_translate_y);
   /* wxTextCtrl *text4 = */ (void) new wxTextCtrl(this, wxID_PRINTER_Y_TRANS, buf, wxPoint(320, yPos), wxSize(100, -1));
  
   Fit ();

   delete[] orientation;
   delete[] print_modes;

   wxEndBusyCursor();
}

int wxPostScriptPrintDialog::ShowModal ()
{
  if ( wxDialog::ShowModal() == wxID_OK )
  {
//	  wxTextCtrl *text0 = (wxTextCtrl *)FindWindow(wxID_PRINTER_OPTIONS);
	  wxTextCtrl *text1 = (wxTextCtrl *)FindWindow(wxID_PRINTER_X_SCALE);
	  wxTextCtrl *text2 = (wxTextCtrl *)FindWindow(wxID_PRINTER_Y_SCALE);
	  wxTextCtrl *text3 = (wxTextCtrl *)FindWindow(wxID_PRINTER_X_TRANS);
	  wxTextCtrl *text4 = (wxTextCtrl *)FindWindow(wxID_PRINTER_Y_TRANS);
//	  wxTextCtrl *text_prt = (wxTextCtrl *)FindWindow(wxID_PRINTER_COMMAND);
	  wxRadioBox *radio0 = (wxRadioBox *)FindWindow(wxID_PRINTER_ORIENTATION);
	  wxRadioBox *radio1 = (wxRadioBox *)FindWindow(wxID_PRINTER_MODES);

      StringToDouble (WXSTRINGCAST text1->GetValue (), &wxThePrintSetupData->printerScaleX);
      StringToDouble (WXSTRINGCAST text2->GetValue (), &wxThePrintSetupData->printerScaleY);
      StringToLong (WXSTRINGCAST text3->GetValue (), &wxThePrintSetupData->printerTranslateX);
      StringToLong (WXSTRINGCAST text4->GetValue (), &wxThePrintSetupData->printerTranslateY);

#ifdef __X__
      //      wxThePrintSetupData->SetPrinterOptions(WXSTRINGCAST text0->GetValue ());
      //      wxThePrintSetupData->SetPrinterCommand(WXSTRINGCAST text_prt->GetValue ());
#endif

      wxThePrintSetupData->SetPrinterOrientation((radio0->GetSelection() == 1 ? PS_LANDSCAPE : PS_PORTRAIT));

      // C++ wants this
      switch ( radio1->GetSelection() ) {
		case 0: wxThePrintSetupData->SetPrinterMode(PS_PRINTER); break;
		case 1:  wxThePrintSetupData->SetPrinterMode(PS_FILE); break;
		case 2: wxThePrintSetupData->SetPrinterMode(PS_PREVIEW); break;
      }
	  return wxID_OK;
  }
  return wxID_CANCEL;
}
#endif
 // 0 (redundant)

// PostScript printer settings
// RETAINED FOR BACKWARD COMPATIBILITY
void wxSetPrinterCommand(const char *cmd)
{
  wxThePrintSetupData->SetPrinterCommand(cmd);
}

void wxSetPrintPreviewCommand(const char *cmd)
{
  wxThePrintSetupData->SetPrintPreviewCommand(cmd);
}

void wxSetPrinterOptions(const char *flags)
{
  wxThePrintSetupData->SetPrinterOptions(flags);
}

void wxSetPrinterFile(const char *f)
{
  wxThePrintSetupData->SetPrinterFile(f);
}

void wxSetPrinterOrientation(int orient)
{
  wxThePrintSetupData->SetPrinterOrientation(orient);
}

void wxSetPrinterScaling(double x, double y)
{
  wxThePrintSetupData->SetPrinterScaling(x, y);
}

void wxSetPrinterTranslation(long x, long y)
{
  wxThePrintSetupData->SetPrinterTranslation(x, y);
}

// 1 = Preview, 2 = print to file, 3 = send to printer
void wxSetPrinterMode(int mode)
{
  wxThePrintSetupData->SetPrinterMode(mode);
}

void wxSetAFMPath(const char *f)
{
  wxThePrintSetupData->SetAFMPath(f);
}

// Get current values
char *wxGetPrinterCommand()
{
  return wxThePrintSetupData->GetPrinterCommand();
}

char *wxGetPrintPreviewCommand()
{
  return wxThePrintSetupData->GetPrintPreviewCommand();
}

char *wxGetPrinterOptions()
{
  return wxThePrintSetupData->GetPrinterOptions();
}

char *wxGetPrinterFile()
{
  return wxThePrintSetupData->GetPrinterFile();
}

int wxGetPrinterOrientation()
{
  return wxThePrintSetupData->GetPrinterOrientation();
}

void wxGetPrinterScaling(double* x, double* y)
{
  wxThePrintSetupData->GetPrinterScaling(x, y);
}

void wxGetPrinterTranslation(long *x, long *y)
{
  wxThePrintSetupData->GetPrinterTranslation(x, y);
}

int wxGetPrinterMode()
{
  return wxThePrintSetupData->GetPrinterMode();
}

char *wxGetAFMPath()
{
  return wxThePrintSetupData->GetAFMPath();
}

/*
 * Print setup data
 */

wxPrintSetupData::wxPrintSetupData()
{
  printerCommand = (char *) NULL;
  previewCommand = (char *) NULL;
  printerFlags = (char *) NULL;
  printerOrient = PS_PORTRAIT;
  printerScaleX = (double)1.0;
  printerScaleY = (double)1.0;
  printerTranslateX = 0;
  printerTranslateY = 0;
  // 1 = Preview, 2 = print to file, 3 = send to printer
  printerMode = 3;
  afmPath = (char *) NULL;
  paperName = (char *) NULL;
  printColour = TRUE;
  printerFile = (char *) NULL;
}

wxPrintSetupData::~wxPrintSetupData()
{
  if (printerCommand)
    delete[] printerCommand;
  if (previewCommand)
    delete[] previewCommand;
  if (printerFlags)
    delete[] printerFlags;
  if (afmPath)
    delete[] afmPath;
  if (paperName)
    delete[] paperName;
  if (printerFile)
    delete[] printerFile;
}

void wxPrintSetupData::SetPrinterCommand(const char *cmd)
{
  if (cmd == printerCommand)
    return;

  if (printerCommand)
    delete[] printerCommand;
  if (cmd)
    printerCommand = copystring(cmd);
  else
    printerCommand = (char *) NULL;
}

void wxPrintSetupData::SetPrintPreviewCommand(const char *cmd)
{
  if (cmd == previewCommand)
    return;

  if (previewCommand)
    delete[] previewCommand;
  if (cmd)
    previewCommand = copystring(cmd);
  else
    previewCommand = (char *) NULL;
}

void wxPrintSetupData::SetPaperName(const char *name)
{
  if (name == paperName)
    return;

  if (paperName)
    delete[] paperName;
  if (name)
    paperName = copystring(name);
  else
    paperName = (char *) NULL;
}

void wxPrintSetupData::SetPrinterOptions(const char *flags)
{
  if (printerFlags == flags)
    return;

  if (printerFlags)
    delete[] printerFlags;
  if (flags)
    printerFlags = copystring(flags);
  else
    printerFlags = (char *) NULL;
}

void wxPrintSetupData::SetPrinterFile(const char *f)
{
  if (f == printerFile)
    return;

  if (printerFile)
    delete[] printerFile;
  if (f)
    printerFile = copystring(f);
  else
    printerFile = (char *) NULL;
}

void wxPrintSetupData::SetPrinterOrientation(int orient)
{
  printerOrient = orient;
}

void wxPrintSetupData::SetPrinterScaling(double x, double y)
{
  printerScaleX = x;
  printerScaleY = y;
}

void wxPrintSetupData::SetPrinterTranslation(long x, long y)
{
  printerTranslateX = x;
  printerTranslateY = y;
}

// 1 = Preview, 2 = print to file, 3 = send to printer
void wxPrintSetupData::SetPrinterMode(int mode)
{
  printerMode = mode;
}

void wxPrintSetupData::SetAFMPath(const char *f)
{
  if (f == afmPath)
    return;

  if (afmPath)
    delete[] afmPath;
  if (f)
    afmPath = copystring(f);
  else
    afmPath = (char *) NULL;
}

void wxPrintSetupData::SetColour(bool col)
{
  printColour = col;
}

// Get current values
char *wxPrintSetupData::GetPrinterCommand()
{
  return printerCommand;
}

char *wxPrintSetupData::GetPrintPreviewCommand()
{
  return previewCommand;
}

char *wxPrintSetupData::GetPrinterOptions()
{
  return printerFlags;
}

char *wxPrintSetupData::GetPrinterFile()
{
  return printerFile;
}

char *wxPrintSetupData::GetPaperName()
{
  return paperName;
}

int wxPrintSetupData::GetPrinterOrientation()
{
  return printerOrient;
}

void wxPrintSetupData::GetPrinterScaling(double *x, double *y)
{
  *x = printerScaleX;
  *y = printerScaleY;
}

void wxPrintSetupData::GetPrinterTranslation(long *x, long *y)
{
  *x = printerTranslateX;
  *y = printerTranslateY;
}

int wxPrintSetupData::GetPrinterMode()
{
  return printerMode;
}

char *wxPrintSetupData::GetAFMPath()
{
  return afmPath;
}

bool wxPrintSetupData::GetColour()
{
  return printColour;
}

void wxPrintSetupData::operator=(wxPrintSetupData& data)
{
  SetPrinterCommand(data.GetPrinterCommand());
  SetPrintPreviewCommand(data.GetPrintPreviewCommand());
  SetPrinterOptions(data.GetPrinterOptions());
  long x, y;
  data.GetPrinterTranslation(&x, &y);
  SetPrinterTranslation(x, y);

  double x1, y1;
  data.GetPrinterScaling(&x1, &y1);
  SetPrinterScaling(x1, y1);

  SetPrinterOrientation(data.GetPrinterOrientation());
  SetPrinterMode(data.GetPrinterMode());
  SetAFMPath(data.GetAFMPath());
  SetPaperName(data.GetPaperName());
  SetColour(data.GetColour());
}

void wxInitializePrintSetupData(bool init)
{
  if (init)
  {
    wxThePrintSetupData = new wxPrintSetupData;

    wxThePrintSetupData->SetPrintPreviewCommand(PS_VIEWER_PROG);
    wxThePrintSetupData->SetPrinterOrientation(PS_PORTRAIT);
    wxThePrintSetupData->SetPrinterMode(PS_PREVIEW);
    wxThePrintSetupData->SetPaperName(_("A4 210 x 297 mm"));

    // Could have a .ini file to read in some defaults
    // - and/or use environment variables, e.g. WXWIN
#ifdef __VMS__
    wxThePrintSetupData->SetPrinterCommand("print");
    wxThePrintSetupData->SetPrinterOptions("/nonotify/queue=psqueue");
    wxThePrintSetupData->SetAFMPath("sys$ps_font_metrics:");
#endif
#ifdef __WXMSW__
    wxThePrintSetupData->SetPrinterCommand("print");
    wxThePrintSetupData->SetAFMPath("c:\\windows\\system\\");
    wxThePrintSetupData->SetPrinterOptions(NULL);
#endif
#if !defined(__VMS__) && !defined(__WXMSW__)
    wxThePrintSetupData->SetPrinterCommand("lpr");
    wxThePrintSetupData->SetPrinterOptions((char *) NULL);
    wxThePrintSetupData->SetAFMPath((char *) NULL);
#endif
  }
  else
  {
    if (wxThePrintSetupData)
      delete wxThePrintSetupData;
     wxThePrintSetupData = (wxPrintSetupData *) NULL;
  }
}

/*
 * Paper size database for PostScript
 */

wxPrintPaperType::wxPrintPaperType(const char *name, int wmm, int hmm, int wp, int hp)
{
  widthMM = wmm;
  heightMM = hmm;
  widthPixels = wp;
  heightPixels = hp;
  pageName = copystring(name);
}

wxPrintPaperType::~wxPrintPaperType()
{
  delete[] pageName;
}

/*
 * Print paper database for PostScript
 */

#if !USE_SHARED_LIBRARIES
IMPLEMENT_DYNAMIC_CLASS(wxPrintPaperDatabase, wxList)
#endif

wxPrintPaperDatabase::wxPrintPaperDatabase():wxList(wxKEY_STRING)
{
  DeleteContents(TRUE);
}

wxPrintPaperDatabase::~wxPrintPaperDatabase()
{
}

void wxPrintPaperDatabase::CreateDatabase()
{
  // Need correct values for page size in pixels.
  // Each unit is one 'point' = 1/72 of an inch.
  // NOTE: WE NEED ALSO TO MAKE ADJUSTMENTS WHEN TRANSLATING
  // in wxPostScriptDC code, so we can start from top left.
  // So access this database and translate by appropriate number
  // of points for this paper size. OR IS IT OK ALREADY?
  // Can't remember where the PostScript origin is by default.
  // Heck, someone will know how to make it hunky-dory...
  // JACS 25/5/95

  AddPaperType(_("A4 210 x 297 mm"), 210, 297,         595, 842);
  AddPaperType(_("A3 297 x 420 mm"), 297, 420,         842, 1191);
  AddPaperType(_("Letter 8 1/2 x 11 in"), 216, 279,    612, 791);
  AddPaperType(_("Legal 8 1/2 x 14 in"), 216, 356,     612, 1009);
  
/*
  This is for 100 ppi

  AddPaperType(_("A4 210 x 297 mm"), 210, 297,         210*4, 297*4 );
  AddPaperType(_("A3 297 x 420 mm"), 297, 420,         297*4, 420*4 );
  AddPaperType(_("Letter 8 1/2 x 11 in"), 216, 279,    216*4, 279*4 );
  AddPaperType(_("Legal 8 1/2 x 14 in"), 216, 356,     216*4, 356*4 );
*/
}

void wxPrintPaperDatabase::ClearDatabase()
{
  Clear();
}

void wxPrintPaperDatabase::AddPaperType(const char *name, int wmm, int hmm, int wp, int hp)
{
  Append(name, new wxPrintPaperType(name, wmm, hmm, wp, hp));
}

wxPrintPaperType *wxPrintPaperDatabase::FindPaperType(const char *name)
{
  wxNode *node = Find(name);
  if (node)
    return (wxPrintPaperType *)node->Data();
  else
    return (wxPrintPaperType *) NULL;
}

/*
 * Initialization/cleanup module
 */

bool wxPostScriptModule::OnInit()
{
    wxInitializePrintSetupData();
    wxThePrintPaperDatabase = new wxPrintPaperDatabase;
    wxThePrintPaperDatabase->CreateDatabase();

    return TRUE;
}

void wxPostScriptModule::OnExit()
{
    wxInitializePrintSetupData(FALSE);
    delete wxThePrintPaperDatabase;
    wxThePrintPaperDatabase = NULL;
}

#endif
  // wxUSE_POSTSCRIPT
