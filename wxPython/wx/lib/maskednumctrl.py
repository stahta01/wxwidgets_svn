#----------------------------------------------------------------------------
# Name:         wxPython.lib.maskednumctrl.py
# Author:       Will Sadkin
# Created:      09/06/2003
# Copyright:   (c) 2003 by Will Sadkin
# RCS-ID:      $Id$
# License:     wxWindows license
#----------------------------------------------------------------------------
# NOTE:
#   This was written to provide a numeric edit control for wxPython that
#   does things like right-insert (like a calculator), and does grouping, etc.
#   (ie. the features of wxMaskedTextCtrl), but allows Get/Set of numeric
#   values, rather than text.
#
#   wxMaskedNumCtrl permits integer, and floating point values to be set
#   retrieved or set via .GetValue() and .SetValue() (type chosen based on
#   fraction width, and provides an EVT_MASKEDNUM() event function for trapping
#   changes to the control.
#
#   It supports negative numbers as well as the naturals, and has the option
#   of not permitting leading zeros or an empty control; if an empty value is
#   not allowed, attempting to delete the contents of the control will result
#   in a (selected) value of zero, thus preserving a legitimate numeric value.
#   Similarly, replacing the contents of the control with '-' will result in
#   a selected (absolute) value of -1.
#
#   wxMaskedNumCtrl also supports range limits, with the option of either
#   enforcing them or simply coloring the text of the control if the limits
#   are exceeded.
#
#   wxMaskedNumCtrl is intended to support fixed-point numeric entry, and
#   is derived from  wxMaskedTextCtrl.  As such, it supports a limited range
#   of values to comply with a fixed-width entry mask.
#----------------------------------------------------------------------------
# 12/09/2003 - Jeff Grimmett (grimmtooth@softhome.net)
#
# o Updated for wx namespace
# 

