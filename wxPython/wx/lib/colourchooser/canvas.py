
"""Renamer stub: provides a way to drop the wx prefix from wxPython objects."""

from wx import _rename
from wxPython.lib.colourchooser import canvas
_rename(globals(), canvas.__dict__, modulename='lib.colourchooser.canvas')
del canvas
del _rename
