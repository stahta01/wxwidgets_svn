#----------------------------------------------------------------------
# sizer test code
#----------------------------------------------------------------------

from wxPython.wx             import *
from wxPython.lib.grids      import wxGridSizer, wxFlexGridSizer

#----------------------------------------------------------------------

def makeSimpleBox1(win):
    box = wxBoxSizer(wxHORIZONTAL)
    box.Add(wxButton(win, 1010, "one"), 0, wxEXPAND)
    box.Add(wxButton(win, 1010, "two"), 0, wxEXPAND)
    box.Add(wxButton(win, 1010, "three"), 0, wxEXPAND)
    box.Add(wxButton(win, 1010, "four"), 0, wxEXPAND)

    return box

#----------------------------------------------------------------------

def makeSimpleBox2(win):
    box = wxBoxSizer(wxVERTICAL)
    box.Add(wxButton(win, 1010, "one"), 0, wxEXPAND)
    box.Add(wxButton(win, 1010, "two"), 0, wxEXPAND)
    box.Add(wxButton(win, 1010, "three"), 0, wxEXPAND)
    box.Add(wxButton(win, 1010, "four"), 0, wxEXPAND)

    return box

#----------------------------------------------------------------------

def makeSimpleBox3(win):
    box = wxBoxSizer(wxHORIZONTAL)
    box.Add(wxButton(win, 1010, "one"), 0, wxEXPAND)
    box.Add(wxButton(win, 1010, "two"), 0, wxEXPAND)
    box.Add(wxButton(win, 1010, "three"), 0, wxEXPAND)
    box.Add(wxButton(win, 1010, "four"), 0, wxEXPAND)
    box.Add(wxButton(win, 1010, "five"), 1, wxEXPAND)

    return box

#----------------------------------------------------------------------

def makeSimpleBox4(win):
    box = wxBoxSizer(wxHORIZONTAL)
    box.Add(wxButton(win, 1010, "one"), 0, wxEXPAND)
    box.Add(wxButton(win, 1010, "two"), 0, wxEXPAND)
    box.Add(wxButton(win, 1010, "three"), 1, wxEXPAND)
    box.Add(wxButton(win, 1010, "four"), 1, wxEXPAND)
    box.Add(wxButton(win, 1010, "five"), 1, wxEXPAND)

    return box

#----------------------------------------------------------------------

def makeSimpleBox5(win):
    box = wxBoxSizer(wxHORIZONTAL)
    box.Add(wxButton(win, 1010, "one"), 0, wxEXPAND)
    box.Add(wxButton(win, 1010, "two"), 0, wxEXPAND)
    box.Add(wxButton(win, 1010, "three"), 3, wxEXPAND)
    box.Add(wxButton(win, 1010, "four"), 1, wxEXPAND)
    box.Add(wxButton(win, 1010, "five"), 1, wxEXPAND)

    return box

#----------------------------------------------------------------------

def makeSimpleBox6(win):
    box = wxBoxSizer(wxHORIZONTAL)
    box.Add(wxButton(win, 1010, "one"), 1, wxALIGN_TOP)
    box.Add(wxButton(win, 1010, "two"), 1, wxEXPAND)
    box.Add(wxButton(win, 1010, "three"), 1, wxALIGN_CENTER)
    box.Add(wxButton(win, 1010, "four"), 1, wxEXPAND)
    box.Add(wxButton(win, 1010, "five"), 1, wxALIGN_BOTTOM)

    return box

#----------------------------------------------------------------------

def makeSimpleBox7(win):
    box = wxBoxSizer(wxHORIZONTAL)
    box.Add(wxButton(win, 1010, "one"), 0, wxEXPAND)
    box.Add(wxButton(win, 1010, "two"), 0, wxEXPAND)
    box.Add(wxButton(win, 1010, "three"), 0, wxEXPAND)
    box.Add(60, 20, 0, wxEXPAND)
    box.Add(wxButton(win, 1010, "five"), 1, wxEXPAND)

    return box