"""<html><body>
<P>
<B>wxMaskedNumCtrl:</B>
<UL>
<LI>allows you to get and set integer or floating point numbers as value,</LI>
<LI>provides bounds support and optional value limiting,</LI>
<LI>has the right-insert input style that wxMaskedTextCtrl supports,</LI>
<LI>provides optional automatic grouping, sign control and format, grouping and decimal
character selection, etc. etc.</LI>
</UL>
<P>
Being derived from wxMaskedTextCtrl, the control only allows
fixed-point  notation.  That is, it has a fixed (though reconfigurable)
maximum width for the integer portion and optional fixed width
fractional portion.
<P>
Here's the API:
<DL><PRE>
    <B>wxMaskedNumCtrl</B>(
         parent, id = -1,
         <B>value</B> = 0,
         pos = wxDefaultPosition,
         size = wxDefaultSize,
         style = 0,
         validator = wxDefaultValidator,
         name = "maskednumber",
         <B>integerWidth</B> = 10,
         <B>fractionWidth</B> = 0,
         <B>allowNone</B> = False,
         <B>allowNegative</B> = True,
         <B>useParensForNegatives</B> = False,
         <B>groupDigits</B> = False,
         <B>groupChar</B> = ',',
         <B>decimalChar</B> = '.',
         <B>min</B> = None,
         <B>max</B> = None,
         <B>limited</B> = False,
         <B>selectOnEntry</b> = True,
         <B>foregroundColour</B> = "Black",
         <B>signedForegroundColour</B> = "Red",
         <B>emptyBackgroundColour</B> = "White",
         <B>validBackgroundColour</B> = "White",
         <B>invalidBackgroundColour</B> = "Yellow",
         )
</PRE>
<UL>
    <DT><B>value</B>
    <DD>If no initial value is set, the default will be zero, or
        the minimum value, if specified.  If an illegal string is specified,
        a ValueError will result. (You can always later set the initial
        value with SetValue() after instantiation of the control.)
    <BR>
    <DL><B>integerWidth</B>
    <DD>Indicates how many places to the right of any decimal point
        should be allowed in the control.  This will, perforce, limit
        the size of the values that can be entered. This number need
        not include space for grouping characters or the sign, if either
        of these options are enabled, as the resulting underlying
        mask is automatically by the control.  The default of 10
        will allow any 32 bit integer value.  The minimum value
        for integerWidth is 1.
    <BR>
    <DL><B>fractionWidth</B>
    <DD>Indicates how many decimal places to show for numeric value.
        If default (0), then the control will display and return only
        integer or long values.
    <BR>
    <DT><B>allowNone</B>
    <DD>Boolean indicating whether or not the control is allowed to be
        empty, representing a value of None for the control.
    <BR>
    <DT><B>allowNegative</B>
    <DD>Boolean indicating whether or not control is allowed to hold
        negative numbers.
    <BR>
    <DT><B>useParensForNegatives</B>
    <DD>If true, this will cause negative numbers to be displayed with ()s
        rather than -, (although '-' will still trigger a negative number.)
    <BR>
    <DT><B>groupDigits</B>
    <DD>Indicates whether or not grouping characters should be allowed and/or
        inserted when leaving the control or the decimal character is entered.
    <BR>
    <DT><B>groupChar</B>
    <DD>What grouping character will be used if allowed. (By default ',')
    <BR>
    <DT><B>decimalChar</B>
    <DD>If fractionWidth is > 0, what character will be used to represent
        the decimal point.  (By default '.')
    <BR>
    <DL><B>min</B>
    <DD>The minimum value that the control should allow.  This can be also be
        adjusted with SetMin().  If the control is not limited, any value
        below this bound will result in a background colored with the current
        invalidBackgroundColour.  If the min specified will not fit into the
        control, the min setting will be ignored.
    <BR>
    <DT><B>max</B>
    <DD>The maximum value that the control should allow.  This can be
        adjusted with SetMax().  If the control is not limited, any value
        above this bound will result in a background colored with the current
        invalidBackgroundColour.  If the max specified will not fit into the
        control, the max setting will be ignored.
    <BR>
    <DT><B>limited</B>
    <DD>Boolean indicating whether the control prevents values from
        exceeding the currently set minimum and maximum values (bounds).
        If False and bounds are set, out-of-bounds values will
        result in a background colored with the current invalidBackgroundColour.
    <BR>
    <DT><B>selectOnEntry</B>
    <DD>Boolean indicating whether or not the value in each field of the
        control should be automatically selected (for replacement) when
        that field is entered, either by cursor movement or tabbing.
        This can be desirable when using these controls for rapid data entry.
    <BR>
    <DT><B>foregroundColour</B>
    <DD>Color value used for positive values of the control.
    <BR>
    <DT><B>signedForegroundColour</B>
    <DD>Color value used for negative values of the control.
    <BR>
    <DT><B>emptyBackgroundColour</B>
    <DD>What background color to use when the control is considered
        "empty." (allow_none must be set to trigger this behavior.)
    <BR>
    <DT><B>validBackgroundColour</B>
    <DD>What background color to use when the control value is
        considered valid.
    <BR>
    <DT><B>invalidBackgroundColour</B>
    <DD>Color value used for illegal values or values out-of-bounds of the
        control when the bounds are set but the control is not limited.
</UL>
<BR>
<BR>
<DT><B>EVT_MASKEDNUM(win, id, func)</B>
<DD>Respond to a wxEVT_COMMAND_MASKED_NUMBER_UPDATED event, generated when
the value changes. Notice that this event will always be sent when the
control's contents changes - whether this is due to user input or
comes from the program itself (for example, if SetValue() is called.)
<BR>
<BR>
<DT><B>SetValue(int|long|float|string)</B>
<DD>Sets the value of the control to the value specified, if
possible.  The resulting actual value of the control may be
altered to conform to the format of the control, changed
to conform with the bounds set on the control if limited,
or colored if not limited but the value is out-of-bounds.
A ValueError exception will be raised if an invalid value
is specified.
<BR>
<DT><B>GetValue()</B>
<DD>Retrieves the numeric value from the control.  The value
retrieved will be either be returned as a long if the
fractionWidth is 0, or a float otherwise.
<BR>
<BR>
<DT><B>SetParameters(**kwargs)</B>
<DD>Allows simultaneous setting of various attributes
of the control after construction.  Keyword arguments
allowed are the same parameters as supported in the constructor.
<BR>
<BR>
<DT><B>SetIntegerWidth(value)</B>
<DD>Resets the width of the integer portion of the control.  The
value must be >= 1, or an AttributeError exception will result.
This value should account for any grouping characters that might
be inserted (if grouping is enabled), but does not need to account
for the sign, as that is handled separately by the control.
<DT><B>GetIntegerWidth()</B>
<DD>Returns the current width of the integer portion of the control,
not including any reserved sign position.
<BR>
<BR>
<DT><B>SetFractionWidth(value)</B>
<DD>Resets the width of the fractional portion of the control.  The
value must be >= 0, or an AttributeError exception will result.  If
0, the current value of the control will be truncated to an integer
value.
<DT><B>GetFractionWidth()</B>
<DD>Returns the current width of the fractional portion of the control.
<BR>
<BR>
<DT><B>SetMin(min=None)</B>
<DD>Resets the minimum value of the control.  If a value of <I>None</I>
is provided, then the control will have no explicit minimum value.
If the value specified is greater than the current maximum value,
then the function returns False and the minimum will not change from
its current setting.  On success, the function returns True.
<DT><DD>
If successful and the current value is lower than the new lower
bound, if the control is limited, the value will be automatically
adjusted to the new minimum value; if not limited, the value in the
control will be colored as invalid.
<DT><DD>
If min > the max value allowed by the width of the control,
the function will return False, and the min will not be set.
<BR>
<DT><B>GetMin()</B>
<DD>Gets the current lower bound value for the control.
It will return None if no lower bound is currently specified.
<BR>
<BR>
<DT><B>SetMax(max=None)</B>
<DD>Resets the maximum value of the control. If a value of <I>None</I>
is provided, then the control will have no explicit maximum value.
If the value specified is less than the current minimum value, then
the function returns False and the maximum will not change from its
current setting. On success, the function returns True.
<DT><DD>
If successful and the current value is greater than the new upper
bound, if the control is limited the value will be automatically
adjusted to this maximum value; if not limited, the value in the
control will be colored as invalid.
<DT><DD>
If max > the max value allowed by the width of the control,
the function will return False, and the max will not be set.
<BR>
<DT><B>GetMax()</B>
<DD>Gets the current upper bound value for the control.
It will return None if no upper bound is currently specified.
<BR>
<BR>
<DT><B>SetBounds(min=None,max=None)</B>
<DD>This function is a convenience function for setting the min and max
values at the same time.  The function only applies the maximum bound
if setting the minimum bound is successful, and returns True
only if both operations succeed.  <B><I>Note:</I></B> leaving out an argument
will remove the corresponding bound.
<DT><B>GetBounds()</B>
<DD>This function returns a two-tuple (min,max), indicating the
current bounds of the control.  Each value can be None if
that bound is not set.
<BR>
<BR>
<DT><B>IsInBounds(value=None)</B>
<DD>Returns <I>True</I> if no value is specified and the current value
of the control falls within the current bounds.  This function can also
be called with a value to see if that value would fall within the current
bounds of the given control.
<BR>
<BR>
<DT><B>SetLimited(bool)</B>
<DD>If called with a value of True, this function will cause the control
to limit the value to fall within the bounds currently specified.
If the control's value currently exceeds the bounds, it will then
be limited accordingly.
If called with a value of False, this function will disable value
limiting, but coloring of out-of-bounds values will still take
place if bounds have been set for the control.
<DT><B>GetLimited()</B>
<DT><B>IsLimited()</B>
<DD>Returns <I>True</I> if the control is currently limiting the
value to fall within the current bounds.
<BR>
<BR>
<DT><B>SetAllowNone(bool)</B>
<DD>If called with a value of True, this function will cause the control
to allow the value to be empty, representing a value of None.
If called with a value of False, this function will prevent the value
from being None.  If the value of the control is currently None,
ie. the control is empty, then the value will be changed to that
of the lower bound of the control, or 0 if no lower bound is set.
<DT><B>GetAllowNone()</B>
<DT><B>IsNoneAllowed()</B>
<DD>Returns <I>True</I> if the control currently allows its
value to be None.
<BR>
<BR>
<DT><B>SetAllowNegative(bool)</B>
<DD>If called with a value of True, this function will cause the
control to allow the value to be negative (and reserve space for
displaying the sign. If called with a value of False, and the
value of the control is currently negative, the value of the
control will be converted to the absolute value, and then
limited appropriately based on the existing bounds of the control
(if any).
<DT><B>GetAllowNegative()</B>
<DT><B>IsNegativeAllowed()</B>
<DD>Returns <I>True</I> if the control currently permits values
to be negative.
<BR>
<BR>
<DT><B>SetGroupDigits(bool)</B>
<DD>If called with a value of True, this will make the control
automatically add and manage grouping characters to the presented
value in integer portion of the control.
<DT><B>GetGroupDigits()</B>
<DT><B>IsGroupingAllowed()</B>
<DD>Returns <I>True</I> if the control is currently set to group digits.
<BR>
<BR>
<DT><B>SetGroupChar()</B>
<DD>Sets the grouping character for the integer portion of the
control.  (The default grouping character this is ','.
<DT><B>GetGroupChar()</B>
<DD>Returns the current grouping character for the control.
<BR>
<BR>
<DT><B>SetSelectOnEntry()</B>
<DD>If called with a value of <I>True</I>, this will make the control
automatically select the contents of each field as it is entered
within the control.  (The default is True.)
<DT><B>GetSelectOnEntry()</B>
<DD>Returns <I>True</I> if the control currently auto selects
the field values on entry.
<BR>
<BR>
</DL>
</body></html>
"""

