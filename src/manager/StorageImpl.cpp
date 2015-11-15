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

#include <Polygon4/DataManager/StorageImpl.h>

#include <algorithm>

#ifdef USE_QT
#include <qtreewidget.h>
#endif

#include <sqlite3/sqlite3.h>

#include <Polygon4/DataManager/Database.h>

#define PROGRESS_CALLBACK(p) if (callback) callback(p)

template <class T>
polygon4::detail::OrderedObjectMap getOrderedMap(const polygon4::detail::CTable<T> &array)
{
    polygon4::detail::OrderedObjectMap map;
    for (auto &v : array)
        map.insert(std::make_pair(v.second->getName(), v.second.get()));
    return map;
}

template <class T>
polygon4::detail::OrderedObjectMap getOrderedMap(const polygon4::detail::CTable<T> &array, std::function<bool(polygon4::detail::IObjectBase *)> f)
{
    if (!f)
        return getOrderedMap(array);
    polygon4::detail::OrderedObjectMap map;
    for (auto &v : array)
    {
        auto p = v.second.get();
        if (f(p))
            map.insert(std::make_pair(v->getName(), p));
    }
    return map;
}

namespace polygon4
{

namespace detail
{

#include "detail/StorageImpl.cpp"

} // namespace detail

} // namespace polygon4
