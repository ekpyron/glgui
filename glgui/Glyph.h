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
#ifndef GLGUI_GLYPH_H
#define GLGUI_GLYPH_H

#include <glyphy.h>

namespace glgui {

class FontImpl;

class Glyph {
public:
    Glyph (FontImpl *font, int index);
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
