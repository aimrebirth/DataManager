/*
 * Polygon-4 Data Manager
 * Copyright (C) 2015-2016 lzwdgc
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include <primitives/emitter.h>

using Emitter = primitives::CppEmitter;

struct ModuleEmitter
{
    Emitter cpp;
    Emitter hpp;

    ModuleEmitter() {}
    ModuleEmitter &operator+=(const ModuleEmitter &rhs)
    {
        cpp += rhs.cpp;
        hpp += rhs.hpp;
        return *this;
    }
};
