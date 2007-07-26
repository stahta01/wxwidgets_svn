/////////////////////////////////////////////////////////////////////////////
// Program:     wxWidgets Widgets Sample
// Name:        itemcontainer.h
// Purpose:     Part of the widgets sample showing wxComboBox
// Created:     20.07.07
// Id:          $Id$
// License:     wxWindows license
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_SAMPLE_WIDGETS_ITEMCONTAINER_H_
#define _WX_SAMPLE_WIDGETS_ITEMCONTAINER_H_

// for compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#include "widgets.h"

class ItemContainerWidgetsPage : public WidgetsPage
{
public:
    ItemContainerWidgetsPage(WidgetsBookCtrl *book,
                             wxImageList *image_list,
                             char* icon[]);
    virtual ~ItemContainerWidgetsPage();

    void OnButtonTestItemContainer(wxCommandEvent& event);

    virtual wxItemContainer* GetContainer() const = 0;

private:
    void StartTest(const wxString& label);
    void EndTest(const wxArrayString& result);

    // Track client data in wxItemContainer instances
    wxClientData* CreateClientData(int value);
    void StartTrackingData();
    void StopTrackingData();
    friend class TrackedClientData;

    bool VerifyAllClientDataDestroyed();
    bool VerifyClientData(wxUIntPtr i, const wxString& str);

    wxString DumpContainerData(const wxArrayString& expected) const;

    wxArrayString       m_items;
    wxSortedArrayString m_itemsSorted;
    int m_trackedDataObjects;

    // holds pointer to the widget being tested
    wxItemContainer *m_container;
};

#endif // _WX_SAMPLE_WIDGETS_ITEMCONTAINER_H_