#----------------------------------------------------------------------

def makeSimpleBox8(win):
    box = wxBoxSizer(wxVERTICAL)
    box.Add(wxButton(win, 1010, "one"), 0, wxEXPAND)
    box.Add(0,0, 1)
    box.Add(wxButton(win, 1010, "two"), 0, wxALIGN_CENTER)
    box.Add(0,0, 1)
    box.Add(wxButton(win, 1010, "three"), 0, wxEXPAND)
    box.Add(wxButton(win, 1010, "four"), 0, wxEXPAND)
#    box.Add(wxButton(win, 1010, "five"), 1, wxEXPAND)

    return box

#----------------------------------------------------------------------
#----------------------------------------------------------------------

def makeSimpleBorder1(win):
    bdr = wxBoxSizer(wxHORIZONTAL)
    btn = wxButton(win, 1010, "border")
    btn.SetSize(wxSize(80, 80))
    bdr.Add(btn, 1, wxEXPAND|wxALL, 15)

    return bdr

#----------------------------------------------------------------------

def makeSimpleBorder2(win):
    bdr = wxBoxSizer(wxHORIZONTAL)
    btn = wxButton(win, 1010, "border")
    btn.SetSize(wxSize(80, 80))
    bdr.Add(btn, 1, wxEXPAND | wxEAST | wxWEST, 15)

    return bdr

#----------------------------------------------------------------------

def makeSimpleBorder3(win):
    bdr = wxBoxSizer(wxHORIZONTAL)
    btn = wxButton(win, 1010, "border")
    btn.SetSize(wxSize(80, 80))
    bdr.Add(btn, 1, wxEXPAND | wxNORTH | wxWEST, 15)

    return bdr

#----------------------------------------------------------------------
#----------------------------------------------------------------------

def makeBoxInBox(win):
    box = wxBoxSizer(wxVERTICAL)

    box.Add(wxButton(win, 1010, "one"), 0, wxEXPAND)

    box2 = wxBoxSizer(wxHORIZONTAL)
    box2.AddMany([ (wxButton(win, 1010, "two"), 0, wxEXPAND),
                   (wxButton(win, 1010, "three"), 0, wxEXPAND),
                   (wxButton(win, 1010, "four"), 0, wxEXPAND),
                   (wxButton(win, 1010, "five"), 0, wxEXPAND),
                   ])

    box3 = wxBoxSizer(wxVERTICAL)
    box3.AddMany([ (wxButton(win, 1010, "six"),   0, wxEXPAND),
                   (wxButton(win, 1010, "seven"), 2, wxEXPAND),
                   (wxButton(win, 1010, "eight"), 1, wxEXPAND),
                   (wxButton(win, 1010, "nine"),  1, wxEXPAND),
                   ])

    box2.Add(box3, 1, wxEXPAND)
    box.Add(box2, 1, wxEXPAND)

    box.Add(wxButton(win, 1010, "ten"), 0, wxEXPAND)

    return box

#----------------------------------------------------------------------

def makeBoxInBorder(win):
    bdr = wxBoxSizer(wxHORIZONTAL)
    box = makeSimpleBox3(win)
    bdr.Add(box, 1, wxEXPAND | wxALL, 15)

    return bdr

#----------------------------------------------------------------------

