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
#include "Pointer.h"
#include "Schema.h"
#include "String.h"

namespace polygon4
{

namespace detail
{

class IObjectBase;

struct DATA_MANAGER_API TreeItem
{
    P4String name;
    P4String defaultName;
    EObjectType type = EObjectType::None;
    IObjectBase *object = nullptr;
    TreeItem *parent = nullptr;
    std::vector<Ptr<TreeItem>> children;
    ObjectFlags flags;
    void *objectArrayVariable = nullptr;
    void *inlineVariable = nullptr;

    void *guiItem = nullptr;

    int child_count() const;
    void remove() const;
    void update();
    P4String get_name();
};

} // namespace detail

using detail::TreeItem;

} // namespace polygon4
