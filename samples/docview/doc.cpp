/////////////////////////////////////////////////////////////////////////////
// Name:        doc.cpp
// Purpose:     Implements document functionality
// Author:      Julian Smart
// Modified by:
// Created:     04/01/98
// RCS-ID:      $Id$
// Copyright:   (c) Julian Smart and Markus Holzem
// Licence:   	wxWindows license
/////////////////////////////////////////////////////////////////////////////

#ifdef __GNUG__
// #pragma implementation
#endif

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#if !USE_DOC_VIEW_ARCHITECTURE
#error You must set USE_DOC_VIEW_ARCHITECTURE to 1 in wx_setup.h!
#endif

#include "doc.h"
#include "view.h"

IMPLEMENT_DYNAMIC_CLASS(DrawingDocument, wxDocument)

DrawingDocument::DrawingDocument(void)
{
}

DrawingDocument::~DrawingDocument(void)
{
  doodleSegments.DeleteContents(TRUE);
}

ostream& DrawingDocument::SaveObject(ostream& stream)
{
  wxDocument::SaveObject(stream);

  stream << doodleSegments.Number() << '\n';
  wxNode *node = doodleSegments.First();
  while (node)
  {
    DoodleSegment *segment = (DoodleSegment *)node->Data();
    segment->SaveObject(stream);
    stream << '\n';
    
    node = node->Next();
  }
  return stream;
}

istream& DrawingDocument::LoadObject(istream& stream)
{
  wxDocument::LoadObject(stream);

  int n = 0;
  stream >> n;

  for (int i = 0; i < n; i++)
  {
    DoodleSegment *segment = new DoodleSegment;
    segment->LoadObject(stream);
    doodleSegments.Append(segment);
  }

  return stream;
}

DoodleSegment::DoodleSegment(void)
{
}

DoodleSegment::DoodleSegment(DoodleSegment& seg)
{
  wxNode *node = seg.lines.First();
  while (node)
  {
    DoodleLine *line = (DoodleLine *)node->Data();
    DoodleLine *newLine = new DoodleLine;
    newLine->x1 = line->x1;
    newLine->y1 = line->y1;
    newLine->x2 = line->x2;
    newLine->y2 = line->y2;

    lines.Append(newLine);

    node = node->Next();
  }
}

DoodleSegment::~DoodleSegment(void)
{
  lines.DeleteContents(TRUE);
}

ostream& DoodleSegment::SaveObject(ostream& stream)
{
  stream << lines.Number() << '\n';
  wxNode *node = lines.First();
  while (node)
  {
    DoodleLine *line = (DoodleLine *)node->Data();
    stream << line->x1 << " " << line->y1 << " " << line->x2 << " " << line->y2 << "\n";
    node = node->Next();
  }
  return stream;
}

istream& DoodleSegment::LoadObject(istream& stream)
{
  int n = 0;
  stream >> n;

  for (int i = 0; i < n; i++)
  {
    DoodleLine *line = new DoodleLine;
    stream >> line->x1 >> line->y1 >> line->x2 >> line->y2;
    lines.Append(line);
  }
  return stream;
}

void DoodleSegment::Draw(wxDC *dc)
{
  wxNode *node = lines.First();
  while (node)
  {
    DoodleLine *line = (DoodleLine *)node->Data();
    dc->DrawLine(line->x1, line->y1, line->x2, line->y2);
    node = node->Next();
  }
}

/*
 * Implementation of drawing command
 */

DrawingCommand::DrawingCommand(const wxString& name, int command, DrawingDocument *ddoc, DoodleSegment *seg):
  wxCommand(TRUE, name)
{
  doc = ddoc;
  segment = seg;
  cmd = command;
}

DrawingCommand::~DrawingCommand(void)
{
  if (segment)
    delete segment;
}

bool DrawingCommand::Do(void)
{
  switch (cmd)
  {
    case DOODLE_CUT:
    {
      // Cut the last segment
      if (doc->GetDoodleSegments().Number() > 0)
      {
        wxNode *node = doc->GetDoodleSegments().Last();
        if (segment)
          delete segment;
          
        segment = (DoodleSegment *)node->Data();
        delete node;

        doc->Modify(TRUE);
        doc->UpdateAllViews();
      }
      break;
    }
    case DOODLE_ADD:
    {
      doc->GetDoodleSegments().Append(new DoodleSegment(*segment));
      doc->Modify(TRUE);
      doc->UpdateAllViews();
      break;
    }
  }
  return TRUE;
}

bool DrawingCommand::Undo(void)
{
  switch (cmd)
  {
    case DOODLE_CUT:
    {
      // Paste the segment
      if (segment)
      {
        doc->GetDoodleSegments().Append(segment);
        doc->Modify(TRUE);
        doc->UpdateAllViews();
        segment = NULL;
      }
      doc->Modify(TRUE);
      doc->UpdateAllViews();
      break;
    }
    case DOODLE_ADD:
    {
      // Cut the last segment
      if (doc->GetDoodleSegments().Number() > 0)
      {
        wxNode *node = doc->GetDoodleSegments().Last();
        DoodleSegment *seg = (DoodleSegment *)node->Data();
        delete seg;
        delete node;

        doc->Modify(TRUE);
        doc->UpdateAllViews();
      }
    }
  }
  return TRUE;
}

IMPLEMENT_DYNAMIC_CLASS(TextEditDocument, wxDocument)

// Since text windows have their own method for saving to/loading from files,
// we override OnSave/OpenDocument instead of Save/LoadObject
bool TextEditDocument::OnSaveDocument(const wxString& filename)
{
    TextEditView *view = (TextEditView *)GetFirstView();

    if (!view->textsw->SaveFile(filename))
        return FALSE;
    Modify(FALSE);
    return TRUE;
}

bool TextEditDocument::OnOpenDocument(const wxString& filename)
{
    TextEditView *view = (TextEditView *)GetFirstView();
    if (!view->textsw->LoadFile(filename))
        return FALSE;

    SetFilename(filename, TRUE);
    Modify(FALSE);
    UpdateAllViews();
    return TRUE;
}

bool TextEditDocument::IsModified(void) const
{
  TextEditView *view = (TextEditView *)GetFirstView();
  if (view)
  {
    return (wxDocument::IsModified() || view->textsw->Modified());
  }
  else
    return wxDocument::IsModified();
}

void TextEditDocument::Modify(bool mod)
{
  TextEditView *view = (TextEditView *)GetFirstView();

  wxDocument::Modify(mod);

  if (!mod && view && view->textsw)
    view->textsw->DiscardEdits();
}
