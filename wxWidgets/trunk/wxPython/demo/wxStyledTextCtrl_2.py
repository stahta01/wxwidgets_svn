
from wxPython.wx import *
from wxPython.stc import *

import keyword

#----------------------------------------------------------------------

demoText = """\
## This version of the editor has been set up to edit Python source
## code.  Here is a copy of wxPython/demo/Main.py to play with.


"""

#----------------------------------------------------------------------


if wxPlatform == '__WXMSW__':
    faces = { 'times': 'Times New Roman',
              'mono' : 'Courier New',
              'helv' : 'Arial',
              'other': 'Comic Sans MS',
              'size' : 10,
              'size2': 8,
             }
else:
    faces = { 'times': 'Times',
              'mono' : 'Courier',
              'helv' : 'Helvetica',
              'other': 'new century schoolbook',
              'size' : 13,
              'size2': 11,
             }


#----------------------------------------------------------------------

class PythonSTC(wxStyledTextCtrl):
    def __init__(self, parent, ID):
        wxStyledTextCtrl.__init__(self, parent, ID)

        self.CmdKeyAssign(ord('B'), wxSTC_SCMOD_CTRL, wxSTC_CMD_ZOOMIN)
        self.CmdKeyAssign(ord('N'), wxSTC_SCMOD_CTRL, wxSTC_CMD_ZOOMOUT)

        self.SetLexer(wxSTC_LEX_PYTHON)
        self.SetKeyWords(0, string.join(keyword.kwlist))

        self.SetProperty("fold", "1")
        self.SetProperty("tab.timmy.whinge.level", "1")
        self.SetMargins(0,0)

        self.SetViewWhiteSpace(false)
        #self.SetBufferedDraw(false)

        self.SetEdgeMode(wxSTC_EDGE_BACKGROUND)
        self.SetEdgeColumn(78)

        # Setup a margin to hold fold markers
        #self.SetFoldFlags(16)  ###  WHAT IS THIS VALUE?  WHAT ARE THE OTHER FLAGS?  DOES IT MATTER?
        self.SetMarginType(2, wxSTC_MARGIN_SYMBOL)
        self.SetMarginMask(2, wxSTC_MASK_FOLDERS)
        self.SetMarginSensitive(2, true)
        self.SetMarginWidth(2, 15)
        self.MarkerDefine(wxSTC_MARKNUM_FOLDER, wxSTC_MARK_ARROW, "navy", "navy")
        self.MarkerDefine(wxSTC_MARKNUM_FOLDEROPEN, wxSTC_MARK_ARROWDOWN, "navy", "navy")


        EVT_STC_UPDATEUI(self,    ID, self.OnUpdateUI)
        EVT_STC_MARGINCLICK(self, ID, self.OnMarginClick)


        # Make some styles,  The lexer defines what each style is used for, we
        # just have to define what each style looks like.  This set is adapted from
        # Scintilla sample property files.

        self.StyleClearAll()

        # Global default styles for all languages
        self.StyleSetSpec(wxSTC_STYLE_DEFAULT,     "face:%(helv)s,size:%(size)d" % faces)
        self.StyleSetSpec(wxSTC_STYLE_LINENUMBER,  "back:#C0C0C0,face:%(helv)s,size:%(size2)d" % faces)
        self.StyleSetSpec(wxSTC_STYLE_CONTROLCHAR, "face:%(other)s" % faces)
        self.StyleSetSpec(wxSTC_STYLE_BRACELIGHT,  "fore:#FFFFFF,back:#0000FF,bold")
        self.StyleSetSpec(wxSTC_STYLE_BRACEBAD,    "fore:#000000,back:#FF0000,bold")

        # Python styles
        # White space
        self.StyleSetSpec(wxSTC_P_DEFAULT, "fore:#808080,face:%(helv)s,size:%(size)d" % faces)
        # Comment
        self.StyleSetSpec(wxSTC_P_COMMENTLINE, "fore:#007F00,face:%(other)s,size:%(size)d" % faces)
        # Number
        self.StyleSetSpec(wxSTC_P_NUMBER, "fore:#007F7F,size:%(size)d" % faces)
        # String
        self.StyleSetSpec(wxSTC_P_STRING, "fore:#7F007F,italic,face:%(times)s,size:%(size)d" % faces)
        # Single quoted string
        self.StyleSetSpec(wxSTC_P_CHARACTER, "fore:#7F007F,italic,face:%(times)s,size:%(size)d" % faces)
        # Keyword
        self.StyleSetSpec(wxSTC_P_WORD, "fore:#00007F,bold,size:%(size)d" % faces)
        # Triple quotes
        self.StyleSetSpec(wxSTC_P_TRIPLE, "fore:#7F0000,size:%(size)d" % faces)
        # Triple double quotes
        self.StyleSetSpec(wxSTC_P_TRIPLEDOUBLE, "fore:#7F0000,size:%(size)d" % faces)
        # Class name definition
        self.StyleSetSpec(wxSTC_P_CLASSNAME, "fore:#0000FF,bold,underline,size:%(size)d" % faces)
        # Function or method name definition
        self.StyleSetSpec(wxSTC_P_DEFNAME, "fore:#007F7F,bold,size:%(size)d" % faces)
        # Operators
        self.StyleSetSpec(wxSTC_P_OPERATOR, "bold,size:%(size)d" % faces)
        # Identifiers
        self.StyleSetSpec(wxSTC_P_IDENTIFIER, "fore:#808080,face:%(helv)s,size:%(size)d" % faces)
        # Comment-blocks
        self.StyleSetSpec(wxSTC_P_COMMENTBLOCK, "fore:#7F7F7F,size:%(size)d" % faces)
        # End of line where string is not closed
        self.StyleSetSpec(wxSTC_P_STRINGEOL, "fore:#000000,face:%(mono)s,back:#E0C0E0,eolfilled,size:%(size)d" % faces)


        self.SetCaretForeground("BLUE")

        EVT_KEY_UP(self, self.OnKeyPressed)


    def OnKeyPressed(self, event):
        key = event.KeyCode()
        if key == 32 and event.ControlDown():
            pos = self.GetCurrentPos()
            # Tips
            if event.ShiftDown():
                self.CallTipSetBackground("yellow")
                self.CallTipShow(pos, 'param1, param2')
            # Code completion
            else:
                #lst = []
                #for x in range(50000):
                #    lst.append('%05d' % x)
                #st = string.join(lst)
                #print len(st)
                #self.AutoCompShow(0, st)

                kw = keyword.kwlist[:]
                kw.append("zzzzzz")
                kw.append("aaaaa")
                kw.append("__init__")
                kw.append("zzaaaaa")
                kw.append("zZaaaaa")
                kw.append("this_is_a_longer_value")
                kw.append("this_is_a_much_much_much_much_much_much_much_longer_value")

                #kw.sort()

                self.AutoCompSetIgnoreCase(true)
                self.AutoCompShow(0, string.join(kw))
                #self.AutoCompSelect('br')
        else:
            event.Skip()


    def OnUpdateUI(self, evt):
        # check for matching braces
        braceAtCaret = -1
	braceOpposite = -1
        charBefore = None
        caretPos = self.GetCurrentPos()
        if caretPos > 0:
            charBefore = self.GetCharAt(caretPos - 1)
            styleBefore = self.GetStyleAt(caretPos - 1)

        # check before
        if charBefore and chr(charBefore) in "[]{}()" and styleBefore == wxSTC_P_OPERATOR:
            braceAtCaret = caretPos - 1

        # check after
        if braceAtCaret < 0:
            charAfter = self.GetCharAt(caretPos)
            styleAfter = self.GetStyleAt(caretPos)
            if charAfter and chr(charAfter) in "[]{}()" and styleAfter == wxSTC_P_OPERATOR:
                braceAtCaret = caretPos

        if braceAtCaret >= 0:
            braceOpposite = self.BraceMatch(braceAtCaret)

        if braceAtCaret != -1  and braceOpposite == -1:
            self.BraceBadLight(braceAtCaret)
        else:
            self.BraceHighlight(braceAtCaret, braceOpposite)
            #pt = self.PointFromPosition(braceOpposite)
            #self.Refresh(true, wxRect(pt.x, pt.y, 5,5))
            #print pt
            #self.Refresh(false)


    def OnMarginClick(self, evt):
        # fold and unfold as needed
        if evt.GetMargin() == 2:
            if evt.GetShift() and evt.GetControl():
                self.FoldAll()
            else:
                lineClicked = self.LineFromPosition(evt.GetPosition())
                if self.GetFoldLevel(lineClicked) & wxSTC_FOLDLEVELHEADERFLAG:
                    if evt.GetShift():
                        self.SetFoldExpanded(lineClicked, true)
                        self.Expand(lineClicked, true, true, 1)
                    elif evt.GetControl():
                        if self.GetFoldExpanded(lineClicked):
                            self.SetFoldExpanded(lineClicked, false)
                            self.Expand(lineClicked, false, true, 0)
                        else:
                            self.SetFoldExpanded(lineClicked, true)
                            self.Expand(lineClicked, true, true, 100)
                    else:
                        self.ToggleFold(lineClicked)


    def FoldAll(self):
        lineCount = self.GetLineCount()
        expanding = true

        # find out if we are folding or unfolding
        for lineNum in range(lineCount):
            if self.GetFoldLevel(lineNum) & wxSTC_FOLDLEVELHEADERFLAG:
                expanding = not self.GetFoldExpanded(lineNum)
                break;

        lineNum = 0
        while lineNum < lineCount:
            level = self.GetFoldLevel(lineNum)
            if level & wxSTC_FOLDLEVELHEADERFLAG and \
               (level & wxSTC_FOLDLEVELNUMBERMASK) == wxSTC_FOLDLEVELBASE:

                if expanding:
                    self.SetFoldExpanded(lineNum, true)
                    lineNum = self.Expand(lineNum, true)
                    lineNum = lineNum - 1
                else:
                    lastChild = self.GetLastChild(lineNum, -1)
                    self.SetFoldExpanded(lineNum, false)
                    if lastChild > lineNum:
                        self.HideLines(lineNum+1, lastChild)

            lineNum = lineNum + 1



    def Expand(self, line, doExpand, force=false, visLevels=0, level=-1):
        lastChild = self.GetLastChild(line, level)
	line = line + 1
        while line <= lastChild:
            if force:
                if visLevels > 0:
                    self.ShowLines(line, line)
                else:
                    self.HideLines(line, line)
            else:
                if doExpand:
                    self.ShowLines(line, line)

            if level == -1:
                level = self.GetFoldLevel(line)

            if level & wxSTC_FOLDLEVELHEADERFLAG:
                if force:
                    if visLevels > 1:
                        self.SetFoldExpanded(line, true)
                    else:
                        self.SetFoldExpanded(line, false)
                    line = self.Expand(line, doExpand, force, visLevels-1)

                else:
                    if doExpand and self.GetFoldExpanded(line):
                        line = self.Expand(line, true, force, visLevels-1)
                    else:
                        line = self.Expand(line, false, force, visLevels-1)
            else:
                line = line + 1;

        return line


