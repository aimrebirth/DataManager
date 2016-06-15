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
#include <map>
#include <memory>
#include <stdint.h>
#include <vector>
#include <unordered_map>

#include "Common.h"
#include "Enums.h"
#include "Exception.h"
#include "Localization.h"
#include "Schema.h"
#include "String.h"

#define POLYGON4_NONAME "NONAME"

namespace polygon4
{

struct Settings;

namespace detail
{

using std::to_string;

using P4String = ::polygon4::String;

using IdType = int;

template <class T>
using Ptr = std::shared_ptr<T>;

class IObjectBase;

using OrderedObjectMap = std::multimap< ::polygon4::String, IObjectBase*>;

template <typename K>
using KeyMap = std::unordered_map<K, IObjectBase*>;

using ProgressCallback = std::function<void(double)>;

using alloc_type = uint8_t;
using deleter = void(*)(IObjectBase*);

using ObjectPtr = std::unique_ptr<IObjectBase, deleter>;

struct DLL_EXPORT TreeItem
{
    ::polygon4::String name;
    ::polygon4::String defaultName;
    EObjectType type = EObjectType::None;
    IObjectBase *object = nullptr;
    TreeItem *parent = nullptr;
    std::vector<Ptr<TreeItem>> children;
    ObjectFlags flags;
    void *objectArrayVariable = nullptr;
    void *inlineVariable = nullptr;

    void *guiItem = nullptr;

    int child_count() const;
    void remove() const;
    void update();
    ::polygon4::String get_name();
};

class DLL_EXPORT IObjectBase
{
private:
    static const int alloc_size = 1024;

    // constructors
public:
    IObjectBase() = default;
    IObjectBase(const IObjectBase &) = default;
    IObjectBase &operator=(const IObjectBase &) = default;
    IObjectBase(IObjectBase &&) = default;
    IObjectBase &operator=(IObjectBase &&) = default;
    virtual ~IObjectBase() {}

    // custom functions
public:
    int getId(int defaultValue = 0) const { return hasId() ? id : defaultValue; }
    void setId(int id) { this->id = id; }
    virtual bool hasId() const { return false; }

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
    virtual void setVariableString(int columnId, IObjectBase *ptr) {}
    virtual const IObjectBase *getVariable(int columnId) const { return nullptr; }
    virtual EObjectType getVariableType(int columnId) const { return EObjectType::None; }
    virtual Ptr<TreeItem> printTree() const { return createTreeItem(); }
    virtual ::polygon4::String getName() const { return POLYGON4_NONAME; }
    virtual ::polygon4::String getTextId() const { return "NO_TEXT_ID"; }
    virtual ::polygon4::String getDescription() const { return "NO_DESCRIPTION"; }

    virtual std::tuple<bool, OrderedObjectMap> getOrderedObjectMap(int columnId, class Storage *storage = nullptr) const;

#ifdef USE_UNREAL_EDITOR
    virtual UE4SPtrTreeViewItem printUE4TreeView() const = 0;
#endif

    bool operator<(const IObjectBase &rhs) const { return getName() < rhs.getName(); }

    void setStorage(Storage *storage) { storage_ = storage; }
    Storage *getStorage() const { return storage_; }

    Settings &getSettings();
    const Settings &getSettings() const;

    bool isReplaced() const { return replaced_; }

protected:
    Ptr<TreeItem> createTreeItem(TreeItem *parent = nullptr) const
    {
        auto item = std::make_shared<TreeItem>();
        item->parent = parent;
        item->object = const_cast<IObjectBase*>(this);
        item->update();
        return item;
    }

    // data
protected:
    int id = 0;
private:
    Storage *storage_ = nullptr;
    bool replaced_ = false;

    // static & friends
public:
    static bool replaceable;

    // friends
private:
    friend class StorageImpl;
    template <typename T> friend struct IdPtr;
};

template <typename T>
struct IdPtr
{
    using base_type = IObjectBase;
    using value_type = T;

    IdType id = 0;
    base_type *ptr = nullptr;

    IdPtr()
    {
    }
    IdPtr(IdType id)
        : id(id)
    {
    }
    IdPtr(base_type *p)
        : ptr(p)
    {
        if (ptr)
            id = ptr->id;
    }
    IdPtr(const Ptr<T> &p)
        : IdPtr(p.get())
    {
    }

    IdPtr &operator=(IdType i)
    {
        id = i;
        return *this;
    }
    IdPtr &operator=(const IdPtr &rhs)
    {
        *this = rhs.ptr;
        return *this;
    }
    template <class V>
    IdPtr &operator=(const IdPtr<V> &rhs)
    {
        id = rhs.id;
        return *this = rhs.ptr;
    }
    IdPtr &operator=(const Ptr<T> &rhs)
    {
        return *this = rhs.get();
    }
    IdPtr &operator=(const base_type *rhs)
    {
        ptr = rhs;
        if (ptr)
            id = ptr->id;
        return *this;
    }
    IdPtr &operator=(base_type *rhs)
    {
        ptr = rhs;
        if (ptr)
            id = ptr->id;
        return *this;
    }

    T *operator->() const
    {
        return (T *)&this->operator*();
    }
    T &operator*() const
    {
        if (ptr == nullptr)
            throw EXCEPTION("Value is not present");
        return (T &)*ptr;
    }
    T *get() const
    {
        return (T *)ptr;
    }

    bool operator==(const IdPtr &rhs) const
    {
        return id == rhs.id && ptr == rhs.ptr;
    }
    bool operator==(const T &rhs) const
    {
        return ptr == &rhs;
    }

    operator bool() const
    {
        return ptr != nullptr;
    }
    operator T*() const
    {
        return (T *)ptr;
    }

    void reset()
    {
        clear();
    }
    void clear()
    {
        id = 0;
        ptr = nullptr;
    }
};

inline auto to_string(const ::polygon4::String &s)
{
    return s;
}

inline auto to_wstring(const ::polygon4::String &s)
{
    return s;
}

template<class T>
inline ::polygon4::String to_string(const Ptr<T> &ptr)
{
    if (ptr)
        return ptr->getName();
    return ::polygon4::String();
}

template<class T>
inline ::polygon4::String to_wstring(const Ptr<T> &ptr)
{
    if (ptr)
        return ptr->getName();
    return ::polygon4::String();
}

template<class T>
inline ::polygon4::String to_string(const IdPtr<T> &ptr)
{
    if (ptr.ptr)
        return ptr.ptr->getName();
    return ::polygon4::String();
}

template<class T>
inline ::polygon4::String to_wstring(const IdPtr<T> &ptr)
{
    if (ptr.ptr)
        return ptr.ptr->getName();
    return ::polygon4::String();
}

bool to_bool(const std::string &s);

} // namespace detail

using detail::IdPtr;
using detail::Ptr;
using detail::TreeItem;
using detail::KeyMap;

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

#define DECLARE_INTERFACE_STUB(n) class DLL_EXPORT I ## n : public detail::IObjectBase {}
#include "detail/ObjectInterfaces.h"
#undef DECLARE_INTERFACE_STUB

#include "Table.h"
#include "detail/Types.h"
