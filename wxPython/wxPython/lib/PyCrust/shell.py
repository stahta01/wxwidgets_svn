"""The PyCrust Shell is an interactive text control in which a user
types in commands to be sent to the interpreter. This particular shell
is based on wxPython's wxStyledTextCtrl. The latest files are always
available at the SourceForge project page at
http://sourceforge.net/projects/pycrust/.

Sponsored by Orbtech - Your source for Python programming expertise."""

__author__ = "Patrick K. O'Brien <pobrien@orbtech.com>"
__cvsid__ = "$Id$"
__revision__ = "$Revision$"[11:-2]

import keyword
import os
import sys
import time
from pseudo import PseudoFileIn
from pseudo import PseudoFileOut
from pseudo import PseudoFileErr
from shellmenu import ShellMenu
from version import VERSION
import dispatcher

try:
    import wxd.d_wx
except ImportError:
    from wxPython import wx
else:
    from wxd.d_wx import wx

try:
    import wxd.d_stc
except ImportError:
    from wxPython import stc
else:
    from wxd.d_stc import stc

try:
    True
except NameError:
    True = 1==1
    False = 1==0

sys.ps3 = '<-- '  # Input prompt.

NAVKEYS = (wx.WXK_END, wx.WXK_LEFT, wx.WXK_RIGHT, 
           wx.WXK_UP, wx.WXK_DOWN, wx.WXK_PRIOR, wx.WXK_NEXT)

if wx.wxPlatform == '__WXMSW__':
    faces = { 'times'  : 'Times New Roman',
              'mono'   : 'Courier New',
              'helv'   : 'Lucida Console',
              'lucida' : 'Lucida Console',
              'other'  : 'Comic Sans MS',
              'size'   : 10,
              'lnsize' : 9,
              'backcol': '#FFFFFF',
            }
else:  # GTK
    faces = { 'times'  : 'Times',
              'mono'   : 'Courier',
              'helv'   : 'Helvetica',
              'other'  : 'new century schoolbook',
              'size'   : 12,
              'lnsize' : 10,
              'backcol': '#FFFFFF',
            }


class ShellFrame(wx.wxFrame, ShellMenu):
    """Frame containing the PyCrust shell component."""

    name = 'PyCrust Shell Frame'
    revision = __revision__

    def __init__(self, parent=None, id=-1, title='PyShell', 
                 pos=wx.wxDefaultPosition, size=wx.wxDefaultSize, 
                 style=wx.wxDEFAULT_FRAME_STYLE, locals=None, 
                 InterpClass=None, *args, **kwds):
        """Create a PyCrust ShellFrame instance."""
        wx.wxFrame.__init__(self, parent, id, title, pos, size, style)
        intro = 'PyCrust %s - The Flakiest Python Shell' % VERSION
        intro += '\nSponsored by Orbtech - ' + \
                 'Your source for Python programming expertise.'
        self.CreateStatusBar()
        self.SetStatusText(intro.replace('\n', ', '))
        import images
        self.SetIcon(images.getPyCrustIcon())
        self.shell = Shell(parent=self, id=-1, introText=intro, 
                           locals=locals, InterpClass=InterpClass, 
                           *args, **kwds)
        # Override the shell so that status messages go to the status bar.
        self.shell.setStatusText = self.SetStatusText
        self.createMenus()
        wx.EVT_CLOSE(self, self.OnCloseWindow)

    def OnCloseWindow(self, event):
        """Event handler for closing."""
        # This isn't working the way I want, but I'll leave it for now.
        if self.shell.waiting:
            event.Veto(True)
        else:
            self.shell.destroy()
            self.Destroy()


class ShellFacade:
    """Simplified interface to all shell-related functionality.

    This is a semi-transparent facade, in that all attributes of other
    are accessible, even though only some are visible to the user."""

    name = 'PyCrust Shell Interface'
    revision = __revision__

    def __init__(self, other):
        """Create a ShellFacade instance."""
        methods = [
            'about',
            'ask',
            'clear',
            'pause',
            'prompt',
            'quit',
            'redirectStderr',
            'redirectStdin',
            'redirectStdout',
            'run',
            'runfile',
            'wrap',
            'zoom',
            ]
        for method in methods:
            self.__dict__[method] = getattr(other, method)
        d = self.__dict__
        d['other'] = other
        d['helpText'] = \
