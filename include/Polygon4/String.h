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
#include <string>

#include <codecvt>
#include <locale>
#include <string>

#ifdef USE_QT
#include <qstring.h>
#endif

inline std::wstring to_wstring(std::string s)
{
    static std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    return converter.from_bytes(s.c_str());
}
inline std::wstring to_wstring(const char *s)
{
    return to_wstring(std::string(s));
}

inline std::string to_string(std::wstring s)
{
    static std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    return converter.to_bytes(s.c_str());
}
inline std::string to_string(const wchar_t *s)
{
    return to_string(std::wstring(s));
} 

namespace polygon4
{

class String
{
public:
    String()
    {
    }
    String(const char *s)
    {
        if (s)
            copy(s, strlen(s) + 1);
        else
            clear();
    }
    String(const wchar_t *s)
    {
        if (s)
            copy(s, wcslen(s) + 1, 2);
        else
            clear();
    }
    String(const std::string &s)
    {
        copy(s.c_str(), s.size() + 1);
    }
    String(const std::wstring &s)
    {
        copy(s.c_str(), s.size() + 1, 2);
    }
#ifdef USE_QT
    String(const QString &s)
        : String(s.toStdWString())
    {
    }
#endif
    String &operator=(const char *s)
    {
        if (s)
            copy(s, strlen(s) + 1);
        else
            clear();
        return *this;
    }
    String &operator=(const wchar_t *s)
    {
        if (s)
            copy(s, wcslen(s) + 1, 2);
        else
            clear();
        return *this;
    }
    String &operator=(const std::string &s)
    {
        copy(s.c_str(), s.size() + 1);
        return *this;
    }
    String &operator=(const std::wstring &s)
    {
        copy(s.c_str(), s.size() + 1, 2);
        return *this;
    }
#ifdef USE_QT
    String &operator=(const QString &s)
    {
        return *this = s.toStdWString();
    }
#endif

    ~String()
    {

    }
    
    std::string string() const
    {
        if (data.get() == 0)
            return std::string();
        if (multiplier == 1)
            return (const char *)data.get();
        if (multiplier == 2)
            return to_string((const wchar_t *)data.get());
        return 0;
    }
    std::wstring wstring() const
    {
        if (data.get() == 0)
            return std::wstring();
        if (multiplier == 1)
            return to_wstring((const char *)data.get());
        if (multiplier == 2)
            return (const wchar_t *)data.get();
        return 0;
    }
    operator std::string() const
    {
        return string();
    }
    operator std::wstring() const
    {
        return wstring();
    }
    operator char*() const
    {
        return (char *)data.get();
    }
    operator wchar_t*() const
    {
        return (wchar_t *)data.get();
    }
#ifdef USE_QT
    operator QString() const
    {
        return QString::fromStdWString(*this);
    }
#endif

    bool empty() const
    {
        return length == 0 || length == multiplier;
    }

    bool operator<(const String &rhs) const
    {
        if (multiplier == 1)
            return this->string() < rhs.string();
        if (multiplier == 2)
            return this->wstring() < rhs.wstring();
        return true;
    }
    bool operator==(const String &rhs) const
    {
        if (data == rhs.data)
            return true;
        if (!data || !rhs.data)
            return false;
        if (multiplier == 1 && rhs.multiplier == 1)
            return strcmp(data.get(), rhs.data.get()) == 0;
        return wcscmp((wchar_t *)data.get(), (wchar_t *)rhs.data.get()) == 0;
    }
    bool operator!=(const String &rhs) const
    {
        return !operator==(rhs);
    }

    void clear()
    {
        data.reset();
        length = 0;
        multiplier = 1;
    }

    // operator +
    String &operator+(const String &rhs)
    {
        if (rhs.length == 0)
            return *this;
        std::wstring s = wstring() + rhs.wstring();
        copy(s.c_str(), s.size() + 1, 2);
        return *this;
    }
    String &operator+(const std::wstring &rhs)
    {
        if (rhs.empty())
            return *this;
        std::wstring s = wstring() + rhs;
        copy(s.c_str(), s.size() + 1, 2);
        return *this;
    }
    String &operator+(const std::string &rhs)
    {
        if (rhs.empty())
            return *this;
        std::wstring s = wstring() + to_wstring(rhs);
        copy(s.c_str(), s.size() + 1, 2);
        return *this;
    }

private:
    std::shared_ptr<char> data;
    size_t length = 0;
    int multiplier = 1;
    
    void copy(const void *data, size_t length, size_t multiplier = 1)
    {
        length *= multiplier;
        auto s = new char[length];
        memcpy(s, data, length);
        this->data = std::shared_ptr<char>(s, [](char *s){ delete[] s; });
        this->length = length;
        this->multiplier = multiplier;
    }
};

inline std::string to_string(const String &s)
{
    return s.string();
}

inline std::wstring to_wstring(const String &s)
{
    return s.wstring();
}

}
