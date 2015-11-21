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
#ifndef GLGUI_H
#define GLGUI_H

#include "Atlas.h"
#include "Box.h"
#include "Cube.h"
#include "Font.h"
#include "Mesh.h"
#include "Renderer.h"
#include "Text.h"
#include "Widget.h"
#include "Window.h"

namespace glgui {

void Init (oglp::GetProcAddressCallback getprocaddress);

} /* namespace glgui */

#endif /* !defined GLGUI_H */
