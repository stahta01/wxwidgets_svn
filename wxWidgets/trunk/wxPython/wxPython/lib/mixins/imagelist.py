#----------------------------------------------------------------------------
# Name:        wxPython.lib.mixins.listctrl
# Purpose:     Helpful mix-in classes for using a wxImageList
#
# Author:      Robin Dunn
#
# Created:     15-May-2001
# RCS-ID:      $Id$
# Copyright:   (c) 2001 by Total Control Software
# Licence:     wxWindows license
#----------------------------------------------------------------------------

from wxPython.wx import *

#----------------------------------------------------------------------------

class MagicImageList:
    '''
    Mix-in to provide "magic" growing image lists
    By Mike Fletcher
    '''

    ### LAZYTREE and LISTCONTROL Methods
    DEFAULTICONSIZE = 16

    def SetupIcons(self, images=(), size=None):
        self.__size = size or self.DEFAULTICONSIZE
        self.__magicImageList = wxImageList (self.__size,self.__size)
        self.__magicImageListMapping = {}
        self.SetImageList (
            self.__magicImageList, {
                16:wxIMAGE_LIST_SMALL,
                32:wxIMAGE_LIST_NORMAL,
            }[self.__size]
        )
        for image in images:
            self.AddIcon (image)

    def GetIcons (self, node):
        '''Get icon indexes for a given node, or None if no associated icon'''
        icon = self.GetIcon( node )
        if icon:
            index = self.AddIcon (icon)
            return index, index
        return None


    ### Local methods...
    def AddIcon(self, icon, mask = wxNullBitmap):
        '''Add an icon to the image list, or get the index if already there'''
        index = self.__magicImageListMapping.get (id (icon))
        if index is None:
            if isinstance( icon, wxIcon ):
                index = self.__magicImageList.AddIcon( icon )
            elif isinstance( icon, wxBitmap ):
                if isinstance( mask, wxColour ):
                    index = self.__magicImageList.AddWithColourMask( icon, mask )
                else:
                    index = self.__magicImageList.Add( icon, mask )
            else:
                raise ValueError("Unexpected icon object %s, "
                                 "expected wxIcon or wxBitmap" % (icon))
            self.__magicImageListMapping [id (icon)] = index
        return index

    ### Customisation point...
    def GetIcon( self, node ):
        '''Get the actual icon object for a node'''
        if hasattr (node,"DIAGRAMICON"):
            return node.DIAGRAMICON



