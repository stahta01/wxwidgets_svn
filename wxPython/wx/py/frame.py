
"""Renamer stub: provides a way to drop the wx prefix from wxPython objects."""

from wx import _rename
from wxPython.py import frame
_rename(globals(), frame.__dict__, modulename='py.frame')
del frame
del _rename
