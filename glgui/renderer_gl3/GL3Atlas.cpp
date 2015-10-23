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

#include "GL3Atlas.h"

namespace glgui {

GL3Atlas::GL3Atlas (void) : size (4096), cursor (0), texture (GL_TEXTURE_BUFFER) {
    buffer.Data (size * 4, nullptr, GL_STATIC_DRAW);
    texture.Buffer (GL_RGBA8, buffer);
}

GL3Atlas::~GL3Atlas (void) {
}

unsigned int GL3Atlas::alloc (void *data, unsigned int len)
{
    if (cursor + len >= size) {
        gl::Buffer newbuffer;
        size = std::max (cursor + len, size * 2);
        newbuffer.Data (size * 4, nullptr, GL_STATIC_DRAW);
        gl::Buffer::CopySubData (buffer, newbuffer, 0, 0, cursor * 4);
        buffer = std::move (newbuffer);
        texture.Buffer (GL_RGBA8, buffer);
    }
    unsigned int pos = cursor;
    cursor += len;
    buffer.SubData (4 * pos, 4 * len, data);
    return pos;
}

} /* namespace glgui */
