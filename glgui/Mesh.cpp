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

#include "Mesh.h"
#include <fstream>
#include <openvf/openvf.h>
#include <iostream>

namespace glgui {

class VF {
public:
    VF (void) : vf (vfAlloc ()) {
    }
    VF (const VF&) = delete;
    ~VF (void) {
        vfFree (vf);
    }
    VF &operator= (const VF&) = delete;
    void Load (std::istream &stream) {
        if (vfLoadCustom (vf, readfn, &stream) != VF_NO_ERROR)
            throw std::runtime_error ("cannot load vf data");
    }
    operator vf_t* (void) {
        return vf;
    }
private:
    static uint64_t readfn (void *buf, uint64_t length, void *userdata) {
        std::istream &stream = *reinterpret_cast<std::istream*> (userdata);
        stream.read (static_cast<char*> (buf), length);
        return stream.gcount ();
    }
    vf_t *vf;
};

Mesh::Mesh (const std::string &filename) {
    std::ifstream stream (filename, std::ios_base::in);
    if (!stream.is_open ()) throw std::runtime_error ("cannot open " + filename);
    try {
        Load (stream);
    } catch (...) {
        std::throw_with_nested (std::runtime_error ("cannot load " + filename));
    }
}

Mesh::~Mesh (void) {
}

Mesh::Mesh (std::istream &stream) {
    Load (stream);
}

void Mesh::Load (std::istream &stream) {
    VF vf;
    vf.Load (stream);

    vf_set_t *set = vfGetSet (vf, "VERTICES");
    if (!set) throw std::runtime_error ("no vertices");
    if (vfGetComponents (set) != 3)
        throw std::runtime_error ("invalid number of vertices");

    switch (vfGetDataType (set)) {
        case VF_BYTE:
            break;
    }

    for (vf_set_t *set = vfGetFirstSet (vf); set; set = vfGetNextSet (set)) {
        std::cout << vfGetIdentifier (set) << std::endl;

    }
}

} /* namespace glgui */
