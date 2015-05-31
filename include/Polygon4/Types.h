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

#include <functional>
#include <map>
#include <memory>
#include <set>
#include <stdint.h>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <unordered_set>

#include "String.h"

#ifdef USE_QT
#include <qobject.h>

class QTreeWidgetItem;
#endif

#define POLYGON4_NONAME "NONAME"

namespace polygon4
{

extern int gCurrentLocalizationId;

namespace detail
{

using std::to_string;

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
    Ptr<T> ptr;

    bool operator==(const IdPtr<T> &rhs) const
    {
        if (ptr == rhs.ptr)
            return true;
        if (!ptr || !rhs.ptr)
            return false;
        return *ptr.get() == *rhs.ptr.get();
    }

    IdPtr &operator=(const Ptr<T> &item)
    {
        if (!item)
            return *this = IdPtr<T>();
        id = item->id;
        ptr = item;
        return *this;
    }

    Ptr<T> operator->() const
    {
        return ptr;
    }

    operator bool() const
    {
        return !!ptr;
    }

    operator Ptr<T>() const
    {
        return ptr;
    }
};

template<class T>
inline Text to_string(IdPtr<T> ptr)
{
    if (ptr.ptr)
        return to_string(ptr.ptr->getName());
    return Text();
}

class Coordinate;
inline Text to_string(const Coordinate &coordinate);

typedef std::function<void(double)> ProgressCallback;

} // namespace detail

using detail::Ptr;

} // namespace polygon4

#include "detail/Types.h"
