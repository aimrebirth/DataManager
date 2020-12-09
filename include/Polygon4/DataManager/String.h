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

#include <algorithm>
#include <functional>
#include <memory>
#include <string>

#ifdef USE_QT
#include <qstring.h>
#endif

#ifdef __UNREAL__
//#include <Text.h>
#endif

DATA_MANAGER_API std::wstring string2wstring(const std::string &s);
DATA_MANAGER_API std::string wstring2string(const std::wstring &s);

DATA_MANAGER_API std::u16string string2u16string(const std::string &s);
DATA_MANAGER_API std::string u16string2string(const std::u16string &s);

namespace polygon4
{

namespace detail
{

using base_u8string = ::std::string;

using base_string = ::std::u16string;
using base_char = char16_t;
inline auto utf8to_base_string(const base_u8string &s) { return ::string2u16string(s); }
inline auto base_string_to_utf8(const base_string &s) { return ::u16string2string(s); }

//using base_string = ::std::wstring;
//using base_char = wchar_t;
//inline auto utf8to_base_string(const base_u8string &s) { return ::string2wstring(s); }
//inline auto base_string_to_utf8(const base_string &s) { return ::wstring2string(s); }

}

class String : public detail::base_string
{
public:
    using base = detail::base_string;

public:
    String() = default;
    String(const String &) = default;
    String &operator=(const String &s) = default;
    String(String &&) = default;
    String &operator=(String &&s) = default;
    ~String() = default;

    // string
    String(const char *s)
    {
        if (s == nullptr)
            return;
        *this = String(detail::base_u8string(s));
    }
    String(const unsigned char *s)
        : String((const char *)s)
    {
    }
    String(const detail::base_u8string &s)
        : base(detail::utf8to_base_string(s))
    {
    }
    String &operator=(const char *s)
    {
        assign(s);
        return *this;
    }
    String &operator=(const detail::base_u8string &s)
    {
        assign(s);
        return *this;
    }
    detail::base_u8string toString() const
    {
        return detail::base_string_to_utf8(*this);
    }
    operator detail::base_u8string() const { return toString(); }

    // wstring
    String(const detail::base_char *s)
    {
        if (s == nullptr)
            return;
        *this = String(base(s));
    }
#ifdef _WIN32
    String(const wchar_t *s)
    {
        if (s == nullptr)
            return;
        *this = wstring2string(s);
    }
#endif
    String(const base &s)
        : base(s)
    {
    }
    String &operator=(const detail::base_char *s)
    {
        assign(s);
        return *this;
    }
#ifdef _WIN32
    String &operator=(const wchar_t *s)
    {
        assign(s);
        return *this;
    }
#endif
    String &operator=(const base &s)
    {
        assign(s);
        return *this;
    }

    operator const detail::base_char *() const { return c_str(); }
#ifdef _WIN32
    operator const wchar_t *() const { return (const wchar_t *)c_str(); }
#endif

#ifdef USE_QT
    String(const QString &s)
        : String(s.toStdString())
    {
    }
    String &operator=(const QString &s)
    {
        assign(s);
        return *this;
    }
    QString toQString() const
    {
        return QString::fromStdString(*this);
    }
    operator QString() const { return toQString(); }
#endif

#ifdef __UNREAL__
    String(const FString &s)
        : base((const char16_t *)s.GetCharArray().GetData())
    {}
    String &operator=(const FString &s)
    {
        assign(s);
        return *this;
    }
    FString toFString() const { return (const wchar_t*)c_str(); }
    operator FString() const { return toFString(); }

    String(const FText &s)
    {
        if (!s.IsEmpty())
            *this = String(s.ToString());
    }
    String &operator=(const FText &s)
    {
        assign(s);
        return *this;
    }
    FText toFText() const { return FText::FromString(toFString()); }
    operator FText() const { return toFText(); }
#endif

    bool operator==(const String &s) const
    {
        return compare(s) == 0;
    }
    bool operator!=(const String &s) const
    {
        return !operator==(s);
    }
    bool operator<(const String &s) const
    {
        return compare(s) < 0;
    }
    bool operator>(const String &s) const
    {
        return compare(s) > 0;
    }

    String operator+(const char *s) const
    {
        if (s == nullptr)
            return *this;
        String tmp = *this;
        tmp.base::operator+=(String(s));
        return tmp;
    }
    String operator+(const detail::base_char *s) const
    {
        if (s == nullptr)
            return *this;
        String tmp = *this;
        tmp.base::operator+=(String(s));
        return tmp;
    }
#ifdef _WIN32
    String operator+(const wchar_t *s) const
    {
        if (s == nullptr)
            return *this;
        String tmp = *this;
        tmp.base::operator+=(String(s));
        return tmp;
    }
#endif
    String operator+(const String &s) const
    {
        String tmp = *this;
        tmp.base::operator+=(s);
        return tmp;
    }
    String &operator+=(const detail::base_u8string &s)
    {
        base::operator+=(String(s));
        return *this;
    }
    String &operator+=(String &&s)
    {
        base::operator+=(std::move(s));
        return *this;
    }

private:
    template <typename T>
    String &assign(const T &s)
    {
        String tmp(s);
        std::swap(*this, tmp);
        return *this;
    }

    int compare(const String &s) const
    {
        return base::compare(s);
    }
};

class DATA_MANAGER_API Blob
{
public:
    using element_type = uint8_t;
    using data_type = std::shared_ptr<element_type>;

public:
    Blob();
    Blob(const void *data, size_t length);
    Blob(const Blob &rhs);
    ~Blob();

    Blob &operator=(const Blob &rhs);
    Blob &operator=(const String &rhs) { return *this; }

    bool empty() const;

    bool operator==(const Blob &rhs) const;
    bool operator!=(const Blob &rhs) const;

    void clear();

    data_type getData() const;
    element_type *getRawData() const;
    size_t getLength() const;

private:
    data_type data;
    size_t length = 0;

    void copy(const void *bytes, size_t len);
};

}

namespace std
{
    template<>
    struct hash<polygon4::String>
    {
        std::size_t operator()(const polygon4::String &s) const
        {
            return hash<typename polygon4::String::base>()(s);
        }
    };
}

using P4String = ::polygon4::String;
