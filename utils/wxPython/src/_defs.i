/////////////////////////////////////////////////////////////////////////////
// Name:        _defs.i
// Purpose:     Definitions and stuff
//
// Author:      Robin Dunn
//
// Created:     6/24/97
// RCS-ID:      $Id$
// Copyright:   (c) 1998 by Total Control Software
// Licence:     wxWindows license
/////////////////////////////////////////////////////////////////////////////



//---------------------------------------------------------------------------
// Forward declares...

class wxPyApp;
class wxEvtHandler;
class wxWindow;
class wxFrame;
class wxMiniFrame;
class wxPanel;
class wxDialog;
class wxMenu;
class wxPyMenu;
class wxMenuBar;
class wxMenuItem;
class wxEvent;
class wxSizeEvent;
class wxCloseEvent;
class wxCommandEvent;
class wxScrollEvent;
class wxMouseEvent;
class wxKeyEvent;
class wxMoveEvent;
class wxPaintEvent;
class wxEraseEvent;
class wxFocusEvent;
class wxActivateEvent;
class wxInitDialogEvent;
class wxMenuEvent;
class wxShowEvent;
class wxIconizeEvent;
class wxMaximizeEvent;
class wxJoystickEvent;
class wxDropFilesEvent;
class wxIdleEvent;
class wxUpdateUIEvent;
class wxSysColourChangedEvent;
class wxSize;
class wxRealPoint;
class wxPoint;
class wxRect;
class wxBitmap;
class wxMask;
class wxIcon;
class wxCursor;
class wxFont;
class wxColour;
class wxPen;
class wxBrush;
class wxDC;
class wxMemoryDC;
class wxScreenDC;
class wxClientDC;
class wxPaintDC;
class wxPostScriptDC;
class wxPrinterDC;
class wxMetaFileDC;
class wxMDIParentFrame;
class wxMDIChildFrame;
class wxMDIClientWindow;
class wxControl;
class wxButton;
class wxBitmapButton;
class wxCheckBox;
class wxChoice;
class wxComboBox;
class wxGauge;
class wxStaticBox;
class wxStaticText;
class wxListBox;
class wxTextCtrl;
class wxScrollBar;
class wxSpinButton;
class wxStaticBitmap;
class wxRadioBox;
class wxRadioButton;
class wxSlider;

class wxPyTimer;
class wxIndividualLayoutConstraint;
class wxLayoutConstraints;
class wxToolBar;
class wxStatusBar;

//---------------------------------------------------------------------------

// some definitions for SWIG only
typedef unsigned char   byte;
typedef short int       WXTYPE;
typedef int             wxWindowID;
typedef unsigned int    uint;
typedef signed   int    EBool;


//---------------------------------------------------------------------------

// General numeric #define's and etc.  Making them all enums makes SWIG use the
// real macro when making the Python Int

enum {
    wxMAJOR_VERSION,
    wxMINOR_VERSION,
    wxRELEASE_NUMBER,

    NOT_FOUND,

