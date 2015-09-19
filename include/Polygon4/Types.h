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

#include <functional>
#include <map>
#include <memory>
#include <stdint.h>
#include <vector>

#include "Exception.h"
#include "String.h"

#include "detail/ObjectTypes.h"

#ifdef USE_QT
#include <qobject.h>

class QTreeWidgetItem;
#endif

#define POLYGON4_NONAME "NONAME"

namespace polygon4
{

extern int gCurrentLocalizationId;

namespace detail
{

using std::to_string;

typedef String Text;
typedef uint8_t EnumType;

template <typename T>
using Ptr = std::shared_ptr<T>;
template <typename T>
using CVector = std::vector<Ptr<T>>;
using OrderedObjectMap = std::multimap<Text, Ptr<class IObject>>;
using ProgressCallback = std::function<void(double)>;

template <class T>
struct IdPtr
{
    int id = 0;
    Ptr<T> ptr;

    bool operator==(const IdPtr<T> &rhs) const
    {
        if (ptr == rhs.ptr)
            return true;
        if (!ptr || !rhs.ptr)
            return false;
        return *ptr.get() == *rhs.ptr.get();
    }

    IdPtr &operator=(const Ptr<T> &item)
    {
        if (!item)
            return *this = IdPtr<T>();
        id = item->id;
        ptr = item;
        return *this;
    }

    Ptr<T> operator->() const
    {
        if (ptr)
            return ptr;
        throw EXCEPTION("Value is missing for id (" + std::to_string(id) + ")");
    }

    operator bool() const
    {
        return !!ptr;
    }

    operator Ptr<T>() const
    {
        return ptr;
    }
};

template<class T>
inline Text to_string(IdPtr<T> ptr)
{
    if (ptr)
        return to_string(ptr->getName());
    return Text();
}

class IObject
{
public:
    virtual ~IObject() {}

    virtual EObjectType getType() const = 0;
    virtual Text getVariableString(int columnId) const = 0;
    virtual void setVariableString(int columnId, Text text, Ptr<IObject> ptr = Ptr<IObject>()) = 0;
    virtual Text getName() const
    {
        return POLYGON4_NONAME;
    }
#ifdef USE_QT
    virtual QTreeWidgetItem *printQtTreeView(QTreeWidgetItem *parent) const = 0;
#endif

    bool operator<(const IObject &rhs) const
    {
        return getName() < rhs.getName();
    }

private:
    friend class StorageImpl;
    template <class T> friend struct IdPtr;
};

} // namespace detail

using detail::Ptr;

} // namespace polygon4

#include "detail/Types.h"
