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

#ifndef GLGUI_ATLAS_H
#define GLGUI_ATLAS_H

#include <oglp/oglp.h>

namespace glgui {

class Atlas {
public:
    Atlas (void);
    Atlas (const Atlas &) = delete;
    ~Atlas (void);
    Atlas &operator= (const Atlas &) = delete;

    unsigned int alloc (void *data, unsigned int len);

    const gl::Texture &GetTex (void) const {
        return texture;
    }

private:
    gl::Texture texture;
    gl::Buffer buffer;
    unsigned int cursor;
    unsigned int size;
};

} /* namespace glgui */

#endif /* !defined GLGUI_ATLAS_H */
