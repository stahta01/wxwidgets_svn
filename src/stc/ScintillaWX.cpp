////////////////////////////////////////////////////////////////////////////
// Name:        ScintillaWX.cxx
// Purpose:     A wxWindows implementation of Scintilla.  A class derived
//              from ScintillaBase that uses the "wx platform" defined in
//              PlatformWX.cxx  This class is one end of a bridge between
//              the wx world and the Scintilla world.  It needs a peer
//              object of type wxStyledTextCtrl to function.
//
// Author:      Robin Dunn
//
// Created:     13-Jan-2000
// RCS-ID:      $Id$
// Copyright:   (c) 2000 by Total Control Software
// Licence:     wxWindows license
/////////////////////////////////////////////////////////////////////////////

#include <ctype.h>

#include "ScintillaWX.h"
#include "wx/stc/stc.h"


//----------------------------------------------------------------------

const int H_SCROLL_MAX  = 2000;
const int H_SCROLL_STEP = 20;
const int H_SCROLL_PAGE = 200;

//----------------------------------------------------------------------
// Helper classes

class wxSTCTimer : public wxTimer {
public:
    wxSTCTimer(ScintillaWX* swx) {
        this->swx = swx;
    }

    void Notify() {
        swx->DoTick();
    }

private:
    ScintillaWX* swx;
};



bool wxSTCDropTarget::OnDropText(wxCoord x, wxCoord y, const wxString& data) {
    return swx->DoDropText(x, y, data);
}

wxDragResult  wxSTCDropTarget::OnEnter(wxCoord x, wxCoord y, wxDragResult def) {
    return swx->DoDragEnter(x, y, def);
}

wxDragResult  wxSTCDropTarget::OnDragOver(wxCoord x, wxCoord y, wxDragResult def) {
    return swx->DoDragOver(x, y, def);
}

void  wxSTCDropTarget::OnLeave() {
    swx->DoDragLeave();
}


class wxSTCCallTip : public wxWindow {
public:
    wxSTCCallTip(wxWindow* parent, int ID, CallTip* ct)
        : wxWindow(parent, ID)
        {
            m_ct = ct;
        }

    void OnPaint(wxPaintEvent& evt) {
        wxPaintDC dc(this);
        Surface surfaceWindow;
        surfaceWindow.Init(&dc);
        m_ct->PaintCT(&surfaceWindow);
        surfaceWindow.Release();
    }

    CallTip*    m_ct;
    DECLARE_EVENT_TABLE()
};

BEGIN_EVENT_TABLE(wxSTCCallTip, wxWindow)
    EVT_PAINT(wxSTCCallTip::OnPaint)
END_EVENT_TABLE()

//----------------------------------------------------------------------
// Constructor/Destructor


ScintillaWX::ScintillaWX(wxStyledTextCtrl* win) {
    capturedMouse = false;
    wMain = win;
    stc   = win;
    wheelRotation = 0;
    Initialise();
}


ScintillaWX::~ScintillaWX() {
    SetTicking(false);
}

//----------------------------------------------------------------------
// base class virtuals


void ScintillaWX::Initialise() {
    //ScintillaBase::Initialise();
    dropTarget = new wxSTCDropTarget;
    dropTarget->SetScintilla(this);
    stc->SetDropTarget(dropTarget);
}


void ScintillaWX::Finalise() {
    ScintillaBase::Finalise();
}


void ScintillaWX::StartDrag() {
    wxString dragText(drag.s, drag.len);

    // Send an event to allow the drag text to be changed
    wxStyledTextEvent evt(wxEVT_STC_START_DRAG, stc->GetId());
    evt.SetEventObject(stc);
    evt.SetDragText(dragText);
    evt.SetDragAllowMove(TRUE);
    evt.SetPosition(wxMin(stc->GetSelectionStart(),
                          stc->GetSelectionEnd()));
    stc->GetEventHandler()->ProcessEvent(evt);
    dragText = evt.GetDragText();

    if (dragText.Length()) {
        wxDropSource        source(wMain.GetID());
        wxTextDataObject    data(dragText);
        wxDragResult        result;

        source.SetData(data);
        dropWentOutside = TRUE;
        result = source.DoDragDrop(evt.GetDragAllowMove());
        if (result == wxDragMove && dropWentOutside)
            ClearSelection();
        inDragDrop = FALSE;
        SetDragPosition(invalidPosition);
    }
}