def makeBorderInBox(win):
    insideBox = wxBoxSizer(wxHORIZONTAL)

    box2 = wxBoxSizer(wxHORIZONTAL)
    box2.AddMany([ (wxButton(win, 1010, "one"), 0, wxEXPAND),
                   (wxButton(win, 1010, "two"), 0, wxEXPAND),
                   (wxButton(win, 1010, "three"), 0, wxEXPAND),
                   (wxButton(win, 1010, "four"), 0, wxEXPAND),
                   (wxButton(win, 1010, "five"), 0, wxEXPAND),
                 ])

    insideBox.Add(box2, 0, wxEXPAND)

    bdr = wxBoxSizer(wxHORIZONTAL)
    bdr.Add(wxButton(win, 1010, "border"), 1, wxEXPAND | wxALL)
    insideBox.Add(bdr, 1, wxEXPAND | wxALL, 20)

    box3 = wxBoxSizer(wxVERTICAL)
    box3.AddMany([ (wxButton(win, 1010, "six"),   0, wxEXPAND),
                   (wxButton(win, 1010, "seven"), 2, wxEXPAND),
                   (wxButton(win, 1010, "eight"), 1, wxEXPAND),
                   (wxButton(win, 1010, "nine"),  1, wxEXPAND),
                 ])
    insideBox.Add(box3, 1, wxEXPAND)

    outsideBox = wxBoxSizer(wxVERTICAL)
    outsideBox.Add(wxButton(win, 1010, "top"), 0, wxEXPAND)
    outsideBox.Add(insideBox, 1, wxEXPAND)
    outsideBox.Add(wxButton(win, 1010, "bottom"), 0, wxEXPAND)

    return outsideBox


#----------------------------------------------------------------------

def makeGrid1(win):
    gs = wxGridSizer(3, 3, 2, 2)  # rows, cols, hgap, vgap

    gs.AddMany([ (wxButton(win, 1010, 'one'),   0, wxEXPAND),
                 (wxButton(win, 1010, 'two'),   0, wxEXPAND),
                 (wxButton(win, 1010, 'three'), 0, wxEXPAND),
                 (wxButton(win, 1010, 'four'),  0, wxEXPAND),
                 (wxButton(win, 1010, 'five'),  0, wxEXPAND),
                 #(75, 50),
                 (wxButton(win, 1010, 'six'),   0, wxEXPAND),
                 (wxButton(win, 1010, 'seven'), 0, wxEXPAND),
                 (wxButton(win, 1010, 'eight'), 0, wxEXPAND),
                 (wxButton(win, 1010, 'nine'),  0, wxEXPAND),
                 ])

    return gs

#----------------------------------------------------------------------

def makeGrid2(win):
    gs = wxGridSizer(3, 3)  # rows, cols, hgap, vgap

    box = wxBoxSizer(wxVERTICAL)
    box.Add(wxButton(win, 1010, 'A'), 0, wxEXPAND)
    box.Add(wxButton(win, 1010, 'B'), 1, wxEXPAND)

    gs2 = wxGridSizer(2,2, 4, 4)
    gs2.AddMany([ (wxButton(win, 1010, 'C'), 0, wxEXPAND),
                  (wxButton(win, 1010, 'E'), 0, wxEXPAND),
                  (wxButton(win, 1010, 'F'), 0, wxEXPAND),
                  (wxButton(win, 1010, 'G'), 0, wxEXPAND)])

    gs.AddMany([ (wxButton(win, 1010, 'one'),   0, wxALIGN_RIGHT | wxALIGN_BOTTOM),
                 (wxButton(win, 1010, 'two'),   0, wxEXPAND),
                 (wxButton(win, 1010, 'three'), 0, wxALIGN_LEFT | wxALIGN_BOTTOM),
                 (wxButton(win, 1010, 'four'),  0, wxEXPAND),
                 (wxButton(win, 1010, 'five'),  0, wxALIGN_CENTER),
                 (wxButton(win, 1010, 'six'),   0, wxEXPAND),
                 (box,                          0, wxEXPAND | wxALL, 10),
                 (wxButton(win, 1010, 'eight'), 0, wxEXPAND),
                 (gs2,                          0, wxEXPAND | wxALL, 4),
                 ])

    return gs

#----------------------------------------------------------------------

