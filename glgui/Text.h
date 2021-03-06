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

#ifndef GLGUI_TEXT_H
#define GLGUI_TEXT_H

#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <oglp/oglp.h>
#include "TextRenderer.h"
#include "Widget.h"

namespace glgui {

class Font;
class FontImpl;
class Glyph;

typedef struct charinfo {
    glm::vec4 vPositions;
    glm::ivec3 vInfo;
    int padding;
} charinfo_t;

class Text : public Widget {
public:
    Text (Widget *parent = nullptr);
    ~Text (void);
    void SetWidth (float width);
    void SetContent (Font &font, const std::string &content);
    void SetBreakOnWords (bool breakOnWords);
    void SetContrast (float contrast);
    void SetBoldness (float boldness);
    void SetColor (const glm::vec4 &color);
    virtual void SetPivot (unsigned int pivot);
    const bool &GetBreakOnWords (void) const {
        return breakOnWords;
    }
    const glm::vec2 &GetMinPos (void);
    const glm::vec2 &GetMaxPos (void);
    virtual void Paint (Renderer *renderer);
private:
    void SetMatrix (const glm::mat4 &mat);
    void Layout (void);
    void LayoutLine (std::vector<charinfo_t> &charinfos, const std::string &line, float starty);
    float width;
    std::string content;
    FontImpl *font;
    gl::Buffer buffer;
    gl::Buffer uniforms;
    typedef struct uniformdata {
        glm::mat4 mat;
        glm::vec4 color;
        float contrast;
        float boldness;
        float padding[2];
    } uniformdata_t;
    unsigned int numglyphs;
    bool needlayout;
    TextRenderer renderer;
    glm::vec2 min_pos;
    glm::vec2 max_pos;
    bool breakOnWords;
};

} /* namespace glgui */


#endif /* !defined GLGUI_TEXT_H */