void ScintillaWX::SetTicking(bool on) {
    wxSTCTimer* steTimer;
    if (timer.ticking != on) {
        timer.ticking = on;
        if (timer.ticking) {
            steTimer = new wxSTCTimer(this);
            steTimer->Start(timer.tickSize);
            timer.tickerID = (int)steTimer;
        } else {
            steTimer = (wxSTCTimer*)timer.tickerID;
            steTimer->Stop();
            delete steTimer;
            timer.tickerID = 0;
        }
    }
    timer.ticksToWait = caret.period;
}


void ScintillaWX::SetMouseCapture(bool on) {
    if (on && !capturedMouse)
        wMain.GetID()->CaptureMouse();
    else if (!on && capturedMouse)
        wMain.GetID()->ReleaseMouse();
    capturedMouse = on;
}


bool ScintillaWX::HaveMouseCapture() {
    return capturedMouse;
}


void ScintillaWX::ScrollText(int linesToMove) {
    int dy = vs.lineHeight * (linesToMove);
    wMain.GetID()->ScrollWindow(0, dy);
    wMain.GetID()->Update();
}

void ScintillaWX::SetVerticalScrollPos() {
    wMain.GetID()->SetScrollPos(wxVERTICAL, topLine);
}

void ScintillaWX::SetHorizontalScrollPos() {
    wMain.GetID()->SetScrollPos(wxHORIZONTAL, xOffset);
}


bool ScintillaWX::ModifyScrollBars(int nMax, int nPage) {
    bool modified = false;
    int  sbMax    = wMain.GetID()->GetScrollRange(wxVERTICAL);
    int  sbThumb  = wMain.GetID()->GetScrollThumb(wxVERTICAL);
    int  sbPos    = wMain.GetID()->GetScrollPos(wxVERTICAL);


    if (sbMax != nMax || sbThumb != nPage) {
        wMain.GetID()->SetScrollbar(wxVERTICAL, sbPos, nPage, nMax);
        modified = true;
    }

    sbMax    = wMain.GetID()->GetScrollRange(wxHORIZONTAL);
    sbThumb  = wMain.GetID()->GetScrollThumb(wxHORIZONTAL);
    if ((sbMax != H_SCROLL_MAX) || (sbThumb != H_SCROLL_STEP)) {
        wMain.GetID()->SetScrollbar(wxHORIZONTAL, 0, H_SCROLL_STEP, H_SCROLL_MAX);
        modified = true;
    }
    return modified;
}


void ScintillaWX::NotifyChange() {
    stc->NotifyChange();
}


void ScintillaWX::NotifyParent(SCNotification scn) {
    stc->NotifyParent(&scn);
}



void ScintillaWX::Copy() {
    if (currentPos != anchor) {
        SelectionText st;
        CopySelectionRange(&st);
        wxTheClipboard->Open();
        wxTheClipboard->SetData(new wxTextDataObject(wxString(st.s, st.len)));
        wxTheClipboard->Close();
    }
}


void ScintillaWX::Paste() {
    pdoc->BeginUndoAction();
    ClearSelection();

    wxTextDataObject data;
    bool gotData;

    wxTheClipboard->Open();
    gotData = wxTheClipboard->GetData(data);
    wxTheClipboard->Close();
    if (gotData) {
        wxString str = data.GetText();
        int      len = str.Length();
        pdoc->InsertString(currentPos, str.c_str(), len);
        SetEmptySelection(currentPos + len);
    }

    pdoc->EndUndoAction();
    NotifyChange();
    Redraw();
}


bool ScintillaWX::CanPaste() {
    bool canPaste;

    wxTheClipboard->Open();
    canPaste = wxTheClipboard->IsSupported( wxDF_TEXT );
    wxTheClipboard->Close();

    return canPaste;
}

void ScintillaWX::CreateCallTipWindow(PRectangle) {
    ct.wCallTip = new wxSTCCallTip(wMain.GetID(), -1, &ct);
    ct.wDraw = ct.wCallTip;
}


void ScintillaWX::AddToPopUp(const char *label, int cmd, bool enabled) {
    if (!label[0])
        popup.GetID()->AppendSeparator();
    else
        popup.GetID()->Append(cmd, label);

    if (!enabled)
        popup.GetID()->Enable(cmd, enabled);
}


void ScintillaWX::ClaimSelection() {

}


long ScintillaWX::DefWndProc(unsigned int /*iMessage*/, unsigned long /*wParam*/, long /*lParam*/) {
    return 0;
}

long ScintillaWX::WndProc(unsigned int iMessage, unsigned long wParam, long lParam) {
//      switch (iMessage) {
//      case EM_CANPASTE:
//          return CanPaste();
//      default:
        return ScintillaBase::WndProc(iMessage, wParam, lParam);
//      }
//      return 0;
}



