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

#include <Polygon4/DataManager/String.h>

#include <codecvt>
#include <locale>

static std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;

std::wstring string2wstring(const std::string &s)
{
    return converter.from_bytes(s.c_str());
}

std::string wstring2string(const std::wstring &s)
{
    return converter.to_bytes(s.c_str());
}

namespace polygon4
{

Data::Data()
    : d(nullptr, [](element_type *p) { delete[]p; })
{
}


String::String()
{
}

String::String(const char *s)
{
    if (s)
        copy(s, strlen(s) + 1);
    else
        clear();
}

String::String(const wchar_t *s)
{
    if (s)
        copy(s, wcslen(s) + 1, 2);
    else
        clear();
}

String::String(const std::string &s)
{
    copy(s.c_str(), s.size() + 1);
}

String::String(const std::wstring &s)
{
    copy(s.c_str(), s.size() + 1, 2);
}

#ifdef USE_QT
String::String(const QString &s)
    : String(s.toStdWString())
{
}
#endif

String &String::operator=(const char *s)
{
    if (s)
        copy(s, strlen(s) + 1);
    else
        clear();
    return *this;
}

String &String::operator=(const wchar_t *s)
{
    if (s)
        copy(s, wcslen(s) + 1, 2);
    else
        clear();
    return *this;
}

String &String::operator=(const std::string &s)
{
    copy(s.c_str(), s.size() + 1);
    return *this;
}

String &String::operator=(const std::wstring &s)
{
    copy(s.c_str(), s.size() + 1, 2);
    return *this;
}

#ifdef USE_QT
String &String::operator=(const QString &s)
{
    return *this = s.toStdWString();
}
#endif

String::~String()
{
}

#ifdef USE_QT
QString String::toQString() const
{
    return QString::fromStdWString(to_wstring(*this));
}
#endif

void String::clear()
{
    data.d.reset();
    length = 0;
    multiplier = 1;
}

bool String::empty() const
{
    return length == 0 || length == multiplier;
}

bool String::operator<(const String &rhs) const
{
    if (multiplier == 1)
        return this->string() < rhs.string();
    if (multiplier == 2)
        return this->wstring() < rhs.wstring();
    return true;
}

bool String::operator==(const String &rhs) const
{
    if (data.d == rhs.data.d)
        return true;
    if (!data.d || !rhs.data.d)
        return false;
    if (multiplier == 1 && rhs.multiplier == 1)
        return strcmp((const char *)data.d.get(), (const char *)rhs.data.d.get()) == 0;
    if (multiplier != rhs.multiplier)
    {
        if (multiplier == 1)
            return rhs == wstring();
        return *this == rhs.wstring();
    }
    return wcscmp((const wchar_t *)data.d.get(), (const wchar_t *)rhs.data.d.get()) == 0;
}

bool String::operator!=(const String &rhs) const
{
    return !operator==(rhs);
}

String operator+(const String &lhs, const String &rhs)
{
    if (rhs.length == 0)
        return lhs;
    auto s = lhs.wstring() + rhs.wstring();
    return String(s);
}

Data String::getData() const { return data; }
int String::getMultiplier() const { return multiplier; }
size_t String::getLength() const { return length; }

void String::copy(const void *bytes, size_t len, size_t mult)
{
    len *= mult;
    if (len == mult)
    {
        // empty
        this->data.d.reset();
        return;
    }
    auto s = new Data::element_type[len];
    memcpy(s, bytes, len);
    data.d = Data::data_type(s, [](auto s) { delete[] s; });
    length = len;
    multiplier = mult;
}

std::string String::string() const
{
    return std::to_string(*this);
}

std::wstring String::wstring() const
{
    return std::to_wstring(*this);
}


std::size_t StringHash::operator()(const polygon4::String &s) const
{
    if (s.multiplier == 1)
        return std::hash<std::string>()(s.string());
    if (s.multiplier == 2)
        return std::hash<std::wstring>()(s.wstring());
    return 0;
}


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
Blob &Blob::operator=(const String &s)
{
    data = s.getData();
    length = s.getLength();
    return *this;
}

void Blob::clear()
{
    data.d.reset();
    length = 0;
}

bool Blob::empty() const
{
    return length == 0;
}

bool Blob::operator==(const Blob &rhs) const
{
    if (data.d == rhs.data.d)
        return true;
    if (!data.d || !rhs.data.d)
        return false;
    if (length != rhs.length)
        return false;
    return memcmp(data.d.get(), rhs.data.d.get(), length) == 0;
}

bool Blob::operator!=(const Blob &rhs) const
{
    return !operator==(rhs);
}

Data Blob::getData() const { return data; }
Data::element_type *Blob::getRawData() const { return data.d.get(); }
size_t Blob::getLength() const { return length; }

void Blob::copy(const void *bytes, size_t len)
{
    auto s = new Data::element_type[len];
    memcpy(s, bytes, len);
    data.d = Data::data_type(s, [](Data::element_type *s) { delete[] s; });
    length = len;
}
    
} // namespace polygon4
