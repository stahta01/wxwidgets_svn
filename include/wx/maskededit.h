/////////////////////////////////////////////////////////////////////////////
// Name:        wx/maskededit.h
// Purpose:     wxMaskedEdit interface
// Author:      Julien Weinzorn
// RCS-ID:      $Id: $
// Copyright:   (c) 2010 Julien Weinzorn
// Licence:     wxWindows license
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_MASKED_EDIT_H_
#define _WX_MASKED_EDIT_H_


#include "wx/defs.h"
#include "wx/colour.h"
#include "wx/dynarray.h"
#include "wx/maskedfield.h"


WX_DEFINE_ARRAY(wxMaskedField *, fieldsArray);


class WXDLLIMPEXP_CORE wxMaskedEdit
{
public:
    wxMaskedEdit();

    wxMaskedEdit(const wxMaskedEdit& maskedEdit);


    wxMaskedEdit( const wxString& mask
                , const wxString &formatCode   = wxDEFAULT_FORMAT_CODES
                , const wxString &defaultValue = wxDEFAULT_VALUE);


    wxMaskedEdit( const wxString& mask
                , const wxArrayString& formatCode = wxArrayString()
                , const wxString& defaultValue  = wxDEFAULT_VALUE);

    ~wxMaskedEdit();

    void Create(  const wxString& mask
                , const wxArrayString& formatCode = wxArrayString()
                , const wxString& defaultValue  = wxEmptyString);

    //Apply formatCodes on the string
    wxString ApplyFormatCodes(const wxString& string);

    //    Return the value of the wstring associated
    //    without the mask
    wxString GetPlainValue(const wxString& string);


    //Clear the mask and the cursor return in the beginning of the mask
    void ClearValue();

    //Test if the current sequence is valid
    bool IsValid(const wxString& string) const;

    bool IsCharValid(wxChar charTest, unsigned int index);

    //Test if the current sequence is empty
    bool IsEmpty(const wxString& string);

    //Set the mask to a new value.
    bool SetMask(const wxString& mask);


    wxString GetMask() const;

    wxString GetFormatCode() const;

    wxString GetDefaultValue() const;

    bool SetDefaultValue(const wxString& defaultValue);

    wxArrayString GetChoices() const;

    bool AddChoice(const wxString& choice);

    bool AddChoices(const wxArrayString& choices);

    wxString GetChoice(unsigned int index);

    wxString GetNextChoices() const;

    wxString GetCurrentChoices() const;

    wxString GetPreviousChoices() const;

    unsigned int GetNumberOfFields() const ;

    void SetEmptyBackgroundColour(const wxColour& colour);

    void SetInvalidBackgroundColour(const wxColour& colour);

    void SetValidBackgroundColour(const wxColour& colour);

    wxColour GetEmptyBackgroundColour() const;

    wxColour GetInvalidBackgroundColour() const;

    wxColour GetValidBackgroundColour() const;


    bool SetMask(unsigned int fieldIndex, wxString& mask);

    wxString GetMask(unsigned int fieldIndex) const;

    wxString GetFormatCodes(unsigned int fieldIndex) const;

    wxArrayString GetChoices(unsigned int fieldIndex) const;


    bool AddChoice(unsigned int fieldIndex, const wxString& choice);

    bool AddChoices(unsigned int fieldIndex, const wxArrayString& choices);

    wxString GetChoice(unsigned int fieldIndex, unsigned int index);

    wxString GetNextChoices(unsigned int fieldIndex) const;

    wxString GetCurrentChoices(unsigned int fieldIndex) const;

    wxString GetPreviousChoices(unsigned int fieldIndex) const;

    unsigned int NumberOfChoices();

    wxString GetEmptyMask() const;

    unsigned int GetFieldIndex(unsigned int position);

    unsigned int GetMinFieldPosition(unsigned int fieldIndex);

    unsigned int GetMaxFieldPosition(unsigned int fieldIndex);

    bool IsNumber();

    wxString GetDecimalPoint();

    void SetFillChar(wxChar newFillChar);
    wxChar GetFillChar() const;

    bool IsAutoSelect() const;
    void SetAutoSelect(bool val = true);
    wxString GetAutoSelect(const wxString& begin) const;

    bool IsAutoSelect(unsigned int fieldIndex) const;
    void SetAutoSelect(unsigned int fieldIndex, bool val = true);
    wxString GetAutoSelect(unsigned int fieldIndex, const wxString& begin) const;

    int GetNextChoiceKey() const;
    void SetNextChoiceKey(int keycode);

    int GetPreviousChoiceKey() const;
    void SetPreviousChoiceKey(int keycode);


private:
    //mask caracteristics
    fieldsArray m_mask;
    wxString m_maskValue;
    bool m_isNumber;

    int m_nextChoiceKey;
    int m_previousChoiceKey;

    //Control behavior
    wxColour m_emptyBg;
    wxColour m_invalidBg;
    wxColour m_validBg;

};

#endif // _WX_MASKED_EDIT_H_
