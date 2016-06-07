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
#include <stdint.h>
#include <type_traits>
#include <unordered_map>
#include <utility>

#include "Exception.h"
#include "Types.h"

namespace polygon4
{

namespace detail
{

template <class T>
class CVector : public std::vector<T*>
{
public:
    using base = std::vector<T*>;
    using value_type = typename base::value_type;
    using iterator = typename base::iterator;
    using const_iterator = typename base::const_iterator;

public:
    void insert(value_type p)
    {
        if (p)
            push_back(p);
    }

    OrderedObjectMap getOrderedObjectMap() const
    {
        OrderedObjectMap m;
        for (auto &d : *this)
        {
            m.insert({ d->getName(), d });
        }
        return m;
    }

    template <typename F>
    OrderedObjectMap getOrderedObjectMap(F &&f) const
    {
        OrderedObjectMap m;
        for (auto &d : *this)
        {
            if (f(d))
                m.insert({ d->getName(), d });
        }
        return m;
    }

    operator OrderedObjectMap() const
    {
        return getOrderedObjectMap();
    }

    template <typename F>
    iterator find_if(F &&f) { return std::find_if(begin(), end(), f); }

    template <typename F>
    const_iterator find_if(F &&f) const { return std::find_if(begin(), end(), f); }
};

class Storage;

template <class T>
class StorageTable : public std::unordered_map<IdType, T*>
{
public:
    using base = std::unordered_map<IdType, T*>;

    using iterator = typename base::iterator;
    using const_iterator = typename base::const_iterator;

    using key_type = typename base::key_type;
    using value_type = typename base::value_type;
    using ptr_type = T*;
    using id_ptr_type = IdPtr<T>;

public:
    // create value
    template <class... Args>
    ptr_type create(Args&&... args)
    {
        auto ptr = T::template create<T>(std::forward<Args>(args)...);
        auto raw = ptr.get();
        storage->db_objects.emplace_back(std::move(ptr));
        raw->setStorage(storage);
        return raw;
    }

    // create value and append it to the end of container
    template <class... Args>
    ptr_type createAtEnd(Args&&... args)
    {
        auto v = create(std::forward<Args>(args)...);
        v->setId(maxId);
        return base::operator[](maxId++) = v;
    }

    // here we insert element with checking its id
    ptr_type insert(const ptr_type &v)
    {
        auto key = v->getId(maxId);
        if (key < 1)
        {
            throw EXCEPTION("Bad id (" + std::to_string(key) + ") < 1 detected. Table: '" + name + "'" +
                ", value: '" + v->getName().toString() + "'");
        }
        auto old = find(key);
        if (old != end())
        {
            throw EXCEPTION("Duplicate key (" + std::to_string(key) + ") detected. Table '" + name + "'" +
                ", old value: '" + old->second->getName().toString() + "'" +
                ", new value: '" + old->second->getName().toString() + "'");
        }
        if (key < maxId)
            base::operator[](key) = v;
        else if (key == maxId)
            insertAtEnd(v);
        else
        {
            maxId = key;
            insertAtEnd(v);
        }
        return v;
    }
    ptr_type insertAtEnd(const ptr_type &v)
    {
        v->setId(maxId);
        base::operator[](maxId++) = v;
        return v;
    }

    // get by key
    ptr_type operator[](const key_type &i) const // does not change the container
    {
        auto v = find(i);
        if (v == end())
        {
            throw EXCEPTION("key (" + std::to_string(i) + ") not found in table '" + name + "'");
        }
        return v->second;
    }

    OrderedObjectMap getOrderedObjectMap() const
    {
        OrderedObjectMap m;
        for (auto &d : *this)
        {
            auto p = d.second;
            m.insert({ p->getName(), p });
        }
        return m;
    }

    template <typename F>
    OrderedObjectMap getOrderedObjectMap(F &&f) const
    {
        OrderedObjectMap m;
        for (auto &d : *this)
        {
            auto p = d.second;
            if (f(p))
                m.insert({ p->getName(), p });
        }
        return m;
    }

    operator OrderedObjectMap() const
    {
        return getOrderedObjectMap();
    }

    // container interface
public:
    id_ptr_type get_id_ptr(const key_type &i) const
    {
        auto v = find(i);
        if (v == end())
        {
            throw EXCEPTION("key (" + std::to_string(i) + ") not found in table '" + name + "'");
        }
        return id_ptr_type(v->second);
    }

    template <typename K>
    KeyMap<K> get_key_map(K T::*p) const
    {
        KeyMap<K> map;
        for (auto &v : *this)
            map[v.second->*p] = v.second;
        return map;
    }

    template <typename F>
    iterator find_if(F &&f) { return std::find_if(begin(), end(), f); }

    template <typename F>
    const_iterator find_if(F &&f) const { return std::find_if(begin(), end(), f); }

    size_t erase(const ptr_type &v)
    {
        return erase(v.get());
    }
    size_t erase(const mapped_type *v)
    {
        if (!v)
            return 0;
        size_t erased = 0;
        while (1)
        {
            auto i = find_if([v](const auto &p) { return p.second.get() == v; });
            if (i == end())
                break;
            base::erase(i);
            vacuumIds();
            erased++;
        }
        return erased;
    }
    size_t erase(const key_type &i)
    {
        auto ret = base::erase(i);
        if (ret)
            vacuumIds();
        return ret;
    }

    // other functions
public:
    void setName(const std::string &name) { this->name = name; }
    void setStorage(Storage &storage) { this->storage = &storage; }

private:
    std::string name;
    key_type maxId = 1;
    Storage *storage;

    void vacuumIds()
    {
        for (; maxId > 0; --maxId)
        {
            auto i = find(maxId);
            if (i != end())
            {
                maxId++; // restore one id
                break;
            }
        }
        if (maxId == 0)
            maxId = 1;
    }
};

template <typename T, typename K = int32_t>
class CObjectArray : public CVector<T>
{
public:
    using base = CVector<T>;
    using value_type = typename base::value_type;

    using unordered = std::unordered_map<K, value_type>;
    using ordered = std::map<K, value_type>;

public:
    using base::base;
    using base::operator[];
    using base::size;

    void insert(value_type p)
    {
        if (!p)
            return;
        insert_to_data(p);
        push_back(p);
    }

    void insert_to_data(value_type p)
    {
        if (!p || !mptr)
            return;
        data[p->*mptr] = p;
    }

    void setMemberPtr(K T::*p) { mptr = p; }

    value_type &operator[](const K &key)
    {
        return data[key];
    }
    const value_type &operator[](const K &key) const
    {
        return data[key];
    }

    size_t count(const K &key) const
    {
        return data.count(key);
    }

    unordered get_unordered() const { return data; }
    ordered get_ordered() const { return ordered(data.begin(), data.end()); }

private:
    unordered data;
    K T::*mptr = nullptr;
};

} // namespace detail

} // namespace polygon4
