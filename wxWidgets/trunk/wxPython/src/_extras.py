#----------------------------------------------------------------------------
# Name:         _extra.py
# Purpose:	This file is appended to the shadow class file generated
#               by SWIG.  We add some unSWIGable things here.
#
# Author:       Robin Dunn
#
# Created:      6/30/97
# RCS-ID:       $Id$
# Copyright:    (c) 1998 by Total Control Software
# Licence:      wxWindows license
#----------------------------------------------------------------------------

import sys

#----------------------------------------------------------------------
# This gives this module's dictionary to the C++ extension code...

_wxSetDictionary(vars())


#----------------------------------------------------------------------
#----------------------------------------------------------------------
# Helper function to link python methods to wxWindows virtual
# functions by name.

def _checkForCallback(obj, name, event, theID=-1):
    try:    cb = getattr(obj, name)
    except: pass
    else:   obj.Connect(theID, -1, event, cb)


#----------------------------------------------------------------------
#----------------------------------------------------------------------
# functions that look and act like the C++ Macros of the same name


# Miscellaneous
def EVT_SIZE(win, func):
    win.Connect(-1, -1, wxEVT_SIZE, func)

def EVT_MOVE(win, func):
    win.Connect(-1, -1, wxEVT_MOVE, func)

def EVT_CLOSE(win, func):
    win.Connect(-1, -1, wxEVT_CLOSE_WINDOW, func)

def EVT_PAINT(win, func):
    win.Connect(-1, -1, wxEVT_PAINT, func)

def EVT_ERASE_BACKGROUND(win, func):
    win.Connect(-1, -1, wxEVT_ERASE_BACKGROUND, func)

def EVT_CHAR(win, func):
    win.Connect(-1, -1, wxEVT_CHAR, func)

def EVT_CHAR_HOOK(win, func):
    win.Connect(-1, -1, wxEVT_CHAR_HOOK, func)

def EVT_KEY_DOWN(win, func):
    win.Connect(-1, -1, wxEVT_KEY_DOWN, func)

def EVT_KEY_UP(win, func):
    win.Connect(-1, -1, wxEVT_KEY_UP, func)

def EVT_MENU_HIGHLIGHT(win, id, func):
    win.Connect(id, -1, wxEVT_MENU_HIGHLIGHT, func)

def EVT_MENU_HIGHLIGHT_ALL(win, func):
    win.Connect(-1, -1, wxEVT_MENU_HIGHLIGHT, func)

def EVT_SET_FOCUS(win, func):
    win.Connect(-1, -1, wxEVT_SET_FOCUS, func)

def EVT_KILL_FOCUS(win, func):
    win.Connect(-1, -1, wxEVT_KILL_FOCUS, func)

def EVT_ACTIVATE(win, func):
    win.Connect(-1, -1, wxEVT_ACTIVATE, func)

def EVT_ACTIVATE_APP(win, func):
    win.Connect(-1, -1, wxEVT_ACTIVATE_APP, func)

def EVT_END_SESSION(win, func):
    win.Connect(-1, -1, wxEVT_END_SESSION, func)

def EVT_QUERY_END_SESSION(win, func):
    win.Connect(-1, -1, wxEVT_QUERY_END_SESSION, func)

def EVT_DROP_FILES(win, func):
    win.Connect(-1, -1, wxEVT_DROP_FILES, func)

def EVT_INIT_DIALOG(win, func):
    win.Connect(-1, -1, wxEVT_INIT_DIALOG, func)

def EVT_SYS_COLOUR_CHANGED(win, func):
    win.Connect(-1, -1, wxEVT_SYS_COLOUR_CHANGED, func)

def EVT_SHOW(win, func):
    win.Connect(-1, -1, wxEVT_SHOW, func)

def EVT_MAXIMIZE(win, func):
    win.Connect(-1, -1, wxEVT_MAXIMIZE, func)

def EVT_ICONIZE(win, func):
    win.Connect(-1, -1, wxEVT_ICONIZE, func)

def EVT_NAVIGATION_KEY(win, func):
    win.Connect(-1, -1, wxEVT_NAVIGATION_KEY, func)

def EVT_PALETTE_CHANGED(win, func):
    win.Connect(-1, -1, wxEVT_PALETTE_CHANGED, func)

