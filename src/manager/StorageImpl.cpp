/*
 * Polygon-4 Data Manager
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

#include <Polygon4/DataManager/StorageImpl.h>

#include <algorithm>

#include <sqlite3.h>

#include <Polygon4/DataManager/Database.h>
#include <Polygon4/DataManager/Localization.h>

#include "Logger.h"
DECLARE_STATIC_LOGGER(logger, "storage_impl");

#define PROGRESS_CALLBACK(p) if (callback) callback(p)

namespace polygon4
{

namespace detail
{

#include "detail/StorageImpl.cpp"

} // namespace detail

} // namespace polygon4