import  copy
import  string
import  types

import  wx

from sys import maxint
MAXINT = maxint     # (constants should be in upper case)
MININT = -maxint-1

from wx.tools.dbg import Logger
from wx.lib.maskededit import wxMaskedEditMixin, wxMaskedTextCtrl, Field

dbg = Logger()
dbg(enable=0)

#----------------------------------------------------------------------------

wxEVT_COMMAND_MASKED_NUMBER_UPDATED = wx.NewEventType()
EVT_MASKEDNUM = wx.PyEventBinder(wxEVT_COMMAND_MASKED_NUMBER_UPDATED, 1)

#----------------------------------------------------------------------------

class wxMaskedNumNumberUpdatedEvent(wx.PyCommandEvent):
    def __init__(self, id, value = 0, object=None):
        wx.PyCommandEvent.__init__(self, wxEVT_COMMAND_MASKED_NUMBER_UPDATED, id)

        self.__value = value
        self.SetEventObject(object)

    def GetValue(self):
        """Retrieve the value of the control at the time
        this event was generated."""
        return self.__value


#----------------------------------------------------------------------------

class wxMaskedNumCtrl(wxMaskedTextCtrl):

    valid_ctrl_params = {
        'integerWidth': 10,                 # by default allow all 32-bit integers
        'fractionWidth': 0,                  # by default, use integers
        'decimalChar': '.',                 # by default, use '.' for decimal point
        'allowNegative': True,              # by default, allow negative numbers
        'useParensForNegatives': False,     # by default, use '-' to indicate negatives
        'groupDigits': True,             # by default, don't insert grouping
        'groupChar': ',',                   # by default, use ',' for grouping
        'min': None,                        # by default, no bounds set
        'max': None,
        'limited': False,                   # by default, no limiting even if bounds set
        'allowNone': False,                 # by default, don't allow empty value
        'selectOnEntry': True,              # by default, select the value of each field on entry
        'foregroundColour': "Black",
        'signedForegroundColour': "Red",
        'emptyBackgroundColour': "White",
        'validBackgroundColour': "White",
        'invalidBackgroundColour': "Yellow",
        'useFixedWidthFont': True,  # by default, use a fixed-width font
        }


    def __init__ (
                self, parent, id=-1, value = 0,
                pos = wx.DefaultPosition, size = wx.DefaultSize,
                style = wx.TE_PROCESS_TAB, validator = wx.DefaultValidator,
                name = "maskednum",
                **kwargs ):

        dbg('wxMaskedNumCtrl::__init__', indent=1)

        # Set defaults for control:
        dbg('setting defaults:')
        for key, param_value in wxMaskedNumCtrl.valid_ctrl_params.items():
            # This is done this way to make setattr behave consistently with
            # "private attribute" name mangling
            setattr(self, '_' + key, copy.copy(param_value))

        # Assign defaults for all attributes:
        init_args = copy.deepcopy(wxMaskedNumCtrl.valid_ctrl_params)
        dbg('kwargs:', kwargs)
        for key, param_value in kwargs.items():
            key = key.replace('Color', 'Colour')
            if key not in wxMaskedNumCtrl.valid_ctrl_params.keys():
                raise AttributeError('invalid keyword argument "%s"' % key)
            else:
                init_args[key] = param_value
        dbg('init_args:', indent=1)
        for key, param_value in init_args.items():
            dbg('%s:' % key, param_value)
        dbg(indent=0)

        # Process initial fields for the control, as part of construction:
        if type(init_args['integerWidth']) != types.IntType:
            raise AttributeError('invalid integerWidth (%s) specified; expected integer' % repr(init_args['integerWidth']))
        elif init_args['integerWidth'] < 1:
            raise AttributeError('invalid integerWidth (%s) specified; must be > 0' % repr(init_args['integerWidth']))

        fields = {}

        if init_args.has_key('fractionWidth'):
            if type(init_args['fractionWidth']) != types.IntType:
                raise AttributeError('invalid fractionWidth (%s) specified; expected integer' % repr(self._fractionWidth))
            elif init_args['fractionWidth'] < 0:
                raise AttributeError('invalid fractionWidth (%s) specified; must be >= 0' % repr(init_args['fractionWidth']))
            self._fractionWidth = init_args['fractionWidth']

        if self._fractionWidth:
            fracmask = '.' + '#{%d}' % self._fractionWidth
            dbg('fracmask:', fracmask)
            fields[1] = Field(defaultValue='0'*self._fractionWidth)
        else:
            fracmask = ''

        self._integerWidth = init_args['integerWidth']
        if init_args['groupDigits']:
            self._groupSpace = (self._integerWidth - 1) / 3
        else:
            self._groupSpace = 0
        intmask = '#{%d}' % (self._integerWidth + self._groupSpace)
        if self._fractionWidth:
            emptyInvalid = False
        else:
            emptyInvalid = True
        fields[0] = Field(formatcodes='r<>', emptyInvalid=emptyInvalid)
        dbg('intmask:', intmask)

        # don't bother to reprocess these arguments:
        del init_args['integerWidth']
        del init_args['fractionWidth']


        mask = intmask+fracmask

        # initial value of state vars
        self._oldvalue = 0
        self._integerEnd = 0
        self._typedSign = False

        # Construct the base control:
        wxMaskedTextCtrl.__init__(
                self, parent, id, '',
                pos, size, style, validator, name,
                mask = mask,
                formatcodes = 'FR<',
                fields = fields,
                validFunc=self.IsInBounds,
                setupEventHandling = False)

        self.Bind(wx.EVT_SET_FOCUS, self._OnFocus )        ## defeat automatic full selection
        self.Bind(wx.EVT_KILL_FOCUS, self._OnKillFocus )   ## run internal validator
        self.Bind(wx.EVT_LEFT_DCLICK, self._OnDoubleClick)  ## select field under cursor on dclick
        self.Bind(wx.EVT_RIGHT_UP, self._OnContextMenu )    ## bring up an appropriate context menu
        self.Bind(wx.EVT_KEY_DOWN, self._OnKeyDown )       ## capture control events not normally seen, eg ctrl-tab.
        self.Bind(wx.EVT_CHAR, self._OnChar )              ## handle each keypress
        self.Bind(wx.EVT_TEXT, self.OnTextChange )      ## color control appropriately & keep
                                                           ## track of previous value for undo

        # Establish any additional parameters, with appropriate error checking
        self.SetParameters(**init_args)

        # Set the value requested (if possible)
