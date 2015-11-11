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

#ifndef GLGUI_GLYPH_H
#define GLGUI_GLYPH_H

#include <glyphy.h>

namespace glgui {

class Font;

class Glyph {
public:
    Glyph (Font *font, int index);
    Glyph (const Glyph&) = delete;
    ~Glyph (void);
    Glyph &operator= (const Glyph&) = delete;
    const glyphy_extents_t &GetExtents (void) const {
        return extents;
    }
    const unsigned int &GetNominalWidth (void) const {
        return nominal_w;
    }
    const unsigned int &GetNominalHeight (void) const {
        return nominal_h;
    }
    const unsigned int &GetBufferOffset (void) const {
        return atlas_pos;
    }
    const int &GetPixelSize (void) const {
        return pixelsize;
    }
    const glyphy_bool_t &IsEmpty (void) const {
        return is_empty;
    }
private:
    glyphy_extents_t extents;
    double           advance;
    glyphy_bool_t    is_empty; /* has no outline; eg. space; don't draw it */
    unsigned int     nominal_w;
    unsigned int     nominal_h;
    unsigned int     atlas_pos;
    const int pixelsize;
};

} /* namespace glgui */

#endif /* !defined GLGUI_GLYPH_H */
