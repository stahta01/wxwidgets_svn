///////////////////////////////////////////////////////////////////////////////
// Name:        bombs1.cpp
// Purpose:     Bombs game
// Author:      P. Foggia 1996
// Modified by:
// Created:     1996
// RCS-ID:      $Id$
// Copyright:   (c) 1996 P. Foggia
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

/*
 * implementation of the methods DrawField and OnEvent of the
 * class BombsCanvas
 */

#ifdef __GNUG__
#pragma implementation
#endif

#include "wx/wxprec.h"

#ifndef  WX_PRECOMP
  #include "wx/wx.h"
#endif //precompiled headers

#include "bombs.h"

/*--------  BombCanvasClass::DrawField(dc, xc1, yc1, xc2, yc2)  -------*/
/* Draws the field on the device context dc                            */
/* xc1,yc1 etc. are the (inclusive) limits of the area to be drawn,    */
/* expressed in cells.                                                 */
/*---------------------------------------------------------------------*/
void BombsCanvasClass::DrawField(wxDC *dc, int xc1, int yc1, int xc2, int yc2)
{ int x,y,xmax,ymax;
  wxChar buf[2];
  long chw, chh;

  wxColour *wxBlack = wxTheColourDatabase->FindColour(_T("BLACK"));
  wxColour *wxWhite = wxTheColourDatabase->FindColour(_T("WHITE"));
  wxColour *wxRed = wxTheColourDatabase->FindColour(_T("RED"));
  wxColour *wxBlue = wxTheColourDatabase->FindColour(_T("BLUE"));
  wxColour *wxGrey = wxTheColourDatabase->FindColour(_T("LIGHT GREY"));
  wxColour *wxGreen = wxTheColourDatabase->FindColour(_T("GREEN"));

  wxPen *blackPen = wxThePenList->FindOrCreatePen(*wxBlack, 1, wxSOLID);
  wxPen *redPen = wxThePenList->FindOrCreatePen(*wxRed, 1, wxSOLID);
  wxPen *bluePen = wxThePenList->FindOrCreatePen(*wxBlue, 1, wxSOLID);
  wxBrush *whiteBrush = wxTheBrushList->FindOrCreateBrush(*wxWhite, wxSOLID);
  wxBrush *greyBrush = wxTheBrushList->FindOrCreateBrush(*wxGrey, wxSOLID);
  wxBrush *redBrush = wxTheBrushList->FindOrCreateBrush(*wxRed, wxSOLID);

  xmax=field_width*x_cell*X_UNIT;
  ymax=field_height*y_cell*Y_UNIT;


  dc->SetPen(* blackPen);
  for(x=xc1; x<=xc2; x++)
    dc->DrawLine(x*x_cell*X_UNIT, 0, x*x_cell*X_UNIT, ymax);
  for(y=xc1; y<=yc2; y++)
    dc->DrawLine(0, y*y_cell*Y_UNIT, xmax, y*y_cell*Y_UNIT);


  wxFont font= BOMBS_FONT;
  dc->SetFont(font); 

  buf[1]=_T('\0');
  for(x=xc1; x<=xc2; x++)
    for(y=yc1; y<=yc2; y++)
      { if (wxGetApp().Game.IsMarked(x,y))
          { dc->SetPen(* blackPen);
            dc->SetBrush(* greyBrush);
            dc->DrawRectangle( x*x_cell*X_UNIT, y*y_cell*Y_UNIT,
                               x_cell*X_UNIT+1, y_cell*Y_UNIT+1);
            *buf=_T('M');
            if (!wxGetApp().Game.IsHidden(x,y) && wxGetApp().Game.IsBomb(x,y))
              dc->SetTextForeground(*wxBlue);
            else
              dc->SetTextForeground(*wxRed);
            dc->SetTextBackground(*wxGrey);
            dc->GetTextExtent(buf, &chw, &chh);
            dc->DrawText( buf,
                  x*x_cell*X_UNIT + (x_cell*X_UNIT-chw)/2,
                  y*y_cell*Y_UNIT + (y_cell*Y_UNIT-chh)/2
                );
            if (!wxGetApp().Game.IsHidden(x,y) && wxGetApp().Game.IsBomb(x,y))
              { dc->SetPen(*redPen);
                dc->DrawLine(x*x_cell*X_UNIT, y*y_cell*Y_UNIT,
                             (x+1)*x_cell*X_UNIT, (y+1)*y_cell*Y_UNIT);
                dc->DrawLine(x*x_cell*X_UNIT, (y+1)*y_cell*Y_UNIT,
                             (x+1)*x_cell*X_UNIT, y*y_cell*Y_UNIT);
              }
          }
        else if (wxGetApp().Game.IsHidden(x,y))
          { dc->SetPen(*blackPen);
            dc->SetBrush(*greyBrush);
            dc->DrawRectangle( x*x_cell*X_UNIT, y*y_cell*Y_UNIT,
                               x_cell*X_UNIT+1, y_cell*Y_UNIT+1);
          }
        else if (wxGetApp().Game.IsBomb(x,y))
          { dc->SetPen(* blackPen);
            dc->SetBrush(* redBrush);
            dc->DrawRectangle( x*x_cell*X_UNIT, y*y_cell*Y_UNIT,
                               x_cell*X_UNIT+1, y_cell*Y_UNIT+1);
            *buf=_T('B');
            dc->SetTextForeground(* wxBlack);
            dc->SetTextBackground(* wxRed);
            dc->GetTextExtent(buf, &chw, &chh);
            dc->DrawText( buf,
                  x*x_cell*X_UNIT + (x_cell*X_UNIT-chw)/2,
                  y*y_cell*Y_UNIT + (y_cell*Y_UNIT-chh)/2
                );
            if (wxGetApp().Game.IsExploded(x,y))
              { dc->SetPen(* bluePen);
                dc->DrawLine(x*x_cell*X_UNIT, y*y_cell*Y_UNIT,
                             (x+1)*x_cell*X_UNIT, (y+1)*y_cell*Y_UNIT);
                dc->DrawLine(x*x_cell*X_UNIT, (y+1)*y_cell*Y_UNIT,
                             (x+1)*x_cell*X_UNIT, y*y_cell*Y_UNIT);
              }
          }
        else   // Display a digit
          { dc->SetPen(* blackPen);
            dc->SetBrush(* whiteBrush);
            dc->DrawRectangle( x*x_cell*X_UNIT, y*y_cell*Y_UNIT,
                               x_cell*X_UNIT+1, y_cell*Y_UNIT+1);
            *buf = (wxGetApp().Game.Get(x,y) & BG_MASK) + _T('0');
            dc->GetTextExtent(buf, &chw, &chh);
            switch(*buf)
              { case _T('0'): dc->SetTextForeground(* wxGreen); break;
                case _T('1'): dc->SetTextForeground(* wxBlue); break;
                default:  dc->SetTextForeground(* wxBlack); break;
              }
            dc->SetTextBackground(* wxWhite);
            dc->DrawText( buf,
                  x*x_cell*X_UNIT + (x_cell*X_UNIT-chw)/2,
                  y*y_cell*Y_UNIT + (y_cell*Y_UNIT-chh)/2
                );
          }
      }
  dc->SetFont(wxNullFont);

  if (wxGetApp().BombsFrame)
    { wxString buf;
      buf.Printf(_T("%d bombs  %d remaining cells"),
                 wxGetApp().Game.GetBombs(),
                 wxGetApp().Game.GetRemainingCells());
      wxGetApp().BombsFrame->SetStatusText(buf, 0);
    }
}

