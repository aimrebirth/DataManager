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

#include <Polygon4/DataManager/Types.h>

#include <algorithm>

#include <Polygon4/DataManager/Storage.h>
#include <Polygon4/DataManager/Table.h>

namespace polygon4
{

namespace detail
{

void TreeItem::remove() const
{
    if (!parent)
        return;
    auto &v = parent->children;
    v.erase(std::remove_if(v.begin(), v.end(), [this](const auto &c) { return c.get() == this; }), v.end());
}

void TreeItem::update()
{
    if (object)
    {
        name = object->getName();
        type = object->getType();
    }
}

bool to_bool(const std::string &s)
{
    if (s.empty() || s == "0" || s == "false")
        return false;
    return true;
}

std::tuple<bool, OrderedObjectMap> IObjectBase::getOrderedObjectMap(int columnId, Storage *storage) const
{
    if (storage)
        return std::make_tuple(true, storage->getOrderedMap(getType()));
    return std::make_tuple(false, OrderedObjectMap());
}

} // namespace detail

namespace detail
{

#include "detail/Types.cpp"

} // namespace detail

} // namespace polygon4
