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

#ifdef USE_QT
#include <qcoreapplication.h>
#endif

#include "Storage.h"

namespace polygon4
{

namespace detail
{

template <class MapObjectType, class ObjectType, typename Function>
OrderedObjectMap getOrderedMapForObject(const std::shared_ptr<Storage> &storage, ObjectType *object, Function function)
{
    if (!object)
        return OrderedObjectMap();
    auto f = [object, function](auto o)
    {
        MapObjectType *mo = (MapObjectType *)o;
        if (!mo)
            return false;
        return function(object, mo);
    };
    return storage->getOrderedMap(MapObjectType::object_type, f);
}

} // namespace detail

namespace detail
{

#include "detail/StorageImpl.h"

} // namespace detail

} // namespace polygon4
