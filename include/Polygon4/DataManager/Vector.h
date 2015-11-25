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
#include <set>
#include <stdint.h>

namespace polygon4
{

template <class T>
class Vector
{
    using value_type = T;

public:
    template <class U>
    class iterator_base : public std::iterator<std::random_access_iterator_tag, U>
    {
    public:
        using base = std::iterator<std::random_access_iterator_tag, U>;

        using value_type = typename base::value_type;
        using difference_type = typename base::difference_type;
        using pointer = typename base::pointer;
        using reference = typename base::reference;

    public:
        iterator_base(pointer v) : p(v) {}

        iterator_base &operator++() { ++p; return *this; }
        iterator_base operator++(int) { iterator_base tmp(*this); operator++(); return tmp; }
        iterator_base &operator--() { --p; return *this; }
        iterator_base operator--(int) { iterator_base tmp(*this); operator--(); return tmp; }

        bool operator==(const iterator_base &rhs) const { return p == rhs.p; }
        bool operator!=(const iterator_base &rhs) const { return p != rhs.p; }
        bool operator <(const iterator_base &rhs) const { return p  < rhs.p; }

        difference_type operator-(const iterator_base &rhs) { return p - rhs.p; }
        iterator_base operator+(difference_type off) { return p + off; }
        iterator_base operator-(difference_type off) { return p - off; }
        
        reference operator*() { return *p; }
        reference operator->() { return *p; }

    private:
        pointer p;
    };

    using iterator = iterator_base<value_type>;
    using const_iterator = iterator_base<const value_type>;

public:
    void push_back(const value_type &t)
    {
        if (size == capacity)
        {
            capacity = capacity == 0 ? 1 : capacity * 2;
            auto data = std::shared_ptr<value_type>(new value_type[capacity], [](auto p) { delete[] p; });
            auto src = this->data.get();
            auto dst = data.get();
            for (size_t i = 0; i < size; i++)
                dst[i] = src[i];
            this->data = data;
        }
        data.get()[size++] = t;
    }

    bool empty() const { return size == 0; }

    iterator begin()
    {
        return iterator(&data.get()[0]);
    }
    iterator end()
    {
        return iterator(&data.get()[size]);
    }

    const_iterator begin() const
    {
        return iterator(&data.get()[0]);
    }
    const_iterator end() const
    {
        return iterator(&data.get()[size]);
    }

    iterator erase(iterator first, iterator last)
    {
        auto diff = last - first;
        auto iter = first;
        for (; last != end(); iter++, last++)
            *iter = *last;
        for (; iter != end(); iter++) // clean (destroy) elements
            *iter = value_type();
        size -= diff;
        return iter;
    }

private:
    std::shared_ptr<value_type> data;
    size_t size = 0;
    size_t capacity = 0;
};

} // namespace polygon4
