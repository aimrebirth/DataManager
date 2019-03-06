/*
 * Polygon-4 Data Manager
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

#include <Polygon4/DataManager/Exception.h>

#include <primitives/log.h>
DECLARE_STATIC_LOGGER(logger, "exception");

namespace polygon4
{

Exception::Exception()
{
}

Exception::Exception(const char *file, const char *function, int line, const std::string &msg)
{
    std::stringstream ss;
    ss << "Exception in file: " << file << ", function: " << function << ", line: " << line << ". "
        << "Error description: " << msg;

    what_str = ss.str();

    LOG_ERROR(logger, what_str);
}

Exception::~Exception()
{
}

const char *Exception::what() const noexcept
{
    return what_str.c_str();
}

}
