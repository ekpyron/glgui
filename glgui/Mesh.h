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

#ifndef GLGUI_MESH_H
#define GLGUI_MESH_H

#include <istream>
#include <oglp/oglp.h>

namespace glgui {

class Mesh {
public:
    Mesh (const std::string &filename);
    Mesh (std::istream &stream);
    Mesh (const Mesh&) = delete;
    ~Mesh (void);
    Mesh &operator= (const Mesh&) = delete;
private:
    void Load (std::istream &stream);
    gl::VertexArray vao;
};

} /* namespace glgui */

#endif /* !defined GLGUI_MESH_H */
