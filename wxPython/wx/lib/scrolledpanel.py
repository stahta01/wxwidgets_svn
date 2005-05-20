
"""Renamer stub: provides a way to drop the wx prefix from wxPython objects."""

from wx import _rename
from wxPython.lib import scrolledpanel
_rename(globals(), scrolledpanel.__dict__, modulename='lib.scrolledpanel')
del scrolledpanel
del _rename