##        wxCallAfter(self.SetValue, value)
        self.SetValue(value)

        # Ensure proper coloring:
        self.Refresh()
        dbg('finished wxMaskedNumCtrl::__init__', indent=0)


    def SetParameters(self, **kwargs):
        """
        This routine is used to initialize and reconfigure the control:
        """
        dbg('wxMaskedNumCtrl::SetParameters', indent=1)
        maskededit_kwargs = {}
        reset_fraction_width = False


        if( (kwargs.has_key('integerWidth') and kwargs['integerWidth'] != self._integerWidth)
            or (kwargs.has_key('fractionWidth') and kwargs['fractionWidth'] != self._fractionWidth)
            or (kwargs.has_key('groupDigits') and kwargs['groupDigits'] != self._groupDigits) ):

            fields = {}

            if kwargs.has_key('fractionWidth'):
                if type(kwargs['fractionWidth']) != types.IntType:
                    raise AttributeError('invalid fractionWidth (%s) specified; expected integer' % repr(kwargs['fractionWidth']))
                elif kwargs['fractionWidth'] < 0:
                    raise AttributeError('invalid fractionWidth (%s) specified; must be >= 0' % repr(kwargs['fractionWidth']))
                else:
                    if self._fractionWidth != kwargs['fractionWidth']:
                        self._fractionWidth = kwargs['fractionWidth']

            if self._fractionWidth:
                fracmask = '.' + '#{%d}' % self._fractionWidth
                fields[1] = Field(defaultValue='0'*self._fractionWidth)
                emptyInvalid = False
            else:
                emptyInvalid = True
                fracmask = ''
            dbg('fracmask:', fracmask)

            if kwargs.has_key('integerWidth'):
                if type(kwargs['integerWidth']) != types.IntType:
                    dbg(indent=0)
                    raise AttributeError('invalid integerWidth (%s) specified; expected integer' % repr(kwargs['integerWidth']))
                elif kwargs['integerWidth'] < 0:
                    dbg(indent=0)
                    raise AttributeError('invalid integerWidth (%s) specified; must be > 0' % repr(kwargs['integerWidth']))
                else:
                    self._integerWidth = kwargs['integerWidth']

            if kwargs.has_key('groupDigits'):
                self._groupDigits = kwargs['groupDigits']

            if self._groupDigits:
                self._groupSpace = (self._integerWidth - 1) / 3
            else:
                self._groupSpace = 0

            intmask = '#{%d}' % (self._integerWidth + self._groupSpace)
            dbg('intmask:', intmask)
            fields[0] = Field(formatcodes='r<>', emptyInvalid=emptyInvalid)
            maskededit_kwargs['fields'] = fields

            # don't bother to reprocess these arguments:
            if kwargs.has_key('integerWidth'):
                del kwargs['integerWidth']
            if kwargs.has_key('fractionWidth'):
                del kwargs['fractionWidth']

            maskededit_kwargs['mask'] = intmask+fracmask

        if kwargs.has_key('groupChar'):
            old_groupchar = self._groupChar     # save so we can reformat properly
            dbg("old_groupchar: '%s'" % old_groupchar)
            maskededit_kwargs['groupChar'] = kwargs['groupChar']
        if kwargs.has_key('decimalChar'):
            old_decimalchar = self._decimalChar
            dbg("old_decimalchar: '%s'" % old_decimalchar)
            maskededit_kwargs['decimalChar'] = kwargs['decimalChar']

        # for all other parameters, assign keyword args as appropriate:
        for key, param_value in kwargs.items():
            key = key.replace('Color', 'Colour')
            if key not in wxMaskedNumCtrl.valid_ctrl_params.keys():
                raise AttributeError('invalid keyword argument "%s"' % key)
            elif key not in wxMaskedEditMixin.valid_ctrl_params.keys():
                setattr(self, '_' + key, param_value)
            elif key in ('mask', 'autoformat'): # disallow explicit setting of mask
                raise AttributeError('invalid keyword argument "%s"' % key)
            else:
                maskededit_kwargs[key] = param_value
        dbg('kwargs:', kwargs)

        # reprocess existing format codes to ensure proper resulting format:
        formatcodes = self.GetFormatcodes()
        if kwargs.has_key('allowNegative'):
            if kwargs['allowNegative'] and '-' not in formatcodes:
                formatcodes += '-'
                maskededit_kwargs['formatcodes'] = formatcodes
            elif not kwargs['allowNegative'] and '-' in formatcodes:
                formatcodes = formatcodes.replace('-','')
                maskededit_kwargs['formatcodes'] = formatcodes

        if kwargs.has_key('groupDigits'):
            if kwargs['groupDigits'] and ',' not in formatcodes:
                formatcodes += ','
                maskededit_kwargs['formatcodes'] = formatcodes
            elif not kwargs['groupDigits'] and ',' in formatcodes:
                formatcodes = formatcodes.replace(',','')
                maskededit_kwargs['formatcodes'] = formatcodes

        if kwargs.has_key('selectOnEntry'):
            self._selectOnEntry = kwargs['selectOnEntry']
            dbg("kwargs['selectOnEntry']?", kwargs['selectOnEntry'], "'S' in formatcodes?", 'S' in formatcodes)
            if kwargs['selectOnEntry'] and 'S' not in formatcodes:
                formatcodes += 'S'
                maskededit_kwargs['formatcodes'] = formatcodes
            elif not kwargs['selectOnEntry'] and 'S' in formatcodes:
                formatcodes = formatcodes.replace('S','')
                maskededit_kwargs['formatcodes'] = formatcodes

        if 'r' in formatcodes and self._fractionWidth:
            # top-level mask should only be right insert if no fractional
            # part will be shown; ie. if reconfiguring control, remove
            # previous "global" setting.
            formatcodes = formatcodes.replace('r', '')
            maskededit_kwargs['formatcodes'] = formatcodes

        if kwargs.has_key('limited'):
            if kwargs['limited'] and not self._limited:
                maskededit_kwargs['validRequired'] = True
            elif not kwargs['limited'] and self._limited:
                maskededit_kwargs['validRequired'] = False
            self._limited = kwargs['limited']

        dbg('maskededit_kwargs:', maskededit_kwargs)
        if maskededit_kwargs.keys():
            self.SetCtrlParameters(**maskededit_kwargs)

        # Record end of integer and place cursor there:
        integerEnd = self._fields[0]._extent[1]
        self.SetInsertionPoint(integerEnd)
        self.SetSelection(integerEnd, integerEnd)

        # Go ensure all the format codes necessary are present:
        orig_intformat = intformat = self.GetFieldParameter(0, 'formatcodes')
        if 'r' not in intformat:
            intformat += 'r'
        if '>' not in intformat:
            intformat += '>'
        if intformat != orig_intformat:
            if self._fractionWidth:
                self.SetFieldParameters(0, formatcodes=intformat)
            else:
                self.SetCtrlParameters(formatcodes=intformat)

        # Set min and max as appropriate:
        if kwargs.has_key('min'):
            min = kwargs['min']
            if( self._max is None
                or min is None
                or (self._max is not None and self._max >= min) ):
                dbg('examining min')
                if min is not None:
                    try:
                        textmin = self._toGUI(min, apply_limits = False)
                    except ValueError:
                        dbg('min will not fit into control; ignoring', indent=0)
                        raise
                dbg('accepted min')
                self._min = min
            else:
                dbg('ignoring min')


        if kwargs.has_key('max'):
            max = kwargs['max']
            if( self._min is None
                or max is None
                or (self._min is not None and self._min <= max) ):
                dbg('examining max')
                if max is not None:
                    try:
                        textmax = self._toGUI(max, apply_limits = False)
                    except ValueError:
                        dbg('max will not fit into control; ignoring', indent=0)
                        raise
                dbg('accepted max')
                self._max = max
            else:
                dbg('ignoring max')

        if kwargs.has_key('allowNegative'):
            self._allowNegative = kwargs['allowNegative']

        # Ensure current value of control obeys any new restrictions imposed:
        text = self._GetValue()
        dbg('text value: "%s"' % text)
        if kwargs.has_key('groupChar') and text.find(old_groupchar) != -1:
            text = text.replace(old_groupchar, self._groupChar)
        if kwargs.has_key('decimalChar') and text.find(old_decimalchar) != -1:
            text = text.replace(old_decimalchar, self._decimalChar)
        if text != self._GetValue():
            wx.TextCtrl.SetValue(self, text)

        value = self.GetValue()

        dbg('self._allowNegative?', self._allowNegative)
        if not self._allowNegative and self._isNeg:
            value = abs(value)
            dbg('abs(value):', value)
            self._isNeg = False

        elif not self._allowNone and wxMaskedTextCtrl.GetValue(self) == '':
            if self._min > 0:
                value = self._min
            else:
                value = 0

        sel_start, sel_to = self.GetSelection()
        if self.IsLimited() and self._min is not None and value < self._min:
            dbg('Set to min value:', self._min)
            self._SetValue(self._toGUI(self._min))

        elif self.IsLimited() and self._max is not None and value > self._max:
            dbg('Setting to max value:', self._max)
            self._SetValue(self._toGUI(self._max))
        else:
            # reformat current value as appropriate to possibly new conditions
            dbg('Reformatting value:', value)
            sel_start, sel_to = self.GetSelection()
            self._SetValue(self._toGUI(value))
        self.Refresh() # recolor as appropriate
        dbg('finished wxMaskedNumCtrl::SetParameters', indent=0)



    def _GetNumValue(self, value):
        """
        This function attempts to "clean up" a text value, providing a regularized
        convertable string, via atol() or atof(), for any well-formed numeric text value.
        """
        return value.replace(self._groupChar, '').replace(self._decimalChar, '.').replace('(', '-').replace(')','').strip()


    def GetFraction(self, candidate=None):
        """
        Returns the fractional portion of the value as a float.  If there is no
        fractional portion, the value returned will be 0.0.
        """
        if not self._fractionWidth:
            return 0.0
        else:
            fracstart, fracend = self._fields[1]._extent
            if candidate is None:
                value = self._toGUI(wxMaskedTextCtrl.GetValue(self))
            else:
                value = self._toGUI(candidate)
            fracstring = value[fracstart:fracend].strip()
            if not value:
                return 0.0
            else:
                return string.atof(fracstring)

    def _OnChangeSign(self, event):
        dbg('wxMaskedNumCtrl::_OnChangeSign', indent=1)
        self._typedSign = True
        wxMaskedEditMixin._OnChangeSign(self, event)
        dbg(indent=0)


    def _disallowValue(self):
        dbg('wxMaskedNumCtrl::_disallowValue')
        # limited and -1 is out of bounds
        if self._typedSign:
            self._isNeg = False
        if not wx.Validator_IsSilent():
            wx.Bell()
        sel_start, sel_to = self._GetSelection()
        dbg('queuing reselection of (%d, %d)' % (sel_start, sel_to))
        wx.CallAfter(self.SetInsertionPoint, sel_start)      # preserve current selection/position
        wx.CallAfter(self.SetSelection, sel_start, sel_to)

    def _SetValue(self, value):
        """
        This routine supersedes the base masked control _SetValue().  It is
        needed to ensure that the value of the control is always representable/convertable
        to a numeric return value (via GetValue().)  This routine also handles
        automatic adjustment and grouping of the value without explicit intervention
        by the user.
        """

        dbg('wxMaskedNumCtrl::_SetValue("%s")' % value, indent=1)

        if( (self._fractionWidth and value.find(self._decimalChar) == -1) or
            (self._fractionWidth == 0 and value.find(self._decimalChar) != -1) ) :
            value = self._toGUI(value)

        numvalue = self._GetNumValue(value)
        dbg('cleansed value: "%s"' % numvalue)
        replacement = None

        if numvalue == "":
            if self._allowNone:
                dbg('calling base wxMaskedTextCtrl._SetValue(self, "%s")' % value)
                wxMaskedTextCtrl._SetValue(self, value)
                self.Refresh()
                return
            elif self._min > 0 and self.IsLimited():
                replacement = self._min
            else:
                replacement = 0
            dbg('empty value; setting replacement:', replacement)

        if replacement is None:
            # Go get the integer portion about to be set and verify its validity
            intstart, intend = self._fields[0]._extent
            dbg('intstart, intend:', intstart, intend)
            dbg('raw integer:"%s"' % value[intstart:intend])
            int = self._GetNumValue(value[intstart:intend])
            numval = self._fromGUI(value)

            dbg('integer: "%s"' % int)
            try:
                fracval = self.GetFraction(value)
            except ValueError, e:
                dbg('Exception:', e, 'must be out of bounds; disallow value')
                self._disallowValue()
                dbg(indent=0)
                return

            if fracval == 0.0:
                dbg('self._isNeg?', self._isNeg)
                if int == '-' and self._oldvalue < 0 and not self._typedSign:
                    dbg('just a negative sign; old value < 0; setting replacement of 0')
                    replacement = 0
                    self._isNeg = False
                elif int[:2] == '-0' and self._fractionWidth == 0:
                    if self._oldvalue < 0:
                        dbg('-0; setting replacement of 0')
                        replacement = 0
                        self._isNeg = False
                    elif not self._limited or (self._min < -1 and self._max >= -1):
                        dbg('-0; setting replacement of -1')
                        replacement = -1
                        self._isNeg = True
                    else:
                        # limited and -1 is out of bounds
                        self._disallowValue()
                        dbg(indent=0)
                        return

                elif int == '-' and (self._oldvalue >= 0 or self._typedSign) and self._fractionWidth == 0:
                    if not self._limited or (self._min < -1 and self._max >= -1):
                        dbg('just a negative sign; setting replacement of -1')
                        replacement = -1
                    else:
                        # limited and -1 is out of bounds
                        self._disallowValue()
                        dbg(indent=0)
                        return

                elif( self._typedSign
                      and int.find('-') != -1
                      and self._limited
                      and not self._min <= numval <= self._max):
                    # changed sign resulting in value that's now out-of-bounds;
                    # disallow
                    self._disallowValue()
                    dbg(indent=0)
                    return

            if replacement is None:
                if int and int != '-':
                    try:
                        string.atol(int)
                    except ValueError:
                        # integer requested is not legal.  This can happen if the user
                        # is attempting to insert a digit in the middle of the control
                        # resulting in something like "   3   45". Disallow such actions:
                        dbg('>>>>>>>>>>>>>>>> "%s" does not convert to a long!' % int)
                        if not wx.Validator_IsSilent():
                            wx.Bell()
                        sel_start, sel_to = self._GetSelection()
                        dbg('queuing reselection of (%d, %d)' % (sel_start, sel_to))
                        wx.CallAfter(self.SetInsertionPoint, sel_start)      # preserve current selection/position
                        wx.CallAfter(self.SetSelection, sel_start, sel_to)
                        dbg(indent=0)
                        return

                    if int[0] == '0' and len(int) > 1:
                        dbg('numvalue: "%s"' % numvalue.replace(' ', ''))
                        if self._fractionWidth:
                            value = self._toGUI(string.atof(numvalue))
                        else:
                            value = self._toGUI(string.atol(numvalue))
                        dbg('modified value: "%s"' % value)

        self._typedSign = False     # reset state var

        if replacement is not None:
            # Value presented wasn't a legal number, but control should do something
            # reasonable instead:
            dbg('setting replacement value:', replacement)
            self._SetValue(self._toGUI(replacement))
            sel_start = wxMaskedTextCtrl.GetValue(self).find(str(abs(replacement)))   # find where it put the 1, so we can select it
            sel_to = sel_start + len(str(abs(replacement)))
            dbg('queuing selection of (%d, %d)' %(sel_start, sel_to))
            wx.CallAfter(self.SetInsertionPoint, sel_start)
            wx.CallAfter(self.SetSelection, sel_start, sel_to)
            dbg(indent=0)
            return

        # Otherwise, apply appropriate formatting to value:

        # Because we're intercepting the value and adjusting it
        # before a sign change is detected, we need to do this here:
        if '-' in value or '(' in value:
            self._isNeg = True
        else:
            self._isNeg = False

        dbg('value:"%s"' % value, 'self._useParens:', self._useParens)
        if self._fractionWidth:
            adjvalue = self._adjustFloat(self._GetNumValue(value).replace('.',self._decimalChar))
        else:
            adjvalue = self._adjustInt(self._GetNumValue(value))
        dbg('adjusted value: "%s"' % adjvalue)


        sel_start, sel_to = self._GetSelection()     # record current insertion point
        dbg('calling base wxMaskedTextCtrl._SetValue(self, "%s")' % adjvalue)
        wxMaskedTextCtrl._SetValue(self, adjvalue)
        # After all actions so far scheduled, check that resulting cursor
        # position is appropriate, and move if not:
        wx.CallAfter(self._CheckInsertionPoint)

        dbg('finished wxMaskedNumCtrl::_SetValue', indent=0)

    def _CheckInsertionPoint(self):
        # If current insertion point is before the end of the integer and
        # its before the 1st digit, place it just after the sign position:
        dbg('wxMaskedNumCtrl::CheckInsertionPoint', indent=1)
        sel_start, sel_to = self._GetSelection()
        text = self._GetValue()
        if sel_to < self._fields[0]._extent[1] and text[sel_to] in (' ', '-', '('):
            text, signpos, right_signpos = self._getSignedValue()
            dbg('setting selection(%d, %d)' % (signpos+1, signpos+1))
            self.SetInsertionPoint(signpos+1)
            self.SetSelection(signpos+1, signpos+1)
        dbg(indent=0)


    def _OnErase( self, event ):
        """
        This overrides the base control _OnErase, so that erasing around
        grouping characters auto selects the digit before or after the
        grouping character, so that the erasure does the right thing.
        """
        dbg('wxMaskedNumCtrl::_OnErase', indent=1)

        #if grouping digits, make sure deletes next to group char always
        # delete next digit to appropriate side:
        if self._groupDigits:
            key = event.GetKeyCode()
            value = wxMaskedTextCtrl.GetValue(self)
            sel_start, sel_to = self._GetSelection()

            if key == wx.WXK_BACK:
                # if 1st selected char is group char, select to previous digit
                if sel_start > 0 and sel_start < len(self._mask) and value[sel_start:sel_to] == self._groupChar:
                    self.SetInsertionPoint(sel_start-1)
                    self.SetSelection(sel_start-1, sel_to)

                # elif previous char is group char, select to previous digit
                elif sel_start > 1 and sel_start == sel_to and value[sel_start-1:sel_start] == self._groupChar:
                    self.SetInsertionPoint(sel_start-2)
                    self.SetSelection(sel_start-2, sel_to)

            elif key == wx.WXK_DELETE:
                if( sel_to < len(self._mask) - 2 + (1 *self._useParens)
                    and sel_start == sel_to
                    and value[sel_to] == self._groupChar ):
                    self.SetInsertionPoint(sel_start)
                    self.SetSelection(sel_start, sel_to+2)

                elif( sel_to < len(self._mask) - 2 + (1 *self._useParens)
                           and value[sel_start:sel_to] == self._groupChar ):
                    self.SetInsertionPoint(sel_start)
                    self.SetSelection(sel_start, sel_to+1)

        wxMaskedTextCtrl._OnErase(self, event)
        dbg(indent=0)


    def OnTextChange( self, event ):
        """
        Handles an event indicating that the text control's value
        has changed, and issue EVT_MaskedNum event.
        NOTE: using wxTextCtrl.SetValue() to change the control's
        contents from within a EVT_CHAR handler can cause double
        text events.  So we check for actual changes to the text
        before passing the events on.
        """
        dbg('wxMaskedNumCtrl::OnTextChange', indent=1)
        if not wxMaskedTextCtrl._OnTextChange(self, event):
            dbg(indent=0)
            return

        # else... legal value

        value = self.GetValue()
        if value != self._oldvalue:
            try:
                self.GetEventHandler().ProcessEvent(
                    wxMaskedNumNumberUpdatedEvent( self.GetId(), self.GetValue(), self ) )
            except ValueError:
                dbg(indent=0)
                return
            # let normal processing of the text continue
            event.Skip()
        self._oldvalue = value # record for next event
        dbg(indent=0)

    def _GetValue(self):
        """
        Override of wxMaskedTextCtrl to allow amixin to get the raw text value of the
        control with this function.
        """
        return wx.TextCtrl.GetValue(self)


    def GetValue(self):
        """
        Returns the current numeric value of the control.
        """
        return self._fromGUI( wxMaskedTextCtrl.GetValue(self) )

    def SetValue(self, value):
        """
        Sets the value of the control to the value specified.
        The resulting actual value of the control may be altered to
        conform with the bounds set on the control if limited,
        or colored if not limited but the value is out-of-bounds.
        A ValueError exception will be raised if an invalid value
        is specified.
        """
        wxMaskedTextCtrl.SetValue( self, self._toGUI(value) )


    def SetIntegerWidth(self, value):
        self.SetCtrlParameters(integerWidth=value)
    def GetIntegerWidth(self):
        return self._integerWidth

    def SetFractionWidth(self, value):
        self.SetCtrlParameters(fractionWidth=value)
    def GetFractionWidth(self):
        return self._fractionWidth



    def SetMin(self, min=None):
        """
        Sets the minimum value of the control.  If a value of None
        is provided, then the control will have no explicit minimum value.
        If the value specified is greater than the current maximum value,
        then the function returns False and the minimum will not change from
        its current setting.  On success, the function returns True.

        If successful and the current value is lower than the new lower
        bound, if the control is limited, the value will be automatically
        adjusted to the new minimum value; if not limited, the value in the
        control will be colored as invalid.

        If min > the max value allowed by the width of the control,
        the function will return False, and the min will not be set.
        """
        dbg('wxMaskedNumCtrl::SetMin(%s)' % repr(min), indent=1)
        if( self._max is None
            or min is None
            or (self._max is not None and self._max >= min) ):
            try:
                self.SetParameters(min=min)
                bRet = True
            except ValueError:
                bRet = False
        else:
            bRet = False
        dbg(indent=0)
        return bRet

    def GetMin(self):
        """
        Gets the lower bound value of the control.  It will return
        None if not specified.
        """
        return self._min


    def SetMax(self, max=None):
        """
        Sets the maximum value of the control. If a value of None
        is provided, then the control will have no explicit maximum value.
        If the value specified is less than the current minimum value, then
        the function returns False and the maximum will not change from its
        current setting. On success, the function returns True.

        If successful and the current value is greater than the new upper
        bound, if the control is limited the value will be automatically
        adjusted to this maximum value; if not limited, the value in the
        control will be colored as invalid.

        If max > the max value allowed by the width of the control,
        the function will return False, and the max will not be set.
        """
        if( self._min is None
            or max is None
            or (self._min is not None and self._min <= max) ):
            try:
                self.SetParameters(max=max)
                bRet = True
            except ValueError:
                bRet = False
        else:
            bRet = False

        return bRet


    def GetMax(self):
        """
        Gets the maximum value of the control.  It will return the current
        maximum integer, or None if not specified.
        """
        return self._max


    def SetBounds(self, min=None, max=None):
        """
        This function is a convenience function for setting the min and max
        values at the same time.  The function only applies the maximum bound
        if setting the minimum bound is successful, and returns True
        only if both operations succeed.
        NOTE: leaving out an argument will remove the corresponding bound.
        """
        ret = self.SetMin(min)
        return ret and self.SetMax(max)


    def GetBounds(self):
        """
        This function returns a two-tuple (min,max), indicating the
        current bounds of the control.  Each value can be None if
        that bound is not set.
        """
        return (self._min, self._max)


    def SetLimited(self, limited):
        """
        If called with a value of True, this function will cause the control
        to limit the value to fall within the bounds currently specified.
        If the control's value currently exceeds the bounds, it will then
        be limited accordingly.

        If called with a value of False, this function will disable value
        limiting, but coloring of out-of-bounds values will still take
        place if bounds have been set for the control.
        """
        self.SetParameters(limited = limited)


    def IsLimited(self):
        """
        Returns True if the control is currently limiting the
        value to fall within the current bounds.
        """
        return self._limited

    def GetLimited(self):
        """ (For regularization of property accessors) """
        return self.IsLimited


    def IsInBounds(self, value=None):
        """
        Returns True if no value is specified and the current value
        of the control falls within the current bounds.  This function can
        also be called with a value to see if that value would fall within
        the current bounds of the given control.
        """
        dbg('IsInBounds(%s)' % repr(value), indent=1)
        if value is None:
            value = self.GetValue()
        else:
            try:
                value = self._GetNumValue(self._toGUI(value))
            except ValueError, e:
                dbg('error getting NumValue(self._toGUI(value)):', e, indent=0)
                return False
            if value == '':
                value = None
            elif self._fractionWidth:
                value = float(value)
            else:
                value = long(value)

        min = self.GetMin()
        max = self.GetMax()
        if min is None: min = value
        if max is None: max = value

        # if bounds set, and value is None, return False
        if value == None and (min is not None or max is not None):
            dbg('finished IsInBounds', indent=0)
            return 0
        else:
            dbg('finished IsInBounds', indent=0)
            return min <= value <= max


    def SetAllowNone(self, allow_none):
        """
        Change the behavior of the validation code, allowing control
        to have a value of None or not, as appropriate.  If the value
        of the control is currently None, and allow_none is False, the
        value of the control will be set to the minimum value of the
        control, or 0 if no lower bound is set.
        """
        self._allowNone = allow_none
        if not allow_none and self.GetValue() is None:
            min = self.GetMin()
            if min is not None: self.SetValue(min)
            else:               self.SetValue(0)


    def IsNoneAllowed(self):
        return self._allowNone
    def GetAllowNone(self):
        """ (For regularization of property accessors) """
        return self.IsNoneAllowed()

    def SetAllowNegative(self, value):
        self.SetParameters(allowNegative=value)
    def IsNegativeAllowed(self):
        return self._allowNegative
    def GetAllowNegative(self):
        """ (For regularization of property accessors) """
        return self.IsNegativeAllowed()

    def SetGroupDigits(self, value):
        self.SetParameters(groupDigits=value)
    def IsGroupingAllowed(self):
        return self._groupDigits
    def GetGroupDigits(self):
        """ (For regularization of property accessors) """
        return self.IsGroupingAllowed()

    def SetGroupChar(self, value):
        self.SetParameters(groupChar=value)
    def GetGroupChar(self):
        return self._groupChar

    def SetDecimalChar(self, value):
        self.SetParameters(decimalChar=value)
    def GetDecimalChar(self):
        return self._decimalChar

    def SetSelectOnEntry(self, value):
        self.SetParameters(selectOnEntry=value)
    def GetSelectOnEntry(self):
        return self._selectOnEntry

    # (Other parameter accessors are inherited from base class)


    def _toGUI( self, value, apply_limits = True ):
        """
        Conversion function used to set the value of the control; does
        type and bounds checking and raises ValueError if argument is
        not a valid value.
        """
        dbg('wxMaskedNumCtrl::_toGUI(%s)' % repr(value), indent=1)
        if value is None and self.IsNoneAllowed():
            dbg(indent=0)
            return self._template

        elif type(value) in (types.StringType, types.UnicodeType):
            value = self._GetNumValue(value)
            dbg('cleansed num value: "%s"' % value)
            try:
                if self._fractionWidth or value.find('.') != -1:
                    value = float(value)
                else:
                    value = long(value)
            except Exception, e:
                dbg('exception raised:', e, indent=0)
                raise ValueError ('wxMaskedNumCtrl requires numeric value, passed %s'% repr(value) )

        elif type(value) not in (types.IntType, types.LongType, types.FloatType):
            dbg(indent=0)
            raise ValueError (
                'wxMaskedNumCtrl requires numeric value, passed %s'% repr(value) )

        if not self._allowNegative and value < 0:
            raise ValueError (
                'control configured to disallow negative values, passed %s'% repr(value) )

        if self.IsLimited() and apply_limits:
            min = self.GetMin()
            max = self.GetMax()
            if not min is None and value < min:
                dbg(indent=0)
                raise ValueError (
                    'value %d is below minimum value of control'% value )
            if not max is None and value > max:
                dbg(indent=0)
                raise ValueError (
                    'value %d exceeds value of control'% value )

        adjustwidth = len(self._mask) - (1 * self._useParens * self._signOk)
        dbg('len(%s):' % self._mask, len(self._mask))
        dbg('adjustwidth - groupSpace:', adjustwidth - self._groupSpace)
        dbg('adjustwidth:', adjustwidth)
        if self._fractionWidth == 0:
            s = str(long(value)).rjust(self._integerWidth)
        else:
            format = '%' + '%d.%df' % (self._integerWidth+self._fractionWidth+1, self._fractionWidth)
            s = format % float(value)
        dbg('s:"%s"' % s, 'len(s):', len(s))
        if len(s) > (adjustwidth - self._groupSpace):
            dbg(indent=0)
            raise ValueError ('value %s exceeds the integer width of the control (%d)' % (s, self._integerWidth))
        elif s[0] not in ('-', ' ') and self._allowNegative and len(s) == (adjustwidth - self._groupSpace):
            dbg(indent=0)
            raise ValueError ('value %s exceeds the integer width of the control (%d)' % (s, self._integerWidth))

        s = s.rjust(adjustwidth).replace('.', self._decimalChar)
        if self._signOk and self._useParens:
            if s.find('-') != -1:
                s = s.replace('-', '(') + ')'
            else:
                s += ' '
        dbg('returned: "%s"' % s, indent=0)
        return s


    def _fromGUI( self, value ):
        """
        Conversion function used in getting the value of the control.
        """
        dbg(suspend=0)
        dbg('wxMaskedNumCtrl::_fromGUI(%s)' % value, indent=1)
        # One or more of the underlying text control implementations
        # issue an intermediate EVT_TEXT when replacing the control's
        # value, where the intermediate value is an empty string.
        # So, to ensure consistency and to prevent spurious ValueErrors,
        # we make the following test, and react accordingly:
        #
        if value == '':
            if not self.IsNoneAllowed():
                dbg('empty value; not allowed,returning 0', indent = 0)
                if self._fractionWidth:
                    return 0.0
                else:
                    return 0
            else:
                dbg('empty value; returning None', indent = 0)
                return None
        else:
            value = self._GetNumValue(value)
            dbg('Num value: "%s"' % value)
            if self._fractionWidth:
                try:
                    dbg(indent=0)
                    return float( value )
                except ValueError:
                    dbg("couldn't convert to float; returning None")
                    return None
                else:
                    raise
            else:
                try:
                    dbg(indent=0)
                    return int( value )
                except ValueError:
                    try:
                       dbg(indent=0)
                       return long( value )
                    except ValueError:
                       dbg("couldn't convert to long; returning None")
                       return None

                    else:
                       raise
                else:
                    dbg('exception occurred; returning None')
                    return None


    def _Paste( self, value=None, raise_on_invalid=False, just_return_value=False ):
        """
        Preprocessor for base control paste; if value needs to be right-justified
        to fit in control, do so prior to paste:
        """
        dbg('wxMaskedNumCtrl::_Paste (value = "%s")' % value)
        if value is None:
            paste_text = self._getClipboardContents()
        else:
            paste_text = value

        # treat paste as "replace number", if appropriate:
        sel_start, sel_to = self._GetSelection()
        if sel_start == sel_to or self._selectOnEntry and (sel_start, sel_to) == self._fields[0]._extent:
            paste_text = self._toGUI(paste_text)
            self._SetSelection(0, len(self._mask))

        return wxMaskedEditMixin._Paste(self,
                                        paste_text,
                                        raise_on_invalid=raise_on_invalid,
                                        just_return_value=just_return_value)



