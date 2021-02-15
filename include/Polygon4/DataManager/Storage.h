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

#include "Enums.h"
#include "Schema.h"
#include "Settings.h"
#include "Table.h"

#include "detail/ForwardDeclarations.h"

#include <primitives/filesystem.h>

#include <memory>
#include <string>

namespace polygon4
{

class Database;

namespace detail
{

class IObjectBase;
struct TreeItem;

using CheckFunction = std::function<bool(IObjectBase*)>;
using ProgressCallback = std::function<void(double)>;

#include "detail/Storage.h"

// create value
template <class T>
template <class... Args>
typename StorageTable<T>::ptr_type StorageTable<T>::create(Args&&... args)
{
    auto ptr = T::template create<T>(std::forward<Args>(args)...);
    auto raw = ptr.get();
    storage->db_objects.emplace_back(std::move(ptr));
    raw->setStorage(storage);
    return raw;
}

} // namespace detail

using detail::Storage;

DATA_MANAGER_API
std::unique_ptr<Storage> initStorage();

} // namespace polygon4