def makeGrid3(win):
    gs = wxFlexGridSizer(3, 3, 2, 2)  # rows, cols, hgap, vgap

    gs.AddMany([ (wxButton(win, 1010, 'one'),   0, wxEXPAND),
                 (wxButton(win, 1010, 'two'),   0, wxEXPAND),
                 (wxButton(win, 1010, 'three'), 0, wxEXPAND),
                 (wxButton(win, 1010, 'four'),  0, wxEXPAND),
                 #(wxButton(win, 1010, 'five'),  0, wxEXPAND),
                 (175, 50),
                 (wxButton(win, 1010, 'six'),   0, wxEXPAND),
                 (wxButton(win, 1010, 'seven'), 0, wxEXPAND),
                 (wxButton(win, 1010, 'eight'), 0, wxEXPAND),
                 (wxButton(win, 1010, 'nine'),  0, wxEXPAND),
                 ])

    gs.AddGrowableRow(0)
    gs.AddGrowableRow(2)
    gs.AddGrowableCol(1)
    return gs

#----------------------------------------------------------------------

def makeGrid4(win):
    bpos = wxDefaultPosition
    bsize = wxSize(100, 50)
    gs = wxGridSizer(3, 3, 2, 2)  # rows, cols, hgap, vgap

    gs.AddMany([ (wxButton(win, 1010, 'one', bpos, bsize),
                  0, wxALIGN_TOP | wxALIGN_LEFT ),
                 (wxButton(win, 1010, 'two', bpos, bsize),
                  0, wxALIGN_TOP | wxALIGN_CENTER_HORIZONTAL ),
                 (wxButton(win, 1010, 'three', bpos, bsize),
                  0, wxALIGN_TOP | wxALIGN_RIGHT ),
                 (wxButton(win, 1010, 'four', bpos, bsize),
                  0, wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT ),
                 (wxButton(win, 1010, 'five', bpos, bsize),
                  0, wxALIGN_CENTER ),
                 (wxButton(win, 1010, 'six', bpos, bsize),
                  0, wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT ),
                 (wxButton(win, 1010, 'seven', bpos, bsize),
                  0, wxALIGN_BOTTOM | wxALIGN_LEFT ),
                 (wxButton(win, 1010, 'eight', bpos, bsize),
                  0, wxALIGN_BOTTOM | wxALIGN_CENTER_HORIZONTAL ),
                 (wxButton(win, 1010, 'nine', bpos, bsize),
                  0, wxALIGN_BOTTOM | wxALIGN_RIGHT ),
                 ])

    return gs

#----------------------------------------------------------------------

def makeShapes(win):
    bpos = wxDefaultPosition
    bsize = wxSize(100, 50)
    gs = wxGridSizer(3, 3, 2, 2)  # rows, cols, hgap, vgap

    gs.AddMany([ (wxButton(win, 1010, 'one', bpos, bsize),
                  0, wxSHAPED | wxALIGN_TOP | wxALIGN_LEFT ),
                 (wxButton(win, 1010, 'two', bpos, bsize),
                  0, wxSHAPED | wxALIGN_TOP | wxALIGN_CENTER_HORIZONTAL ),
                 (wxButton(win, 1010, 'three', bpos, bsize),
                  0, wxSHAPED | wxALIGN_TOP | wxALIGN_RIGHT ),
                 (wxButton(win, 1010, 'four', bpos, bsize),
                  0, wxSHAPED | wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT ),
                 (wxButton(win, 1010, 'five', bpos, bsize),
                  0, wxSHAPED | wxALIGN_CENTER ),
                 (wxButton(win, 1010, 'six', bpos, bsize),
                  0, wxSHAPED | wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT ),
                 (wxButton(win, 1010, 'seven', bpos, bsize),
                  0, wxSHAPED |  wxALIGN_BOTTOM | wxALIGN_LEFT ),
                 (wxButton(win, 1010, 'eight', bpos, bsize),
                  0, wxSHAPED | wxALIGN_BOTTOM | wxALIGN_CENTER_HORIZONTAL ),
                 (wxButton(win, 1010, 'nine', bpos, bsize),
                  0, wxSHAPED | wxALIGN_BOTTOM | wxALIGN_RIGHT ),
                 ])

    return gs

#----------------------------------------------------------------------

