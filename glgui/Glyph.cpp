/*
 * Copyright 2015 Daniel Kirchner
 *
 * This file is part of glgui.
 *
 * glgui is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * glgui is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with glgui.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "Glyph.h"
#include "Font.h"
#include <glyphy.h>
#include <glyphy-freetype.h>

namespace glgui {

template<typename T = double>
constexpr T sqrt2 (void) { return std::sqrt (2.0); }

Glyph::Glyph (Font *font, int index) : pixelsize (font->GetSize ()) {
    constexpr double tolerance_per_em = 1.0 / 1024.0;
    constexpr double MIN_FONT_SIZE = 10.0;

    if (FT_Load_Glyph (font->GetFace (), index,
                       FT_LOAD_NO_BITMAP|FT_LOAD_NO_HINTING|FT_LOAD_NO_AUTOHINT|FT_LOAD_NO_SCALE
                       |FT_LOAD_LINEAR_DESIGN|FT_LOAD_IGNORE_TRANSFORM) != FT_Err_Ok)
        throw std::runtime_error ("cannot load freetype glyph");
    if (font->GetFace ()->glyph->format != FT_GLYPH_FORMAT_OUTLINE)
        throw std::runtime_error ("loaded FreeType glyph is not an outline");

    unsigned int upem = font->GetFace()->units_per_EM;
    double tolerance = upem * tolerance_per_em; /* in font design units */
    double faraway = double (upem) / (MIN_FONT_SIZE * sqrt2 ());
    std::vector<glyphy_arc_endpoint_t> endpoints;

    glyphy_arc_accumulator_t *acc = font->GetArcAccumulator ();

    glyphy_arc_accumulator_reset (acc);
    glyphy_arc_accumulator_set_tolerance (acc, tolerance);
    glyphy_arc_accumulator_set_callback (acc, [] (glyphy_arc_endpoint_t *endpoint, void *user_data) -> glyphy_bool_t {
        static_cast<std::vector<glyphy_arc_endpoint_t>*> (user_data)->push_back (*endpoint);
        return true;
    }, &endpoints);

    if (FT_Err_Ok != glyphy_freetype(outline_decompose) (&font->GetFace ()->glyph->outline, acc))
        throw std::runtime_error ("Failed converting glyph outline to arcs");

    assert (glyphy_arc_accumulator_get_error (acc) <= tolerance);

    if (endpoints.size ())
    {
#if 0
        /* Technically speaking, we want the following code,
     * however, crappy fonts have crappy flags.  So we just
     * fixup unconditionally... */
    if (face->glyph->outline.flags & FT_OUTLINE_EVEN_ODD_FILL)
      glyphy_outline_winding_from_even_odd (&endpoints[0], endpoints.size (), false);
    else if (face->glyph->outline.flags & FT_OUTLINE_REVERSE_FILL)
      glyphy_outline_reverse (&endpoints[0], endpoints.size ());
#else
        glyphy_outline_winding_from_even_odd (&endpoints[0], endpoints.size (), false);
#endif
    }

    glyphy_rgba_t buffer[4096 * 16];
    unsigned int output_len;

    double avg_fetch_achieved;
    if (!glyphy_arc_list_encode_blob (endpoints.size () ? &endpoints[0] : NULL, endpoints.size (),
                                      buffer, sizeof (buffer) / sizeof (buffer[0]),
                                      faraway, 4, /* UNUSED */ &avg_fetch_achieved,
                                      &output_len, &nominal_w, &nominal_h, &extents))
        throw std::runtime_error ("Failed encoding arcs");

    glyphy_extents_scale (&extents, 1. / upem, 1. / upem);

    advance = font->GetFace ()->glyph->metrics.horiAdvance / (double) upem;

    is_empty = glyphy_extents_is_empty (&extents);
    if (!is_empty) {
        atlas_pos = font->GetAtlas ().alloc (buffer, output_len);
    }
}

Glyph::~Glyph (void) {
}

} /* namespace glgui */
