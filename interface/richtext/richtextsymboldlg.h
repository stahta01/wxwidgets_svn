/////////////////////////////////////////////////////////////////////////////
// Name:        richtext/richtextsymboldlg.h
// Purpose:     documentation for wxSymbolPickerDialog class
// Author:      wxWidgets team
// RCS-ID:      $Id$
// Licence:     wxWindows license
/////////////////////////////////////////////////////////////////////////////

/**
    @class wxSymbolPickerDialog
    @headerfile richtextsymboldlg.h wx/richtext/richtextsymboldlg.h

    wxSymbolPickerDialog presents the user with a choice of fonts and a grid
    of available characters. This modal dialog provides the application with
    a selected symbol and optional font selection.

    Although this dialog is contained in the rich text library, the dialog
    is generic and can be used in other contexts.

    To use the dialog, pass a default symbol specified as a string, an initial font
    name,
    and a current font name. The difference between the initial font and
    current font is that the initial font determines what the font control will be
    set to when the dialog shows - an empty string will show the selection @e
    normal text.
    The current font, on the other hand, is used by the dialog to determine what
    font
    to display the characters in, even when no initial font is selected.
    This allows the user (and application) to distinguish between inserting a
    symbol in the current font, and inserting it with a specified font.

    When the dialog is dismissed, the application can get the selected symbol
    with GetSymbol and test whether a font was specified with UseNormalFont,
    fetching the specified font with GetFontName.

    Here's a realistic example, inserting the supplied symbol into a
    rich text control in either the current font or specified font.

    @code
    wxRichTextCtrl* ctrl = (wxRichTextCtrl*) FindWindow(ID_RICHTEXT_CTRL);

        wxTextAttr attr;
        attr.SetFlags(wxTEXT_ATTR_FONT);
        ctrl-GetStyle(ctrl-GetInsertionPoint(), attr);

        wxString currentFontName;
        if (attr.HasFont() && attr.GetFont().Ok())
            currentFontName = attr.GetFont().GetFaceName();

        // Don't set the initial font in the dialog (so the user is choosing
        // 'normal text', i.e. the current font) but do tell the dialog
        // what 'normal text' is.

        wxSymbolPickerDialog dlg(wxT("*"), wxEmptyString, currentFontName, this);

        if (dlg.ShowModal() == wxID_OK)
        {
            if (dlg.HasSelection())
            {
                long insertionPoint = ctrl-GetInsertionPoint();

                ctrl-WriteText(dlg.GetSymbol());

                if (!dlg.UseNormalFont())
                {
                    wxFont font(attr.GetFont());
                    font.SetFaceName(dlg.GetFontName());
                    attr.SetFont(font);
                    ctrl-SetStyle(insertionPoint, insertionPoint+1, attr);
                }
            }
        }
    @endcode

    @library{wxrichtext}
    @category{cmndlg}
*/
class wxSymbolPickerDialog : public wxDialog
{
public:
    //@{
    /**
        Constructors.
        
        @param symbol
            The initial symbol to show. Specify a single character in a string, or an
        empty string.
        @param initialFont
            The initial font to be displayed in the font list. If empty, the item
        normal text will be selected.
        @param normalTextFont
            The font the dialog will use to display the symbols if the initial font is
        empty.
        @param parent
            The dialog's parent.
        @param id
            The dialog's identifier.
        @param title
            The dialog's caption.
        @param pos
            The dialog's position.
        @param size
            The dialog's size.
        @param style
            The dialog's window style.
    */
    wxSymbolPickerDialog(const wxString& symbol,
                         const wxString& initialFont,
                         const wxString& normalTextFont,
                         wxWindow* parent,
                         wxWindowID id = wxID_ANY);
    const wxSize& size = wxDefaultSize, long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER|wxCLOSE_BOX)
    wxSymbolPickerDialog();
    //@}

    /**
        ,  wxPoint&@e pos = wxDefaultPosition,  wxSize&@e size = wxDefaultSize, @b
        long@e style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER|wxCLOSE_BOX)
        Creation: see @ref wxsymbolpickerdialog() "the constructor" for details about
        the parameters.
    */
    bool Create(const wxString& symbol, const wxString& initialFont,
                const wxString& normalTextFont,
                wxWindow* parent,
                wxWindowID id = wxID_ANY) const;

    /**
        Returns the font name (the font reflected in the font list).
    */
    wxString GetFontName() const;

    /**
        Returns @true if the dialog is showing the full range of Unicode characters.
    */
    bool GetFromUnicode() const;

    /**
        Gets the font name used for displaying symbols in the absence of a selected
        font.
    */
    wxString GetNormalTextFontName() const;

    /**
        Gets the current or initial symbol as a string.
    */
    wxString GetSymbol() const;

    /**
        Gets the selected symbol character as an integer.
    */
    int GetSymbolChar() const;

    /**
        Returns @true if a symbol is selected.
    */
    bool HasSelection() const;

    /**
        Sets the initial/selected font name.
    */
    void SetFontName(const wxString& value);

    /**
        Sets the internal flag indicating that the full Unicode range should be
        displayed.
    */
    void SetFromUnicode(bool value);

    /**
        Sets the name of the font to be used in the absence of a selected font.
    */
    void SetNormalTextFontName(const wxString& value);

    /**
        Sets the symbol as a one or zero character string.
    */
    void SetSymbol(const wxString& value);

    /**
        Sets Unicode display mode.
    */
    void SetUnicodeMode(bool unicodeMode);

    /**
        Returns @true if the has specified normal text - that is, there is no selected
        font.
    */
    bool UseNormalFont() const;
};
