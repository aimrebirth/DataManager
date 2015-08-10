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

#include <memory>
#include <stdint.h>
#include <type_traits>
#include <unordered_map>

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
        if (!second)
        {
            throw EXCEPTION("Trying to get empty object for id (" + std::to_string(first) + 
                "). This should not happen. Check your database or source code");
        }
        return second;
    }
    const T2 &operator->() const
    {
        if (!second)
        {
            throw EXCEPTION("Trying to get empty object for id (" + std::to_string(first) + 
                "). This should not happen. Check your database or source code");
        }
        return second;
    }

    operator T2&()
    {
        return second;
    }
    operator const T2&() const
    {
        return second;
    }
};

template <class T>
class CTable
{
    friend class Storage;
    friend class StorageImpl;

public:
    typedef int key_type;
    typedef T mapped_type;
    typedef Ptr<mapped_type> ptr_type;
    typedef TablePair<const key_type, ptr_type> value_type;

    typedef std::unordered_map<key_type, ptr_type> container;
    
    template <class T, class ParentType>
    class iterator_base : public std::iterator<std::bidirectional_iterator_tag, T>
    {
    public:
        iterator_base(const ParentType& iter) : i(iter) {}

        iterator_base &operator++() { ++i; return *this; }
        iterator_base operator++(int) { iterator_base tmp(*this); operator++(); return tmp; }

        template <class T>
        bool operator==(const T &rhs) const {return i == rhs.i;}
        template <class T>
        bool operator!=(const T &rhs) const {return i != rhs.i;}

        T &operator*() { return (T &)*i; }
        T &operator->() { return (T &)*i; }

        // parent iterator
        ParentType i;
    };

    typedef typename iterator_base<value_type, typename container::iterator> iterator;
    typedef typename iterator_base<value_type, typename container::const_iterator> const_iterator;
    
private:
    template <class T>
    struct IdHandler0
    {
        inline key_type getKey(const ptr_type &v, key_type key) const { return key; }
        inline void setKey(ptr_type &v, key_type key) const {}
    };
    template <class T>
    struct IdHandler1
    {
        inline key_type getKey(const ptr_type &v, key_type key) const { return v->id; }
        inline void setKey(ptr_type &v, key_type key) const { v->id = key; }
    };

    typedef typename std::conditional<T::has_id, IdHandler1<T>, IdHandler0<T>>::type IdHandler;
    
public:
	CTable(){}
	~CTable(){}

public:
    // create value
    ptr_type create()
    {
        return std::make_shared<mapped_type>();
    }

    // create value and append it to the end of container
    ptr_type createAtEnd()
    {
        auto v = std::make_shared<mapped_type>();
        idHandler.setKey(v, maxId);
        return data[maxId++] = v;
    }

    // here we insert element with checking its id
    ptr_type insert(const ptr_type &v)
    {
        auto key = idHandler.getKey(v, maxId);
        if (key < 1)
        {
            throw EXCEPTION("Bad id (" + std::to_string(key) + ") < 1 detected. Table: '" + name + "'" +
                ", value: '" + v->getName().string() + "'");
        }
        auto old = data.find(key);
        if (old != data.end())
        {
            throw EXCEPTION("Duplicate key (" + std::to_string(key) + ") detected. Table '" + name + "'" +
                ", old value: '" + old->second->getName().string() + "'" +
                ", new value: '" + old->second->getName().string() + "'");
        }
        if (key < maxId)
            data[key] = v;
        else if (key == maxId)
            data[maxId++] = v;
        else
        {
            maxId = key;
            data[maxId++] = v;
        }
        return v;
    }
    ptr_type insertAtEnd(const ptr_type &v)
    {
        idHandler.setKey(v, maxId);
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

public: // container interface
    iterator begin() { return data.begin(); }
    iterator end() { return data.end(); }
    const_iterator begin() const { return data.begin(); }
    const_iterator end() const { return data.end(); }

    iterator find() { return data.find(key); }
    const_iterator find(key_type key) const { return data.find(key); }

    bool empty() const { data.empty(); }
    void clear() { data.clear(); }
    
    size_t erase(const mapped_type *v)
    {
        size_t erased = 0;
        while (1)
        {
            auto i = find_if(data.begin(), data.end(), [v](const value_type &p){ return p->get() == v; });
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
    template <typename T>
    iterator erase(const T &i)
    {
        return data.erase((const container::iterator &)i);
    }

private:
    std::string name;
	container data;
    key_type maxId = 1;
    IdHandler idHandler;
};

} // namespace detail

} // namespace polygon4