"""
* Key bindings:
Home              Go to the beginning of the command or line.
Shift+Home        Select to the beginning of the command or line.
Shift+End         Select to the end of the line.
End               Go to the end of the line.
Ctrl+C            Copy selected text, removing prompts.
Ctrl+Shift+C      Copy selected text, retaining prompts.
Ctrl+X            Cut selected text.
Ctrl+V            Paste from clipboard.
Ctrl+Shift+V      Paste and run multiple commands from clipboard.
Ctrl+Up Arrow     Retrieve Previous History item.
Alt+P             Retrieve Previous History item.
Ctrl+Down Arrow   Retrieve Next History item.
Alt+N             Retrieve Next History item.
Shift+Up Arrow    Insert Previous History item.
Shift+Down Arrow  Insert Next History item.
F8                Command-completion of History item.
                  (Type a few characters of a previous command and press F8.)
Ctrl+Enter        Insert new line into multiline command.
Ctrl+]            Increase font size.
Ctrl+[            Decrease font size.
Ctrl+=            Default font size.
"""

    def help(self):
        """Display some useful information about how to use the shell."""
        self.write(self.helpText)

    def __getattr__(self, name):
        if hasattr(self.other, name):
            return getattr(self.other, name)
        else:
            raise AttributeError, name

    def __setattr__(self, name, value):
        if self.__dict__.has_key(name):
            self.__dict__[name] = value
        elif hasattr(self.other, name):
            setattr(self.other, name, value)
        else:
            raise AttributeError, name

    def _getAttributeNames(self):
        """Return list of magic attributes to extend introspection."""
        list = ['autoCallTip',
                'autoComplete',
                'autoCompleteCaseInsensitive',
                'autoCompleteIncludeDouble',
                'autoCompleteIncludeMagic',
                'autoCompleteIncludeSingle',
               ]
        list.sort()
        return list


