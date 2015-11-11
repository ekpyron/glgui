/*
 * Copyright (c) 2015 Daniel Kirchner
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
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
