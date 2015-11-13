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
#include <glm/gtc/type_ptr.hpp>
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
        reset ();
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
    void reset (void) { if (buf != nullptr) { hb_buffer_destroy (buf); buf = nullptr; } }
private:
    hb_buffer_t *buf;
};

Text::Text (void) : width (0.0f), font (nullptr), breakOnWords (false), needlayout (true) {
    uniformdata_t uniform_data = {
            glm::mat4 (1), glm::vec4 (1, 1, 1, 1), 1.0f, 0.0f
    };
    uniforms.Storage (sizeof (uniformdata_t), &uniform_data, GL_DYNAMIC_STORAGE_BIT);
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

void Text::SetContent (Font &_font, const std::string &_content) {
    content = _content;
    font = &_font;
    needlayout = true;
}

void Text::SetBoldness (float boldness) {
    uniforms.SubData (offsetof (uniformdata_t, boldness), sizeof (float), &boldness);
}

void Text::SetContrast (float contrast) {
    uniforms.SubData (offsetof (uniformdata_t, contrast), sizeof (float), &contrast);
}

void Text::SetColor (const glm::vec4 &color) {
    uniforms.SubData (offsetof (uniformdata_t, color), sizeof (glm::vec4), glm::value_ptr (color));
}

void LineBreak (Font *font, std::vector<std::string> &lines, const std::string &data, float width, bool wordbreak = false) {
    std::string left = data;

    while (!left.empty ())
    {
        HarfbuzzBuffer buf;
        hb_buffer_set_direction (buf, HB_DIRECTION_LTR);
        hb_buffer_set_script (buf, HB_SCRIPT_LATIN);
        hb_buffer_set_language (buf, hb_language_get_default ());
        hb_buffer_add_utf8 (buf, left.data (), left.size (), 0, -1);

        hb_shape (font->GetHarfbuzzFont (), buf, nullptr, 0);

        unsigned int glyphcount = 0;
        hb_glyph_position_t *glyph_pos = hb_buffer_get_glyph_positions (buf, &glyphcount);
        hb_glyph_info_t *glyph_infos = hb_buffer_get_glyph_infos (buf, &glyphcount);

        float x = 0.0f;
        long last_white_space = -1;

        long i = 0;
        for (i = 0; i < glyphcount; i++) {
            if (U' ' == (left[glyph_infos[i].cluster])) {
                last_white_space = glyph_infos[i].cluster + 1;
            }
            x += glyph_pos[i].x_advance/64.0f;
            if (i > 0 && x >= width) {
                buf.reset ();
                if (wordbreak && last_white_space >= 0) {
                    lines.push_back (left.substr (0, last_white_space));
                    buf.reset ();
                    left.erase (0, last_white_space);
                } else {
                    lines.push_back (left.substr (0, glyph_infos[i].cluster));
                    buf.reset ();
                    left.erase (0, glyph_infos[i].cluster);
                }
                break;
            }
        }
        if (i == glyphcount) {
            lines.push_back (left);
            return;
        }
    }
}

void Text::SetMatrix (const glm::mat4 &mat) {
    uniforms.SubData (offsetof (uniformdata_t, mat), sizeof (glm::mat4), glm::value_ptr (mat));
}

void Text::LayoutLine (std::vector<charinfo_t> &charinfos, const std::string &line, float starty) {
    unsigned int glyphcount = 0;
    HarfbuzzBuffer buf;
    hb_buffer_set_direction (buf, HB_DIRECTION_LTR);
    hb_buffer_set_script (buf, HB_SCRIPT_LATIN);
    hb_buffer_set_language (buf, hb_language_get_default ());
    hb_buffer_add_utf8 (buf, line.data (), line.size (), 0, -1);
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
        info.vPositions = glm::vec4 (pos.x + size * glyph->GetExtents ().min_x,
                                     pos.y + size * glyph->GetExtents ().min_y,
                                     size * (glyph->GetExtents ().max_x - glyph->GetExtents ().min_x),
                                     size * (glyph->GetExtents ().max_y - glyph->GetExtents ().min_y));
        {
            glm::vec2 current_min_pos (info.vPositions.x, info.vPositions.y);
            glm::vec2 current_max_pos = current_min_pos + glm::vec2 (info.vPositions.z, info.vPositions.w);
            min_pos = glm::min (min_pos, current_min_pos);
            min_pos = glm::min (min_pos, current_max_pos);
            max_pos = glm::max (max_pos, current_min_pos);
            max_pos = glm::max (max_pos, current_max_pos);
        }
    }
}

void Text::Layout (void) {
    std::vector<std::string> lines;
    {
        std::streampos start = 0;
        while (true) {
            auto pos = content.find_first_of (U'\n', start);
            LineBreak (font, lines, content.substr (start, pos), width, breakOnWords);
            if (pos == std::string::npos) {
                break;
            }
            start = pos + 1;
        }
    }
    std::vector<charinfo_t> charinfos;
    min_pos = glm::vec2 (std::numeric_limits<float>::max (), std::numeric_limits<float>::max ());
    max_pos = glm::vec2 (-std::numeric_limits<float>::max (), -std::numeric_limits<float>::max ());
    for (int line = 0; line < lines.size (); line++) {
        LayoutLine (charinfos, lines[line], -font->GetSize () * line);
    }
    buffer = gl::Buffer ();
    buffer.Storage (sizeof (charinfo_t) * charinfos.size (), charinfos.data (), 0);
    numglyphs = charinfos.size ();
}

const glm::vec2 &Text::GetMinPos (void) {
    if (needlayout) {
        Layout ();
        needlayout = false;
    }
    return min_pos;
}
const glm::vec2 &Text::GetMaxPos (void) {
    if (needlayout) {
        Layout ();
        needlayout = false;
    }
    return max_pos;
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

    float w = max_pos.x - min_pos.x;

    uniforms.BindBase (GL_UNIFORM_BUFFER, 0);

    gl::Enable (GL_BLEND);
    gl::BlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    gl::BlendEquation (GL_FUNC_ADD);

    gl::DrawArraysInstanced (GL_TRIANGLE_STRIP, 0, 4, numglyphs);

    gl::BindVertexArray (0);
    gl::Disable (GL_BLEND);

    gl::Program::UseNone ();
}

} /* namespace glgui */
