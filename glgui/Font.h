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

#ifndef GLGUI_FONT_H
#define GLGUI_FONT_H

#include <string>
#include <istream>

namespace glgui {

class FontImpl;

class Font {
public:
    Font (const std::string &filename, int index = 0, int size = 32);
    Font (std::istream &stream, int index = 0, int size = 32);
    Font (const Font&) = delete;
    ~Font (void);
    Font &operator= (const Font&) = delete;
    FontImpl *GetImpl (void) {
        return impl;
    }
    const FontImpl *GetImpl (void) const {
        return impl;
    }
private:
    FontImpl *impl;
};

} /* namespace glgui */

#endif /* !defined GLGUI_FONT_H */
