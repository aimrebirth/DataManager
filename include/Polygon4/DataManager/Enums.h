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

#include <map>
#include <unordered_map>
#include <unordered_set>

#include "Common.h"
#include "String.h"

namespace polygon4
{

namespace detail
{

using EnumTable = std::map<String, int>;

template <typename E>
using EnumTextTable = std::unordered_map<E, std::string>;

template <typename E>
using EnumExcludeTable = std::unordered_set<E>;

} // namespace detail

} // namespace polygon4

#include "detail/Enums.h"