//----------------------------------------------------------------------
// Event delegates

void ScintillaWX::DoPaint(wxDC* dc, wxRect rect) {

    paintState = painting;
    Surface surfaceWindow;
    surfaceWindow.Init(dc);
    PRectangle rcPaint = PRectangleFromwxRect(rect);
    dc->BeginDrawing();
    Paint(&surfaceWindow, rcPaint);
    dc->EndDrawing();
    surfaceWindow.Release();
    if (paintState == paintAbandoned) {
        // Painting area was insufficient to cover new styling or brace highlight positions
        FullPaint();
    }
    paintState = notPainting;
#ifdef __WXGTK__
    // On wxGTK the editor window paints can overwrite the listbox...
    if (ac.Active())
        ((wxWindow*)ac.lb.GetID())->Refresh(TRUE);
#endif
}


void ScintillaWX::DoHScroll(int type, int pos) {
    int xPos = xOffset;
    if (type == wxEVT_SCROLLWIN_LINEUP)
        xPos -= H_SCROLL_STEP;
    else if (type == wxEVT_SCROLLWIN_LINEDOWN)
        xPos += H_SCROLL_STEP;
    else if (type == wxEVT_SCROLLWIN_PAGEUP)
        xPos -= H_SCROLL_PAGE;
    else if (type == wxEVT_SCROLLWIN_PAGEDOWN)
        xPos += H_SCROLL_PAGE;
    else if (type == wxEVT_SCROLLWIN_TOP)
        xPos = 0;
    else if (type == wxEVT_SCROLLWIN_BOTTOM)
        xPos = H_SCROLL_MAX;
    else if (type == wxEVT_SCROLLWIN_THUMBTRACK)
        xPos = pos;

    HorizontalScrollTo(xPos);
}

void ScintillaWX::DoVScroll(int type, int pos) {
    int topLineNew = topLine;
    if (type == wxEVT_SCROLLWIN_LINEUP)
        topLineNew -= 1;
    else if (type == wxEVT_SCROLLWIN_LINEDOWN)
        topLineNew += 1;
    else if (type ==  wxEVT_SCROLLWIN_PAGEUP)
        topLineNew -= LinesToScroll();
    else if (type ==  wxEVT_SCROLLWIN_PAGEDOWN)
        topLineNew += LinesToScroll();
    else if (type ==  wxEVT_SCROLLWIN_TOP)
        topLineNew = 0;
    else if (type ==  wxEVT_SCROLLWIN_BOTTOM)
        topLineNew = MaxScrollPos();
    else if (type ==   wxEVT_SCROLLWIN_THUMBTRACK)
        topLineNew = pos;

    ScrollTo(topLineNew);
}


void ScintillaWX::DoMouseWheel(int rotation, int delta, int linesPerAction, int ctrlDown) {
    int topLineNew = topLine;
    int lines;

    if (ctrlDown) {  // Zoom the fonts if Ctrl key down
        if (rotation < 0) {
            KeyCommand(SCI_ZOOMIN);
        }
        else {
            KeyCommand(SCI_ZOOMOUT);
        }
    }
    else { // otherwise just scroll the window
        wheelRotation += rotation;
        lines = wheelRotation / delta;
        wheelRotation -= lines * delta;
        if (lines != 0) {
            lines *= linesPerAction;
            topLineNew -= lines;
            ScrollTo(topLineNew);
        }
    }
}


void ScintillaWX::DoSize(int width, int height) {
    PRectangle rcClient(0,0,width,height);
    SetScrollBarsTo(rcClient);
    DropGraphics();
}

void ScintillaWX::DoLoseFocus(){
    SetFocusState(false);
}

void ScintillaWX::DoGainFocus(){
    SetFocusState(true);
}

void ScintillaWX::DoSysColourChange() {
    InvalidateStyleData();
}

void ScintillaWX::DoButtonDown(Point pt, unsigned int curTime, bool shift, bool ctrl, bool alt) {
    ButtonDown(pt, curTime, shift, ctrl, alt);
}

void ScintillaWX::DoButtonUp(Point pt, unsigned int curTime, bool ctrl) {
    ButtonUp(pt, curTime, ctrl);
}

void ScintillaWX::DoButtonMove(Point pt) {
    ButtonMove(pt);
}


void ScintillaWX::DoAddChar(char ch) {
    AddChar(ch);
}

