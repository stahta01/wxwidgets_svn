
"""Renamer stub: provides a way to drop the wx prefix from wxPython objects."""

from wx import _rename
from wxPython.lib import dialogs
_rename(globals(), dialogs.__dict__, modulename='lib.dialogs')
del dialogs
del _rename