def EVT_QUERY_NEW_PALETTE(win, func):
    win.Connect(-1, -1, wxEVT_QUERY_NEW_PALETTE, func)

def EVT_WINDOW_CREATE(win, func):
    win.Connect(-1, -1, wxEVT_CREATE, func)

def EVT_WINDOW_DESTROY(win, func):
    win.Connect(-1, -1, wxEVT_DESTROY, func)



def EVT_IDLE(win, func):
    win.Connect(-1, -1, wxEVT_IDLE, func)

def EVT_UPDATE_UI(win, id, func):
    win.Connect(id, -1, wxEVT_UPDATE_UI, func)

def EVT_UPDATE_UI_RANGE(win, id, id2, func):
    win.Connect(id, id2, wxEVT_UPDATE_UI, func)


# Mouse Events
def EVT_LEFT_DOWN(win, func):
    win.Connect(-1, -1, wxEVT_LEFT_DOWN, func)

def EVT_LEFT_UP(win, func):
    win.Connect(-1, -1, wxEVT_LEFT_UP, func)

def EVT_MIDDLE_DOWN(win, func):
    win.Connect(-1, -1, wxEVT_MIDDLE_DOWN, func)

def EVT_MIDDLE_UP(win, func):
    win.Connect(-1, -1, wxEVT_MIDDLE_UP, func)

def EVT_RIGHT_DOWN(win, func):
    win.Connect(-1, -1, wxEVT_RIGHT_DOWN, func)

def EVT_RIGHT_UP(win, func):
    win.Connect(-1, -1, wxEVT_RIGHT_UP, func)

def EVT_MOTION(win, func):
    win.Connect(-1, -1, wxEVT_MOTION, func)

def EVT_LEFT_DCLICK(win, func):
    win.Connect(-1, -1, wxEVT_LEFT_DCLICK, func)

def EVT_MIDDLE_DCLICK(win, func):
    win.Connect(-1, -1, wxEVT_MIDDLE_DCLICK, func)

def EVT_RIGHT_DCLICK(win, func):
    win.Connect(-1, -1, wxEVT_RIGHT_DCLICK, func)

def EVT_LEAVE_WINDOW(win, func):
    win.Connect(-1, -1, wxEVT_LEAVE_WINDOW, func)

def EVT_ENTER_WINDOW(win, func):
    win.Connect(-1, -1, wxEVT_ENTER_WINDOW, func)

def EVT_MOUSEWHEEL(win, func):
    win.Connect(-1, -1, wxEVT_MOUSEWHEEL, func)

# all mouse events
def EVT_MOUSE_EVENTS(win, func):
    win.Connect(-1, -1, wxEVT_LEFT_DOWN,     func)
    win.Connect(-1, -1, wxEVT_LEFT_UP,       func)
    win.Connect(-1, -1, wxEVT_MIDDLE_DOWN,   func)
    win.Connect(-1, -1, wxEVT_MIDDLE_UP,     func)
    win.Connect(-1, -1, wxEVT_RIGHT_DOWN,    func)
    win.Connect(-1, -1, wxEVT_RIGHT_UP,      func)
    win.Connect(-1, -1, wxEVT_MOTION,        func)
    win.Connect(-1, -1, wxEVT_LEFT_DCLICK,   func)
    win.Connect(-1, -1, wxEVT_MIDDLE_DCLICK, func)
    win.Connect(-1, -1, wxEVT_RIGHT_DCLICK,  func)
    win.Connect(-1, -1, wxEVT_LEAVE_WINDOW,  func)
    win.Connect(-1, -1, wxEVT_ENTER_WINDOW,  func)

# EVT_COMMAND
def EVT_COMMAND(win, id, cmd, func):
    win.Connect(id, -1, cmd, func)

def EVT_COMMAND_RANGE(win, id1, id2, cmd, func):
    win.Connect(id1, id2, cmd, func)


