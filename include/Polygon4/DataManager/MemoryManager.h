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
#include <vector>

class MemoryManager
{
    class Base
    {
    public:
        virtual ~Base(){}
    };

    template <typename T>
    class Derived : public Base
    {
    public:
        template <class ... Args>
        T *create(Args&&... args)
        {
            p = std::make_unique<T>(std::forward<Args>(args)...);
            return p.get();
        }
    private:
        std::unique_ptr<T> p;
    };

    using value_type = std::unique_ptr<Base>;

public:
    ~MemoryManager()
    {
        clear();
    }

    template <class T, class ... Args>
    T *create(Args&&... args)
    {
        auto d = std::make_unique<Derived<T>>();
        auto p = d->create(std::forward<Args>(args)...);
        m.push_back(std::move(d));
        return p;
    }

    void clear()
    {
        m.clear();
    }

private:
    std::vector<value_type> m;
};
