#ifdef __WXMAC_CLASSIC__
#  error "Native wxDataViewCtrl for classic environment not defined. Please use generic control."
#else
#  include "wx/mac/carbon/dataview.h"
#endif
