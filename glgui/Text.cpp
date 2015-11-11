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
#include "Text.h"
#include "Font.h"
#include "Glyph.h"

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

Text::Text (void) : width (0.0f) {

}

Text::~Text (void) {
}

void Text::SetWidth (float _width) {
    width = _width;
    Layout ();
}

void Text::SetContent (Font &font, const std::u32string &content) {
    HarfbuzzBuffer buf;
    unsigned int glyphcount = 0;
    hb_buffer_set_direction (buf, HB_DIRECTION_LTR);
    hb_buffer_set_script (buf, HB_SCRIPT_LATIN);
    hb_buffer_set_language (buf, hb_language_get_default ());
    hb_buffer_add_utf32 (buf, reinterpret_cast<const unsigned int*> (content.data ()), content.size (), 0, content.size ());
    hb_shape (font.GetHarfbuzzFont (), buf, nullptr, 0);
    hb_glyph_position_t *glyph_pos = hb_buffer_get_glyph_positions (buf, &glyphcount);
    hb_glyph_info_t *glyph_infos = hb_buffer_get_glyph_infos (buf, &glyphcount);
    float x = 0.0f;
    float y = 0.0f;
    glyphs.clear ();
    glyphs.reserve (glyphcount);
    for (auto i = 0; i < glyphcount; i++) {
        glyphs.emplace_back ();
        glyphinfo_t &info = glyphs.back ();
        info.glyph = font.LookupGlyph (glyph_infos[i].codepoint);
        info.pos = glm::vec2 (x + (glyph_pos[i].x_offset/64.0f), y - (glyph_pos[i].y_offset/64.0f));
        x += glyph_pos[i].x_advance/64.0f;
        y -= glyph_pos[i].y_advance/64.0f;
    }
    Layout ();
}

void Text::Layout (void) {
    std::vector<charinfo_t> charinfos;
    charinfos.resize (glyphs.size ());
    float linestart_x = 0.0f;
    int line = 0;
    int last_white_space = -1;

    int current = 0;
    while (current < glyphs.size ()) {
        Glyph *glyph = glyphs[current].glyph;
        glm::vec2 &pos = glyphs[current].pos;
        const int &size = glyph->GetPixelSize ();

        if (glyph->IsEmpty ()) {
            current++;
            last_white_space = current;
            continue;
        }

        charinfos[current].vInfo = glm::ivec3 (glyph->GetBufferOffset (), glyph->GetNominalWidth (), glyph->GetNominalHeight ());
        charinfos[current].vColor = glm::vec4 (1, 1, 1, 1);
        charinfos[current].vPositions = glm::vec4 (pos.x - linestart_x + size * glyph->GetExtents ().min_x,
                                                   -line * 32 + pos.y + size * glyph->GetExtents ().min_y,
                                                   size * (glyph->GetExtents ().max_x - glyph->GetExtents ().min_x),
                                                   size * (glyph->GetExtents ().max_y - glyph->GetExtents ().min_y));
        if (charinfos[current].vPositions.x + charinfos[current].vPositions.z > width) {
            if (last_white_space >= 0) {
                current = last_white_space;
            }

            linestart_x += charinfos[current].vPositions.x;
            line++;
            charinfos[current].vPositions.x = 0;
            charinfos[current].vPositions.y -= 32;
            last_white_space = -1;
        }

        current++;
    }
    buffer = gl::Buffer ();
    buffer.Storage (sizeof (charinfo_t) * charinfos.size (), charinfos.data (), 0);
}

void Text::Render (void) {
    renderer.GetVertexArray ().VertexBuffer (1, buffer, 0, sizeof (charinfo_t));
    renderer.GetVertexArray ().BindingDivisor (1, 1);
    renderer.GetVertexArray ().Bind ();

    renderer.GetProgram ().Use ();

    renderer.GetAtlas ().GetTex ().Bind (0);

    renderer.SetMatrix (glm::translate (glm::ortho (0.0f, 1280.0f, 0.0f, 720.0f, -100.0f, 100.0f), glm::vec3 (0, 360, 0)));

    gl::Enable (GL_BLEND);
    gl::BlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    gl::BlendEquation (GL_FUNC_ADD);

    gl::DrawArraysInstanced (GL_TRIANGLE_STRIP, 0, 4, glyphs.size ());

    gl::BindVertexArray (0);
    gl::Disable (GL_BLEND);

    gl::Program::UseNone ();
}

} /* namespace glgui */