#===========================================================================

if __name__ == '__main__':

    import traceback

    class myDialog(wx.Dialog):
        def __init__(self, parent, id, title,
            pos = wx.DefaultPosition, size = wx.DefaultSize,
            style = wx.DEFAULT_DIALOG_STYLE ):
            wx.Dialog.__init__(self, parent, id, title, pos, size, style)

            self.int_ctrl = wxMaskedNumCtrl(self, wx.NewId(), size=(55,20))
            self.OK = wx.Button( self, wx.ID_OK, "OK")
            self.Cancel = wx.Button( self, wx.ID_CANCEL, "Cancel")

            vs = wx.BoxSizer( wx.VERTICAL )
            vs.Add( self.int_ctrl, 0, wx.ALIGN_CENTRE|wx.ALL, 5 )
            hs = wx.BoxSizer( wx.HORIZONTAL )
            hs.Add( self.OK, 0, wx.ALIGN_CENTRE|wx.ALL, 5 )
            hs.Add( self.Cancel, 0, wx.ALIGN_CENTRE|wx.ALL, 5 )
            vs.Add(hs, 0, wx.ALIGN_CENTRE|wx.ALL, 5 )

            self.SetAutoLayout( True )
            self.SetSizer( vs )
            vs.Fit( self )
            vs.SetSizeHints( self )
            self.Bind(EVT_MASKEDNUM, self.OnChange, self.int_ctrl)

        def OnChange(self, event):
            print 'value now', event.GetValue()

    class TestApp(wx.App):
        def OnInit(self):
            try:
                self.frame = wx.Frame(None, -1, "Test", (20,20), (120,100)  )
                self.panel = wx.Panel(self.frame, -1)
                button = wx.Button(self.panel, -1, "Push Me", (20, 20))
                self.Bind(wx.EVT_BUTTON, self.OnClick, button)
            except:
                traceback.print_exc()
                return False
            return True

        def OnClick(self, event):
            dlg = myDialog(self.panel, -1, "test wxMaskedNumCtrl")
            dlg.int_ctrl.SetValue(501)
            dlg.int_ctrl.SetInsertionPoint(1)
            dlg.int_ctrl.SetSelection(1,2)
            rc = dlg.ShowModal()
            print 'final value', dlg.int_ctrl.GetValue()
            del dlg
            self.frame.Destroy()

        def Show(self):
            self.frame.Show(True)

    try:
        app = TestApp(0)
        app.Show()
        app.MainLoop()
    except:
        traceback.print_exc()

i=0
## To-Do's:
## =============================##
##   1. Add support for printf-style format specification.
##   2. Add option for repositioning on 'illegal' insertion point.
