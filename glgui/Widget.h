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

#ifndef GLGUI_WIDGET_H
#define GLGUI_WIDGET_H

#include <list>
#include <glm/glm.hpp>
#include "Renderer.h"

namespace glgui {

class Widget {
public:
    Widget (Widget *parent = nullptr);
    virtual ~Widget (void);

    static constexpr unsigned int CENTER          = 0x00;
    static constexpr unsigned int BOTTOM_TOP_MASK = 0x03;
    static constexpr unsigned int BOTTOM          = 0x01;
    static constexpr unsigned int TOP             = 0x02;
    static constexpr unsigned int LEFT_RIGHT_MASK = 0x0C;
    static constexpr unsigned int LEFT            = 0x04;
    static constexpr unsigned int RIGHT           = 0x08;
    static constexpr unsigned int FRONT_BACK_MASK = 0x30;
    static constexpr unsigned int FRONT           = 0x10;
    static constexpr unsigned int BACK            = 0x20;

    void Add (Widget *child);
    void Remove (Widget *child);

    void SetSize (float x, float y, float z);
    void SetX (float x, bool relative_x);
    void SetY (float y, bool relative_y);
    void SetZ (float z, bool relative_z);
    void SetAbsolutePosition (float x, float y, float z);
    void SetRelativePosition (float x, float y, float z);
    virtual void SetPivot (unsigned int pivot);
    unsigned int GetPivot (void) const;
    bool IsXRelative (void) const;
    bool IsYRelative (void) const;
    bool IsZRelative (void) const;
    float GetRelativeX (void) const;
    float GetRelativeY (void) const;
    float GetRelativeZ (void) const;
    float GetAbsoluteX (void) const;
    float GetAbsoluteY (void) const;
    float GetAbsoluteZ (void) const;

    void SetWidth (float width);
    void SetHeight (float height);
    void SetDepth (float depth);
    float GetWidth (void) const;
    float GetHeight (void) const;
    float GetDepth (void) const;
    float GetAbsoluteWidth (void) const;
    float GetAbsoluteHeight (void) const;
    float GetAbsoluteDepth (void) const;
    float GetRelativeWidth (void) const;
    float GetRelativeHeight (void) const;
    float GetRelativeDepth (void) const;
    virtual glm::mat4 GetTransformation (void) const;
    virtual void Paint (Renderer *renderer);
    void SetSelectionPointer (void *ptr);
    void *GetSelectionPointer (void);
protected:
    std::list<Widget*> children;
    Widget *parent;
    unsigned int pivot;
    bool relative_x;
    float x;
    bool relative_y;
    float y;
    bool relative_z;
    float z;
    float width;
    float height;
    float depth;
    void *selectionptr;
};

} /* namespace glgui */

#endif /* !defined GLGUI_WIDGET_H */