# Scrolling
def EVT_SCROLL(win, func):
    win.Connect(-1, -1, wxEVT_SCROLL_TOP,       func)
    win.Connect(-1, -1, wxEVT_SCROLL_BOTTOM,    func)
    win.Connect(-1, -1, wxEVT_SCROLL_LINEUP,    func)
    win.Connect(-1, -1, wxEVT_SCROLL_LINEDOWN,  func)
    win.Connect(-1, -1, wxEVT_SCROLL_PAGEUP,    func)
    win.Connect(-1, -1, wxEVT_SCROLL_PAGEDOWN,  func)
    win.Connect(-1, -1, wxEVT_SCROLL_THUMBTRACK,func)
    win.Connect(-1, -1, wxEVT_SCROLL_THUMBRELEASE,func)

def EVT_SCROLL_TOP(win, func):
    win.Connect(-1, -1, wxEVT_SCROLL_TOP, func)

def EVT_SCROLL_BOTTOM(win, func):
    win.Connect(-1, -1, wxEVT_SCROLL_BOTTOM, func)

def EVT_SCROLL_LINEUP(win, func):
    win.Connect(-1, -1, wxEVT_SCROLL_LINEUP, func)

def EVT_SCROLL_LINEDOWN(win, func):
    win.Connect(-1, -1, wxEVT_SCROLL_LINEDOWN, func)

def EVT_SCROLL_PAGEUP(win, func):
    win.Connect(-1, -1, wxEVT_SCROLL_PAGEUP, func)

def EVT_SCROLL_PAGEDOWN(win, func):
    win.Connect(-1, -1, wxEVT_SCROLL_PAGEDOWN, func)

def EVT_SCROLL_THUMBTRACK(win, func):
    win.Connect(-1, -1, wxEVT_SCROLL_THUMBTRACK, func)

def EVT_SCROLL_THUMBRELEASE(win, func):
    win.Connect(-1, -1, wxEVT_SCROLL_THUMBRELEASE, func)



# Scrolling, with an id
def EVT_COMMAND_SCROLL(win, id, func):
    win.Connect(id, -1, wxEVT_SCROLL_TOP,       func)
    win.Connect(id, -1, wxEVT_SCROLL_BOTTOM,    func)
    win.Connect(id, -1, wxEVT_SCROLL_LINEUP,    func)
    win.Connect(id, -1, wxEVT_SCROLL_LINEDOWN,  func)
    win.Connect(id, -1, wxEVT_SCROLL_PAGEUP,    func)
    win.Connect(id, -1, wxEVT_SCROLL_PAGEDOWN,  func)
    win.Connect(id, -1, wxEVT_SCROLL_THUMBTRACK,func)
    win.Connect(id, -1, wxEVT_SCROLL_THUMBRELEASE,func)

def EVT_COMMAND_SCROLL_TOP(win, id, func):
    win.Connect(id, -1, wxEVT_SCROLL_TOP, func)

def EVT_COMMAND_SCROLL_BOTTOM(win, id, func):
    win.Connect(id, -1, wxEVT_SCROLL_BOTTOM, func)

def EVT_COMMAND_SCROLL_LINEUP(win, id, func):
    win.Connect(id, -1, wxEVT_SCROLL_LINEUP, func)

def EVT_COMMAND_SCROLL_LINEDOWN(win, id, func):
    win.Connect(id, -1, wxEVT_SCROLL_LINEDOWN, func)

def EVT_COMMAND_SCROLL_PAGEUP(win, id, func):
    win.Connect(id, -1, wxEVT_SCROLL_PAGEUP, func)

def EVT_COMMAND_SCROLL_PAGEDOWN(win, id, func):
    win.Connect(id, -1, wxEVT_SCROLL_PAGEDOWN, func)

def EVT_COMMAND_SCROLL_THUMBTRACK(win, id, func):
    win.Connect(id, -1, wxEVT_SCROLL_THUMBTRACK, func)

def EVT_COMMAND_SCROLL_THUMBRELEASE(win, id, func):
    win.Connect(id, -1, wxEVT_SCROLL_THUMBRELEASE, func)

#---
def EVT_SCROLLWIN(win, func):
    win.Connect(-1, -1, wxEVT_SCROLLWIN_TOP,         func)
    win.Connect(-1, -1, wxEVT_SCROLLWIN_BOTTOM,      func)
    win.Connect(-1, -1, wxEVT_SCROLLWIN_LINEUP,      func)
    win.Connect(-1, -1, wxEVT_SCROLLWIN_LINEDOWN,    func)
    win.Connect(-1, -1, wxEVT_SCROLLWIN_PAGEUP,      func)
    win.Connect(-1, -1, wxEVT_SCROLLWIN_PAGEDOWN,    func)
    win.Connect(-1, -1, wxEVT_SCROLLWIN_THUMBTRACK,  func)
    win.Connect(-1, -1, wxEVT_SCROLLWIN_THUMBRELEASE,func)

