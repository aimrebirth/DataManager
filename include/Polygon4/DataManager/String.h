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

#include <functional>
#include <memory>
#include <string>

#ifdef USE_QT
#include <qstring.h>
#endif

#ifdef __UNREAL__
#include <Text.h>
#endif

#include "Dll.h"

DLL_EXPORT std::wstring string2wstring(const std::string &s);
DLL_EXPORT std::string wstring2string(const std::wstring &s);

namespace polygon4
{

class DLL_EXPORT Data
{
public:
    using element_type = uint8_t;
    using data_type = std::shared_ptr<element_type>;

    Data();

    data_type d;
};

class DLL_EXPORT String
{
public:
    String();
    String(const char *s);
    String(const wchar_t *s);
    String(const std::string &s);
    String(const std::wstring &s);
#ifdef USE_QT
    String(const QString &s);
#endif
#ifdef __UNREAL__
    String(const FString &s)
        : String(s.GetCharArray().GetData())
    {
    }
    String(const FText &s)
        : String(s.ToString())
    {
    }
#endif
    String &operator=(const char *s);
    String &operator=(const wchar_t *s);
    String &operator=(const std::string &s);
    String &operator=(const std::wstring &s);
#ifdef USE_QT
    String &operator=(const QString &s);
#endif
#ifdef __UNREAL__
    String &operator=(const FString &s)
    {
        String tmp(s);
        std::swap(*this, tmp);
        return *this;
    }
    String &operator=(const FText &s)
    {
        String tmp(s);
        std::swap(*this, tmp);
        return *this;
    }
#endif
    ~String();

#ifdef USE_QT
    QString toQString() const;
#endif

#ifdef __UNREAL__
    FString toFString() const
    {
        return wstring().c_str();
    }

    FText toFText() const
    {
        return FText::FromString(toFString());
    }
#endif

    void clear();
    bool empty() const;

    Data getData() const;
    int getMultiplier() const;
    size_t getLength() const;

    bool operator<(const String &rhs) const;
    bool operator==(const String &rhs) const;
    bool operator!=(const String &rhs) const;

    friend String operator+(const String &lhs, const String &rhs);

private:
    Data data;
    size_t length = 0;
    int multiplier = 1;
    
    void copy(const void *bytes, size_t len, size_t mult = 1);
    std::string string() const;
    std::wstring wstring() const;

    friend struct StringHash;
};

struct DLL_EXPORT StringHash
{
    std::size_t operator()(const polygon4::String &s) const;
};

class DLL_EXPORT Blob
{
public:
    Blob();
    Blob(const void *data, size_t length);
    Blob(const Blob &rhs);
    ~Blob();
    
    Blob &operator=(const Blob &rhs);
    Blob &operator=(const String &s);

    bool empty() const;

    bool operator==(const Blob &rhs) const;
    bool operator!=(const Blob &rhs) const;

    void clear();

    Data getData() const;
    Data::element_type *getRawData() const;
    size_t getLength() const;

private:
    Data data;
    size_t length = 0;
    
    void copy(const void *bytes, size_t len);
};

}


namespace std
{

inline std::string to_string(const polygon4::String &s)
{
    if (s.getData().d.get() == 0)
        return std::string();
    if (s.getMultiplier() == 1)
        return (const char *)s.getData().d.get();
    if (s.getMultiplier() == 2)
        return wstring2string((const wchar_t *)s.getData().d.get());
    return std::string();
}

inline std::wstring to_wstring(const polygon4::String &s)
{
    if (s.getData().d.get() == 0)
        return std::wstring();
    if (s.getMultiplier() == 1)
        return string2wstring((const char *)s.getData().d.get());
    if (s.getMultiplier() == 2)
        return (const wchar_t *)s.getData().d.get();
    return std::wstring();
}

}

using std::to_string;
using std::to_wstring;
