/////////////////////////////////////////////////////////////////////////////
// Name:        eventhandling
// Purpose:     topic overview
// Author:      wxWidgets team
// RCS-ID:      $Id$
// Licence:     wxWindows license
/////////////////////////////////////////////////////////////////////////////

/*!

 @page eventhandling_overview Event handling overview

 Classes: #wxEvtHandler, #wxWindow, #wxEvent
 #Introduction
 @ref eventprocessing_overview
 @ref progevent_overview
 @ref pluggablehandlers_overview
 @ref windowids_overview
 @ref eventmacros_overview
 @ref customevents_overview


 @section eventintroduction Introduction

 Before version 2.0 of wxWidgets, events were handled by the application
 either by supplying callback functions, or by overriding virtual member
 functions such as @b OnSize.
 From wxWidgets 2.0, @e event tables are used instead, with a few exceptions.
 An event table is placed in an implementation file to tell wxWidgets how to map
 events to member functions. These member functions are not virtual functions, but
 they are all similar in form: they take a single wxEvent-derived argument, and have a void return
 type.
 Here's an example of an event table.

 @code
 BEGIN_EVENT_TABLE(MyFrame, wxFrame)
   EVT_MENU    (wxID_EXIT, MyFrame::OnExit)
   EVT_MENU    (DO_TEST,   MyFrame::DoTest)
   EVT_SIZE    (           MyFrame::OnSize)
   EVT_BUTTON  (BUTTON1,   MyFrame::OnButton1)
 END_EVENT_TABLE()
 @endcode

 The first two entries map menu commands to two different member functions. The
 EVT_SIZE macro doesn't need a window identifier, since normally you are only
 interested in the current window's size events.
 The EVT_BUTTON macro demonstrates that the originating event does not have to
 come from the window class implementing the event table -- if the event source
 is a button within a panel within a frame, this will still work, because event
 tables are searched up through the hierarchy of windows for the command events.
 In this case, the button's event table will be searched, then the parent
 panel's, then the frame's.
 As mentioned before, the member functions that handle events do not have to be
 virtual. Indeed, the member functions should not be virtual as the event
 handler ignores that the functions are virtual, i.e. overriding a virtual
 member function in a derived class will not have any effect. These member
 functions take an event argument, and the class of event differs according to
 the type of event and the class of the originating window. For size events,
 #wxSizeEvent is used. For menu commands and most
 control commands (such as button presses),
 #wxCommandEvent is used. When controls get more
 complicated, then specific event classes are used, such as
 #wxTreeEvent for events from
 #wxTreeCtrl windows.
 As well as the event table in the implementation file, there must also be a
 DECLARE_EVENT_TABLE macro somewhere in the class declaration. For example:

 @code
 class MyFrame : public wxFrame
 {
 public:
   ...
   void OnExit(wxCommandEvent& event);
   void OnSize(wxSizeEvent& event);

 protected:
   int       m_count;
   ...

   DECLARE_EVENT_TABLE()
 };
 @endcode

 Note that this macro may occur in any section of the class (public, protected
 or private) but that it is probably better to insert it at the end, as shown,
 because this macro implicitly changes the access to protected which may be
 quite unexpected if there is anything following it.
 Finally, if you don't like using macros for static initialization of the event
 tables you may also use wxEvtHandler::Connect to
 connect the events to the handlers dynamically, during run-time. See the
 @ref sampleevent_overview for an example of doing it.


 @section eventprocessing How events are processed

 When an event is received from the windowing system, wxWidgets calls
 wxEvtHandler::ProcessEvent on the first
 event handler object belonging to the window generating the event.
 It may be noted that wxWidgets' event processing system implements something
 very close to virtual methods in normal C++, i.e. it is possible to alter
 the behaviour of a class by overriding its event handling functions. In
 many cases this works even for changing the behaviour of native controls.
 For example it is possible to filter out a number of key events sent by the
 system to a native text control by overriding wxTextCtrl and defining a
 handler for key events using EVT_KEY_DOWN. This would indeed prevent
 any key events from being sent to the native control - which might not be
 what is desired. In this case the event handler function has to call Skip()
 so as to indicate that the search for the event handler should continue.
 To summarize, instead of explicitly calling the base class version as you
 would have done with C++ virtual functions (i.e. @e wxTextCtrl::OnChar()),
 you should instead call #Skip.
 In practice, this would look like this if the derived text control only
 accepts 'a' to 'z' and 'A' to 'Z':

 @code
 void MyTextCtrl::OnChar(wxKeyEvent& event)
 {
     if ( isalpha( event.KeyCode() ) )
     {
        // key code is within legal range. we call event.Skip() so the
        // event can be processed either in the base wxWidgets class
        // or the native control.

        event.Skip();
     }
     else
     {
        // illegal key hit. we don't call event.Skip() so the
        // event is not processed anywhere else.

        wxBell();
     }
 }
 @endcode


 The normal order of event table searching by ProcessEvent is as follows:


  If the object is disabled (via a call to wxEvtHandler::SetEvtHandlerEnabled)
 the function skips to step (6).
  If the object is a wxWindow, @b ProcessEvent is recursively called on the window's
 #wxValidator. If this returns @true, the function exits.
  @b SearchEventTable is called for this event handler. If this fails, the base
 class table is tried, and so on until no more tables exist or an appropriate function was found,
 in which case the function exits.
  The search is applied down the entire chain of event handlers (usually the chain has a length
 of one). If this succeeds, the function exits.
  If the object is a wxWindow and the event is set to set to propagate (in the library only
 wxCommandEvent based events are set to propagate), @b ProcessEvent is recursively applied
 to the parent window's event handler. If this returns @true, the function exits.
  Finally, @b ProcessEvent is called on the wxApp object.


 @b Pay close attention to Step 5.  People often overlook or get
 confused by this powerful feature of the wxWidgets event processing
 system.  To put it a different way, events set to propagate
 (@ref eventshouldpropagate_overview)
 (most likely derived either directly or indirectly from wxCommandEvent)
 will travel up the containment hierarchy from child to parent until the
 maximal propagation level is reached or an event handler is found that
 doesn't call #event.Skip().
 Finally, there is another additional complication (which, in fact, simplifies
 life of wxWidgets programmers significantly): when propagating the command
 events upwards to the parent window, the event propagation stops when it
 reaches the parent dialog, if any. This means that you don't risk to get
 unexpected events from the dialog controls (which might be left unprocessed by
 the dialog itself because it doesn't care about them) when a modal dialog is
 popped up. The events do propagate beyond the frames, however. The rationale
 for this choice is that there are only a few frames in a typical application
 and their parent-child relation are well understood by the programmer while it
 may be very difficult, if not impossible, to track down all the dialogs which
 may be popped up in a complex program (remember that some are created
 automatically by wxWidgets). If you need to specify a different behaviour for
 some reason, you can use
 #SetExtraStyle(wxWS_EX_BLOCK_EVENTS)
 explicitly to prevent the events from being propagated beyond the given window
 or unset this flag for the dialogs which have it on by default.
 Typically events that deal with a window as a window (size, motion,
 paint, mouse, keyboard, etc.) are sent only to the window.  Events
 that have a higher level of meaning and/or are generated by the window
 itself, (button click, menu select, tree expand, etc.) are command
 events and are sent up to the parent to see if it is interested in the
 event.
 Note that your application may wish to override ProcessEvent to redirect processing of
 events. This is done in the document/view framework, for example, to allow event handlers
 to be defined in the document or view. To test for command events (which will probably
 be the only events you wish to redirect), you may use
 wxEvent::IsCommandEvent for efficiency,
 instead of using the slower run-time type system.
 As mentioned above, only command events are recursively applied to the parents event
 handler in the library itself. As this quite often causes confusion for users,
 here is a list of system events which will NOT get sent to the parent's event handler:






 #wxEvent




 The event base class





 #wxActivateEvent




 A window or application activation event





 #wxCloseEvent




 A close window or end session event





 #wxEraseEvent




 An erase background event





 #wxFocusEvent




 A window focus event





 #wxKeyEvent




 A keypress event





 #wxIdleEvent




 An idle event





 #wxInitDialogEvent




 A dialog initialisation event





 #wxJoystickEvent




 A joystick event





 #wxMenuEvent




 A menu event





 #wxMouseEvent




 A mouse event





 #wxMoveEvent




 A move event





 #wxPaintEvent




 A paint event





 #wxQueryLayoutInfoEvent




 Used to query layout information





 #wxSetCursorEvent




 Used for special cursor processing based on current mouse position





 #wxSizeEvent




 A size event





 #wxScrollWinEvent




 A scroll event sent by a scrolled window (not a scroll bar)





 #wxSysColourChangedEvent




 A system colour change event





 In some cases, it might be desired by the programmer to get a certain number
 of system events in a parent window, for example all key events sent to, but not
 used by, the native controls in a dialog. In this case, a special event handler
 will have to be written that will override ProcessEvent() in order to pass
 all events (or any selection of them) to the parent window.


 @section progevent Events generated by the user vs programmatically generated events

 While generically #wxEvents can be generated both by user
 actions (e.g. resize of a #wxWindow) and by calls to functions
 (e.g. wxWindow::SetSize), wxWidgets controls
 normally send #wxCommandEvent-derived events only for
 the user-generated events. The only @b exceptions to this rule are:






 wxNotebook::AddPage




 No event-free alternatives





 wxNotebook::AdvanceSelection




 No event-free alternatives





 wxNotebook::DeletePage




 No event-free alternatives





 wxNotebook::SetSelection




 Use wxNotebook::ChangeSelection instead, as wxNotebook::SetSelection is deprecated





 wxTreeCtrl::Delete




 No event-free alternatives





 wxTreeCtrl::DeleteAllItems




 No event-free alternatives





 wxTreeCtrl::EditLabel




 No event-free alternatives





 All #wxTextCtrl methods




 wxTextCtrl::ChangeValue can be used instead
 of wxTextCtrl::SetValue but the other functions,
 such as #Replace or #WriteText
 don't have event-free equivalents







 @section pluggablehandlers Pluggable event handlers

 In fact, you don't have to derive a new class from a window class
 if you don't want to. You can derive a new class from wxEvtHandler instead,
 defining the appropriate event table, and then call
 wxWindow::SetEventHandler (or, preferably,
 wxWindow::PushEventHandler) to make this
 event handler the object that responds to events. This way, you can avoid
 a lot of class derivation, and use instances of the same event handler class (but different
 objects as the same event handler object shouldn't be used more than once) to
 handle events from instances of different widget classes. If you ever have to call a window's event handler
 manually, use the GetEventHandler function to retrieve the window's event handler and use that
 to call the member function. By default, GetEventHandler returns a pointer to the window itself
 unless an application has redirected event handling using SetEventHandler or PushEventHandler.
 One use of PushEventHandler is to temporarily or permanently change the
 behaviour of the GUI. For example, you might want to invoke a dialog editor
 in your application that changes aspects of dialog boxes. You can
 grab all the input for an existing dialog box, and edit it 'in situ',
 before restoring its behaviour to normal. So even if the application
 has derived new classes to customize behaviour, your utility can indulge
 in a spot of body-snatching. It could be a useful technique for on-line
 tutorials, too, where you take a user through a serious of steps and
 don't want them to diverge from the lesson. Here, you can examine the events
 coming from buttons and windows, and if acceptable, pass them through to
 the original event handler. Use PushEventHandler/PopEventHandler
 to form a chain of event handlers, where each handler processes a different
 range of events independently from the other handlers.

 @section windowids Window identifiers

 Window identifiers are integers, and are used to
 uniquely determine window identity in the event system (though you can use it
 for other purposes). In fact, identifiers do not need to be unique
 across your entire application just so long as they are unique within a
 particular context you're interested in, such as a frame and its children. You
 may use the @c wxID_OK identifier, for example, on any number of dialogs so
 long as you don't have several within the same dialog.
 If you pass @c wxID_ANY to a window constructor, an identifier will be
 generated for you automatically by wxWidgets. This is useful when you don't
 care about the exact identifier either because you're not going to process the
 events from the control being created at all or because you process the events
 from all controls in one place (in which case you should specify @c wxID_ANY
 in the event table or wxEvtHandler::Connect call
 as well. The automatically generated identifiers are always negative and so
 will never conflict with the user-specified identifiers which must be always
 positive.
 The following standard identifiers are supplied. You can use wxID_HIGHEST to
 determine the number above which it is safe to define your own identifiers. Or,
 you can use identifiers below wxID_LOWEST.

 @code
 #define wxID_ANY                -1

 #define wxID_LOWEST             4999

 #define wxID_OPEN               5000
 #define wxID_CLOSE              5001
 #define wxID_NEW                5002
 #define wxID_SAVE               5003
 #define wxID_SAVEAS             5004
 #define wxID_REVERT             5005
 #define wxID_EXIT               5006
 #define wxID_UNDO               5007
 #define wxID_REDO               5008
 #define wxID_HELP               5009
 #define wxID_PRINT              5010
 #define wxID_PRINT_SETUP        5011
 #define wxID_PREVIEW            5012
 #define wxID_ABOUT              5013
 #define wxID_HELP_CONTENTS      5014
 #define wxID_HELP_COMMANDS      5015
 #define wxID_HELP_PROCEDURES    5016
 #define wxID_HELP_CONTEXT       5017

 #define wxID_CUT                5030
 #define wxID_COPY               5031
 #define wxID_PASTE              5032
 #define wxID_CLEAR              5033
 #define wxID_FIND               5034
 #define wxID_DUPLICATE          5035
 #define wxID_SELECTALL          5036
 #define wxID_DELETE             5037
 #define wxID_REPLACE            5038
 #define wxID_REPLACE_ALL        5039
 #define wxID_PROPERTIES         5040

 #define wxID_VIEW_DETAILS       5041
 #define wxID_VIEW_LARGEICONS    5042
 #define wxID_VIEW_SMALLICONS    5043
 #define wxID_VIEW_LIST          5044
 #define wxID_VIEW_SORTDATE      5045
 #define wxID_VIEW_SORTNAME      5046
 #define wxID_VIEW_SORTSIZE      5047
 #define wxID_VIEW_SORTTYPE      5048

 #define wxID_FILE1              5050
 #define wxID_FILE2              5051
 #define wxID_FILE3              5052
 #define wxID_FILE4              5053
 #define wxID_FILE5              5054
 #define wxID_FILE6              5055
 #define wxID_FILE7              5056
 #define wxID_FILE8              5057
 #define wxID_FILE9              5058

 #define wxID_OK                 5100
 #define wxID_CANCEL             5101
 #define wxID_APPLY              5102
 #define wxID_YES                5103
 #define wxID_NO                 5104
 #define wxID_STATIC             5105

 #define wxID_HIGHEST            5999
 @endcode


 @section eventmacros Event macros summary

 @b Macros listed by event class
 The documentation for specific event macros is organised by event class. Please refer
 to these sections for details.






 #wxActivateEvent




 The EVT_ACTIVATE and EVT_ACTIVATE_APP macros intercept
 activation and deactivation events.





 #wxCommandEvent




 A range of commonly-used control events.





 #wxCloseEvent




 The EVT_CLOSE macro handles window closure
 called via wxWindow::Close.





 #wxDropFilesEvent




 The EVT_DROP_FILES macros handles
 file drop events.





 #wxEraseEvent




 The EVT_ERASE_BACKGROUND macro is used to handle window erase requests.





 #wxFocusEvent




 The EVT_SET_FOCUS and EVT_KILL_FOCUS macros are used to handle keyboard focus events.





 #wxKeyEvent




 EVT_CHAR, EVT_KEY_DOWN and
 EVT_KEY_UP macros handle keyboard input for any window.





 #wxIdleEvent




 The EVT_IDLE macro handle application idle events
 (to process background tasks, for example).





 #wxInitDialogEvent




 The EVT_INIT_DIALOG macro is used
 to handle dialog initialisation.





 #wxListEvent




 These macros handle #wxListCtrl events.





 #wxMenuEvent




 These macros handle special menu events (not menu commands).





 #wxMouseEvent




 Mouse event macros can handle either individual
 mouse events or all mouse events.





 #wxMoveEvent




 The EVT_MOVE macro is used to handle a window move.





 #wxPaintEvent




 The EVT_PAINT macro is used to handle window paint requests.





 #wxScrollEvent




 These macros are used to handle scroll events from
 #wxScrollBar, #wxSlider,and #wxSpinButton.





 #wxSetCursorEvent




 The EVT_SET_CURSOR macro is used for special cursor processing.





 #wxSizeEvent




 The EVT_SIZE macro is used to handle a window resize.





 #wxSplitterEvent




 The EVT_SPLITTER_SASH_POS_CHANGED, EVT_SPLITTER_UNSPLIT
 and EVT_SPLITTER_DCLICK macros are used to handle the various splitter window events.





 #wxSysColourChangedEvent




 The EVT_SYS_COLOUR_CHANGED macro is used to handle
 events informing the application that the user has changed the system colours (Windows only).





 #wxTreeEvent




 These macros handle #wxTreeCtrl events.





 #wxUpdateUIEvent




 The EVT_UPDATE_UI macro is used to handle user interface
 update pseudo-events, which are generated to give the application the chance to update the visual state of menus,
 toolbars and controls.






 @section customevents Custom event summary

 @b General approach
 Since version 2.2.x of wxWidgets, each event type is identified by ID which
 is given to the event type @e at runtime which makes it possible to add
 new event types to the library or application without risking ID clashes
 (two different event types mistakingly getting the same event ID). This
 event type ID is stored in a struct of type @b const wxEventType.
 In order to define a new event type, there are principally two choices.
 One is to define a entirely new event class (typically deriving from
 #wxEvent or #wxCommandEvent.
 The other is to use the existing event classes and give them an new event
 type. You'll have to define and declare a new event type using either way,
 and this is done using the following macros:

 @code
 // in the header of the source file
 BEGIN_DECLARE_EVENT_TYPES()
 DECLARE_EVENT_TYPE(name, value)
 END_DECLARE_EVENT_TYPES()

 // in the implementation
 DEFINE_EVENT_TYPE(name)
 @endcode

 You can ignore the @e value parameter of the DECLARE_EVENT_TYPE macro
 since it is used only for backwards compatibility with wxWidgets 2.0.x based
 applications where you have to give the event type ID an explicit value.
 See also the @ref sampleevent_overview for an example of code
 defining and working with the custom event types.
 @b Using existing event classes
 If you just want to use a #wxCommandEvent with
 a new event type, you can then use one of the generic event table macros
 listed below, without having to define a new macro yourself. This also
 has the advantage that you won't have to define a new wxEvent::Clone()
 method for posting events between threads etc. This could look like this
 in your code:

 @code
 DECLARE_EVENT_TYPE(wxEVT_MY_EVENT, -1)

 DEFINE_EVENT_TYPE(wxEVT_MY_EVENT)

 // user code intercepting the event

 BEGIN_EVENT_TABLE(MyFrame, wxFrame)
   EVT_MENU    (wxID_EXIT, MyFrame::OnExit)
   // ....
   EVT_COMMAND  (ID_MY_WINDOW, wxEVT_MY_EVENT, MyFrame::OnMyEvent)
 END_EVENT_TABLE()

 void MyFrame::OnMyEvent( wxCommandEvent  )
 {
     // do something
     wxString text = event.GetText();
 }


 // user code sending the event

 void MyWindow::SendEvent()
 {
     wxCommandEvent event( wxEVT_MY_EVENT, GetId() );
     event.SetEventObject( this );
     // Give it some contents
     event.SetText( wxT("Hallo") );
     // Send it
     GetEventHandler()-ProcessEvent( event );
 }
 @endcode


 @b Generic event table macros






 @b EVT_CUSTOM(event, id, func)




 Allows you to add a custom event table
 entry by specifying the event identifier (such as wxEVT_SIZE), the window identifier,
 and a member function to call.





 @b EVT_CUSTOM_RANGE(event, id1, id2, func)




 The same as EVT_CUSTOM,
 but responds to a range of window identifiers.





 @b EVT_COMMAND(id, event, func)




 The same as EVT_CUSTOM, but
 expects a member function with a wxCommandEvent argument.





 @b EVT_COMMAND_RANGE(id1, id2, event, func)




 The same as EVT_CUSTOM_RANGE, but
 expects a member function with a wxCommandEvent argument.





 @b EVT_NOTIFY(event, id, func)




 The same as EVT_CUSTOM, but
 expects a member function with a wxNotifyEvent argument.





 @b EVT_NOTIFY_RANGE(event, id1, id2, func)




 The same as EVT_CUSTOM_RANGE, but
 expects a member function with a wxNotifyEvent argument.






 @b Defining your own event class
 Under certain circumstances, it will be required to define your own event
 class e.g. for sending more complex data from one place to another. Apart
 from defining your event class, you will also need to define your own
 event table macro (which is quite long). Watch out to put in enough
 casts to the inherited event function. Here is an example:

 @code
 // code defining event

 class wxPlotEvent: public wxNotifyEvent
 {
 public:
     wxPlotEvent( wxEventType commandType = wxEVT_@NULL, int id = 0 );

     // accessors
     wxPlotCurve *GetCurve()
         { return m_curve; }

     // required for sending with wxPostEvent()
     virtual wxEvent *Clone() const;

 private:
     wxPlotCurve   *m_curve;
 };

 DECLARE_EVENT_TYPE( wxEVT_PLOT_ACTION, -1 )

 typedef void (wxEvtHandler::*wxPlotEventFunction)(wxPlotEvent&);

 #define EVT_PLOT(id, fn) \
     DECLARE_EVENT_TABLE_ENTRY( wxEVT_PLOT_ACTION, id, -1, \
     (wxObjectEventFunction) (wxEventFunction) (wxCommandEventFunction) (wxNotifyEventFunction) \
     wxStaticCastEvent( wxPlotEventFunction, & fn ), (wxObject *) @NULL ),


 // code implementing the event type and the event class

 DEFINE_EVENT_TYPE( wxEVT_PLOT_ACTION )

 wxPlotEvent::wxPlotEvent( ...


 // user code intercepting the event

 BEGIN_EVENT_TABLE(MyFrame, wxFrame)
   EVT_PLOT  (ID_MY_WINDOW,  MyFrame::OnPlot)
 END_EVENT_TABLE()

 void MyFrame::OnPlot( wxPlotEvent  )
 {
     wxPlotCurve *curve = event.GetCurve();
 }


 // user code sending the event

 void MyWindow::SendEvent()
 {
     wxPlotEvent event( wxEVT_PLOT_ACTION, GetId() );
     event.SetEventObject( this );
     event.SetCurve( m_curve );
     GetEventHandler()-ProcessEvent( event );
 }
 @endcode

 */


