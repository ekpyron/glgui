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

#ifndef GLGUI_LOADPROGRAM_H
#define GLGUI_LOADPROGRAM_H

#include <oglp/oglp.h>
#include <string>
#include <initializer_list>
#include <glgui/shader.h>

namespace glgui {

typedef struct shaderdesc {
    shaderdesc (const std::string &name, const GLenum &type, const shader::source_t &source);
    shaderdesc (const std::string &name, const GLenum &type, const std::string &source);
    std::string name;
    GLenum type;
    std::string source;
} shaderdesc_t;

void LoadProgram (gl::Program &program, const std::string &name, const std::string &definitions,
    const std::initializer_list<shaderdesc_t> &shaders);

} /* namespace glgui */

#endif /* !defined GLGUI_LOADPROGRAM_H */