def EVT_SCROLLWIN_TOP(win, func):
    win.Connect(-1, -1, wxEVT_SCROLLWIN_TOP, func)

def EVT_SCROLLWIN_BOTTOM(win, func):
    win.Connect(-1, -1, wxEVT_SCROLLWIN_BOTTOM, func)

def EVT_SCROLLWIN_LINEUP(win, func):
    win.Connect(-1, -1, wxEVT_SCROLLWIN_LINEUP, func)

def EVT_SCROLLWIN_LINEDOWN(win, func):
    win.Connect(-1, -1, wxEVT_SCROLLWIN_LINEDOWN, func)

def EVT_SCROLLWIN_PAGEUP(win, func):
    win.Connect(-1, -1, wxEVT_SCROLLWIN_PAGEUP, func)

def EVT_SCROLLWIN_PAGEDOWN(win, func):
    win.Connect(-1, -1, wxEVT_SCROLLWIN_PAGEDOWN, func)

def EVT_SCROLLWIN_THUMBTRACK(win, func):
    win.Connect(-1, -1, wxEVT_SCROLLWIN_THUMBTRACK, func)

def EVT_SCROLLWIN_THUMBRELEASE(win, func):
    win.Connect(-1, -1, wxEVT_SCROLLWIN_THUMBRELEASE, func)



# Scrolling, with an id
def EVT_COMMAND_SCROLLWIN(win, id, func):
    win.Connect(id, -1, wxEVT_SCROLLWIN_TOP,         func)
    win.Connect(id, -1, wxEVT_SCROLLWIN_BOTTOM,      func)
    win.Connect(id, -1, wxEVT_SCROLLWIN_LINEUP,      func)
    win.Connect(id, -1, wxEVT_SCROLLWIN_LINEDOWN,    func)
    win.Connect(id, -1, wxEVT_SCROLLWIN_PAGEUP,      func)
    win.Connect(id, -1, wxEVT_SCROLLWIN_PAGEDOWN,    func)
    win.Connect(id, -1, wxEVT_SCROLLWIN_THUMBTRACK,  func)
    win.Connect(id, -1, wxEVT_SCROLLWIN_THUMBRELEASE,func)

def EVT_COMMAND_SCROLLWIN_TOP(win, id, func):
    win.Connect(id, -1, wxEVT_SCROLLWIN_TOP, func)

def EVT_COMMAND_SCROLLWIN_BOTTOM(win, id, func):
    win.Connect(id, -1, wxEVT_SCROLLWIN_BOTTOM, func)

def EVT_COMMAND_SCROLLWIN_LINEUP(win, id, func):
    win.Connect(id, -1, wxEVT_SCROLLWIN_LINEUP, func)

def EVT_COMMAND_SCROLLWIN_LINEDOWN(win, id, func):
    win.Connect(id, -1, wxEVT_SCROLLWIN_LINEDOWN, func)

def EVT_COMMAND_SCROLLWIN_PAGEUP(win, id, func):
    win.Connect(id, -1, wxEVT_SCROLLWIN_PAGEUP, func)

def EVT_COMMAND_SCROLLWIN_PAGEDOWN(win, id, func):
    win.Connect(id, -1, wxEVT_SCROLLWIN_PAGEDOWN, func)

def EVT_COMMAND_SCROLLWIN_THUMBTRACK(win, id, func):
    win.Connect(id, -1, wxEVT_SCROLLWIN_THUMBTRACK, func)

def EVT_COMMAND_SCROLLWIN_THUMBRELEASE(win, id, func):
    win.Connect(id, -1, wxEVT_SCROLLWIN_THUMBRELEASE, func)


# Convenience commands
def EVT_BUTTON(win, id, func):
    win.Connect(id, -1, wxEVT_COMMAND_BUTTON_CLICKED, func)

def EVT_CHECKBOX(win, id, func):
    win.Connect(id, -1, wxEVT_COMMAND_CHECKBOX_CLICKED, func)

