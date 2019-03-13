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

#include "String.h"

#include <primitives/exceptions.h>

#include <memory>

namespace polygon4
{

struct Settings;

namespace detail
{

using IdType = int;

template <class T>
using Ptr = std::shared_ptr<T>;

class IObjectBase;

using alloc_type = uint8_t;
using deleter = void(*)(IObjectBase*);

using ObjectPtr = std::unique_ptr<IObjectBase, deleter>;

template <typename T>
struct IdPtr
{
    using base_type = IObjectBase;
    using value_type = T;

    IdType id = 0;
    base_type *ptr = nullptr;

    IdPtr()
    {
    }
    IdPtr(IdType id)
        : id(id)
    {
    }
    IdPtr(base_type *p)
        : ptr(p)
    {
        if (ptr)
            id = ptr->id;
    }
    IdPtr(const Ptr<T> &p)
        : IdPtr(p.get())
    {
    }

    IdPtr &operator=(IdType i)
    {
        id = i;
        return *this;
    }
    IdPtr &operator=(const IdPtr &rhs)
    {
        *this = rhs.ptr;
        return *this;
    }
    template <class V>
    IdPtr &operator=(const IdPtr<V> &rhs)
    {
        id = rhs.id;
        return *this = rhs.ptr;
    }
    IdPtr &operator=(const Ptr<T> &rhs)
    {
        return *this = rhs.get();
    }
    IdPtr &operator=(const base_type *rhs)
    {
        ptr = rhs;
        if (ptr)
            id = ptr->id;
        return *this;
    }
    IdPtr &operator=(base_type *rhs)
    {
        ptr = rhs;
        if (ptr)
            id = ptr->id;
        return *this;
    }

    T *operator->() const
    {
        return (T *)&this->operator*();
    }
    T &operator*() const
    {
        if (ptr == nullptr)
            throw SW_EXCEPTION("Value is not present");
        return (T &)*ptr;
    }
    T *get() const
    {
        return (T *)ptr;
    }

    bool operator==(const IdPtr &rhs) const
    {
        return id == rhs.id && ptr == rhs.ptr;
    }
    bool operator==(const T &rhs) const
    {
        return ptr == &rhs;
    }

    operator bool() const
    {
        return ptr != nullptr;
    }
    operator T*() const
    {
        return (T *)ptr;
    }

    void reset()
    {
        clear();
    }
    void clear()
    {
        id = 0;
        ptr = nullptr;
    }
};

template<class T>
inline ::polygon4::String to_string(const Ptr<T> &ptr)
{
    if (ptr)
        return ptr->getName();
    return ::polygon4::String();
}

template<class T>
inline ::polygon4::String to_wstring(const Ptr<T> &ptr)
{
    if (ptr)
        return ptr->getName();
    return ::polygon4::String();
}

template<class T>
inline ::polygon4::String to_string(const IdPtr<T> &ptr)
{
    if (ptr.ptr)
        return ptr.ptr->getName();
    return ::polygon4::String();
}

template<class T>
inline ::polygon4::String to_wstring(const IdPtr<T> &ptr)
{
    if (ptr.ptr)
        return ptr.ptr->getName();
    return ::polygon4::String();
}

} // namespace detail

using detail::IdPtr;
using detail::Ptr;

} // namespace polygon4
