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

#include "Common.h"
#include "Enums.h"
#include "Localization.h"
#include "ObjectMap.h"
#include "Pointer.h"
#include "Schema.h"
#include "String.h"

#define POLYGON4_NONAME "NONAME"

namespace polygon4
{

namespace detail
{

using std::to_string;

struct TreeItem;

class DATA_MANAGER_API IObjectBase
{
private:
    static const int alloc_size = 1024;

    // constructors
public:
    IObjectBase() = default;
    IObjectBase(const IObjectBase &o);
    IObjectBase &operator=(const IObjectBase &o);
    IObjectBase(IObjectBase &&) = default;
    IObjectBase &operator=(IObjectBase &&) = default;
    virtual ~IObjectBase();

    // custom functions
public:
    int getId(int defaultValue = 0) const;
    void setId(int id_in);
    virtual bool hasId() const;

    virtual const Class &getClass() const = 0;

public:
#ifndef SWIG
    template <class T, class... Args>
    static auto create(Args&&... args)
    {
        static_assert(sizeof(T) <= alloc_size, "Object size is greater than maximum alloc size. Increase limit.");

        auto size = sizeof(T);
        if (T::replaceable)
            size = alloc_size;
        auto memory = new alloc_type[size];
        auto raw = new (memory) T(std::forward<Args>(args)...);
        auto p = std::unique_ptr<T, deleter>(raw, [](auto p) { delete[](alloc_type*)p; });
        return p;
    }

    template <class New, class Old, class... Args>
    static New *replace(Old *p, Args&&... args)
    {
        static_assert(sizeof(New) <= alloc_size, "Object size is greater than maximum alloc size. Increase limit.");

        if (p->replaced_)
            return (New *)p;
        Old o = *p;
        p->~Old();
        auto n = new (p) New(o, std::forward<Args>(args)...);
        p->replaced_ = true;
        return n;
    }
#endif

    // generated functions
public:
    virtual EObjectType getType() const = 0;
    virtual ::polygon4::String getVariableString(int columnId) const = 0;
    virtual void setVariableString(int columnId, const ::polygon4::String &text) = 0;
    virtual void setVariableString(int columnId, IObjectBase *ptr);
    virtual const IObjectBase *getVariable(int columnId) const;
    virtual EObjectType getVariableType(int columnId) const;
    virtual Ptr<TreeItem> printTree() const;
    virtual ::polygon4::String getName() const;
    virtual ::polygon4::String getTextId() const;
    virtual ::polygon4::String getDescription() const;

    virtual std::tuple<bool, OrderedObjectMap> getOrderedObjectMap(int columnId, class Storage *storage = nullptr) const;

#ifdef USE_UNREAL_EDITOR
    virtual UE4SPtrTreeViewItem printUE4TreeView() const = 0;
#endif

    bool operator<(const IObjectBase &rhs) const;

    void setStorage(Storage *storage);
    Storage *getStorage() const;

    Settings &getSettings();
    const Settings &getSettings() const;

    bool isReplaced() const;

protected:
    Ptr<TreeItem> createTreeItem(TreeItem *parent = nullptr) const;

    // data
protected:
    int id = 0;
private:
    Storage *storage_ = nullptr;
    bool replaced_ = false;

    void copyFrom(const IObjectBase &o);

    // static & friends
public:
    static bool replaceable;

    // friends
private:
    friend class StorageImpl;
    template <typename T> friend struct IdPtr;
};

inline auto to_string(const ::polygon4::String &s)
{
    return s;
}

inline auto to_wstring(const ::polygon4::String &s)
{
    return s;
}

bool to_bool(const std::string &s);

} // namespace detail

} // namespace polygon4

#include "detail/ForwardDeclarations.h"
#include "detail/Enums.h"

namespace polygon4
{

// forward decls for interfaces
class ScriptEngine;

namespace detail
{

#include "interface/IConfiguration.h"
#include "Interface/IConfigurationWeapon.h"
#include "interface/IGlider.h"
#include "interface/IMap.h"
#include "interface/IMapObject.h"
#include "interface/IMapBuilding.h"
#include "interface/IModification.h"
#include "Interface/IMechanoid.h"

} // namespace detail

} // namespace polygon4

#define DECLARE_INTERFACE_STUB(n) class DATA_MANAGER_API I ## n : public detail::IObjectBase {}
#include "detail/ObjectInterfaces.h"
#undef DECLARE_INTERFACE_STUB

#include "Table.h"
#include "detail/Types.h"
