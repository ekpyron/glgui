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

#include "Box.h"

namespace glgui {

Box::Box (Widget *parent) : Widget (parent) {
}

Box::~Box (void) {
}

void Box::SetColor (float r, float g, float b, float a) {
    color = glm::vec4 (r, g, b, a);
}

void Box::Paint (Renderer *renderer) {
    renderer->RenderBox (GetTransformation (), color, GetSelectionPointer ());
    Widget::Paint (renderer);
}

} /* namespace glgui */
