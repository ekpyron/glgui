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

#include "GL3TextRenderer.h"
#include <glgui/shader.h>

GLGUI_IMPORT_SHADER (glyphy, sdf)

namespace glgui {

GL3TextRenderer::GL3TextRenderer (void) {
    acc = glyphy_arc_accumulator_create ();
}

GL3TextRenderer::~GL3TextRenderer (void) {
    glyphy_arc_accumulator_destroy (acc);
}

} /* namespace glgui */