    wxVSCROLL,
    wxHSCROLL,
    wxCAPTION,
    wxDOUBLE_BORDER,
    wxSUNKEN_BORDER,
    wxRAISED_BORDER,
    wxBORDER,
    wxSIMPLE_BORDER,
    wxSTATIC_BORDER,
    wxTRANSPARENT_WINDOW,
    wxNO_BORDER,
    wxUSER_COLOURS,
    wxNO_3D,
//wxOVERRIDE_KEY_TRANSLATIONS,
    wxTAB_TRAVERSAL,
    wxHORIZONTAL,
    wxVERTICAL,
    wxBOTH,
    wxCENTER_FRAME,
    wxSTAY_ON_TOP,
    wxICONIZE,
    wxMINIMIZE,
    wxMAXIMIZE,
    wxTHICK_FRAME,
    wxSYSTEM_MENU,
    wxMINIMIZE_BOX,
    wxMAXIMIZE_BOX,
    wxTINY_CAPTION_HORIZ,
    wxTINY_CAPTION_VERT,
    wxRESIZE_BOX,
    wxRESIZE_BORDER,
    wxDIALOG_MODAL,
    wxDIALOG_MODELESS,
    wxDEFAULT_FRAME_STYLE,
    wxDEFAULT_DIALOG_STYLE,
    wxRETAINED,
    wxBACKINGSTORE,
    wxTB_3DBUTTONS,
    wxTB_HORIZONTAL,
    wxTB_VERTICAL,
    wxTB_FLAT,
    wxCOLOURED,
    wxFIXED_LENGTH,
    wxALIGN_LEFT,
    wxALIGN_CENTER,
    wxALIGN_CENTRE,
    wxALIGN_RIGHT,
    wxLB_NEEDED_SB,
    wxLB_ALWAYS_SB,
    wxLB_SORT,
    wxLB_SINGLE,
    wxLB_MULTIPLE,
    wxLB_EXTENDED,
    wxLB_OWNERDRAW,
    wxLB_HSCROLL,
    wxPROCESS_ENTER,
    wxPASSWORD,
    wxTE_PROCESS_ENTER,
    wxTE_PASSWORD,
    wxTE_READONLY,
    wxTE_MULTILINE,
    wxCB_SIMPLE,
    wxCB_DROPDOWN,
    wxCB_SORT,
    wxCB_READONLY,
    wxRA_HORIZONTAL,
    wxRA_VERTICAL,
    wxRB_GROUP,
    wxGA_PROGRESSBAR,
    wxGA_HORIZONTAL,
    wxGA_VERTICAL,
    wxSL_HORIZONTAL,
    wxSL_VERTICAL,
    wxSL_AUTOTICKS,
    wxSL_LABELS,
    wxSL_LEFT,
    wxSL_TOP,
    wxSL_RIGHT,
    wxSL_BOTTOM,
    wxSL_BOTH,
    wxSL_SELRANGE,
    wxSB_HORIZONTAL,
    wxSB_VERTICAL,
    wxBU_AUTODRAW,
    wxBU_NOAUTODRAW,
    wxTR_HAS_BUTTONS,
    wxTR_EDIT_LABELS,
    wxLC_ICON,
    wxLC_SMALL_ICON,
    wxLC_LIST,
    wxLC_REPORT,
    wxLC_ALIGN_TOP,
    wxLC_ALIGN_LEFT,
    wxLC_AUTOARRANGE,
    wxLC_USER_TEXT,
    wxLC_EDIT_LABELS,
    wxLC_NO_HEADER,
    wxLC_NO_SORT_HEADER,
    wxLC_SINGLE_SEL,
    wxLC_SORT_ASCENDING,
    wxLC_SORT_DESCENDING,
    wxLC_MASK_TYPE,
    wxLC_MASK_ALIGN,
    wxLC_MASK_SORT,
    wxSP_VERTICAL,
    wxSP_HORIZONTAL,
    wxSP_ARROW_KEYS,
    wxSP_WRAP,
    wxSP_NOBORDER,
    wxSP_3D,
    wxSP_BORDER,
    wxTAB_MULTILINE,
    wxTAB_RIGHTJUSTIFY,
    wxTAB_FIXEDWIDTH,
    wxTAB_OWNERDRAW,
//    wxSB_SIZEGRIP,
    wxFLOOD_SURFACE,
    wxFLOOD_BORDER,
    wxODDEVEN_RULE,
    wxWINDING_RULE,
    wxTOOL_TOP,
    wxTOOL_BOTTOM,
    wxTOOL_LEFT,
    wxTOOL_RIGHT,
    wxOK,
    wxYES_NO,
    wxCANCEL,
    wxYES,
    wxNO,
    wxICON_EXCLAMATION,
    wxICON_HAND,
    wxICON_QUESTION,
    wxICON_INFORMATION,
    wxICON_STOP,
    wxICON_ASTERISK,
    wxICON_MASK,
    wxCENTRE,
    wxCENTER,
    wxSIZE_AUTO_WIDTH,
    wxSIZE_AUTO_HEIGHT,
    wxSIZE_AUTO,
    wxSIZE_USE_EXISTING,
    wxSIZE_ALLOW_MINUS_ONE,
    wxDF_TEXT,
    wxDF_BITMAP,
    wxDF_METAFILE,
    wxDF_DIB,
    wxDF_OEMTEXT,
    wxDF_FILENAME,
    wxPORTRAIT,
    wxLANDSCAPE,
    wxID_OPEN,
    wxID_CLOSE,
    wxID_NEW,
    wxID_SAVE,
    wxID_SAVEAS,
    wxID_REVERT,
    wxID_EXIT,
    wxID_UNDO,
    wxID_REDO,
    wxID_HELP,
    wxID_PRINT,
    wxID_PRINT_SETUP,
    wxID_PREVIEW,
    wxID_ABOUT,
    wxID_HELP_CONTENTS,
    wxID_HELP_COMMANDS,
    wxID_HELP_PROCEDURES,
    wxID_HELP_CONTEXT,
    wxID_CUT,
    wxID_COPY,
    wxID_PASTE,
    wxID_CLEAR,
    wxID_FIND,
    wxID_FILE1,
    wxID_FILE2,
    wxID_FILE3,
    wxID_FILE4,
    wxID_FILE5,
    wxID_FILE6,
    wxID_FILE7,
    wxID_FILE8,
    wxID_FILE9,
    wxID_OK,
    wxID_CANCEL,
    wxID_APPLY,
    wxID_YES,
    wxID_NO,
    wxBITMAP_TYPE_BMP,
    wxBITMAP_TYPE_BMP_RESOURCE,
    wxBITMAP_TYPE_ICO,
    wxBITMAP_TYPE_ICO_RESOURCE,
    wxBITMAP_TYPE_CUR,
    wxBITMAP_TYPE_CUR_RESOURCE,
    wxBITMAP_TYPE_XBM,
    wxBITMAP_TYPE_XBM_DATA,
    wxBITMAP_TYPE_XPM,
    wxBITMAP_TYPE_XPM_DATA,
    wxBITMAP_TYPE_TIF,
    wxBITMAP_TYPE_TIF_RESOURCE,
    wxBITMAP_TYPE_GIF,
    wxBITMAP_TYPE_GIF_RESOURCE,
    wxBITMAP_TYPE_PNG,
    wxBITMAP_TYPE_PNG_RESOURCE,
    wxBITMAP_TYPE_ANY,
    wxBITMAP_TYPE_RESOURCE,

