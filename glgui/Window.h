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

#ifndef GLGUI_WINDOW_H
#define GLGUI_WINDOW_H

#include "Widget.h"
#include "Renderer.h"

namespace glgui {

class Window : public Widget {
public:
    Window (unsigned int width, unsigned int height, unsigned int depth, bool perspective = false, Widget *parent = nullptr);
    ~Window (void);
    virtual glm::mat4 GetTransformation (void) const;
private:
    bool perspective;
};

} /* namespace glgui */

#endif /* !defined GLGUI_WINDOW_H */
