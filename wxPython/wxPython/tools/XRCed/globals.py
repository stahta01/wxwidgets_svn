# Name:         globals.py
# Purpose:      XRC editor, global variables
# Author:       Roman Rolinsky <rolinsky@mema.ucl.ac.be>
# Created:      02.12.2002
# RCS-ID:       $Id$

from wxPython.wx import *
from wxPython.xrc import *

# Global constants

sysFont = wxSystemSettings_GetFont(wxSYS_SYSTEM_FONT)
if wxPlatform == '__WXGTK__':
    labelFont = wxFont(sysFont.GetPointSize(), wxDEFAULT, wxNORMAL, wxBOLD)
    modernFont = wxFont(sysFont.GetPointSize(), wxMODERN, wxNORMAL, wxNORMAL)
    smallerFont = wxFont(sysFont.GetPointSize() - 2, wxDEFAULT, wxNORMAL, wxNORMAL)
else:
    labelFont = wxFont(10, wxDEFAULT, wxNORMAL, wxBOLD)
    modernFont = wxFont(10, wxMODERN, wxNORMAL, wxNORMAL)
    smallerFont = wxFont(8, wxDEFAULT, wxNORMAL, wxNORMAL)

progname = 'XRCed'
version = '0.1.0'

# Global variables

class Globals:
    panel = None
    tree = None
    frame = None
    tools = None
    undoMan = None
    testWin = None
    testWinPos = wxDefaultPosition
    currentXXX = None
    xmlFlags = wxXRC_USE_LOCALE | wxXRC_NO_SUBCLASSING

g = Globals()
