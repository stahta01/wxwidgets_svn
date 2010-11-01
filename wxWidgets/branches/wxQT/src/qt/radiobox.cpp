/////////////////////////////////////////////////////////////////////////////
// Name:        src/qt/radiobox.cpp
// Author:      Peter Most
// Id:          $Id$
// Copyright:   (c) Peter Most
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#include "wx/radiobox.h"
#include "wx/qt/utils.h"
#include "wx/qt/converter.h"

IMPLEMENT_DYNAMIC_CLASS( wxRadioBox, wxControl )

wxRadioBox::wxRadioBox()
{
}

wxRadioBox::wxRadioBox(wxWindow *parent,
           wxWindowID id,
           const wxString& title,
           const wxPoint& pos,
           const wxSize& size,
           int n, const wxString choices[],
           int majorDim,
           long style,
           const wxValidator& val,
           const wxString& name)
{
    Create( parent, id, title, pos, size, n, choices, majorDim, style, val, name );
}

wxRadioBox::wxRadioBox(wxWindow *parent,
           wxWindowID id,
           const wxString& title,
           const wxPoint& pos,
           const wxSize& size,
           const wxArrayString& choices,
           int majorDim,
           long style,
           const wxValidator& val,
           const wxString& name)
{
    Create( parent, id, title, pos, size, choices, majorDim, style, val, name );
}



bool wxRadioBox::Create(wxWindow *parent,
            wxWindowID id,
            const wxString& title,
            const wxPoint& pos,
            const wxSize& size,
            const wxArrayString& choices,
            int majorDim,
            long style,
            const wxValidator& val,
            const wxString& name)
{
    return Create( parent, id, title, pos, size, choices.size(), &choices[ 0 ],
        majorDim, style, val, name );
}


template < typename QtButton >
static void AddChoices( QButtonGroup *qtButtonGroup, int count, const wxString choices[] )
{
    while ( count-- > 0 )
        qtButtonGroup->addButton( new QtButton( wxQtConvertString( *choices++ )));
}


bool wxRadioBox::Create(wxWindow *parent,
            wxWindowID id,
            const wxString& title,
            const wxPoint& pos,
            const wxSize& size,
            int n, const wxString choices[],
            int majorDim,
            long style,
            const wxValidator& val,
            const wxString& name)
{
    if ( !CreateControl( parent, id, pos, size, style, val, name ))
        return false;

    m_qtGroupBox = new QGroupBox( wxQtConvertString( title ), parent->GetHandle() );
    m_qtButtonGroup = new QButtonGroup( m_qtGroupBox );

    if ( style & wxRA_SPECIFY_ROWS )
        wxMISSING_IMPLEMENTATION( wxSTRINGIZE( wxRA_SPECIFY_ROWS ));
    else if ( style & wxRA_SPECIFY_COLS )
        wxMISSING_IMPLEMENTATION( wxSTRINGIZE( wxRA_SPECIFY_COLS ));

    if ( style & wxRA_USE_CHECKBOX )
        AddChoices< QCheckBox >( m_qtButtonGroup, n, choices );
    else
        AddChoices< QRadioButton >( m_qtButtonGroup, n, choices );

    return true;
}

static QAbstractButton *GetButtonAt( const QButtonGroup *group, unsigned int n )
{
    // At first glance it might look like QButtonGroup::button( id ) would be a
    // better way to retrieve the button via an index. But even without looking
    // at the implementation of it, we can assume that it probably takes a linear
    // search algorithm because we can assign arbitrary IDs.

    QList< QAbstractButton * > buttons = group->buttons();
    return n < static_cast< unsigned >( buttons.size() ) ? buttons.at( n ) : NULL;
}

#define INVALID_INDEX_MESSAGE wxT( "invalid radio box index" )

#define CHECK_BUTTON( button, rc ) \
    wxCHECK_MSG( button != NULL, rc, INVALID_INDEX_MESSAGE )

bool wxRadioBox::Enable(unsigned int n, bool enable)
{
    QAbstractButton *qtButton = GetButtonAt( m_qtButtonGroup, n );
    CHECK_BUTTON( qtButton, false );

    qtButton->setEnabled( enable );
    return true;
}

bool wxRadioBox::Show(unsigned int n, bool show)
{
    QAbstractButton *qtButton = GetButtonAt( m_qtButtonGroup, n );
    CHECK_BUTTON( qtButton, false );

    qtButton->setVisible( show );
    return true;
}

bool wxRadioBox::IsItemEnabled(unsigned int n) const
{
    QAbstractButton *qtButton = GetButtonAt( m_qtButtonGroup, n );
    CHECK_BUTTON( qtButton, false );

    return qtButton->isEnabled();
}

bool wxRadioBox::IsItemShown(unsigned int n) const
{
    QAbstractButton *qtButton = GetButtonAt( m_qtButtonGroup, n );
    CHECK_BUTTON( qtButton, false );

    return qtButton->isVisible();
}

unsigned wxRadioBox::GetCount() const
{
    QList< QAbstractButton * > buttons = m_qtButtonGroup->buttons();
    return buttons.size();
}

wxString wxRadioBox::GetString(unsigned int n) const
{
    QAbstractButton *qtButton = GetButtonAt( m_qtButtonGroup, n );
    CHECK_BUTTON( qtButton, wxEmptyString );

    return wxQtConvertString( qtButton->text() );
}

void wxRadioBox::SetString(unsigned int n, const wxString& s)
{
    QAbstractButton *qtButton = GetButtonAt( m_qtButtonGroup, n );
    wxCHECK_RET( qtButton != NULL, INVALID_INDEX_MESSAGE );

    qtButton->setText( wxQtConvertString( s ));
}

void wxRadioBox::SetSelection(int n)
{
    QAbstractButton *qtButton = GetButtonAt( m_qtButtonGroup, n );
    wxCHECK_RET( qtButton != NULL, INVALID_INDEX_MESSAGE );
    qtButton->setChecked( true );
}

int wxRadioBox::GetSelection() const
{
    QAbstractButton *qtButton = m_qtButtonGroup->checkedButton();
    if ( qtButton != NULL )
    {
        QList< QAbstractButton * > buttons = m_qtButtonGroup->buttons();
        return buttons.indexOf( qtButton );
    }
    else
        return wxNOT_FOUND;
}

QGroupBox *wxRadioBox::GetHandle() const
{
    return m_qtGroupBox;
}