#----------------------------------------------------------------------

_USE_PANEL = 1

def runTest(frame, nb, log):
    if not _USE_PANEL:
        ed = p = PythonSTC(nb, -1)
    else:
        p = wxPanel(nb, -1)
        ed = PythonSTC(p, -1)
        s = wxBoxSizer(wxHORIZONTAL)
        s.Add(ed, 1, wxEXPAND)
        p.SetSizer(s)
        p.SetAutoLayout(true)


    ed.SetText(demoText + open('Main.py').read())
    ed.EmptyUndoBuffer()
    ed.Colourise(0, -1)

    # line numbers in the margin
    ed.SetMarginType(1, wxSTC_MARGIN_NUMBER)
    ed.SetMarginWidth(1, 25)

    return p



#----------------------------------------------------------------------


overview = """\
<html><body>
Once again, no docs yet.  <b>Sorry.</b>  But <a href="data/stc.h.html">this</a>
and <a href="http://www.scintilla.org/ScintillaDoc.html">this</a> should
be helpful.
</body><html>
"""


if __name__ == '__main__':
    import sys
    app = wxPySimpleApp()
    frame = wxFrame(None, -1, "Tester...", size=(640, 480))
    win = runTest(frame, frame, sys.stdout)
    frame.Show(true)
    app.MainLoop()






#----------------------------------------------------------------------
#----------------------------------------------------------------------