def makeSimpleBoxShaped(win):
    box = wxBoxSizer(wxHORIZONTAL)
    box.Add(wxButton(win, 1010, "one"), 0, wxEXPAND)
    box.Add(wxButton(win, 1010, "two"), 0, wxEXPAND)
    box.Add(wxButton(win, 1010, "three"), 0, wxEXPAND)
    box.Add(wxButton(win, 1010, "four"), 0, wxEXPAND)
    box.Add(wxButton(win, 1010, "five"), 1, wxSHAPED)

    return box

#----------------------------------------------------------------------

theTests = [
    ("Simple horizontal boxes", makeSimpleBox1,
     "This is a HORIZONTAL box sizer with four non-stretchable buttons held "
     "within it.  Notice that the buttons are added and aligned in the horizontal "
     "dimension.  Also notice that they are fixed size in the horizontal dimension, "
     "but will stretch vertically."
     ),

    ("Simple vertical boxes", makeSimpleBox2,
     "Exactly the same as the previous sample but using a VERTICAL box sizer "
     "instead of a HORIZONTAL one."
     ),

    ("Add a stretchable", makeSimpleBox3,
     "We've added one more button with the strechable flag turned on.  Notice "
     "how it grows to fill the extra space in the otherwise fixed dimension."
     ),

    ("More than one stretchable", makeSimpleBox4,
     "Here there are several items that are stretchable, they all divide up the "
     "extra space evenly."
     ),

    ("Weighting factor", makeSimpleBox5,
     "This one shows more than one strechable, but one of them has a weighting "
     "factor so it gets more of the free space."
     ),

    ("Edge Affinity", makeSimpleBox6,
     "For items that don't completly fill their allotted space, and don't "
     "stretch, you can specify which side (or the center) they should stay "
     "attached to."
     ),

    ("Spacer", makeSimpleBox7,
     "You can add empty space to be managed by a Sizer just as if it were a "
     "window or another Sizer."
     ),

    ("Centering in available space", makeSimpleBox8,
     "This one shows an item that does not expand to fill it's space, but rather"
     "stays centered within it."
     ),

#    ("Percent Sizer", makeSimpleBox6,
#     "You can use the wxBoxSizer like a Percent Sizer.  Just make sure that all "
#     "the weighting factors add up to 100!"
#     ),

    ("", None, ""),

    ("Simple border sizer", makeSimpleBorder1,
     "The wxBoxSizer can leave empty space around its contents.  This one "
     "gives a border all the way around."
     ),

    ("East and West border", makeSimpleBorder2,
     "You can pick and choose which sides have borders."
     ),

    ("North and West border", makeSimpleBorder3,
     "You can pick and choose which sides have borders."
     ),

    ("", None, ""),

    ("Boxes inside of boxes", makeBoxInBox,
     "This one shows nesting of boxes within boxes within boxes, using both "
     "orientations.  Notice also that button seven has a greater weighting "
     "factor than its siblings."
     ),

    ("Boxes inside a Border", makeBoxInBorder,
     "Sizers of different types can be nested withing each other as well. "
     "Here is a box sizer with several buttons embedded within a border sizer."
     ),

    ("Border in a Box", makeBorderInBox,
     "Another nesting example.  This one has Boxes and a Border inside another Box."
     ),

    ("", None, ""),

    ("Simple Grid", makeGrid1,
     "This is an example of the wxGridSizer.  In this case all row heights "
     "and column widths are kept the same as all the others and all items "
     "fill their available space.  The horzontal and vertical gaps are set to "
     "2 pixels each."
     ),

    ("More Grid Features", makeGrid2,
     "This is another example of the wxGridSizer.  This one has no gaps in the grid, "
     "but various cells are given different alignment options and some of them "
     "hold nested sizers."
     ),

    ("Flexible Grid", makeGrid3,
     "This grid allows the rows to have different heights and the columns to have "
     "different widths.  You can also specify rows and columns that are growable, "
     "which we have done for the first and last row and the middle column for "
     "this example.\n"
     "\nThere is also a spacer in the middle cell instead of an actual window."
     ),

    ("Grid with Alignment", makeGrid4,
     "New alignment flags allow for the positioning of items in any corner or centered "
     "position."
     ),

    ("", None, ""),

    ("Proportional resize", makeSimpleBoxShaped,
     "Managed items can preserve their original aspect ratio.  The last item has the "
     "wxSHAPED flag set and will resize proportional to its origingal size."
     ),

    ("Proportional resize with Alignments", makeShapes,
     "This one shows various alignments as well as proportional resizing for all items."
     ),

    ]