/*--------  BombCanvasClass::Refresh(xc1, yc1, xc2, yc2)  -------------*/
/* Refreshes the field image                                           */
/* xc1,yc1 etc. are the (inclusive) limits of the area to be drawn,    */
/* expressed in cells.                                                 */
/*---------------------------------------------------------------------*/
void BombsCanvasClass::Refresh(int xc1, int yc1, int xc2, int yc2)
  {
    wxClientDC dc(this);
    DrawField(& dc, xc1, yc1, xc2, yc2);
    if (bmp)
      { wxMemoryDC memDC;
        memDC.SelectObject(* bmp);
        DrawField(&memDC, xc1, yc1, xc2, yc2);
        memDC.SelectObject(wxNullBitmap);
      }
  }

// Called when uncovering a cell.
void BombsCanvasClass::Uncover(int x, int y)
{
  wxGetApp().Game.Unhide(x,y);
  Refresh(x, y, x, y);
  if (wxGetApp().Game.IsBomb(x,y) || wxGetApp().Game.GetRemainingCells()==0)
    { wxBell();
      if (!wxGetApp().Game.IsBomb(x,y))
       { wxMessageBox(_T("Nice! You found all the bombs!"), _T("wxWin Bombs"),
                      wxOK|wxCENTRE, wxGetApp().BombsFrame);
       }
      else // x,y is a bomb
       { wxGetApp().Game.Explode(x, y);
       }
      for(x=0; x<field_width; x++)
       for(y=0; y<field_height; y++)
         wxGetApp().Game.Unhide(x,y);
      Refresh(0, 0, field_width-1, field_height-1);
    }
  else if (!wxGetApp().Game.Get(x, y))
    { int left = ( x > 0 ) ? x-1 : 0;
      int right = ( x < wxGetApp().Game.GetWidth() - 1 )?
       x+1 : wxGetApp().Game.GetWidth() - 1;
      int top = ( y > 0 ) ? y-1 : 0;
      int bottom = ( y < wxGetApp().Game.GetHeight() - 1 )?
       y+1 : wxGetApp().Game.GetHeight() - 1;
      int i,j;
      for (j = top; j <= bottom; j++)
       for (i=left; i <= right; i++)
         if ((i != x || j != y) && wxGetApp().Game.IsHidden(i,j)
             && !wxGetApp().Game.IsMarked(i,j))
           Uncover(i,j);
    }
}

// Called when the canvas receives a mouse event.
void BombsCanvasClass::OnEvent(wxMouseEvent& event)
{
  wxCoord fx, fy;
  event.GetPosition(&fx, &fy);
  int x = fx/(x_cell*X_UNIT);
  int y = fy/(y_cell*Y_UNIT);
  if (x<field_width && y<field_height)
    { if ( (event.RightDown() || (event.LeftDown() && event.ShiftDown()))
           && (wxGetApp().Game.IsHidden(x,y)
               || wxGetApp().Game.GetRemainingCells()==0))
        { wxGetApp().Game.Mark(x,y);
          Refresh(x, y, x, y);
          return;
        }
      else if (event.LeftDown() && wxGetApp().Game.IsHidden(x,y)
               && !wxGetApp().Game.IsMarked(x,y))
        { Uncover(x,y);
         return;
        }
    }
}

