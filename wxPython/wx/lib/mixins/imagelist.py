
"""Renamer stub: provides a way to drop the wx prefix from wxPython objects."""

__cvsid__ = "$Id$"
__revision__ = "$Revision$"[11:-2]

from wx import _rename
from wxPython.lib.mixins import imagelist
_rename(globals(), imagelist.__dict__, modulename='lib.mixins.imagelist')
del imagelist
del _rename