    wxOPEN,
    wxSAVE,
    wxHIDE_READONLY,
    wxOVERWRITE_PROMPT,

#ifdef __WXMSW__
    wxACCEL_ALT,
    wxACCEL_CTRL,
    wxACCEL_SHIFT,
#endif

};


/// Standard error codes
enum  ErrCode
{
  ERR_PARAM = (-4000),
  ERR_NODATA,
  ERR_CANCEL,
  ERR_SUCCESS = 0
};


enum {
  wxDEFAULT ,
  wxDECORATIVE,
  wxROMAN,
  wxSCRIPT,
  wxSWISS,
  wxMODERN,
  wxTELETYPE,
  wxVARIABLE,
  wxFIXED,
  wxNORMAL,
  wxLIGHT,
  wxBOLD,
  wxITALIC,
  wxSLANT,
  wxSOLID,
  wxDOT,
  wxLONG_DASH,
  wxSHORT_DASH,
  wxDOT_DASH,
  wxUSER_DASH,
  wxTRANSPARENT,
  wxSTIPPLE,
  wxBDIAGONAL_HATCH,
  wxCROSSDIAG_HATCH,
  wxFDIAGONAL_HATCH,
  wxCROSS_HATCH,
  wxHORIZONTAL_HATCH,
  wxVERTICAL_HATCH,
  wxJOIN_BEVEL,
  wxJOIN_MITER,
  wxJOIN_ROUND,
  wxCAP_ROUND,
  wxCAP_PROJECTING,
  wxCAP_BUTT
};

typedef enum {
  wxCLEAR,      // 0
  wxXOR,        // src XOR dst
  wxINVERT,     // NOT dst
  wxOR_REVERSE, // src OR (NOT dst)
  wxAND_REVERSE,// src AND (NOT dst)
  wxCOPY,       // src
  wxAND,        // src AND dst
  wxAND_INVERT, // (NOT src) AND dst
  wxNO_OP,      // dst
  wxNOR,        // (NOT src) AND (NOT dst)
  wxEQUIV,      // (NOT src) XOR dst
  wxSRC_INVERT, // (NOT src)
  wxOR_INVERT,  // (NOT src) OR dst
  wxNAND,       // (NOT src) OR (NOT dst)
  wxOR,         // src OR dst
  wxSET,        // 1
  wxSRC_OR,     // source _bitmap_ OR destination
  wxSRC_AND     // source _bitmap_ AND destination
} form_ops_t;

