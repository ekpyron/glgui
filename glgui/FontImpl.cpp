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

#include "FontImpl.h"
#include <fstream>
#include <harfbuzz/hb-ft.h>
#include "Glyph.h"

namespace glgui {

std::atomic_flag FTLib::lock = ATOMIC_FLAG_INIT;
int FTLib::refcount = 0;
FT_Library FTLib::library = nullptr;

FontImpl::FontImpl (const std::string &filename, int index, int _size) : size (_size) {
    std::ifstream stream (filename, std::ios_base::in);
    Load (stream, index);
}

FontImpl::FontImpl (std::istream &stream, int index, int _size) : size (_size) {
    Load (stream, index);
}

FontImpl::~FontImpl (void) {
    hb_face_destroy (hbface);
    hb_font_destroy (hbfont);
    FT_Done_Face (face);
}

Glyph *FontImpl::LookupGlyph (int index) {
    auto it = glyphs.find (index);
    if (it == glyphs.end ()) {
        it = glyphs.emplace (index, std::make_unique<Glyph> (this, index)).first;
    }
    return it->second.get ();
}

void FontImpl::Load (std::istream &stream, int index) {
    data.clear ();
    while (stream) {
        size_t offset = data.size ();
        data.resize (offset + 4096);
        stream.read (reinterpret_cast<char*> (data.data () + offset), 4096);
        data.resize (offset + stream.gcount ());
    }
    data.shrink_to_fit ();
    if (stream.bad ()) throw std::runtime_error ("Cannot read font stream.");
    if (FT_New_Memory_Face (ftlib, data.data (), data.size (), index, &face)) {
        throw std::runtime_error ("Cannot load font.");
    }
    FT_Set_Pixel_Sizes (face, size, size);
    hbfont = hb_ft_font_create (face, nullptr);
    if (!hbfont) {
        FT_Done_Face (face);
        throw std::runtime_error ("Cannot create harfbuzz font.");
    }
    hbface = hb_ft_face_create (face, nullptr);
    if (!hbface) {
        hb_font_destroy (hbfont);
        FT_Done_Face (face);
        throw std::runtime_error ("Cannot create harfbuzz face.");
    }
}

} /* namespace glgui */
