## This file reverse renames symbols in the wx package to give
## them their wx prefix again, for backwards compatibility.
##
## Generated by BuildRenamers in config.py

# This silly stuff here is so the wxPython.wx module doesn't conflict
# with the wx package.  We need to import modules from the wx package
# here, then we'll put the wxPython.wx entry back in sys.modules.
import sys
_wx = None
if sys.modules.has_key('wxPython.wx'):
    _wx = sys.modules['wxPython.wx']
    del sys.modules['wxPython.wx']

import wx.animate

sys.modules['wxPython.wx'] = _wx
del sys, _wx


# Now assign all the reverse-renamed names:
wxAnimationControlNameStr = wx.animate.AnimationControlNameStr
wxANIM_UNSPECIFIED = wx.animate.ANIM_UNSPECIFIED
wxANIM_DONOTREMOVE = wx.animate.ANIM_DONOTREMOVE
wxANIM_TOBACKGROUND = wx.animate.ANIM_TOBACKGROUND
wxANIM_TOPREVIOUS = wx.animate.ANIM_TOPREVIOUS
wxAnimationPlayer = wx.animate.AnimationPlayer
wxAnimationPlayerPtr = wx.animate.AnimationPlayerPtr
wxAnimationBase = wx.animate.AnimationBase
wxAnimationBasePtr = wx.animate.AnimationBasePtr
wxGIFAnimation = wx.animate.GIFAnimation
wxGIFAnimationPtr = wx.animate.GIFAnimationPtr
wxAN_FIT_ANIMATION = wx.animate.AN_FIT_ANIMATION
wxGIFAnimationCtrl = wx.animate.GIFAnimationCtrl
wxGIFAnimationCtrlPtr = wx.animate.GIFAnimationCtrlPtr
wxPreGIFAnimationCtrl = wx.animate.PreGIFAnimationCtrl