enum _Virtual_keycodes {
 WXK_BACK    =   8,
 WXK_TAB     =   9,
 WXK_RETURN  =	13,
 WXK_ESCAPE  =	27,
 WXK_SPACE   =	32,
 WXK_DELETE  = 127,

 WXK_START   = 300,
 WXK_LBUTTON,
 WXK_RBUTTON,
 WXK_CANCEL,
 WXK_MBUTTON,
 WXK_CLEAR,
 WXK_SHIFT,
 WXK_CONTROL,
 WXK_MENU,
 WXK_PAUSE,
 WXK_CAPITAL,
 WXK_PRIOR,  // Page up
 WXK_NEXT,   // Page down
 WXK_END,
 WXK_HOME,
 WXK_LEFT,
 WXK_UP,
 WXK_RIGHT,
 WXK_DOWN,
 WXK_SELECT,
 WXK_PRINT,
 WXK_EXECUTE,
 WXK_SNAPSHOT,
 WXK_INSERT,
 WXK_HELP,
 WXK_NUMPAD0,
 WXK_NUMPAD1,
 WXK_NUMPAD2,
 WXK_NUMPAD3,
 WXK_NUMPAD4,
 WXK_NUMPAD5,
 WXK_NUMPAD6,
 WXK_NUMPAD7,
 WXK_NUMPAD8,
 WXK_NUMPAD9,
 WXK_MULTIPLY,
 WXK_ADD,
 WXK_SEPARATOR,
 WXK_SUBTRACT,
 WXK_DECIMAL,
 WXK_DIVIDE,
 WXK_F1,
 WXK_F2,
 WXK_F3,
 WXK_F4,
 WXK_F5,
 WXK_F6,
 WXK_F7,
 WXK_F8,
 WXK_F9,
 WXK_F10,
 WXK_F11,
 WXK_F12,
 WXK_F13,
 WXK_F14,
 WXK_F15,
 WXK_F16,
 WXK_F17,
 WXK_F18,
 WXK_F19,
 WXK_F20,
 WXK_F21,
 WXK_F22,
 WXK_F23,
 WXK_F24,
 WXK_NUMLOCK,
 WXK_SCROLL,
 WXK_PAGEUP,
 WXK_PAGEDOWN
};

typedef enum {
 wxCURSOR_NONE = 0,
 wxCURSOR_ARROW =  1,
 wxCURSOR_BULLSEYE,
 wxCURSOR_CHAR,
 wxCURSOR_CROSS,
 wxCURSOR_HAND,
 wxCURSOR_IBEAM,
 wxCURSOR_LEFT_BUTTON,
 wxCURSOR_MAGNIFIER,
 wxCURSOR_MIDDLE_BUTTON,
 wxCURSOR_NO_ENTRY,
 wxCURSOR_PAINT_BRUSH,
 wxCURSOR_PENCIL,
 wxCURSOR_POINT_LEFT,
 wxCURSOR_POINT_RIGHT,
 wxCURSOR_QUESTION_ARROW,
 wxCURSOR_RIGHT_BUTTON,
 wxCURSOR_SIZENESW,
 wxCURSOR_SIZENS,
 wxCURSOR_SIZENWSE,
 wxCURSOR_SIZEWE,
 wxCURSOR_SIZING,
 wxCURSOR_SPRAYCAN,
 wxCURSOR_WAIT,
 wxCURSOR_WATCH,
 wxCURSOR_BLANK
//  #ifndef __WXMSW__
//    /* Not yet implemented for Windows */
//    , wxCURSOR_CROSS_REVERSE,
//    wxCURSOR_DOUBLE_ARROW,
//    wxCURSOR_BASED_ARROW_UP,
//    wxCURSOR_BASED_ARROW_DOWN
//  #endif
} _standard_cursors_t;


#define FALSE 0
#define false 0
#define TRUE 1
#define true 1


//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

/*
 * Event types
 *
 */
