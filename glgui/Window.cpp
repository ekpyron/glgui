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
#include <iostream>
#include "Window.h"

namespace glgui {

Window::Window (unsigned int width, unsigned int height, unsigned int depth, bool _perspective, Widget *parent)
        : Widget (parent), perspective (_perspective) {
    SetSize (width, height, depth);

}

Window::~Window (void) {

}

glm::mat4 Window::GetTransformation (void) const {
    if (perspective) {
        return glm::translate (glm::infinitePerspective (glm::radians (100.0f), 1.0f, 0.01f), glm::vec3 (0, 0, -1));
    } else {
        return glm::mat4 (1);
    }
}

} /* namespace glgui */
