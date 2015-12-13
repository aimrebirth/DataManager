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

namespace detail
{

using std::to_string;

using Text = String;

template <class T>
using Ptr = std::shared_ptr<T>;

class IObjectBase;

using OrderedObjectMap = std::multimap<Text, IObjectBase *>;

struct DLL_EXPORT TreeItem
{
    Text name;
    EObjectType type = EObjectType::None;
    IObjectBase *object = nullptr;
    TreeItem *parent = nullptr;
    std::vector<Ptr<TreeItem>> children;
    ObjectFlags flags;
    void *objectArrayVariable = nullptr;

    int child_count() const;
    void remove() const;
    void update();
};

class DLL_EXPORT IObjectBase
{
public:

private:
    using alloc_type = uint8_t;
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

protected:
    virtual void initChildren() {}

public:
    template <class T, class... Args>
    static std::shared_ptr<T> create(Args&&... args)
    {
        static_assert(sizeof(T) <= alloc_size, "Object size is greater than maximum alloc size. Increase limit.");

        auto memory = new alloc_type[alloc_size];
        auto raw = new (memory) T(std::forward<Args>(args)...);
        auto p = std::shared_ptr<T>(raw, [](auto p) { delete[](alloc_type*)p; });
        p->initChildren();
        return p;
    }

    template <class New, class Old, class... Args>
    static New *replace(Old *p, Args&&... args)
    {
        static_assert(
            sizeof(New) <= alloc_size &&
            sizeof(New) >= sizeof(Old),
            "Object size is greater than maximum alloc size. Increase limit.");

        Old o = *p;
        p->~Old();
        auto n = new (p) New(o, std::forward<Args>(args)...);
        n->initChildren();
        return n;
    }

    // generated functions
public:
    virtual EObjectType getType() const = 0;
    virtual Text getVariableString(int columnId) const = 0;
    virtual void setVariableString(int columnId, const Text &text) = 0;
    virtual void setVariableString(int columnId, IObjectBase *ptr) {}
    virtual const IObjectBase *getVariable(int columnId) const { return nullptr; }
    virtual EObjectType getVariableType(int columnId) const { return EObjectType::None; }
    virtual Ptr<TreeItem> printTree() const { return createTreeItem(); }
    virtual Text getName() const { return POLYGON4_NONAME; }

    virtual std::tuple<bool, OrderedObjectMap> getOrderedObjectMap(int columnId, class Storage *storage = nullptr) const;

#ifdef USE_UNREAL_EDITOR
    virtual UE4SPtrTreeViewItem printUE4TreeView() const = 0;
#endif

    bool operator<(const IObjectBase &rhs) const { return getName() < rhs.getName(); }

    // data
protected:
    int id = 0;

    Ptr<TreeItem> createTreeItem(TreeItem *parent = nullptr) const
    {
        auto item = std::make_shared<TreeItem>();
        item->parent = parent;
        item->object = const_cast<IObjectBase*>(this);
        item->update();
        return item;
    }
    
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

    int id = 0;
    base_type *ptr = nullptr;

    IdPtr()
    {
    }
    IdPtr(int id)
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

    IdPtr &operator=(int i)
    {
        id = i;
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

    void clear()
    {
        id = 0;
        ptr = nullptr;
    }
};

template <typename T>
using CObjectArray = std::vector<IdPtr<T>>;

using ProgressCallback = std::function<void(double)>;

template<class T>
inline Text to_string(const Ptr<T> &ptr)
{
    if (ptr)
        return ptr->getName();
    return Text();
}

template<class T>
inline Text to_wstring(const Ptr<T> &ptr)
{
    if (ptr)
        return ptr->getName();
    return Text();
}

template<class T>
inline Text to_string(const IdPtr<T> &ptr)
{
    if (ptr.ptr)
        return ptr.ptr->getName();
    return Text();
}

template<class T>
inline Text to_wstring(const IdPtr<T> &ptr)
{
    if (ptr.ptr)
        return ptr.ptr->getName();
    return Text();
}

bool to_bool(const std::string &s);

} // namespace detail

using detail::IdPtr;
using detail::Ptr;
using detail::TreeItem;

} // namespace polygon4

#include "detail/ForwardDeclarations.h"

namespace polygon4
{

#include "interface/IMap.h"
#include "interface/IMapObject.h"
#include "interface/IMapBuilding.h"
#include "interface/IModification.h"
#include "Interface/IMechanoid.h"

} // namespace polygon4

#define DECLARE_INTERFACE_STUB(n) class DLL_EXPORT I ## n : public detail::IObjectBase {}
#include "detail/ObjectInterfaces.h"
#undef DECLARE_INTERFACE_STUB

#include "Table.h"
#include "detail/Enums.h"
#include "detail/Types.h"