enum wxEventType {
 wxEVT_NULL = 0,
 wxEVT_FIRST = 10000,

 // New names
 wxEVT_COMMAND_BUTTON_CLICKED,
 wxEVT_COMMAND_CHECKBOX_CLICKED,
 wxEVT_COMMAND_CHOICE_SELECTED,
 wxEVT_COMMAND_LISTBOX_SELECTED,
 wxEVT_COMMAND_LISTBOX_DOUBLECLICKED,
 wxEVT_COMMAND_CHECKLISTBOX_TOGGLED,
 wxEVT_COMMAND_TEXT_UPDATED,
 wxEVT_COMMAND_TEXT_ENTER,
 wxEVT_COMMAND_MENU_SELECTED,
 wxEVT_COMMAND_SLIDER_UPDATED,
 wxEVT_COMMAND_RADIOBOX_SELECTED,
 wxEVT_COMMAND_RADIOBUTTON_SELECTED,
// wxEVT_COMMAND_SCROLLBAR_UPDATED is now obsolete since we use wxEVT_SCROLL... events
 wxEVT_COMMAND_SCROLLBAR_UPDATED,
 wxEVT_COMMAND_VLBOX_SELECTED,
 wxEVT_COMMAND_COMBOBOX_SELECTED,
 wxEVT_COMMAND_TOOL_CLICKED,
 wxEVT_COMMAND_TOOL_RCLICKED,
 wxEVT_COMMAND_TOOL_ENTER,
 wxEVT_SET_FOCUS,
 wxEVT_KILL_FOCUS,

/* Mouse event types */
 wxEVT_LEFT_DOWN,
 wxEVT_LEFT_UP,
 wxEVT_MIDDLE_DOWN,
 wxEVT_MIDDLE_UP,
 wxEVT_RIGHT_DOWN,
 wxEVT_RIGHT_UP,
 wxEVT_MOTION,
 wxEVT_ENTER_WINDOW,
 wxEVT_LEAVE_WINDOW,
 wxEVT_LEFT_DCLICK,
 wxEVT_MIDDLE_DCLICK,
 wxEVT_RIGHT_DCLICK,

 // Non-client mouse events
 wxEVT_NC_LEFT_DOWN = wxEVT_FIRST + 100,
 wxEVT_NC_LEFT_UP,
 wxEVT_NC_MIDDLE_DOWN,
 wxEVT_NC_MIDDLE_UP,
 wxEVT_NC_RIGHT_DOWN,
 wxEVT_NC_RIGHT_UP,
 wxEVT_NC_MOTION,
 wxEVT_NC_ENTER_WINDOW,
 wxEVT_NC_LEAVE_WINDOW,
 wxEVT_NC_LEFT_DCLICK,
 wxEVT_NC_MIDDLE_DCLICK,
 wxEVT_NC_RIGHT_DCLICK,

/* Character input event type  */
 wxEVT_CHAR,

 /*
  * Scrollbar event identifiers
  */
 wxEVT_SCROLL_TOP,
 wxEVT_SCROLL_BOTTOM,
 wxEVT_SCROLL_LINEUP,
 wxEVT_SCROLL_LINEDOWN,
 wxEVT_SCROLL_PAGEUP,
 wxEVT_SCROLL_PAGEDOWN,
 wxEVT_SCROLL_THUMBTRACK,

 wxEVT_SIZE = wxEVT_FIRST + 200,
 wxEVT_MOVE,
 wxEVT_CLOSE_WINDOW,
 wxEVT_END_SESSION,
 wxEVT_QUERY_END_SESSION,
 wxEVT_ACTIVATE_APP,
 wxEVT_POWER,
 wxEVT_CHAR_HOOK,
 wxEVT_KEY_UP,
 wxEVT_ACTIVATE,
 wxEVT_CREATE,
 wxEVT_DESTROY,
 wxEVT_SHOW,
 wxEVT_ICONIZE,
 wxEVT_MAXIMIZE,
 wxEVT_MOUSE_CAPTURE_CHANGED,
 wxEVT_PAINT,
 wxEVT_ERASE_BACKGROUND,
 wxEVT_NC_PAINT,
 wxEVT_PAINT_ICON,
 wxEVT_MENU_CHAR,
 wxEVT_MENU_INIT,
 wxEVT_MENU_HIGHLIGHT,
 wxEVT_POPUP_MENU_INIT,
 wxEVT_CONTEXT_MENU,
 wxEVT_SYS_COLOUR_CHANGED,
 wxEVT_SETTING_CHANGED,
 wxEVT_QUERY_NEW_PALETTE,
 wxEVT_PALETTE_CHANGED,
 wxEVT_JOY_BUTTON_DOWN,
 wxEVT_JOY_BUTTON_UP,
 wxEVT_JOY_MOVE,
 wxEVT_JOY_ZMOVE,
 wxEVT_DROP_FILES,
 wxEVT_DRAW_ITEM,
 wxEVT_MEASURE_ITEM,
 wxEVT_COMPARE_ITEM,
 wxEVT_INIT_DIALOG,
 wxEVT_IDLE,
 wxEVT_UPDATE_UI,

