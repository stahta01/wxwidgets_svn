"""PyFillingApp is a python namespace inspection application."""

__author__ = "Patrick K. O'Brien <pobrien@orbtech.com>"
__cvsid__ = "$Id$"
__revision__ = "$Revision$"[11:-2]

# We use this object to get more introspection when run standalone.
application = None

import filling

# These are imported just to have something interesting to inspect.
from PyCrust import crust
from PyCrust import interpreter
from PyCrust import introspect
from PyCrust import pseudo
from PyCrust import shell
import sys
from wxPython import wx


def main():
    """Create and run the application."""
    global application
    application = filling.App(0)
    root = application.fillingFrame.filling.fillingTree.root
    application.fillingFrame.filling.fillingTree.Expand(root)
    application.MainLoop()


if __name__ == '__main__':
    main()


