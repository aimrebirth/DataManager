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

#include <Polygon4/StorageImpl.h>

#include <algorithm>

#ifdef USE_QT
#include <qtreewidget.h>
#endif

#include <sqlite3/sqlite3.h>

#include <Polygon4/Database.h>

#define PROGRESS_CALLBACK(p) if (callback) callback(p)

using polygon4::detail::Ptr;
using polygon4::detail::CVector;
using polygon4::detail::IObject;
using polygon4::detail::OrderedObjectMap;

template <class T>
OrderedObjectMap getOrderedMap(const T &array)
{
    OrderedObjectMap map;
    for (auto &v : array)
        map.insert(std::make_pair(v.second->getName(), v.second));
    return map;
}

template <class T>
OrderedObjectMap getOrderedMap(const CVector<T> &array)
{
    OrderedObjectMap map;
    for (auto &v : array)
        map.insert(std::make_pair(v->getName(), v));
    return map;
}

template <class T>
OrderedObjectMap getOrderedMap(const T &array, std::function<bool(Ptr<IObject>)> f)
{
    if (!f)
        return getOrderedMap(array);
    polygon4::detail::OrderedObjectMap map;
    for (auto &v : array)
    {
        auto &value = v.second;
        if (f(value))
            map.insert(std::make_pair(value->getName(), value));
    }
    return map;
}

template <class T>
polygon4::detail::OrderedObjectMap getOrderedMap(const polygon4::detail::CVector<T> &array, std::function<bool(Ptr<IObject>)> f)
{
    if (!f)
        return getOrderedMap(array);
    polygon4::detail::OrderedObjectMap map;
    for (auto &v : array)
        if (f(v))
            map.insert(std::make_pair(v->getName(), v));
    return map;
}

namespace polygon4
{

namespace detail
{

#include "detail/StorageImpl.cpp"

} // namespace detail

} // namespace polygon4
