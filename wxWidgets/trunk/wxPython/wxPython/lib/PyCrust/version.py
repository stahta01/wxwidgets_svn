"""Provides an object representing the current "version" or "release" of
PyCrust as a whole. Individual classes, such as the shell, filling and
interpreter, each have a revision property based on the CVS Revision."""

__author__ = "Patrick K. O'Brien <pobrien@orbtech.com>"
__cvsid__ = "$Id$"
__revision__ = "$Revision$"[11:-2]

VERSION = '0.7.2'
 