 /* Generic command events */
 // Note: a click is a higher-level event
 // than button down/up
 wxEVT_COMMAND_LEFT_CLICK,
 wxEVT_COMMAND_LEFT_DCLICK,
 wxEVT_COMMAND_RIGHT_CLICK,
 wxEVT_COMMAND_RIGHT_DCLICK,
 wxEVT_COMMAND_SET_FOCUS,
 wxEVT_COMMAND_KILL_FOCUS,
 wxEVT_COMMAND_ENTER,

 /* Tree control event types */
 wxEVT_COMMAND_TREE_BEGIN_DRAG,
 wxEVT_COMMAND_TREE_BEGIN_RDRAG,
 wxEVT_COMMAND_TREE_BEGIN_LABEL_EDIT,
 wxEVT_COMMAND_TREE_END_LABEL_EDIT,
 wxEVT_COMMAND_TREE_DELETE_ITEM,
 wxEVT_COMMAND_TREE_GET_INFO,
 wxEVT_COMMAND_TREE_SET_INFO,
 wxEVT_COMMAND_TREE_ITEM_EXPANDED,
 wxEVT_COMMAND_TREE_ITEM_EXPANDING,
 wxEVT_COMMAND_TREE_SEL_CHANGED,
 wxEVT_COMMAND_TREE_SEL_CHANGING,
 wxEVT_COMMAND_TREE_KEY_DOWN,

 /* List control event types */
 wxEVT_COMMAND_LIST_BEGIN_DRAG,
 wxEVT_COMMAND_LIST_BEGIN_RDRAG,
 wxEVT_COMMAND_LIST_BEGIN_LABEL_EDIT,
 wxEVT_COMMAND_LIST_END_LABEL_EDIT,
 wxEVT_COMMAND_LIST_DELETE_ITEM,
 wxEVT_COMMAND_LIST_DELETE_ALL_ITEMS,
 wxEVT_COMMAND_LIST_GET_INFO,
 wxEVT_COMMAND_LIST_SET_INFO,
 wxEVT_COMMAND_LIST_ITEM_SELECTED,
 wxEVT_COMMAND_LIST_ITEM_DESELECTED,
 wxEVT_COMMAND_LIST_KEY_DOWN,
 wxEVT_COMMAND_LIST_INSERT_ITEM,
 wxEVT_COMMAND_LIST_COL_CLICK,

 /* Tab and notebook control event types */
 wxEVT_COMMAND_TAB_SEL_CHANGED,
 wxEVT_COMMAND_TAB_SEL_CHANGING,
 wxEVT_COMMAND_NOTEBOOK_PAGE_CHANGED,
 wxEVT_COMMAND_NOTEBOOK_PAGE_CHANGING
};




/////////////////////////////////////////////////////////////////////////////
//
// $Log$
// Revision 1.5  1998/10/02 06:40:32  RD
// Version 0.4 of wxPython for MSW.
//
// Revision 1.4  1998/08/18 19:48:11  RD
// more wxGTK compatibility things.
//
// It builds now but there are serious runtime problems...
//
// Revision 1.3  1998/08/14 23:36:33  RD
// Beginings of wxGTK compatibility
//
// Revision 1.2  1998/08/14 03:16:35  RD
// removed some definitions that got removed from defs.h
//
// Revision 1.1  1998/08/09 08:25:49  RD
// Initial version
//
//
