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
#include "Widget.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

namespace glgui {

Widget::Widget (Widget *_parent) : parent (nullptr), width (-1.0f), height (-1.0f), depth (-1.0f),
                                   x (0.0f), y (0.0f), z (0.0f), pivot (BOTTOM|LEFT|FRONT),
                                   relative_x (true), relative_y (true), relative_z (true),
                                   selectionptr (this) {
    if (_parent) {
        _parent->Add (this);
    }
}

Widget::~Widget (void) {
    if (parent) parent->Remove (this);
    for (auto &child : children) {
        child->parent = nullptr;
        delete child;
    }
}

void Widget::SetSelectionPointer (void *ptr) {
    selectionptr = ptr;
}

void *Widget::GetSelectionPointer (void) {
    return selectionptr;
}

void Widget::SetAbsolutePosition (float x, float y, float z) {
    SetX (x, false);
    SetY (y, false);
    SetZ (z, false);
}

void Widget::SetRelativePosition (float x, float y, float z) {
    SetX (x, true);
    SetY (y, true);
    SetZ (z, true);
}

void Widget::SetX (float _x, bool _relative_x) {
    x = _x; relative_x = _relative_x;
}

void Widget::SetY (float _y, bool _relative_y) {
    y = _y; relative_y = _relative_y;
}

void Widget::SetZ (float _z, bool _relative_z) {
    z = _z; relative_z = _relative_z;
}
void Widget::SetPivot (unsigned int _pivot) {
    pivot = _pivot;
}
unsigned int Widget::GetPivot (void) const {
    return pivot;
}
bool Widget::IsXRelative (void) const {
    return relative_x;
}
bool Widget::IsYRelative (void) const {
    return relative_y;
}
bool Widget::IsZRelative (void) const {
    return relative_z;
}
float Widget::GetRelativeX (void) const {
    if (relative_x) {
        return x;
    } else {
        if (parent) {
            float parentwidth = parent->GetAbsoluteWidth ();
            if (parentwidth == 0.0f) return 0.0f;
            else return x / parentwidth;
        } else {
            return 0.0f;
        }
    }
}
float Widget::GetRelativeY (void) const {
    if (relative_y) {
        return y;
    } else {
        if (parent) {
            float parentheight = parent->GetAbsoluteHeight ();
            if (parentheight == 0.0f) return 0.0f;
            else return y / parentheight;
        } else {
            return 0.0f;
        }
    }
}
float Widget::GetRelativeZ (void) const {
    if (relative_z) {
        return z;
    } else {
        if (parent) {
            float parentdepth = parent->GetAbsoluteDepth ();
            if (parentdepth == 0.0f) return 0.0f;
            else return z / parentdepth;
        } else {
            return 0.0f;
        }
    }
}
float Widget::GetAbsoluteX (void) const {
    if (!relative_x) {
        return x;
    } else {
        if (parent) {
            return x * parent->GetAbsoluteWidth ();
        } else {
            return 0.0f;
        }
    }
}
float Widget::GetAbsoluteY (void) const {
    if (!relative_y) {
        return y;
    } else {
        if (parent) {
            return y * parent->GetAbsoluteWidth ();
        } else {
            return 0.0f;
        }
    }
}
float Widget::GetAbsoluteZ (void) const {
    if (!relative_z) {
        return z;
    } else {
        if (parent) {
            return z * parent->GetAbsoluteWidth ();
        } else {
            return 0.0f;
        }
    }
}

void Widget::SetWidth (float _width) {
    width = _width;
}

void Widget::SetHeight (float _height) {
    height = _height;
}

void Widget::SetDepth (float _depth) {
    depth = _depth;
}

void Widget::SetSize (float width, float height, float depth) {
    SetWidth (width); SetHeight (height); SetDepth (depth);
}

glm::mat4 Widget::GetTransformation (void) const {
    float x = GetRelativeX ();
    float y = GetRelativeY ();
    float z = GetRelativeZ ();
    float w = GetRelativeWidth ();
    float h = GetRelativeHeight ();
    float d = GetRelativeDepth ();
    switch (pivot&BOTTOM_TOP_MASK) {
        case BOTTOM:
            y += 0.5f * h - 0.5f;
            break;
        case TOP:
            y = 0.5f - 0.5f * h - y;
            break;
    }
    switch (pivot&LEFT_RIGHT_MASK) {
        case LEFT:
            x += 0.5f * w - 0.5f;
            break;
        case RIGHT:
            x = 0.5f - 0.5f * w - x;
            break;
    }
    switch (pivot&FRONT_BACK_MASK) {
        case FRONT:
            z += 0.5f * d - 0.5f;
            break;
        case BACK:
            z = 0.5f - 0.5f * d - z;
            break;
    }
    glm::mat4 mat = parent ? parent->GetTransformation () : glm::mat4 (1.0f);
    mat = glm::translate (mat, glm::vec3 (2.0f * x, 2.0f * y, 2.0f * z));
    mat = glm::scale (mat, glm::vec3 (w, h, d));
    return mat;
}

float Widget::GetWidth (void) const {
    return width;
}

float Widget::GetHeight (void) const {
    return height;
}
float Widget::GetDepth (void) const {
    return depth;
}

float Widget::GetAbsoluteWidth (void) const {
    if (width >= 0.0f) {
        return width;
    }
    else {
        if (parent) {
            return -width * parent->GetAbsoluteWidth ();
        } else {
            return 0.0f;
        }
    }
}

float Widget::GetAbsoluteHeight (void) const {
    if (height >= 0.0f) {
        return height;
    } else {
        if (parent) {
            return -height * parent->GetAbsoluteHeight ();
        } else {
            return 0.0f;
        }
    }
}
float Widget::GetAbsoluteDepth (void) const {
    if (depth >= 0.0f) {
        return depth;
    } else {
        if (parent) {
            return -depth * parent->GetAbsoluteDepth ();
        } else {
            return 0.0f;
        }
    }
}
float Widget::GetRelativeWidth (void) const {
    if (width <= 0.0f) {
        return -width;
    } else {
        if (parent) {
            float parentwidth = parent->GetAbsoluteWidth ();
            if (parentwidth == 0.0f) return 1.0f;
            else return width / parentwidth;
        } else {
            return 1.0f;
        }
    }
}
float Widget::GetRelativeHeight (void) const {
    if (height <= 0.0f) {
        return -height;
    } else {
        if (parent) {
            float parentheight = parent->GetAbsoluteHeight ();
            if (parentheight == 0.0f) return 1.0f;
            else return height / parentheight;
        } else {
            return 1.0f;
        }
    }
}
float Widget::GetRelativeDepth (void) const {
    if (depth <= 0.0f) {
        return -depth;
    } else {
        if (parent) {
            float parentdepth = parent->GetAbsoluteDepth ();
            if (parentdepth == 0.0f) return 0.0f;
            else return depth / parentdepth;
        } else {
            return 0.0f;
        }
    }
}

void Widget::Add (Widget *child) {
    // TODO: should this check for circular parenthood?
    if (child->parent) {
        child->parent->Remove (child);
    }
    children.push_back (child);
    child->parent = this;
}

void Widget::Remove (Widget *child) {
    // TODO: should this be an assertion or at least fail if false?
    if (child->parent == this) {
        children.remove (child);
        child->parent = nullptr;
    }
}

void Widget::Paint (Renderer *renderer) {
    for (auto &child : children) {
        child->Paint (renderer);
    }
}

} /* namespace glgui */
