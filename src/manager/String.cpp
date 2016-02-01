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

#include <Polygon4/DataManager/String.h>

#include <codecvt>
#include <locale>

inline auto& get_string_converter()
{
    static std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    return converter;
}

std::wstring string2wstring(const std::string &s)
{
    auto &converter = get_string_converter();
    return converter.from_bytes(s.c_str());
}

std::string wstring2string(const std::wstring &s)
{
    auto &converter = get_string_converter();
    return converter.to_bytes(s.c_str());
}

namespace polygon4
{

Blob::Blob()
{
}

Blob::Blob(const void *data, size_t length)
{
    copy(data, length);
}

Blob::Blob(const Blob &rhs)
{
    data = rhs.data;
    length = rhs.length;
}

Blob::~Blob()
{
}

Blob &Blob::operator=(const Blob &rhs)
{
    data = rhs.data;
    length = rhs.length;
    return *this;
}

void Blob::clear()
{
    data.reset();
    length = 0;
}

bool Blob::empty() const
{
    return length == 0;
}

bool Blob::operator==(const Blob &rhs) const
{
    if (data == rhs.data)
        return true;
    if (!data || !rhs.data)
        return false;
    if (length != rhs.length)
        return false;
    return memcmp(data.get(), rhs.data.get(), length) == 0;
}

bool Blob::operator!=(const Blob &rhs) const
{
    return !operator==(rhs);
}

Blob::data_type Blob::getData() const { return data; }
Blob::element_type *Blob::getRawData() const { return data.get(); }
size_t Blob::getLength() const { return length; }

void Blob::copy(const void *bytes, size_t len)
{
    auto s = new element_type[len];
    memcpy(s, bytes, len);
    data = data_type(s, [](element_type *s) { delete[] s; });
    length = len;
}

} // namespace polygon4
