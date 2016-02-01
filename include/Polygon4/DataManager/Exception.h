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

#include <exception>
#include <sstream>
#include <string>

#include "Dll.h"

#define EXCEPTION_PARAMS __FILE__, __FUNCTION__, __LINE__
#define EXCEPTION(msg) polygon4::Exception(EXCEPTION_PARAMS, msg)

namespace polygon4
{

class DLL_EXPORT Exception : public std::exception
{
public:
    Exception();
    Exception(const char *file, const char *function, int line, const std::string &msg);
    virtual ~Exception();

    virtual const char *what() const noexcept;

protected:
    std::string what_str;
};

} //namepsace polygon4