def EVT_CHOICE(win, id, func):
    win.Connect(id, -1, wxEVT_COMMAND_CHOICE_SELECTED, func)

def EVT_LISTBOX(win, id, func):
    win.Connect(id, -1, wxEVT_COMMAND_LISTBOX_SELECTED, func)

def EVT_LISTBOX_DCLICK(win, id, func):
    win.Connect(id, -1, wxEVT_COMMAND_LISTBOX_DOUBLECLICKED, func)

def EVT_TEXT(win, id, func):
    win.Connect(id, -1, wxEVT_COMMAND_TEXT_UPDATED, func)

def EVT_TEXT_ENTER(win, id, func):
    win.Connect(id, -1, wxEVT_COMMAND_TEXT_ENTER, func)

def EVT_TEXT_URL(win, id, func):
    win.Connect(id, -1, wxEVT_COMMAND_TEXT_URL, func)

def EVT_TEXT_MAXLEN(win, id, func):
    win.Connect(id, -1, wxEVT_COMMAND_TEXT_MAXLEN, func)

def EVT_MENU(win, id, func):
    win.Connect(id, -1, wxEVT_COMMAND_MENU_SELECTED, func)

def EVT_MENU_RANGE(win, id1, id2, func):
    win.Connect(id1, id2, wxEVT_COMMAND_MENU_SELECTED, func)

def EVT_SLIDER(win, id, func):
    win.Connect(id, -1, wxEVT_COMMAND_SLIDER_UPDATED, func)

def EVT_RADIOBOX(win, id, func):
    win.Connect(id, -1, wxEVT_COMMAND_RADIOBOX_SELECTED, func)

def EVT_RADIOBUTTON(win, id, func):
    win.Connect(id, -1, wxEVT_COMMAND_RADIOBUTTON_SELECTED, func)

def EVT_VLBOX(win, id, func):
    win.Connect(id, -1, wxEVT_COMMAND_VLBOX_SELECTED, func)

def EVT_COMBOBOX(win, id, func):
    win.Connect(id, -1, wxEVT_COMMAND_COMBOBOX_SELECTED, func)

def EVT_TOOL(win, id, func):
    win.Connect(id, -1, wxEVT_COMMAND_TOOL_CLICKED, func)

def EVT_TOOL_RANGE(win, id, id2, func):
    win.Connect(id, id2, wxEVT_COMMAND_TOOL_CLICKED, func)

def EVT_TOOL_RCLICKED(win, id, func):
    win.Connect(id, -1, wxEVT_COMMAND_TOOL_RCLICKED, func)

def EVT_TOOL_RCLICKED_RANGE(win, id, id2, func):
    win.Connect(id, id2, wxEVT_COMMAND_TOOL_RCLICKED, func)

def EVT_TOOL_ENTER(win, id, func):
    win.Connect(id, -1, wxEVT_COMMAND_TOOL_ENTER, func)

def EVT_CHECKLISTBOX(win, id, func):
    win.Connect(id, -1, wxEVT_COMMAND_CHECKLISTBOX_TOGGLED, func)

def EVT_SPINCTRL(win, id, func):
    win.Connect(id, -1, wxEVT_COMMAND_SPINCTRL_UPDATED, func)



# Generic command events

def EVT_COMMAND_LEFT_CLICK(win, id, func):
    win.Connect(id, -1, wxEVT_COMMAND_LEFT_CLICK, func)

def EVT_COMMAND_LEFT_DCLICK(win, id, func):
    win.Connect(id, -1, wxEVT_COMMAND_LEFT_DCLICK, func)

def EVT_COMMAND_RIGHT_CLICK(win, id, func):
    win.Connect(id, -1, wxEVT_COMMAND_RIGHT_CLICK, func)

def EVT_COMMAND_RIGHT_DCLICK(win, id, func):
    win.Connect(id, -1, wxEVT_COMMAND_RIGHT_DCLICK, func)

def EVT_COMMAND_SET_FOCUS(win, id, func):
    win.Connect(id, -1, wxEVT_COMMAND_SET_FOCUS, func)

def EVT_COMMAND_KILL_FOCUS(win, id, func):
    win.Connect(id, -1, wxEVT_COMMAND_KILL_FOCUS, func)

