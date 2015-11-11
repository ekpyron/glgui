/*
 * Copyright 2015 Daniel Kirchner
 *
 * This file is part of GLGUI.
 *
 * GLGUI is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * GLGUI is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with GLGUI.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <glm/gtc/matrix_transform.hpp>
#include <hb-icu.h>
#include <codecvt>
#include "Text.h"
#include "Font.h"
#include "Glyph.h"
#include <locale>

namespace glgui {

class HarfbuzzBuffer {
public:
    HarfbuzzBuffer (void) : buf (hb_buffer_create ()) {
    }
    HarfbuzzBuffer (const HarfbuzzBuffer&) = delete;
    HarfbuzzBuffer (HarfbuzzBuffer &&hb) : buf (hb.buf) {
        hb.buf = nullptr;
    }
    ~HarfbuzzBuffer (void) {
        if (buf != nullptr) hb_buffer_destroy (buf);
    }
    HarfbuzzBuffer &operator= (const HarfbuzzBuffer&) = delete;
    HarfbuzzBuffer &operator= (HarfbuzzBuffer &&hb) noexcept {
        buf = hb.buf; hb.buf = nullptr;
        return *this;
    }
    operator hb_buffer_t* (void) {
        return buf;
    }
    operator const hb_buffer_t* (void) const {
        return buf;
    }
private:
    hb_buffer_t *buf;
};

Text::Text (void) : width (0.0f), font (nullptr), breakOnWords (false), needlayout (true) {

}

Text::~Text (void) {
}

void Text::SetWidth (float _width) {
    width = _width;
    needlayout = true;
}

void Text::SetBreakOnWords (bool _breakOnWords) {
    breakOnWords = _breakOnWords;
    needlayout = true;
}

void Text::SetContent (Font &_font, const std::u32string &_content) {
    content = _content;
    font = &_font;
    needlayout = true;
}

void LineBreak (Font *font, std::vector<std::u32string> &lines, const std::u32string &data, float width, bool wordbreak = false) {
    std::u32string left = data;

    while (!left.empty ())
    {
        HarfbuzzBuffer buf;
        hb_buffer_set_direction (buf, HB_DIRECTION_LTR);
        hb_buffer_set_script (buf, HB_SCRIPT_LATIN);
        hb_buffer_set_language (buf, hb_language_get_default ());
        hb_buffer_add_utf32 (buf, reinterpret_cast<const unsigned int*> (left.data ()), left.size (), 0, left.size ());

        hb_shape (font->GetHarfbuzzFont (), buf, nullptr, 0);

        unsigned int glyphcount = 0;
        hb_glyph_position_t *glyph_pos = hb_buffer_get_glyph_positions (buf, &glyphcount);
        hb_glyph_info_t *glyph_infos = hb_buffer_get_glyph_infos (buf, &glyphcount);

        float x = 0.0f;
        long last_white_space = -1;

        for (auto i = 0; i < glyphcount; i++) {
            if (U' ' == (left[glyph_infos[i].cluster])) {
                last_white_space = glyph_infos[i].cluster+1;
            }
            x += glyph_pos[i].x_advance/64.0f;
            if (x >= width) {
                if (wordbreak && last_white_space >= 0) {
                    std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> conv;
                    lines.push_back (left.substr (0, last_white_space));
                    left.erase (0, last_white_space);
                } else {
                    lines.push_back (left.substr (0, glyph_infos[i-1].cluster));
                    left.erase (0, glyph_infos[i-1].cluster);
                }
                break;
            }
        }
        if (left.size () == glyphcount) {
            lines.push_back (left);
            break;
        }
    }
}

void LayoutLine (Font *font, std::vector<charinfo_t> &charinfos, const std::u32string &line, float starty) {
    unsigned int glyphcount = 0;
    HarfbuzzBuffer buf;
    hb_buffer_set_direction (buf, HB_DIRECTION_LTR);
    hb_buffer_set_script (buf, HB_SCRIPT_LATIN);
    hb_buffer_set_language (buf, hb_language_get_default ());
    hb_buffer_add_utf32 (buf, reinterpret_cast<const unsigned int*> (line.data ()), line.size (), 0, line.size ());
    hb_shape (font->GetHarfbuzzFont (), buf, nullptr, 0);
    hb_glyph_position_t *glyph_pos = hb_buffer_get_glyph_positions (buf, &glyphcount);
    hb_glyph_info_t *glyph_infos = hb_buffer_get_glyph_infos (buf, &glyphcount);

    float x = 0.0f;
    float y = starty;

    typedef struct glyphinfo {
        Glyph *glyph;
        glm::vec2 pos;
    } glyphinfo_t;

    for (auto i = 0; i < glyphcount; i++) {
        glm::vec2 pos = glm::vec2 (x + (glyph_pos[i].x_offset/64.0f), y - (glyph_pos[i].y_offset/64.0f));
        Glyph *glyph = font->LookupGlyph (glyph_infos[i].codepoint);
        const int &size = glyph->GetPixelSize ();
        x += glyph_pos[i].x_advance/64.0f;
        y -= glyph_pos[i].y_advance/64.0f;

        if (glyph->IsEmpty ()) {
            continue;
        }

        charinfos.emplace_back ();
        charinfo_t &info = charinfos.back ();

        info.vInfo = glm::ivec3 (glyph->GetBufferOffset (), glyph->GetNominalWidth (), glyph->GetNominalHeight ());
        info.vColor = glm::vec4 (1, 1, 1, 1);
        info.vPositions = glm::vec4 (pos.x + size * glyph->GetExtents ().min_x,
                                     pos.y + size * glyph->GetExtents ().min_y,
                                     size * (glyph->GetExtents ().max_x - glyph->GetExtents ().min_x),
                                     size * (glyph->GetExtents ().max_y - glyph->GetExtents ().min_y));
    }
}

void Text::Layout (void) {
    std::vector<std::u32string> lines;
    {
        std::streampos start = 0;
        while (true) {
            auto pos = content.find_first_of (U'\n', start);
            LineBreak (font, lines, content.substr (start, pos), width, breakOnWords);
            if (pos == std::u32string::npos) {
                break;
            }
            start = pos + 1;
        }
    }
    std::vector<charinfo_t> charinfos;
    for (int line = 0; line < lines.size (); line++) {
        LayoutLine (font, charinfos, lines[line], -font->GetSize () * line);
    }
    buffer = gl::Buffer ();
    buffer.Storage (sizeof (charinfo_t) * charinfos.size (), charinfos.data (), 0);
    numglyphs = charinfos.size ();
}

void Text::Render (void) {
    if (needlayout) {
        Layout ();
        needlayout = false;
    }
    renderer.GetVertexArray ().VertexBuffer (1, buffer, 0, sizeof (charinfo_t));
    renderer.GetVertexArray ().BindingDivisor (1, 1);
    renderer.GetVertexArray ().Bind ();

    renderer.GetProgram ().Use ();

    renderer.GetAtlas ().GetTex ().Bind (0);

    renderer.SetMatrix (glm::translate (glm::ortho (0.0f, 1280.0f, 0.0f, 720.0f, -100.0f, 100.0f), glm::vec3 (0, 360, 0)));

    gl::Enable (GL_BLEND);
    gl::BlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    gl::BlendEquation (GL_FUNC_ADD);

    gl::DrawArraysInstanced (GL_TRIANGLE_STRIP, 0, 4, numglyphs);

    gl::BindVertexArray (0);
    gl::Disable (GL_BLEND);

    gl::Program::UseNone ();
}

} /* namespace glgui */
