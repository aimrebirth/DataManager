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

template <class T1, class T2>
struct TablePair : public std::pair<T1, T2>
{
    T2 &operator->()
    {
        if (!this->second)
        {
            throw EXCEPTION("Trying to get empty object for id (" + std::to_string(this->first) +
                            "). This should not happen. Check your database or source code");
        }
        return this->second;
    }

    const T2 &operator->() const
    {
        if (!this->second)
        {
            throw EXCEPTION("Trying to get empty object for id (" + std::to_string(this->first) +
                "). This should not happen. Check your database or source code");
        }
        return this->second;
    }

    operator T2&()
    {
        return this->second;
    }

    operator const T2&() const
    {
        return this->second;
    }
};

template <class T>
class CTable
{
public:
    using key_type = int;
    using mapped_type = T;
    using ptr_type = Ptr<T>;
    using value_type = TablePair<const key_type, ptr_type>;
    using container = std::unordered_map<key_type, ptr_type>;
    
    template <class V, class ParentType>
    class iterator_base : public std::iterator<std::bidirectional_iterator_tag, V>
    {
        using base = std::iterator<std::bidirectional_iterator_tag, V>;

        using value_type = typename base::value_type;
        using reference = typename base::reference;
        using pointer = typename base::pointer;

    public:
        iterator_base(const ParentType& iter) : i(iter) {}

        iterator_base &operator++() { ++i; return *this; }
        iterator_base operator++(int) { iterator_base tmp(*this); operator++(); return tmp; }

        bool operator==(const iterator_base &rhs) const { return i == rhs.i; }
        bool operator!=(const iterator_base &rhs) const { return i != rhs.i; }

        reference operator*() { return (reference)*i; }
        reference operator->() { return (reference)*i; }

    private:
        // parent iterator
        ParentType i;
    };

    using iterator = iterator_base<value_type, typename container::iterator>;
    using const_iterator = iterator_base<value_type, typename container::const_iterator>;

public:
    // create value
    ptr_type create()
    {
        return T::template create<T>();
    }

    // create value and append it to the end of container
    ptr_type createAtEnd()
    {
        auto v = create();
        v->setId(maxId);
        return data[maxId++] = v;
    }

    // here we insert element with checking its id
    ptr_type insert(const ptr_type &v)
    {
        auto key = v->getId(maxId);
        if (key < 1)
        {
            throw EXCEPTION("Bad id (" + std::to_string(key) + ") < 1 detected. Table: '" + name + "'" +
                ", value: '" + to_string(v->getName()) + "'");
        }
        auto old = data.find(key);
        if (old != data.end())
        {
            throw EXCEPTION("Duplicate key (" + std::to_string(key) + ") detected. Table '" + name + "'" +
                ", old value: '" + to_string(old->second->getName()) + "'" +
                ", new value: '" + to_string(old->second->getName()) + "'");
        }
        if (key < maxId)
            data[key] = v;
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
        data[maxId++] = v;
        return v;
    }

    // get by key
    ptr_type operator[](const key_type &i) const // does not change the container
    {
        auto v = data.find(i);
        if (v == data.end())
        {
            throw EXCEPTION("key (" + std::to_string(i) + ") not found in table '" + name + "'");
        }
        return v->second;
    }

    OrderedObjectMap getOrderedObjectMap() const
    {
        OrderedObjectMap m;
        for (auto &d : data)
        {
            auto p = d.second.get();
            m.insert({ p->getName(), p });
        }
        return m;
    }
    template <typename F>
    OrderedObjectMap getOrderedObjectMap(F &&f) const
    {
        OrderedObjectMap m;
        for (auto &d : data)
        {
            auto p = d.second.get();
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
    iterator begin() { return data.begin(); }
    iterator end() { return data.end(); }
    const_iterator begin() const { return data.begin(); }
    const_iterator end() const { return data.end(); }
    const_iterator cbegin() const { return data.cbegin(); }
    const_iterator cend() const { return data.cend(); }

    iterator find(key_type key) { return data.find(key); }
    const_iterator find(key_type key) const { return data.find(key); }

    bool empty() const { data.empty(); }
    void clear() { data.clear(); }
    size_t count(const key_type &k) const { return data.count(k); }

    size_t erase(const ptr_type &v)
    {
        size_t erased = 0;
        while (1)
        {
            auto i = find_if(data.begin(), data.end(), [v](const auto &p) { return p.second == v; });
            if (i == data.end())
                break;
            data.erase(i);
            erased++;
        }
        return erased;
    }
    size_t erase(const mapped_type *v)
    {
        size_t erased = 0;
        while (1)
        {
            auto i = find_if(data.begin(), data.end(), [v](const auto &p){ return p.second.ptr.get() == v; });
            if (i == data.end())
                break;
            data.erase(i);
            erased++;
        }
        return erased;
    }
    size_t erase(const key_type &i)
    {
        return data.erase(i);
    }

    // other functions
public:
    void setName(const std::string &name) { this->name = name; }
    
private:
    std::string name;
	container data;
    key_type maxId = 1;
};

} // namespace detail

} // namespace polygon4