def EVT_COMMAND_ENTER(win, id, func):
    win.Connect(id, -1, wxEVT_COMMAND_ENTER, func)


# wxNotebook events
def EVT_NOTEBOOK_PAGE_CHANGED(win, id, func):
    win.Connect(id, -1, wxEVT_COMMAND_NOTEBOOK_PAGE_CHANGED, func)

def EVT_NOTEBOOK_PAGE_CHANGING(win, id, func):
    win.Connect(id, -1, wxEVT_COMMAND_NOTEBOOK_PAGE_CHANGING, func)


# wxSpinButton
def EVT_SPIN_UP(win, id, func):
    win.Connect(id, -1, wxEVT_SCROLL_LINEUP, func)

def EVT_SPIN_DOWN(win, id, func):
    win.Connect(id, -1, wxEVT_SCROLL_LINEDOWN, func)

def EVT_SPIN(win, id, func):
    win.Connect(id, -1, wxEVT_SCROLL_THUMBTRACK,func)


# wxTaskBarIcon
def EVT_TASKBAR_MOVE(win, func):
    win.Connect(-1, -1, wxEVT_TASKBAR_MOVE, func)

def EVT_TASKBAR_LEFT_DOWN(win, func):
    win.Connect(-1, -1, wxEVT_TASKBAR_LEFT_DOWN, func)

def EVT_TASKBAR_LEFT_UP(win, func):
    win.Connect(-1, -1, wxEVT_TASKBAR_LEFT_UP, func)

def EVT_TASKBAR_RIGHT_DOWN(win, func):
    win.Connect(-1, -1, wxEVT_TASKBAR_RIGHT_DOWN, func)

def EVT_TASKBAR_RIGHT_UP(win, func):
    win.Connect(-1, -1, wxEVT_TASKBAR_RIGHT_UP, func)

def EVT_TASKBAR_LEFT_DCLICK(win, func):
    win.Connect(-1, -1, wxEVT_TASKBAR_LEFT_DCLICK, func)

def EVT_TASKBAR_RIGHT_DCLICK(win, func):
    win.Connect(-1, -1, wxEVT_TASKBAR_RIGHT_DCLICK, func)


# wxSashWindow
def EVT_SASH_DRAGGED(win, id, func):
    win.Connect(id, -1, wxEVT_SASH_DRAGGED, func)

def EVT_SASH_DRAGGED_RANGE(win, id1, id2, func):
    win.Connect(id1, id2, wxEVT_SASH_DRAGGED, func)

def EVT_QUERY_LAYOUT_INFO(win, func):
    win.Connect(-1, -1, wxEVT_EVT_QUERY_LAYOUT_INFO, func)

def EVT_CALCULATE_LAYOUT(win, func):
    win.Connect(-1, -1, wxEVT_EVT_CALCULATE_LAYOUT, func)


#wxSplitterWindow
def EVT_SPLITTER_SASH_POS_CHANGING(win, id, func):
    win.Connect(id, -1, wxEVT_COMMAND_SPLITTER_SASH_POS_CHANGING, func)

def EVT_SPLITTER_SASH_POS_CHANGED(win, id, func):
    win.Connect(id, -1, wxEVT_COMMAND_SPLITTER_SASH_POS_CHANGED, func)

def EVT_SPLITTER_UNSPLIT(win, id, func):
    win.Connect(id, -1, wxEVT_COMMAND_SPLITTER_UNSPLIT, func)

def EVT_SPLITTER_DOUBLECLICKED(win, id, func):
    win.Connect(id, -1, wxEVT_COMMAND_SPLITTER_DOUBLECLICKED, func)


# wxTimer
def EVT_TIMER(win, id, func):
    win.Connect(id, -1, wxEVT_TIMER, func)

# wxProcess
def EVT_END_PROCESS(eh, id, func):
    eh.Connect(id, -1, wxEVT_END_PROCESS, func)


# wxJoyStick
def EVT_JOY_DOWN(win, func):
    win.Connect(-1, -1, wxEVT_JOY_BUTTON_DOWN, func)

def EVT_JOY_UP(win, func):
    win.Connect(-1, -1, wxEVT_JOY_BUTTON_UP, func)

