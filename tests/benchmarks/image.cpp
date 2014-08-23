/////////////////////////////////////////////////////////////////////////////
// Name:        tests/benchmarks/image.cpp
// Purpose:     wxImage benchmarks
// Author:      Vadim Zeitlin
// Created:     2013-06-30
// Copyright:   (c) 2013 Vadim Zeitlin <vadim@wxwidgets.org>
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#include "wx/image.h"

#include "bench.h"

BENCHMARK_FUNC(LoadBMP)
{
    wxImage image;
    return image.LoadFile("horse.bmp");
}

BENCHMARK_FUNC(LoadJPEG)
{
    static bool s_handlerAdded = false;
    if ( !s_handlerAdded )
    {
        s_handlerAdded = true;
        wxImage::AddHandler(new wxJPEGHandler);
    }

    wxImage image;
    return image.LoadFile("horse.jpg");
}

BENCHMARK_FUNC(LoadPNG)
{
    static bool s_handlerAdded = false;
    if ( !s_handlerAdded )
    {
        s_handlerAdded = true;
        wxImage::AddHandler(new wxPNGHandler);
    }

    wxImage image;
    return image.LoadFile("horse.png");
}

BENCHMARK_FUNC(LoadTIFF)
{
    static bool s_handlerAdded = false;
    if ( !s_handlerAdded )
    {
        s_handlerAdded = true;
        wxImage::AddHandler(new wxTIFFHandler);
    }

    wxImage image;
    return image.LoadFile("horse.tif");
}

static const wxImage& GetTestImage()
{
    static wxImage s_image;
    static bool s_triedToLoad = false;
    if ( !s_triedToLoad )
    {
        s_triedToLoad = true;
        s_image.LoadFile("horse.bmp");
    }

    return s_image;
}

BENCHMARK_FUNC(EnlargeNormal)
{
    return GetTestImage().Scale(300, 300, wxIMAGE_QUALITY_NORMAL).IsOk();
}

BENCHMARK_FUNC(EnlargeHighQuality)
{
    return GetTestImage().Scale(300, 300, wxIMAGE_QUALITY_HIGH).IsOk();
}

BENCHMARK_FUNC(ShrinkNormal)
{
    return GetTestImage().Scale(50, 50, wxIMAGE_QUALITY_NORMAL).IsOk();
}

BENCHMARK_FUNC(ShrinkHighQuality)
{
    return GetTestImage().Scale(50, 50, wxIMAGE_QUALITY_HIGH).IsOk();
}