#----------------------------------------------------------------------

class TestFrame(wxFrame):
    def __init__(self, parent, title, sizerFunc):
        wxFrame.__init__(self, parent, -1, title)
        EVT_BUTTON(self, 1010, self.OnButton)

        self.sizer = sizerFunc(self)
        self.CreateStatusBar()
        self.SetStatusText("Resize this frame to see how the sizers respond...")
        self.sizer.Fit(self)

        self.SetAutoLayout(True)
        self.SetSizer(self.sizer)
        EVT_CLOSE(self, self.OnCloseWindow)

    def OnCloseWindow(self, event):
        self.MakeModal(False)
        self.Destroy()

    def OnButton(self, event):
        self.Close(True)

#----------------------------------------------------------------------



class TestSelectionPanel(wxPanel):
    def __init__(self, parent, frame):
        wxPanel.__init__(self, parent, -1)
        self.frame = frame

        self.list = wxListBox(self, 401,
                              wxDLG_PNT(self, 10, 10), wxDLG_SZE(self, 100, 100),
                              [])
        EVT_LISTBOX(self, 401, self.OnSelect)
        EVT_LISTBOX_DCLICK(self, 401, self.OnDClick)

        self.btn = wxButton(self, 402, "Try it!", wxDLG_PNT(self, 120, 10)).SetDefault()
        EVT_BUTTON(self, 402, self.OnDClick)

        self.text = wxTextCtrl(self, -1, "",
                               wxDLG_PNT(self, 10, 115),
                               wxDLG_SZE(self, 200, 50),
                               wxTE_MULTILINE | wxTE_READONLY)

        for item in theTests:
            self.list.Append(item[0])



    def OnSelect(self, event):
        pos = self.list.GetSelection()
        self.text.SetValue(theTests[pos][2])


    def OnDClick(self, event):
        pos = self.list.GetSelection()
        title = theTests[pos][0]
        func = theTests[pos][1]

        if func:
            win = TestFrame(self, title, func)
            win.CentreOnParent(wxBOTH)
            win.Show(True)
            win.MakeModal(True)

#----------------------------------------------------------------------

def runTest(frame, nb, log):
    win = TestSelectionPanel(nb, frame)
    return win

overview = ""
#wxSizer.__doc__        + '\n' + '-' * 80 + '\n' + \
#wxBoxSizer.__doc__     + '\n' + '-' * 80 + '\n' + \
#wxBorderSizer.__doc__

#----------------------------------------------------------------------



if __name__ == '__main__':

    class MainFrame(wxFrame):
        def __init__(self):
            wxFrame.__init__(self, None, -1, "Testing...")

            self.CreateStatusBar()
            mainmenu = wxMenuBar()
            menu = wxMenu()
            menu.Append(200, 'E&xit', 'Get the heck outta here!')
            mainmenu.Append(menu, "&File")
            self.SetMenuBar(mainmenu)
            EVT_MENU(self, 200, self.OnExit)
            self.panel = TestSelectionPanel(self, self)
            self.SetSize(wxSize(400, 380))
            EVT_CLOSE(self, self.OnCloseWindow)

        def OnCloseWindow(self, event):
            self.Destroy()

        def OnExit(self, event):
            self.Close(True)


    class TestApp(wxApp):
        def OnInit(self):
            frame = MainFrame()
            frame.Show(True)
            self.SetTopWindow(frame)
            return True

    app = TestApp(0)
    app.MainLoop()


#----------------------------------------------------------------------