def EVT_JOY_MOVE(win, func):
    win.Connect(-1, -1, wxEVT_JOY_MOVE, func)

def EVT_JOY_ZMOVE(win, func):
    win.Connect(-1, -1, wxEVT_JOY_ZMOVE, func)

def EVT_JOYSTICK_EVENTS(win, func):
    win.Connect(-1, -1, wxEVT_JOY_BUTTON_DOWN, func)
    win.Connect(-1, -1, wxEVT_JOY_BUTTON_UP, func)
    win.Connect(-1, -1, wxEVT_JOY_MOVE, func)
    win.Connect(-1, -1, wxEVT_JOY_ZMOVE, func)


def EVT_TOGGLEBUTTON(win, id, func):
    win.Connect(id, -1, wxEVT_COMMAND_TOGGLEBUTTON_CLICKED, func)



# Help events
def EVT_HELP(win, id, func):
    win.Connect(id, -1, wxEVT_HELP, func)

def EVT_HELP_RANGE(win, id, id2, func):
    win.Connect(id, id2, wxEVT_HELP, func)

def EVT_DETAILED_HELP(win, id, func):
    win.Connect(id, -1, wxEVT_DETAILED_HELP, func)

def EVT_DETAILED_HELP_RANGE(win, id, id2, func):
    win.Connect(id, id2, wxEVT_DETAILED_HELP, func)

def EVT_CONTEXT_MENU(win, func):
    win.Connect(-1, -1, wxEVT_CONTEXT_MENU, func)


#----------------------------------------------------------------------

class wxTimer(wxPyTimer):
    def __init__(self, evtHandler = None, id = -1):
        if evtHandler is None:
            wxPyTimer.__init__(self, self.Notify)   # derived class must provide
                                                    # Notify(self) method.
        else:
            wxPyTimer.__init__(self, None)
            self.SetOwner(evtHandler, id)

#----------------------------------------------------------------------

NULL = None  # For backwards compatibility only.  You should really be
             # using None now.


#----------------------------------------------------------------------
# aliases

wxColor      = wxColour
wxNamedColor = wxNamedColour
wxPen        = wxPyPen

# backwards compatibility
wxNoRefBitmap       = wxBitmap
wxPyDefaultPosition = wxDefaultPosition
wxPyDefaultSize     = wxDefaultSize


#----------------------------------------------------------------------
# wxGTK sets the locale when initialized.  Doing this at the Python
# level should set it up to match what GTK is doing at the C level.
try:
    import locale
    locale.setlocale(locale.LC_ALL, "")
except:
    pass



#----------------------------------------------------------------------
# wxWindows version numbers.  wxPython version is in __version__.

wxMAJOR_VERSION   = wxc.wxMAJOR_VERSION
wxMINOR_VERSION   = wxc.wxMINOR_VERSION
wxRELEASE_NUMBER  = wxc.wxRELEASE_NUMBER
wxVERSION_STRING  = wxc.wxVERSION_STRING
wxVERSION_NUMBER  = wxc.wxVERSION_NUMBER

wxVERSION = (wxMAJOR_VERSION, wxMINOR_VERSION, wxRELEASE_NUMBER)


#----------------------------------------------------------------------
# This helper function will take a wxPython object and convert it to
# another wxPython object type.  This will not be able to create objects
# that are derived from wxPython classes by the user, only those that are
# actually part of wxPython and directly corespond to C++ objects.
#
# This is useful in situations where some method returns a generic
# type such as wxWindow, but you know that it is actually some
# derived type such as a wxTextCtrl.  You can't call wxTextCtrl specific
# methods on a wxWindow object, but you can use this function to
# create a wxTextCtrl object that will pass the same pointer to
# the C++ code.  You use it like this:
#
#    textCtrl = wxPyTypeCast(window, "wxTextCtrl")
#
#
# WARNING:  Using this function to type cast objects into types that
#           they are not is not recommended and is likely to cause your
#           program to crash...  Hard.
#

def wxPyTypeCast(obj, typeStr):
    if obj is None:
        return None
    theClass = globals()[typeStr+"Ptr"]
    typeStr = __wxPyPtrTypeMap.get(typeStr, typeStr)
    if hasattr(obj, "this"):
        if obj.__class__ is theClass:   # if already the right type then just return it
            return obj
        newPtr = ptrcast(obj.this, typeStr+"_p")
    else:
        newPtr = ptrcast(obj, typeStr+"_p")
    theObj = theClass(newPtr)
    if hasattr(obj, "this"):
        theObj.thisown = obj.thisown
    return theObj


