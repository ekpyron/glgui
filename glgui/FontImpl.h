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

#ifndef GLGUI_FONTIMPL_H
#define GLGUI_FONTIMPL_H

#include <string>
#include <istream>
#include <limits>
#include <glm/glm.hpp>
#include <vector>
#include <ft2build.h>
#include FT_FREETYPE_H
#include <atomic>
#include <harfbuzz/hb.h>
#include <memory>
#include <map>
#include "TextRenderer.h"

namespace glgui {

class Glyph;

class FTLib {
public:
    FTLib (void) {
        FT_Error err = 0;
        while (lock.test_and_set (std::memory_order_acquire));
        if (!refcount) {
            err = FT_Init_FreeType (&library);
            if (err == 0) {
                refcount = 1;
            }
        } else {
            refcount++;
        }
        lock.clear (std::memory_order_release);
        if (err) {
            throw std::runtime_error ("Could not initialize freetype.");
        }
    }
    FTLib (const FTLib&) = delete;
    ~FTLib (void) {
        while (lock.test_and_set (std::memory_order_acquire));
        if (refcount) refcount--;
        if (!refcount) FT_Done_FreeType (library);
        lock.clear (std::memory_order_release);
    }
    FTLib &operator= (const FTLib &) = delete;
    operator const FT_Library () const {
        return library;
    }
    operator FT_Library () {
        return library;
    }
private:
    static std::atomic_flag lock;
    static int refcount;
    static FT_Library library;
};

class FontImpl {
public:
    FontImpl (const std::string &filename, int index = 0, int size = 32);
    FontImpl (std::istream &stream, int index = 0, int size = 32);
    ~FontImpl (void);
    const hb_font_t *GetHarfbuzzFont (void) const {
        return hbfont;
    }
    hb_font_t *GetHarfbuzzFont (void) {
        return hbfont;
    }
    const FT_Face &GetFace (void) const {
        return face;
    }
    FT_Face &GetFace (void) {
        return face;
    }
    Atlas &GetAtlas (void) {
        return renderer.GetAtlas ();
    }
    const int &GetSize (void) {
        return size;
    }
    Glyph *LookupGlyph (int index);
    glyphy_arc_accumulator_t *GetArcAccumulator (void) {
        return renderer.GetArcAccumulator ();
    }
private:
    void Load (std::istream &stream, int index = 0);
    std::vector<FT_Byte> data;
    std::map<int, std::unique_ptr<Glyph>> glyphs;
    FTLib ftlib;
    FT_Face face;
    hb_font_t *hbfont;
    hb_face_t *hbface;
    const int size;
    TextRenderer renderer;
};

} /* namespace glgui */

#endif /* !defined GLGUI_FONTIMPL_H */
