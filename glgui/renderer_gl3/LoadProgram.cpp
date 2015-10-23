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

#include "LoadProgram.h"
#include <lz4.h>

namespace glgui {


shaderdesc::shaderdesc (const std::string &_name, const GLenum &_type, const shader::source_t &_source)
        : name (_name), type (_type)
{
    std::vector<char> src;
    src.resize (_source.length);
    if (LZ4_decompress_fast (reinterpret_cast<const char*> (_source.data), &src[0], _source.length) < 0)
        throw std::runtime_error (std::string ("Failed to load shader ") + name + ": invalid lz4 stream.");
    source = std::string (src.data (), _source.length);
}

shaderdesc::shaderdesc (const std::string &_name, const GLenum &_type, const std::string &_source)
        : name (_name), type (_type), source (_source)
{
}

void LoadProgram (gl::Program &program, const std::string &name, const std::string &definitions,
                  const std::initializer_list<shaderdesc_t> &shaders) {
    std::vector<gl::Shader> shaderobjs;
    shaderobjs.reserve (shaders.size ());
    for (auto &shader : shaders) {
        shaderobjs.emplace_back (shader.type);
        std::vector<std::string> sources;
        if (!definitions.empty ()) sources.push_back (definitions);
        sources.push_back (shader.source);
        shaderobjs.back ().Source (sources);

        if (!shaderobjs.back ().Compile ())
        { ;
            throw std::runtime_error ("Failed to compile shader " + shader.name + ": " + shaderobjs.back ().GetInfoLog ());
        }

        program.Attach (shaderobjs.back ());
    }

    if (!program.Link ())
    {
        throw std::runtime_error ("Failed to link program " + name + ": " + program.GetInfoLog());
    }

    for (auto &obj : shaderobjs) {
        program.Detach (obj);
    }
}

} /* namespace glgui */