#----------------------------------------------------------------------
#----------------------------------------------------------------------

class wxPyOnDemandOutputWindow:
    def __init__(self, title = "wxPython: stdout/stderr"):
        self.frame  = None
        self.title  = title
        self.parent = None

    def SetParent(self, parent):
        self.parent = parent

    def OnCloseWindow(self, event):
        if self.frame != None:
            self.frame.Destroy()
        self.frame = None
        self.text  = None

    # These methods provide the file-like output behaviour.
    def write(self, str):
        if not wxThread_IsMain():
            # Aquire the GUI mutex before making GUI calls.  Mutex is released
            # when locker is deleted at the end of this function.
            locker = wxMutexGuiLocker()

        if not self.frame:
            self.frame = wxFrame(self.parent, -1, self.title,
                                 style=wxDEFAULT_FRAME_STYLE|wxNO_FULL_REPAINT_ON_RESIZE)
            self.text  = wxTextCtrl(self.frame, -1, "",
                                    style = wxTE_MULTILINE|wxTE_READONLY)
            self.frame.SetSize(wxSize(450, 300))
            self.frame.Show(true)
            EVT_CLOSE(self.frame, self.OnCloseWindow)
        self.text.AppendText(str)

    def close(self):
        if self.frame != None:
            if not wxThread_IsMain():
                locker = wxMutexGuiLocker()
            self.frame.Close()


_defRedirect = (wxPlatform == '__WXMSW__')

#----------------------------------------------------------------------
# The main application class.  Derive from this and implement an OnInit
# method that creates a frame and then calls self.SetTopWindow(frame)

class wxApp(wxPyApp):
    error = 'wxApp.error'
    outputWindowClass = wxPyOnDemandOutputWindow

    def __init__(self, redirect=_defRedirect, filename=None):
        wxPyApp.__init__(self)
        self.stdioWin = None
        self.saveStdio = (sys.stdout, sys.stderr)
        if redirect:
            self.RedirectStdio(filename)
        # this initializes wxWindows and then calls our OnInit
        _wxStart(self.OnInit)


    def __del__(self):
        try:
            self.RestoreStdio()
        except:
            pass


    def SetTopWindow(self, frame):
        if self.stdioWin:
            self.stdioWin.SetParent(frame)
        wxPyApp.SetTopWindow(self, frame)


    def MainLoop(self):
        wxPyApp.MainLoop(self)
        self.RestoreStdio()


    def RedirectStdio(self, filename):
        if filename:
            sys.stdout = sys.stderr = open(filename, 'a')
        else:
            self.stdioWin = self.outputWindowClass() # wxPyOnDemandOutputWindow
            sys.stdout = sys.stderr = self.stdioWin


    def RestoreStdio(self):
        sys.stdout, sys.stderr = self.saveStdio


#----------------------------------------------------------------------------

class wxPySimpleApp(wxApp):
    def __init__(self, flag=0):
        wxApp.__init__(self, flag)
    def OnInit(self):
        return true


class wxPyWidgetTester(wxApp):
    def __init__(self, size = (250, 100)):
        self.size = size
        wxApp.__init__(self, 0)

    def OnInit(self):
        self.frame = wxFrame(None, -1, "Widget Tester", pos=(0,0), size=self.size)
        self.SetTopWindow(self.frame)
        return true

    def SetWidget(self, widgetClass, *args):
        w = apply(widgetClass, (self.frame,) + args)
        self.frame.Show(true)

#----------------------------------------------------------------------------
# DO NOT hold any other references to this object.  This is how we know when
# to cleanup system resources that wxWin is holding.  When this module is
# unloaded, the refcount on __cleanMeUp goes to zero and it calls the
# wxApp_CleanUp function.

class __wxPyCleanup:
    def __init__(self):
        self.cleanup = wxc.wxApp_CleanUp
    def __del__(self):
        self.cleanup()

__cleanMeUp = __wxPyCleanup()

#----------------------------------------------------------------------------
#----------------------------------------------------------------------------