class Shell(stc.wxStyledTextCtrl):
    """PyCrust Shell based on wxStyledTextCtrl."""

    name = 'PyCrust Shell'
    revision = __revision__

    def __init__(self, parent, id=-1, pos=wx.wxDefaultPosition, 
                 size=wx.wxDefaultSize, style=wx.wxCLIP_CHILDREN,
                 introText='', locals=None, InterpClass=None, *args, **kwds):
        """Create a PyCrust Shell instance."""
        stc.wxStyledTextCtrl.__init__(self, parent, id, pos, size, style)
        if locals is None:
            locals = {}
        # Grab these so they can be restored by self.redirect* methods.
        self.stdin = sys.stdin
        self.stdout = sys.stdout
        self.stderr = sys.stderr
        # Add the current working directory "." to the search path.
        sys.path.insert(0, os.curdir)
        # Import a default interpreter class if one isn't provided.
        if InterpClass == None:
            from interpreter import Interpreter
        else:
            Interpreter = InterpClass
        # Create a replacement for stdin.
        self.reader = PseudoFileIn(self.readline, self.readlines)
        self.reader.input = ''
        self.reader.isreading = 0
        # Set up the interpreter.
        self.interp = Interpreter(locals=locals, 
                                  rawin=self.raw_input, 
                                  stdin=self.reader, 
                                  stdout=PseudoFileOut(self.writeOut), 
                                  stderr=PseudoFileErr(self.writeErr), 
                                  *args, **kwds)
        # Find out for which keycodes the interpreter will autocomplete.
        self.autoCompleteKeys = self.interp.getAutoCompleteKeys()
        # Keep track of the last non-continuation prompt positions.
        self.promptPosStart = 0
        self.promptPosEnd = 0
        # Keep track of multi-line commands.
        self.more = 0
        # Create the command history.  Commands are added into the
        # front of the list (ie. at index 0) as they are entered.
        # self.historyIndex is the current position in the history; it
        # gets incremented as you retrieve the previous command,
        # decremented as you retrieve the next, and reset when you hit
        # Enter.  self.historyIndex == -1 means you're on the current
        # command, not in the history.
        self.history = []
        self.historyIndex = -1
        # Assign handlers for keyboard events.
        wx.EVT_KEY_DOWN(self, self.OnKeyDown)
        wx.EVT_CHAR(self, self.OnChar)
        # Assign handlers for wxSTC events.
        stc.EVT_STC_UPDATEUI(self, id, self.OnUpdateUI)
        # Assign handler for idle time.
        self.waiting = False
        wx.EVT_IDLE(self, self.OnIdle)
        dispatcher.connect(receiver=self.fontsizer, signal='FontIncrease')
        dispatcher.connect(receiver=self.fontsizer, signal='FontDecrease')
        dispatcher.connect(receiver=self.fontsizer, signal='FontDefault')
        # Configure various defaults and user preferences.
        self.config()
        # Display the introductory banner information.
        self.showIntro(introText)
        # Assign some pseudo keywords to the interpreter's namespace.
        self.setBuiltinKeywords()
        # Add 'shell' to the interpreter's local namespace.
        self.setLocalShell()
        # Do this last so the user has complete control over their
        # environment.  They can override anything they want.
        self.execStartupScript(self.interp.startupScript)
        wx.wxCallAfter(self.ScrollToLine, 0)

    def fontsizer(self, signal):
        """Receiver for Font* signals."""
        size = self.GetZoom()
        if signal == 'FontIncrease':
            size += 1
        elif signal == 'FontDecrease':
            size -= 1
        elif signal == 'FontDefault':
            size = 0
        self.SetZoom(size)

    def destroy(self):
        del self.interp
        pass

    def config(self):
        """Configure shell based on user preferences."""
        self.SetMarginType(1, stc.wxSTC_MARGIN_NUMBER)
        self.SetMarginWidth(1, 40)

        self.SetLexer(stc.wxSTC_LEX_PYTHON)
        self.SetKeyWords(0, ' '.join(keyword.kwlist))

        self.setStyles(faces)
        self.SetViewWhiteSpace(0)
        self.SetTabWidth(4)
        self.SetUseTabs(0)
        # Do we want to automatically pop up command completion options?
        self.autoComplete = 1
        self.autoCompleteIncludeMagic = 1
        self.autoCompleteIncludeSingle = 1
        self.autoCompleteIncludeDouble = 1
        self.autoCompleteCaseInsensitive = 1
        self.AutoCompSetIgnoreCase(self.autoCompleteCaseInsensitive)
        self.AutoCompSetAutoHide(False)
        self.AutoCompStops(' .,;:([)]}\'"\\<>%^&+-=*/|`')
        # Do we want to automatically pop up command argument help?
        self.autoCallTip = 1
        self.CallTipSetBackground(wx.wxColour(255, 255, 232))
        self.wrap()
        try:
            self.SetEndAtLastLine(False)
        except AttributeError:
            pass

    def showIntro(self, text=''):
        """Display introductory text in the shell."""
        if text:
            if not text.endswith(os.linesep):
                text += os.linesep
            self.write(text)
        try:
            self.write(self.interp.introText)
        except AttributeError:
            pass

    def setBuiltinKeywords(self):
        """Create pseudo keywords as part of builtins.

        This sets `close`, `exit` and `quit` to a helpful string.
        """
        import __builtin__
        __builtin__.close = __builtin__.exit = __builtin__.quit = \
            'Click on the close button to leave the application.'

    def quit(self):
        """Quit the application."""

        # XXX Good enough for now but later we want to send a close event.

        # In the close event handler we can make sure they want to
        # quit.  Other applications, like PythonCard, may choose to
        # hide rather than quit so we should just post the event and
        # let the surrounding app decide what it wants to do.
        self.write('Click on the close button to leave the application.')

    def setLocalShell(self):
        """Add 'shell' to locals as reference to ShellFacade instance."""
        self.interp.locals['shell'] = ShellFacade(other=self)

    def execStartupScript(self, startupScript):
        """Execute the user's PYTHONSTARTUP script if they have one."""
        if startupScript and os.path.isfile(startupScript):
            text = 'Startup script executed: ' + startupScript
            self.push('print %r; execfile(%r)' % (text, startupScript))
        else:
            self.push('')

    def setStyles(self, faces):
        """Configure font size, typeface and color for lexer."""

        # Default style
        self.StyleSetSpec(stc.wxSTC_STYLE_DEFAULT,
                          "face:%(mono)s,size:%(size)d,back:%(backcol)s" % \
                          faces)

        self.StyleClearAll()

        # Built in styles
        self.StyleSetSpec(stc.wxSTC_STYLE_LINENUMBER,
                          "back:#C0C0C0,face:%(mono)s,size:%(lnsize)d" % faces)
        self.StyleSetSpec(stc.wxSTC_STYLE_CONTROLCHAR,
                          "face:%(mono)s" % faces)
        self.StyleSetSpec(stc.wxSTC_STYLE_BRACELIGHT,
                          "fore:#0000FF,back:#FFFF88")
        self.StyleSetSpec(stc.wxSTC_STYLE_BRACEBAD,
                          "fore:#FF0000,back:#FFFF88")

        # Python styles
        self.StyleSetSpec(stc.wxSTC_P_DEFAULT,
                          "face:%(mono)s" % faces)
        self.StyleSetSpec(stc.wxSTC_P_COMMENTLINE,
                          "fore:#007F00,face:%(mono)s" % faces)
        self.StyleSetSpec(stc.wxSTC_P_NUMBER,
                          "")
        self.StyleSetSpec(stc.wxSTC_P_STRING,
                          "fore:#7F007F,face:%(mono)s" % faces)
        self.StyleSetSpec(stc.wxSTC_P_CHARACTER,
                          "fore:#7F007F,face:%(mono)s" % faces)
        self.StyleSetSpec(stc.wxSTC_P_WORD,
                          "fore:#00007F,bold")
        self.StyleSetSpec(stc.wxSTC_P_TRIPLE,
                          "fore:#7F0000")
        self.StyleSetSpec(stc.wxSTC_P_TRIPLEDOUBLE,
                          "fore:#000033,back:#FFFFE8")
        self.StyleSetSpec(stc.wxSTC_P_CLASSNAME,
                          "fore:#0000FF,bold")
        self.StyleSetSpec(stc.wxSTC_P_DEFNAME,
                          "fore:#007F7F,bold")
        self.StyleSetSpec(stc.wxSTC_P_OPERATOR,
                          "")
        self.StyleSetSpec(stc.wxSTC_P_IDENTIFIER,
                          "")
        self.StyleSetSpec(stc.wxSTC_P_COMMENTBLOCK,
                          "fore:#7F7F7F")
        self.StyleSetSpec(stc.wxSTC_P_STRINGEOL,
                          "fore:#000000,face:%(mono)s,back:#E0C0E0,eolfilled" % faces)

    def about(self):
        """Display information about PyCrust."""
        text = """
Author: %r
PyCrust Version: %s
Shell Revision: %s
Interpreter Revision: %s
Python Version: %s
wxPython Version: %s
Platform: %s""" % \
        (__author__, VERSION, self.revision, self.interp.revision,
         sys.version.split()[0], wx.__version__, sys.platform)
        self.write(text.strip())

    def OnIdle(self, event):
        """Free the CPU to do other things."""
        if self.waiting:
            time.sleep(0.05)

    def OnUpdateUI(self, event):
        """Check for matching braces."""
        # If the auto-complete window is up let it do its thing.
        if self.AutoCompActive():
            return
        braceAtCaret = -1
        braceOpposite = -1
        charBefore = None
        caretPos = self.GetCurrentPos()
        if caretPos > 0:
            charBefore = self.GetCharAt(caretPos - 1)
            styleBefore = self.GetStyleAt(caretPos - 1)

        # Check before.
        if charBefore and chr(charBefore) in '[]{}()' \
        and styleBefore == stc.wxSTC_P_OPERATOR:
            braceAtCaret = caretPos - 1

        # Check after.
        if braceAtCaret < 0:
            charAfter = self.GetCharAt(caretPos)
            styleAfter = self.GetStyleAt(caretPos)
            if charAfter and chr(charAfter) in '[]{}()' \
            and styleAfter == stc.wxSTC_P_OPERATOR:
                braceAtCaret = caretPos

        if braceAtCaret >= 0:
            braceOpposite = self.BraceMatch(braceAtCaret)

        if braceAtCaret != -1  and braceOpposite == -1:
            self.BraceBadLight(braceAtCaret)
        else:
            self.BraceHighlight(braceAtCaret, braceOpposite)

    def OnChar(self, event):
        """Keypress event handler.
        
        Only receives an event if OnKeyDown calls event.Skip() for the
        corresponding event."""

        # Prevent modification of previously submitted
        # commands/responses.
        if not self.CanEdit():
            return
        key = event.KeyCode()
        currpos = self.GetCurrentPos()
        stoppos = self.promptPosEnd
        # Return (Enter) needs to be ignored in this handler.
        if key == wx.WXK_RETURN:
            pass
        elif key in self.autoCompleteKeys:
            # Usually the dot (period) key activates auto completion.
            # Get the command between the prompt and the cursor.  Add
            # the autocomplete character to the end of the command.
            if self.AutoCompActive(): 
                self.AutoCompCancel()
            command = self.GetTextRange(stoppos, currpos) + chr(key)
            self.write(chr(key))
            if self.autoComplete: 
                self.autoCompleteShow(command)
        elif key == ord('('):
            # The left paren activates a call tip and cancels an
            # active auto completion.
            if self.AutoCompActive(): 
                self.AutoCompCancel()
            # Get the command between the prompt and the cursor.  Add
            # the '(' to the end of the command.
            self.ReplaceSelection('')
            command = self.GetTextRange(stoppos, currpos) + '('
            self.write('(')
            if self.autoCallTip: 
                self.autoCallTipShow(command)
        else:
            # Allow the normal event handling to take place.
            event.Skip()

    def OnKeyDown(self, event):
        """Key down event handler."""

        key = event.KeyCode()
        # If the auto-complete window is up let it do its thing.
        if self.AutoCompActive():
            event.Skip()
            return
        # Prevent modification of previously submitted
        # commands/responses.
        controlDown = event.ControlDown()
        altDown = event.AltDown()
        shiftDown = event.ShiftDown()
        currpos = self.GetCurrentPos()
        endpos = self.GetTextLength()
        selecting = self.GetSelectionStart() != self.GetSelectionEnd()
        # Return (Enter) is used to submit a command to the
        # interpreter.
        if not controlDown and key == wx.WXK_RETURN:
            if self.CallTipActive(): 
                self.CallTipCancel()
            self.processLine()
        # Ctrl+Return (Cntrl+Enter) is used to insert a line break.
        elif controlDown and key == wx.WXK_RETURN:
            if self.CallTipActive(): 
                self.CallTipCancel()
            if currpos == endpos:
                self.processLine()
            else:
                self.insertLineBreak()
        # Let Ctrl-Alt-* get handled normally.
        elif controlDown and altDown:
            event.Skip()
        # Clear the current, unexecuted command.
        elif key == wx.WXK_ESCAPE:
            if self.CallTipActive():
                event.Skip()
            else:
                self.clearCommand()
        # Increase font size.
        elif controlDown and key in (ord(']'),):
            dispatcher.send(signal='FontIncrease')
        # Decrease font size.
        elif controlDown and key in (ord('['),):
            dispatcher.send(signal='FontDecrease')
        # Default font size.
        elif controlDown and key in (ord('='),):
            dispatcher.send(signal='FontDefault')
        # Cut to the clipboard.
        elif (controlDown and key in (ord('X'), ord('x'))) \
        or (shiftDown and key == wx.WXK_DELETE):
            self.Cut()
        # Copy to the clipboard.
        elif controlDown and not shiftDown \
            and key in (ord('C'), ord('c'), wx.WXK_INSERT):
            self.Copy()
        # Copy to the clipboard, including prompts.
        elif controlDown and shiftDown \
            and key in (ord('C'), ord('c'), wx.WXK_INSERT):
            self.CopyWithPrompts()
        # Copy to the clipboard, including prefixed prompts.
        elif altDown and not controlDown \
            and key in (ord('C'), ord('c'), wx.WXK_INSERT):
            self.CopyWithPromptsPrefixed()
        # Home needs to be aware of the prompt.
        elif key == wx.WXK_HOME:
            home = self.promptPosEnd
            if currpos > home:
                self.SetCurrentPos(home)
                if not selecting and not shiftDown:
                    self.SetAnchor(home)
                    self.EnsureCaretVisible()
            else:
                event.Skip()
        #
        # The following handlers modify text, so we need to see if
        # there is a selection that includes text prior to the prompt.
        #
        # Don't modify a selection with text prior to the prompt.
        elif selecting and key not in NAVKEYS and not self.CanEdit():
            pass
        # Paste from the clipboard.
        elif (controlDown and not shiftDown and key in (ord('V'), ord('v'))) \
                 or (shiftDown and not controlDown and key == wx.WXK_INSERT):
            self.Paste()
        # Paste from the clipboard, run commands.
        elif controlDown and shiftDown and key in (ord('V'), ord('v')):
            self.PasteAndRun()
        # Replace with the previous command from the history buffer.
        elif (controlDown and key == wx.WXK_UP) \
                 or (altDown and key in (ord('P'), ord('p'))):
            self.OnHistoryReplace(step=+1)
        # Replace with the next command from the history buffer.
        elif (controlDown and key == wx.WXK_DOWN) \
                 or (altDown and key in (ord('N'), ord('n'))):
            self.OnHistoryReplace(step=-1)
        # Insert the previous command from the history buffer.
        elif (shiftDown and key == wx.WXK_UP) and self.CanEdit():
            self.OnHistoryInsert(step=+1)
        # Insert the next command from the history buffer.
        elif (shiftDown and key == wx.WXK_DOWN) and self.CanEdit():
            self.OnHistoryInsert(step=-1)
        # Search up the history for the text in front of the cursor.
        elif key == wx.WXK_F8:
            self.OnHistorySearch()
        # Don't backspace over the latest non-continuation prompt.
        elif key == wx.WXK_BACK:
            if selecting and self.CanEdit():
                event.Skip()
            elif currpos > self.promptPosEnd:
                event.Skip()
        # Only allow these keys after the latest prompt.
        elif key in (wx.WXK_TAB, wx.WXK_DELETE):
            if self.CanEdit():
                event.Skip()
        # Don't toggle between insert mode and overwrite mode.
        elif key == wx.WXK_INSERT:
            pass
        # Don't allow line deletion.
        elif controlDown and key in (ord('L'), ord('l')):
            pass
        # Don't allow line transposition.
        elif controlDown and key in (ord('T'), ord('t')):
            pass
        # Basic navigation keys should work anywhere.
        elif key in NAVKEYS:
            event.Skip()
        # Protect the readonly portion of the shell.
        elif not self.CanEdit():
            pass
        else:
            event.Skip()

    def clearCommand(self):
        """Delete the current, unexecuted command."""
        startpos = self.promptPosEnd
        endpos = self.GetTextLength()
        self.SetSelection(startpos, endpos)
        self.ReplaceSelection('')
        self.more = 0

    def OnHistoryReplace(self, step):
        """Replace with the previous/next command from the history buffer."""
        self.clearCommand()
        self.replaceFromHistory(step)

    def replaceFromHistory(self, step):
        """Replace selection with command from the history buffer."""
        ps2 = str(sys.ps2)
        self.ReplaceSelection('')
        newindex = self.historyIndex + step
        if -1 <= newindex <= len(self.history):
            self.historyIndex = newindex
        if 0 <= newindex <= len(self.history)-1:
            command = self.history[self.historyIndex]
            command = command.replace('\n', os.linesep + ps2)
            self.ReplaceSelection(command)

    def OnHistoryInsert(self, step):
        """Insert the previous/next command from the history buffer."""
        if not self.CanEdit():
            return
        startpos = self.GetCurrentPos()
        self.replaceFromHistory(step)
        endpos = self.GetCurrentPos()
        self.SetSelection(endpos, startpos)

    def OnHistorySearch(self):
        """Search up the history buffer for the text in front of the cursor."""
        if not self.CanEdit():
            return
        startpos = self.GetCurrentPos()
        # The text up to the cursor is what we search for.
        numCharsAfterCursor = self.GetTextLength() - startpos
        searchText = self.getCommand(rstrip=0)
        if numCharsAfterCursor > 0:
            searchText = searchText[:-numCharsAfterCursor]
        if not searchText:
            return
        # Search upwards from the current history position and loop
        # back to the beginning if we don't find anything.
        if (self.historyIndex <= -1) \
        or (self.historyIndex >= len(self.history)-2):
            searchOrder = range(len(self.history))
        else:
            searchOrder = range(self.historyIndex+1, len(self.history)) + \
                          range(self.historyIndex)
        for i in searchOrder:
            command = self.history[i]
            if command[:len(searchText)] == searchText:
                # Replace the current selection with the one we found.
                self.ReplaceSelection(command[len(searchText):])
                endpos = self.GetCurrentPos()
                self.SetSelection(endpos, startpos)
                # We've now warped into middle of the history.
                self.historyIndex = i
                break

    def setStatusText(self, text):
        """Display status information."""

        # This method will likely be replaced by the enclosing app to
        # do something more interesting, like write to a status bar.
        print text

    def insertLineBreak(self):
        """Insert a new line break."""
        if self.CanEdit():
            self.write(os.linesep)
            self.more = 1
            self.prompt()

    def processLine(self):
        """Process the line of text at which the user hit Enter."""

        # The user hit ENTER and we need to decide what to do. They
        # could be sitting on any line in the shell.

        thepos = self.GetCurrentPos()        
        startpos = self.promptPosEnd
        endpos = self.GetTextLength()
        ps2 = str(sys.ps2)
        # If they hit RETURN inside the current command, execute the
        # command.
        if self.CanEdit():
            self.SetCurrentPos(endpos)
            self.interp.more = 0
            command = self.GetTextRange(startpos, endpos)
            lines = command.split(os.linesep + ps2)
            lines = [line.rstrip() for line in lines]
            command = '\n'.join(lines)
            if self.reader.isreading:
                if not command:
                    # Match the behavior of the standard Python shell
                    # when the user hits return without entering a
                    # value.
                    command = '\n'
                self.reader.input = command
                self.write(os.linesep)
            else:
                self.push(command)
        # Or replace the current command with the other command.
        else:
            # If the line contains a command (even an invalid one).
            if self.getCommand(rstrip=0):
                command = self.getMultilineCommand()
                self.clearCommand()
                self.write(command)
            # Otherwise, put the cursor back where we started.
            else:
                self.SetCurrentPos(thepos)
                self.SetAnchor(thepos)

    def getMultilineCommand(self, rstrip=1):
        """Extract a multi-line command from the editor.

        The command may not necessarily be valid Python syntax."""
        # XXX Need to extract real prompts here. Need to keep track of
        # the prompt every time a command is issued.
        ps1 = str(sys.ps1)
        ps1size = len(ps1)
        ps2 = str(sys.ps2)
        ps2size = len(ps2)
        # This is a total hack job, but it works.
        text = self.GetCurLine()[0]
        line = self.GetCurrentLine()
        while text[:ps2size] == ps2 and line > 0:
            line -= 1
            self.GotoLine(line)
            text = self.GetCurLine()[0]
        if text[:ps1size] == ps1:
            line = self.GetCurrentLine()
            self.GotoLine(line)
            startpos = self.GetCurrentPos() + ps1size
            line += 1
            self.GotoLine(line)
            while self.GetCurLine()[0][:ps2size] == ps2:
                line += 1
                self.GotoLine(line)
            stoppos = self.GetCurrentPos()
            command = self.GetTextRange(startpos, stoppos)
            command = command.replace(os.linesep + ps2, '\n')
            command = command.rstrip()
            command = command.replace('\n', os.linesep + ps2)
        else:
            command = ''
        if rstrip:
            command = command.rstrip()
        return command

    def getCommand(self, text=None, rstrip=1):
        """Extract a command from text which may include a shell prompt.

        The command may not necessarily be valid Python syntax."""
        if not text:
            text = self.GetCurLine()[0]
        # Strip the prompt off the front leaving just the command.
        command = self.lstripPrompt(text)
        if command == text:
            command = ''  # Real commands have prompts.
        if rstrip:
            command = command.rstrip()
        return command

    def lstripPrompt(self, text):
        """Return text without a leading prompt."""
        ps1 = str(sys.ps1)
        ps1size = len(ps1)
        ps2 = str(sys.ps2)
        ps2size = len(ps2)
        # Strip the prompt off the front of text.
        if text[:ps1size] == ps1:
            text = text[ps1size:]
        elif text[:ps2size] == ps2:
            text = text[ps2size:]
        return text
    
    def push(self, command):
        """Send command to the interpreter for execution."""
        self.write(os.linesep)
        busy = wx.wxBusyCursor()
        self.waiting = True
        self.more = self.interp.push(command)
        self.waiting = False
        del busy
        if not self.more:
            self.addHistory(command.rstrip())
        self.prompt()

    def addHistory(self, command):
        """Add command to the command history."""
        # Reset the history position.
        self.historyIndex = -1
        # Insert this command into the history, unless it's a blank
        # line or the same as the last command.
        if command != '' \
        and (len(self.history) == 0 or command != self.history[0]):
            self.history.insert(0, command)

    def write(self, text):
        """Display text in the shell.

        Replace line endings with OS-specific endings."""
        text = self.fixLineEndings(text)
        self.AddText(text)
        self.EnsureCaretVisible()

    def fixLineEndings(self, text):
        """Return text with line endings replaced by OS-specific endings."""
        lines = text.split('\r\n')
        for l in range(len(lines)):
            chunks = lines[l].split('\r')
            for c in range(len(chunks)):
                chunks[c] = os.linesep.join(chunks[c].split('\n'))
            lines[l] = os.linesep.join(chunks)
        text = os.linesep.join(lines)
        return text

    def prompt(self):
        """Display proper prompt for the context: ps1, ps2 or ps3.

        If this is a continuation line, autoindent as necessary."""
        isreading = self.reader.isreading
        skip = 0
        if isreading:
            prompt = str(sys.ps3)
        elif self.more:
            prompt = str(sys.ps2)
        else:
            prompt = str(sys.ps1)
        pos = self.GetCurLine()[1]
        if pos > 0:
            if isreading:
                skip = 1
            else:
                self.write(os.linesep)
        if not self.more:
            self.promptPosStart = self.GetCurrentPos()
        if not skip:
            self.write(prompt)
        if not self.more:
            self.promptPosEnd = self.GetCurrentPos()
            # Keep the undo feature from undoing previous responses.
            self.EmptyUndoBuffer()
        # XXX Add some autoindent magic here if more.
        if self.more:
            self.write(' '*4)  # Temporary hack indentation.
        self.EnsureCaretVisible()
        self.ScrollToColumn(0)

    def readline(self):
        """Replacement for stdin.readline()."""
        input = ''
        reader = self.reader
        reader.isreading = 1
        self.prompt()
        try:
            while not reader.input:
                wx.wxYieldIfNeeded()
            input = reader.input
        finally:
            reader.input = ''
            reader.isreading = 0
        input = str(input)  # In case of Unicode.
        return input

    def readlines(self):
        """Replacement for stdin.readlines()."""
        lines = []
        while lines[-1:] != ['\n']:
            lines.append(self.readline())
        return lines

    def raw_input(self, prompt=''):
        """Return string based on user input."""
        if prompt:
            self.write(prompt)
        return self.readline()

    def ask(self, prompt='Please enter your response:'):
        """Get response from the user using a dialog box."""
        dialog = wx.wxTextEntryDialog(None, prompt, 
                                      'Input Dialog (Raw)', '')
        try:
            if dialog.ShowModal() == wx.wxID_OK:
                text = dialog.GetValue()
                return text
        finally:
            dialog.Destroy()
        return ''

    def pause(self):
        """Halt execution pending a response from the user."""
        self.ask('Press enter to continue:')

    def clear(self):
        """Delete all text from the shell."""
        self.ClearAll()

    def run(self, command, prompt=1, verbose=1):
        """Execute command as if it was typed in directly.
        >>> shell.run('print "this"')
        >>> print "this"
        this
        >>> 
        """
        # Go to the very bottom of the text.
        endpos = self.GetTextLength()
        self.SetCurrentPos(endpos)        
        command = command.rstrip()
        if prompt: self.prompt()
        if verbose: self.write(command)
        self.push(command)

    def runfile(self, filename):
        """Execute all commands in file as if they were typed into the
        shell."""
        file = open(filename)
        try:
            self.prompt()
            for command in file.readlines():
                if command[:6] == 'shell.':
                    # Run shell methods silently.
                    self.run(command, prompt=0, verbose=0)
                else:
                    self.run(command, prompt=0, verbose=1)
        finally:
            file.close()

    def autoCompleteShow(self, command):
        """Display auto-completion popup list."""
        list = self.interp.getAutoCompleteList(command, 
                    includeMagic=self.autoCompleteIncludeMagic, 
                    includeSingle=self.autoCompleteIncludeSingle, 
                    includeDouble=self.autoCompleteIncludeDouble)
        if list and len(list) < 2000:
            options = ' '.join(list)
            offset = 0
            self.AutoCompShow(offset, options)

    def autoCallTipShow(self, command):
        """Display argument spec and docstring in a popup window."""
        if self.CallTipActive():
            self.CallTipCancel()
        (name, argspec, tip) = self.interp.getCallTip(command)
        if argspec:
            startpos = self.GetCurrentPos()
            self.write(argspec + ')')
            endpos = self.GetCurrentPos()
            self.SetSelection(endpos, startpos)
        if tip:
            curpos = self.GetCurrentPos()
            tippos = curpos - (len(name) + 1)
            fallback = curpos - self.GetColumn(curpos)
            # In case there isn't enough room, only go back to the
            # fallback.
            tippos = max(tippos, fallback)
            dispatcher.send(signal='Shell.calltip', sender=self, calltip=tip)
            self.CallTipShow(tippos, tip)

    def writeOut(self, text):
        """Replacement for stdout."""
        self.write(text)

    def writeErr(self, text):
        """Replacement for stderr."""
        self.write(text)

    def redirectStdin(self, redirect=1):
        """If redirect is true then sys.stdin will come from the shell."""
        if redirect:
            sys.stdin = self.reader
        else:
            sys.stdin = self.stdin

    def redirectStdout(self, redirect=1):
        """If redirect is true then sys.stdout will go to the shell."""
        if redirect:
            sys.stdout = PseudoFileOut(self.writeOut)
        else:
            sys.stdout = self.stdout

    def redirectStderr(self, redirect=1):
        """If redirect is true then sys.stderr will go to the shell."""
        if redirect:
            sys.stderr = PseudoFileErr(self.writeErr)
        else:
            sys.stderr = self.stderr

    def CanCut(self):
        """Return true if text is selected and can be cut."""
        if self.GetSelectionStart() != self.GetSelectionEnd() \
               and self.GetSelectionStart() >= self.promptPosEnd \
               and self.GetSelectionEnd() >= self.promptPosEnd:
            return 1
        else:
            return 0

    def CanCopy(self):
        """Return true if text is selected and can be copied."""
        return self.GetSelectionStart() != self.GetSelectionEnd()

    def CanPaste(self):
        """Return true if a paste should succeed."""
        if self.CanEdit() and stc.wxStyledTextCtrl.CanPaste(self):
            return 1
        else:
            return 0

    def CanEdit(self):
        """Return true if editing should succeed."""
        if self.GetSelectionStart() != self.GetSelectionEnd():
            if self.GetSelectionStart() >= self.promptPosEnd \
                   and self.GetSelectionEnd() >= self.promptPosEnd:
                return 1
            else:
                return 0
        else:
            return self.GetCurrentPos() >= self.promptPosEnd

    def Cut(self):
        """Remove selection and place it on the clipboard."""
        if self.CanCut() and self.CanCopy():
            if self.AutoCompActive():
                self.AutoCompCancel()
            if self.CallTipActive():
                self.CallTipCancel()
            self.Copy()
            self.ReplaceSelection('')

    def Copy(self):
        """Copy selection and place it on the clipboard."""
        if self.CanCopy():
            ps1 = str(sys.ps1)
            ps2 = str(sys.ps2)
            command = self.GetSelectedText()
            command = command.replace(os.linesep + ps2, os.linesep)
            command = command.replace(os.linesep + ps1, os.linesep)
            command = self.lstripPrompt(text=command)
            data = wx.wxTextDataObject(command)
            self._clip(data)

    def CopyWithPrompts(self):
        """Copy selection, including prompts, and place it on the clipboard."""
        if self.CanCopy():
            command = self.GetSelectedText()
            data = wx.wxTextDataObject(command)
            self._clip(data)

    def CopyWithPromptsPrefixed(self):
        """Copy selection, including prompts prefixed with four
        spaces, and place it on the clipboard."""
        if self.CanCopy():
            command = self.GetSelectedText()
            spaces = ' ' * 4
            command = spaces + command.replace(os.linesep,
                                               os.linesep + spaces)
            data = wx.wxTextDataObject(command)
            self._clip(data)

    def _clip(self, data):
        if wx.wxTheClipboard.Open():
            wx.wxTheClipboard.UsePrimarySelection(False)
            wx.wxTheClipboard.SetData(data)
            wx.wxTheClipboard.Flush()
            wx.wxTheClipboard.Close()

    def Paste(self):
        """Replace selection with clipboard contents."""
        if self.CanPaste() and wx.wxTheClipboard.Open():
            ps2 = str(sys.ps2)
            if wx.wxTheClipboard.IsSupported(wx.wxDataFormat(wx.wxDF_TEXT)):
                data = wx.wxTextDataObject()
                if wx.wxTheClipboard.GetData(data):
                    self.ReplaceSelection('')
                    command = data.GetText()
                    command = command.rstrip()
                    command = self.fixLineEndings(command)
                    command = self.lstripPrompt(text=command)
                    command = command.replace(os.linesep + ps2, '\n')
                    command = command.replace(os.linesep, '\n')
                    command = command.replace('\n', os.linesep + ps2)
                    self.write(command)
            wx.wxTheClipboard.Close()

    def PasteAndRun(self):
        """Replace selection with clipboard contents, run commands."""
        if wx.wxTheClipboard.Open():
            ps1 = str(sys.ps1)
            ps2 = str(sys.ps2)
            if wx.wxTheClipboard.IsSupported(wx.wxDataFormat(wx.wxDF_TEXT)):
                data = wx.wxTextDataObject()
                if wx.wxTheClipboard.GetData(data):
                    endpos = self.GetTextLength()
                    self.SetCurrentPos(endpos)
                    startpos = self.promptPosEnd
                    self.SetSelection(startpos, endpos)
                    self.ReplaceSelection('')
                    text = data.GetText()
                    text = text.lstrip()
                    text = self.fixLineEndings(text)
                    text = self.lstripPrompt(text)
                    text = text.replace(os.linesep + ps1, '\n')
                    text = text.replace(os.linesep + ps2, '\n')
                    text = text.replace(os.linesep, '\n')
                    lines = text.split('\n')
                    commands = []
                    command = ''
                    for line in lines:
                        if line.strip() == ps2.strip():
                            # If we are pasting from something like a
                            # web page that drops the trailing space
                            # from the ps2 prompt of a blank line.
                            line = ''
                        if line.strip() != '' and line.lstrip() == line:
                            # New command.
                            if command:
                                # Add the previous command to the list.
                                commands.append(command)
                            # Start a new command, which may be multiline.
                            command = line
                        else:
                            # Multiline command. Add to the command.
                            command += '\n'
                            command += line
                    commands.append(command)
                    for command in commands:    
                        command = command.replace('\n', os.linesep + ps2)
                        self.write(command)
                        self.processLine()
            wx.wxTheClipboard.Close()

    def wrap(self, wrap=1):
        """Sets whether text is word wrapped."""
        try:
            self.SetWrapMode(wrap)
        except AttributeError:
            return 'Wrapping is not available in this version of PyCrust.'

    def zoom(self, points=0):
        """Set the zoom level.
        
        This number of points is added to the size of all fonts.  It
        may be positive to magnify or negative to reduce."""
        self.SetZoom(points)

