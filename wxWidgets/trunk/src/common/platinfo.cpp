///////////////////////////////////////////////////////////////////////////////
// Name:        src/common/platinfo.cpp
// Purpose:     implements wxPlatformInfo class
// Author:      Francesco Montorsi
// Modified by:
// Created:     07.07.2006 (based on wxToolkitInfo)
// RCS-ID:      $Id$
// Copyright:   (c) 2006 Francesco Montorsi
// License:     wxWindows license
///////////////////////////////////////////////////////////////////////////////

// ============================================================================
// declarations
// ============================================================================

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

// for compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
    #include "wx/utils.h"
#endif //WX_PRECOMP

#include "wx/platinfo.h"
#include "wx/apptrait.h"

// ----------------------------------------------------------------------------
// constants
// ----------------------------------------------------------------------------

static wxString wxOperatingSystemIdNames[] =
{
    _T("Apple Mac OS"),
    _T("Apple Mac OS X"),

    _T("Microsoft Windows 9X"),
    _T("Microsoft Windows NT"),
    _T("Microsoft Windows Micro"),
    _T("Microsoft Windows CE"),

    _T("Linux"),
    _T("FreeBSD"),
    _T("OpenBSD"),
    _T("NetBSD"),

    _T("SunOS"),
    _T("AIX"),
    _T("HPUX"),

    _T("DOS"),
    _T("OS/2")
};

static wxString wxPortIdNames[] =
{
    _T("wxBase"),
    _T("wxMSW"),
    _T("wxMotif"),
    _T("wxGTK"),
    _T("wxMGL"),
    _T("wxX11"),
    _T("wxOS2"),
    _T("wxMac"),
    _T("wxCocoa"),
    _T("wxWinCE"),
    _T("wxPalmOS")
};

static wxString wxArchitectureNames[] =
{
    _T("32 bit"),
    _T("64 bit")
};

static wxString wxEndiannessNames[] =
{
    _T("Big endian"),
    _T("Little endian"),
    _T("PDP endian")
};

// ----------------------------------------------------------------------------
// local functions
// ----------------------------------------------------------------------------

// returns log in base 2 of the value, this maps the enum values to the
// corresponding indices
static int wxGetIndexFromEnumValue(int value)
{
    wxCHECK_MSG( value, -1, _T("invalid enum value") );

    int n = 0;
    while ( !(value & 1) )
    {
        value >>= 1;
        n++;
    }

    wxASSERT_MSG( value == 1, _T("more than one bit set in enum value") );

    return n;
}

// ----------------------------------------------------------------------------
// wxPlatformInfo
// ----------------------------------------------------------------------------

wxPlatformInfo::wxPlatformInfo()
{
    // autodetect all informations
    const wxAppTraits * const traits = wxTheApp ? wxTheApp->GetTraits() : NULL;
    if ( !traits )
    {
        wxFAIL_MSG( _T("failed to initialize wxPlatformInfo") );

        m_port = wxPORT_UNKNOWN;
        m_tkVersionMajor =
        m_tkVersionMinor = 0;
    }
    else
    {
        m_port = traits->GetToolkitVersion(&m_tkVersionMajor, &m_tkVersionMinor);
    }

    m_os = wxGetOsVersion(&m_osVersionMajor, &m_osVersionMinor);
    m_endian = wxIsPlatformLittleEndian() ? wxENDIAN_LITTLE : wxENDIAN_BIG;
    m_arch = wxIsPlatform64Bit() ? wxARCH_64 : wxARCH_32;
}

wxPlatformInfo::wxPlatformInfo(wxPortId pid, int tkMajor, int tkMinor,
                               wxOperatingSystemId id, int osMajor, int osMinor,
                               wxArchitecture arch,
                               wxEndianness endian)
{
    m_tkVersionMajor = tkMajor;
    m_tkVersionMinor = tkMinor;
    m_port = pid;

    m_os = id;
    m_osVersionMajor = osMajor;
    m_osVersionMinor = osMinor;

    m_endian = endian;
    m_arch = arch;
}

bool wxPlatformInfo::operator==(const wxPlatformInfo &t) const
{
    return m_tkVersionMajor == t.m_tkVersionMajor &&
           m_tkVersionMinor == t.m_tkVersionMinor &&
           m_osVersionMajor == t.m_osVersionMajor &&
           m_osVersionMinor == t.m_osVersionMinor &&
           m_os == t.m_os &&
           m_port == t.m_port &&
           m_arch == t.m_arch &&
           m_endian == t.m_endian;
}

