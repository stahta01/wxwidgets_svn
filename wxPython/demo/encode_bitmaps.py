#!/usr/bin/env python
#----------------------------------------------------------------------

"""
This is a way to save the startup time when running img2py on lots of
files...
"""

import sys, string
sys.path.insert(0, "../tools")

import img2py


command_lines = [
    "   -n Mondrian bmp_source/mondrian.ico images.py",

    "-a -n _10s_ bmp_source/10s.gif images.py",
    "-a -n _01c_ bmp_source/01c.gif images.py",
    "-a -n _12h_ bmp_source/12h.gif images.py",
    "-a -n _13d_ bmp_source/13d.gif images.py",
    "-a -n Background bmp_source/backgrnd.png images.py",
    "-a -n TestStar -m #FFFFFF bmp_source/teststar.png images.py",
    "-a -n TestStar2 bmp_source/teststar.png images.py",
    "-a -n TestMask bmp_source/testmask.bmp images.py",

    "-a -n Test2 bmp_source/test2.bmp images.py",
#    "-a -n Robin bmp_source/robin.jpg images.py",

    "-a -n Bulb1 bmp_source/lb1.bmp images.py",
    "-a -n Bulb2 bmp_source/lb2.bmp images.py",

    "-a -n Calendar bmp_source/Calend.bmp images.py",
    "-a -n DbDec bmp_source/DbDec.bmp images.py",
    "-a -n Dec bmp_source/Dec.bmp images.py",
    "-a -n Pt bmp_source/Pt.bmp images.py",
    "-a -n DbInc bmp_source/DbInc.bmp images.py",
    "-a -n Inc bmp_source/Inc.bmp images.py",

    "-a -n New   -m #C0C0C0 bmp_source/new.bmp images.py",
    "-a -n Open  -m #C0C0C0 bmp_source/open.bmp images.py",
    "-a -n Copy  -m #C0C0C0 bmp_source/copy.bmp images.py",
    "-a -n Paste -m #C0C0C0 bmp_source/paste.bmp images.py",
    "-a -n Tog1  -m #C0C0C0 bmp_source/tog1.bmp images.py",
    "-a -n Tog2  -m #C0C0C0 bmp_source/tog2.bmp images.py",

    "-a -n Smiles -m #FFFFFF bmp_source/smiles.bmp images.py",

    "-a -n GridBG bmp_source/GridBG.gif images.py",

    "-a -n SmallUpArrow  -m #0000FF bmp_source/sm_up.bmp images.py",
    "-a -n SmallDnArrow  -m #0000FF bmp_source/sm_down.bmp images.py",

    "-a -n Folder1 bmp_source/folder1.ico images.py",
    "-a -n Folder2 bmp_source/folder2.ico images.py",
    "-a -n File1   bmp_source/file1.ico   images.py",

    ]


for line in command_lines:
    args = string.split(line)
    img2py.main(args)

