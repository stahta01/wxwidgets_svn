#!/usr/bin/env python
#----------------------------------------------------------------------------
# Name:         buildall.py
# Purpose:	Invokes the build script for all modules defined in
#               MODULELIST
#
# Author:       Robin Dunn
#
# Created:      18-Aug-1999
# RCS-ID:       $Id$
# Copyright:    (c) 1999 by Total Control Software
# Licence:      wxWindows license
#----------------------------------------------------------------------------

import sys, os
sys.path.insert(0, '../distrib')
import build

MODULELIST = ['html', 'glcanvas', 'utils', 'lseditor']



for module in MODULELIST:
    cwd = os.getcwd()
    print "**** Building %s" % module
    build.main([sys.argv[0], '-C', module] + sys.argv[1:])
    os.chdir(cwd)