// ----------------------------------------------------------------------------
// wxPlatformInfo - enum -> string conversions
// ----------------------------------------------------------------------------

wxString wxPlatformInfo::GetOperatingSystemFamilyName(wxOperatingSystemId os)
{
    if ( os & wxOS_MAC )
        return _T("Macintosh");
    else if ( os & wxOS_WINDOWS )
        return _T("Windows");
    else if ( os & wxOS_UNIX )
        return _T("Unix");
    else if ( os == wxOS_DOS )
        return _T("DOS");
    else if ( os == wxOS_OS2 )
        return _T("OS/2");

    return _T("Unknown");
}

wxString wxPlatformInfo::GetOperatingSystemIdName(wxOperatingSystemId os)
{
    const int idx = wxGetIndexFromEnumValue(os);

    wxCHECK_MSG( idx < WXSIZEOF(wxOperatingSystemIdNames), wxEmptyString,
                 _T("invalid OS id") );

    return wxOperatingSystemIdNames[idx];
}

wxString wxPlatformInfo::GetPortIdName(wxPortId port)
{
    const int idx = wxGetIndexFromEnumValue(port);

    wxCHECK_MSG( idx < WXSIZEOF(wxPortIdNames), wxEmptyString,
                 _T("invalid port id") );

    wxString ret = wxPortIdNames[idx];

    if ( IsUsingUniversalWidgets() )
        ret += wxT("/wxUniversal");

    return ret;
}

wxString wxPlatformInfo::GetPortIdShortName(wxPortId port)
{
    const int idx = wxGetIndexFromEnumValue(port);

    wxCHECK_MSG( idx < WXSIZEOF(wxPortIdNames), wxEmptyString,
                 _T("invalid port id") );

    wxString ret = wxPortIdNames[idx];
    ret = ret.Mid(2).Lower();       // remove 'wx' prefix

    if ( IsUsingUniversalWidgets() )
        ret += wxT("univ");

    return ret;
}

wxString wxPlatformInfo::GetArchName(wxArchitecture arch)
{
    wxCOMPILE_TIME_ASSERT( WXSIZEOF(wxArchitectureNames) == wxARCH_MAX,
                           wxArchitectureNamesMismatch );

    return wxArchitectureNames[arch];
}

wxString wxPlatformInfo::GetEndiannessName(wxEndianness end)
{
    wxCOMPILE_TIME_ASSERT( WXSIZEOF(wxEndiannessNames) == wxENDIAN_MAX,
                           wxEndiannessNamesMismatch );

    return wxEndiannessNames[end];
}


// ----------------------------------------------------------------------------
// wxPlatformInfo - string -> enum conversions
// ----------------------------------------------------------------------------

wxOperatingSystemId wxPlatformInfo::GetOperatingSystemId(const wxString &str)
{
    for ( size_t i = 0; i < WXSIZEOF(wxOperatingSystemIdNames); i++ )
    {
        if ( wxOperatingSystemIdNames[i].CmpNoCase(str) == 0 )
            return (wxOperatingSystemId)(1 << i);
    }

    return wxOS_UNKNOWN;
}

wxPortId wxPlatformInfo::GetPortId(const wxString &str)
{
    // recognize both short and long port names
    for ( size_t i = 0; i < WXSIZEOF(wxPortIdNames); i++ )
    {
        wxPortId current = (wxPortId)(1 << i);

        if ( wxPortIdNames[i].CmpNoCase(str) == 0 )
            return current;
        if ( GetPortIdShortName(current).CmpNoCase(str) == 0 )
            return current;
    }

    return wxPORT_UNKNOWN;
}

wxArchitecture wxPlatformInfo::GetArch(const wxString &arch)
{
    if ( arch.Contains(wxT("32")) )
        return wxARCH_32;

    if ( arch.Contains(wxT("64")) )
        return wxARCH_64;

    return wxARCH_INVALID;
}

wxEndianness wxPlatformInfo::GetEndianness(const wxString& end)
{
    wxString endl(end.Lower());
    if ( end.StartsWith(wxT("little")) )
        return wxENDIAN_LITTLE;

    if ( end.StartsWith(wxT("big")) )
        return wxENDIAN_BIG;

    return wxENDIAN_INVALID;
}

