
from wxPython.wx import *
from Main import opj

#----------------------------------------------------------------------

def runTest(frame, nb, log):
    bmp = wxImage(opj('bitmaps/image.bmp'), wxBITMAP_TYPE_BMP).ConvertToBitmap()
    gif = wxImage(opj('bitmaps/image.gif'), wxBITMAP_TYPE_GIF).ConvertToBitmap()
    png = wxImage(opj('bitmaps/image.png'), wxBITMAP_TYPE_PNG).ConvertToBitmap()
    jpg = wxImage(opj('bitmaps/image.jpg'), wxBITMAP_TYPE_JPEG).ConvertToBitmap()

    panel = wxPanel(nb, -1)
    pos = 10
    wxStaticBitmap(panel, -1, bmp, wxPoint(10, pos),
                   wxSize(bmp.GetWidth(), bmp.GetHeight()))

    pos = pos + bmp.GetHeight() + 10
    wxStaticBitmap(panel, -1, gif, wxPoint(10, pos),
                   wxSize(gif.GetWidth(), gif.GetHeight()))

    pos = pos + gif.GetHeight() + 10
    wxStaticBitmap(panel, -1, png, wxPoint(10, pos),
                   wxSize(png.GetWidth(), png.GetHeight()))

    pos = pos + png.GetHeight() + 10
    wxStaticBitmap(panel, -1, jpg, wxPoint(10, pos),
                   wxSize(jpg.GetWidth(), jpg.GetHeight()))

    return panel

#----------------------------------------------------------------------







overview = """\
"""
