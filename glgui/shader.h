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
#ifndef GLGUI_SHADER_H
#define GLGUI_SHADER_H

#include <stdint.h>

namespace shader {

typedef struct source
{
    uint32_t length;
    const uint8_t *data;
} source_t;

} /* namespace shader */

#define _GLGUI_IMPORT_SHADER_1(_1) extern const struct source _1;
#define _GLGUI_IMPORT_SHADER_2(_1,_2) namespace _1 { _GLGUI_IMPORT_SHADER_1 (_2) }
#define _GLGUI_IMPORT_SHADER_3(_1,_2,_3) namespace _1 { _GLGUI_IMPORT_SHADER_2 (_2,_3) }
#define _GLGUI_IMPORT_SHADER_4(_1,_2,_3,_4) namespace _1 { _GLGUI_IMPORT_SHADER_3 (_2,_3,_4) }
#define _GLGUI_IMPORT_SHADER_5(_1,_2,_3,_4,_5) namespace _1 { _GLGUI_IMPORT_SHADER_4 (_2,_3,_4,_5) }
#define _GLGUI_IMPORT_SHADER_6(_1,_2,_3,_4,_5,_6) namespace _1 { _GLGUI_IMPORT_SHADER_5 (_2,_3,_4,_5,_6) }
#define _GLGUI_IMPORT_SHADER_7(_1,_2,_3,_4,_5,_6,_7) namespace _1 { _GLGUI_IMPORT_SHADER_6 (_2,_3,_4,_5,_6,_7) }
#define _GLGUI_IMPORT_SHADER_8(_1,_2,_3,_4,_5,_6,_7,_8) namespace _1 { _GLGUI_IMPORT_SHADER_7 (_2,_3,_4,_5,_6,_7,_8) }
#define _GLGUI_IMPORT_SHADER_GETMACRO(_1,_2,_3,_4,_5,_6,_7,_8,NAME,...) NAME
#define GLGUI_IMPORT_SHADER(...) namespace shader { _GLGUI_IMPORT_SHADER_GETMACRO(__VA_ARGS__,\
_GLGUI_IMPORT_SHADER_8, _GLGUI_IMPORT_SHADER_7, _GLGUI_IMPORT_SHADER_6, _GLGUI_IMPORT_SHADER_5,\
_GLGUI_IMPORT_SHADER_4, _GLGUI_IMPORT_SHADER_3, _GLGUI_IMPORT_SHADER_2, _GLGUI_IMPORT_SHADER_1)(__VA_ARGS__) }

#endif /* !defined GLGUI_SHADER_H */