int  ScintillaWX::DoKeyDown(int key, bool shift, bool ctrl, bool alt, bool* consumed) {
#ifdef __WXGTK__
    // Ctrl chars (A-Z) end up with the wrong keycode on wxGTK...
    if (ctrl && key >= 1 && key <= 26)
        key += 'A' - 1;
#endif

    switch (key) {
    case WXK_DOWN: key = SCK_DOWN; break;
    case WXK_UP: key = SCK_UP; break;
    case WXK_LEFT: key = SCK_LEFT; break;
    case WXK_RIGHT: key = SCK_RIGHT; break;
    case WXK_HOME: key = SCK_HOME; break;
    case WXK_END: key = SCK_END; break;
    case WXK_PRIOR: key = SCK_PRIOR; break;
    case WXK_NEXT: key = SCK_NEXT; break;
    case WXK_DELETE: key = SCK_DELETE; break;
    case WXK_INSERT: key = SCK_INSERT; break;
    case WXK_ESCAPE: key = SCK_ESCAPE; break;
    case WXK_BACK: key = SCK_BACK; break;
    case WXK_TAB: key = SCK_TAB; break;
    case WXK_RETURN: key = SCK_RETURN; break;
    case WXK_ADD:
    case WXK_NUMPAD_ADD:
        key = SCK_ADD; break;
    case WXK_SUBTRACT:
    case WXK_NUMPAD_SUBTRACT:
        key = SCK_SUBTRACT; break;
    case WXK_DIVIDE:
    case WXK_NUMPAD_DIVIDE:
        key = SCK_DIVIDE; break;
    case WXK_CONTROL: key = 0; break;
    case WXK_ALT: key = 0; break;
    case WXK_SHIFT: key = 0; break;
    case WXK_MENU: key = 0; break;
    }

    int rv = KeyDown(key, shift, ctrl, alt, consumed);

    if (key)
        return rv;
    else
        return 1;
}


void ScintillaWX::DoCommand(int ID) {
    Command(ID);
}


void ScintillaWX::DoContextMenu(Point pt) {
    ContextMenu(pt);
}

void ScintillaWX::DoOnListBox() {
    AutoCompleteCompleted();
}

//----------------------------------------------------------------------

bool ScintillaWX::DoDropText(long x, long y, const wxString& data) {
    SetDragPosition(invalidPosition);

    // Send an event to allow the drag details to be changed
    wxStyledTextEvent evt(wxEVT_STC_DO_DROP, stc->GetId());
    evt.SetEventObject(stc);
    evt.SetDragResult(dragResult);
    evt.SetX(x);
    evt.SetY(y);
    evt.SetPosition(PositionFromLocation(Point(x,y)));
    evt.SetDragText(data);
    stc->GetEventHandler()->ProcessEvent(evt);

    dragResult = evt.GetDragResult();
    if (dragResult == wxDragMove || dragResult == wxDragCopy) {
        DropAt(evt.GetPosition(),
               evt.GetDragText(),
               dragResult == wxDragMove,
               FALSE); // TODO: rectangular?
        return TRUE;
    }
    return FALSE;
}


wxDragResult ScintillaWX::DoDragEnter(wxCoord x, wxCoord y, wxDragResult def) {
    dragResult = def;
    return dragResult;
}


wxDragResult ScintillaWX::DoDragOver(wxCoord x, wxCoord y, wxDragResult def) {
    SetDragPosition(PositionFromLocation(Point(x, y)));

    // Send an event to allow the drag result to be changed
    wxStyledTextEvent evt(wxEVT_STC_DRAG_OVER, stc->GetId());
    evt.SetEventObject(stc);
    evt.SetDragResult(def);
    evt.SetX(x);
    evt.SetY(y);
    evt.SetPosition(PositionFromLocation(Point(x,y)));
    stc->GetEventHandler()->ProcessEvent(evt);

    dragResult = evt.GetDragResult();
    return dragResult;
}


void ScintillaWX::DoDragLeave() {
    SetDragPosition(invalidPosition);
}

//----------------------------------------------------------------------

// Redraw all of text area. This paint will not be abandoned.
void ScintillaWX::FullPaint() {
    paintState = painting;
    rcPaint = GetTextRectangle();
    paintingAllText = true;
    wxClientDC dc(wMain.GetID());
    Surface surfaceWindow;
    surfaceWindow.Init(&dc);
    Paint(&surfaceWindow, rcPaint);
    surfaceWindow.Release();

//     wMain.GetID()->Refresh(FALSE);

    paintState = notPainting;
}


void ScintillaWX::DoScrollToLine(int line) {
    ScrollTo(line);
}


void ScintillaWX::DoScrollToColumn(int column) {
    HorizontalScrollTo(column * vs.spaceWidth);
}



//----------------------------------------------------------------------
//----------------------------------------------------------------------
