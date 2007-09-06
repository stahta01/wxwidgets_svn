/////////////////////////////////////////////////////////////////////////////
// Name:        wx/cairo.h
// Purpose:     Cairo library
// Author:      Anthony Bretaudeau
// Created:     2007-08-25
// RCS-ID:      $Id: cairo.h 47254 2007-08-25 10:09:52Z VS $
// Copyright:   (c) Anthony Bretaudeau
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_CAIRO_H_BASE_
#define _WX_CAIRO_H_BASE_

 // for now GTK+ only
#ifdef __WXGTK210__
  #define wxUSE_CAIRO  1
#endif

#if wxUSE_CAIRO

#include "wx/dynlib.h"
#include <cairo.h>


class wxCairoLibrary
{
public:
    wxCairoLibrary();
    ~wxCairoLibrary();

    static wxCairoLibrary* Get();
    static void CleanUp();

    bool IsOk();
    void InitializeMethods();

private:
    bool              m_ok;
    wxDynamicLibrary *m_cairo_lib;
    
    static wxCairoLibrary *s_lib;

public:
    wxDL_METHOD_DEFINE( void, cairo_arc,
        (cairo_t *cr, double xc, double yc, double radius, double angle1, double angle2), (cr, xc, yc, radius, angle1, angle2), /**/)
    wxDL_METHOD_DEFINE( void, cairo_arc_negative,
        (cairo_t *cr, double xc, double yc, double radius, double angle1, double angle2), (cr, xc, yc, radius, angle1, angle2), /**/)
    wxDL_METHOD_DEFINE( void, cairo_clip,
        (cairo_t *cr), (cr), /**/)
    wxDL_METHOD_DEFINE( void, cairo_close_path,
        (cairo_t *cr), (cr), /**/)
    wxDL_METHOD_DEFINE( cairo_t*, cairo_create,
        (cairo_surface_t *target), (target), NULL)
    wxDL_METHOD_DEFINE( void, cairo_curve_to,
        (cairo_t *cr, double x1, double y1, double x2, double y2, double x3, double y3), (cr, x1, y1, x2, y2, x3, y3), /**/)
    wxDL_METHOD_DEFINE( void, cairo_destroy,
        (cairo_t *cr), (cr), /**/)
    wxDL_METHOD_DEFINE( void, cairo_fill,
        (cairo_t *cr), (cr), /**/)
    wxDL_METHOD_DEFINE( void, cairo_fill_preserve,
        (cairo_t *cr), (cr), /**/)
    wxDL_METHOD_DEFINE( cairo_surface_t*, cairo_get_target,
        (cairo_t *cr), (cr), NULL)
    wxDL_METHOD_DEFINE( cairo_surface_t*, cairo_image_surface_create_for_data,
        (unsigned char *data, cairo_format_t format, int width, int height, int stride), (data, format, width, height, stride), NULL)
    wxDL_METHOD_DEFINE( void, cairo_line_to,
        (cairo_t *cr, double x, double y), (cr, x, y), /**/)
    wxDL_METHOD_DEFINE( void, cairo_move_to,
        (cairo_t *cr, double x, double y), (cr, x, y), /**/)
    wxDL_METHOD_DEFINE( void, cairo_new_path,
        (cairo_t *cr), (cr), /**/)
    wxDL_METHOD_DEFINE( void, cairo_paint,
        (cairo_t *cr), (cr), /**/)
    wxDL_METHOD_DEFINE( void, cairo_pattern_add_color_stop_rgba,
        (cairo_pattern_t *pattern, double offset, double red, double green, double blue, double alpha), (pattern, offset, red, green, blue, alpha), /**/)
    wxDL_METHOD_DEFINE( cairo_pattern_t*, cairo_pattern_create_for_surface,
        (cairo_surface_t *surface), (surface), NULL)
    wxDL_METHOD_DEFINE( cairo_pattern_t*, cairo_pattern_create_linear,
        (double x0, double y0, double x1, double y1), (x0, y0, x1, y1), NULL)
    wxDL_METHOD_DEFINE( cairo_pattern_t*, cairo_pattern_create_radial,
        (double cx0, double cy0, double radius0, double cx1, double cy1, double radius1), (cx0, cy0, radius0, cx1, cy1, radius1), NULL)
    wxDL_METHOD_DEFINE( void, cairo_pattern_destroy,
        (cairo_pattern_t *pattern), (pattern), /**/)
    wxDL_METHOD_DEFINE( void, cairo_pattern_set_extend,
        (cairo_pattern_t *pattern, cairo_extend_t extend), (pattern, extend), /**/)
    wxDL_METHOD_DEFINE( void, cairo_pattern_set_filter,
        (cairo_pattern_t *pattern, cairo_filter_t filter), (pattern, filter), /**/)
    wxDL_METHOD_DEFINE( void, cairo_rectangle,
        (cairo_t *cr, double x, double y, double width, double height), (cr, x, y, width, height), /**/)
    wxDL_METHOD_DEFINE( void, cairo_reset_clip,
        (cairo_t *cr), (cr), /**/)
    wxDL_METHOD_DEFINE( void, cairo_restore,
        (cairo_t *cr), (cr), /**/)
    wxDL_METHOD_DEFINE( void, cairo_rotate,
        (cairo_t *cr, double angle), (cr, angle), /**/)
    wxDL_METHOD_DEFINE( void, cairo_save,
        (cairo_t *cr), (cr), /**/)
    wxDL_METHOD_DEFINE( void, cairo_scale,
        (cairo_t *cr, double sx, double sy), (cr, sx, sy), /**/)
    wxDL_METHOD_DEFINE( void, cairo_set_dash,
        (cairo_t *cr, const double *dashes, int num_dashes, double offset), (cr, dashes, num_dashes, offset), /**/)
    wxDL_METHOD_DEFINE( void, cairo_set_fill_rule,
        (cairo_t *cr, cairo_fill_rule_t fill_rule), (cr, fill_rule), /**/)
    wxDL_METHOD_DEFINE( void, cairo_set_line_cap,
        (cairo_t *cr, cairo_line_cap_t line_cap), (cr, line_cap), /**/)
    wxDL_METHOD_DEFINE( void, cairo_set_line_join,
        (cairo_t *cr, cairo_line_join_t line_join), (cr, line_join), /**/)
    wxDL_METHOD_DEFINE( void, cairo_set_line_width,
        (cairo_t *cr, double width), (cr, width), /**/)
    wxDL_METHOD_DEFINE( void, cairo_set_operator,
        (cairo_t *cr, cairo_operator_t op), (cr, op), /**/)
    wxDL_METHOD_DEFINE( void, cairo_set_source,
        (cairo_t *cr, cairo_pattern_t *source), (cr, source), /**/)
    wxDL_METHOD_DEFINE( void, cairo_set_source_rgba,
        (cairo_t *cr, double red, double green, double blue, double alpha), (cr, red, green, blue, alpha), /**/)
    wxDL_METHOD_DEFINE( void, cairo_stroke,
        (cairo_t *cr), (cr), /**/)
    wxDL_METHOD_DEFINE( void, cairo_stroke_preserve,
        (cairo_t *cr), (cr), /**/)
    wxDL_METHOD_DEFINE( cairo_surface_t*, cairo_surface_create_similar,
        (cairo_surface_t *other, cairo_content_t content, int width, int height), (other, content, width, height), NULL)
    wxDL_METHOD_DEFINE( void, cairo_surface_destroy,
        (cairo_surface_t *surface), (surface), /**/)
    wxDL_METHOD_DEFINE( void, cairo_translate,
        (cairo_t *cr, double tx, double ty), (cr, tx, ty), /**/)

    DECLARE_NO_COPY_CLASS(wxCairoLibrary)
};

#endif
 // wxUSE_CAIRO

#endif
  // _WX_CAIRO_H_BASE_
