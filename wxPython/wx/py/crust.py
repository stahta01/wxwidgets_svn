
"""Renamer stub: provides a way to drop the wx prefix from wxPython objects."""

__cvsid__ = "$Id$"
__revision__ = "$Revision$"[11:-2]

from wx import _rename
from wxPython.py import crust
_rename(globals(), crust.__dict__, modulename='py.crust')
del crust
del _rename
