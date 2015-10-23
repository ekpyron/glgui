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

#ifndef GLGUI_CUBE_H
#define GLGUI_CUBE_H

#include "Widget.h"

namespace glgui {

class Cube : public Widget {
public:
    Cube (Widget *parent = nullptr);
    ~Cube (void);
private:
    void SetColor (float r, float g, float b, float a = 1.0f);
protected:
    virtual void Paint (Renderer *renderer);
private:
    glm::vec4 color;
};

} /* namespace glgui */

#endif /* !defined GLGUI_CUBE_H */
