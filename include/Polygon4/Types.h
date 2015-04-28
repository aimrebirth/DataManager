/*
 * Polygon-4 Engine
 * Copyright (C) 2015 lzwdgc
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

#include <map>
#include <memory>
#include <set>
#include <stdint.h>
#include <vector>

#include "String.h"

namespace polygon4
{

namespace detail
{

typedef String Text;
typedef uint8_t EnumType;

template <typename T>
using Ptr = std::shared_ptr<T>;

template <typename T>
using CVector = std::vector<T>;

template <typename T>
using CSet = std::set<T>;

template <typename T>
using CMap = std::map<int, T>;

template <typename T>
struct IdPtr
{
    int id = 0;
    std::shared_ptr<T> ptr;
};

#include "detail/Types.h"

} // namespace detail

} // namespace polygon4
