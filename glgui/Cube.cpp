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

#include "Cube.h"

namespace glgui {

Cube::Cube (Widget *parent) : Widget (parent) {
}

Cube::~Cube (void) {
}

void Cube::SetColor (float r, float g, float b, float a) {
    color = glm::vec4 (r, g, b, a);
}

void Cube::Paint (Renderer *renderer) {
    renderer->RenderCube (GetTransformation (), color, GetSelectionPointer ());
    Widget::Paint (renderer);
}

} /* namespace glgui */
