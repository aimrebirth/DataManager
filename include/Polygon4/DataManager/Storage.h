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

#include <memory>
#include <string>

#include "Dll.h"
#include "Schema.h"
#include "Types.h"

namespace polygon4
{

namespace detail
{

using CheckFunction = std::function<bool(IObjectBase*)>;

#include "detail/Storage.h"

} // namespace detail

using detail::Storage;
class Database;

DLL_EXPORT
std::shared_ptr<Storage> initStorage(std::string filename);
DLL_EXPORT
std::shared_ptr<Storage> initStorage(std::shared_ptr<Database> db);

} // namespace polygon4
