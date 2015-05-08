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

#include <map>
#include <memory>
#include <set>
#include <stdint.h>
#include <vector>

namespace polygon4
{

template <class T>
class Vector
{
public:
	Vector(){}
	~Vector(){}

	void push_back(const T &t)
	{
		if (size == capacity)
		{
			if (capacity == 0)
				capacity = 1;
			else
				capacity *= 2;
			std::shared_ptr<T*> data = std::shared_ptr<T*>(new T*[capacity], [](T **ptr){ delete[] ptr; });
			for (size_t i = 0; i < size; i++)
				data.get()[i] = this->data.get()[i];
			this->data = data;
		}
		data.get()[size++] = new T(t);
	}

	std::set<T> set() const
	{
		std::set<T> s;
		for (size_t i = 0; i < size; i++)
			s.insert(*data.get()[i]);
		return s;
	}

private:
	std::shared_ptr<T*> data;
	size_t size = 0;
	size_t capacity = 0;
};

} // namespace polygon4
