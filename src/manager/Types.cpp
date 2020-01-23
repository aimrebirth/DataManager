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

#include <Polygon4/DataManager/Types.h>

#include <Polygon4/DataManager/Storage.h>
#include <Polygon4/DataManager/TreeItem.h>

namespace polygon4
{

namespace detail
{

bool IObjectBase::replaceable = false;

int TreeItem::child_count() const
{
    int cc = 0;
    for (auto &c : children)
    {
        if (type == EObjectType::InlineVariables)
        {
            cc++;
            continue;
        }
        if (c->type == EObjectType::InlineVariables)
            continue;
        if (c->type == EObjectType::None)
            cc += c->child_count();
        if (c->object)
            cc++;
    }
    return cc;
}

void TreeItem::remove() const
{
    if (!parent)
        return;
    auto &v = parent->children;
    v.erase(std::remove_if(v.begin(), v.end(),[this](const auto &c) { return c.get() == this; }), v.end());
}

void TreeItem::update()
{
    if (object)
    {
        name = object->getName();
        type = object->getType();
    }
}

::polygon4::String TreeItem::get_name()
{
    update();
    ::polygon4::String s;
    if (!defaultName.empty())
    {
        s = tr(defaultName);
        if (!name.empty())
            s += u" - ";
    }
    s += tr(name);
    int cc = child_count();
    if (cc)
        s += " [" + std::to_string(cc) + "]";
    replace_all<::polygon4::String>(s, u"\n", u" ");

    static const size_t max_size = 50;
    if (s.size() > max_size)
        s.resize(max_size);
    return s;
}

IObjectBase::IObjectBase(const IObjectBase &o)
{
    copyFrom(o);
}

IObjectBase &IObjectBase::operator=(const IObjectBase &o)
{
    copyFrom(o);
    return *this;
}

IObjectBase::~IObjectBase()
{
}

int IObjectBase::getId(int defaultValue) const
{
    return hasId() ? id : defaultValue;
}

void IObjectBase::setId(int id_in)
{
    id = id_in;
}

bool IObjectBase::hasId() const
{
    return false;
}

void IObjectBase::setVariableString(int columnId, IObjectBase *ptr)
{
}

const IObjectBase *IObjectBase::getVariable(int columnId) const
{
    return nullptr;
}

EObjectType IObjectBase::getVariableType(int columnId) const
{
    return EObjectType::None;
}

Ptr<TreeItem> IObjectBase::printTree() const
{
    return createTreeItem();
}

::polygon4::String IObjectBase::getName() const
{
    return POLYGON4_NONAME;
}

::polygon4::String IObjectBase::getTextId() const
{
    return "NO_TEXT_ID";
}

::polygon4::String IObjectBase::getDescription() const
{
    return "NO_DESCRIPTION";
}

bool IObjectBase::operator<(const IObjectBase &rhs) const
{
    return getName() < rhs.getName();
}

void IObjectBase::setStorage(Storage *storage)
{
    storage_ = storage;
}

Storage *IObjectBase::getStorage() const
{
    return storage_;
}

bool IObjectBase::isReplaced() const
{
    return replaced_;
}

Ptr<TreeItem> IObjectBase::createTreeItem(TreeItem *parent) const
{
    auto item = std::make_shared<TreeItem>();
    item->parent = parent;
    item->object = const_cast<IObjectBase*>(this);
    item->update();
    return item;
}

void IObjectBase::copyFrom(const IObjectBase &o)
{
    id = o.id;
    storage_ = o.storage_;
    // do not copy replaced_ flag!
}

std::tuple<bool, OrderedObjectMap> IObjectBase::getOrderedObjectMap(int columnId, Storage *storage) const
{
    if (storage)
        return std::make_tuple(true, storage->getOrderedMap(getType()));
    return std::make_tuple(false, OrderedObjectMap());
}

Settings &IObjectBase::getSettings()
{
    return getStorage()->getSettings();
}

const Settings &IObjectBase::getSettings() const
{
    return getStorage()->getSettings();
}

bool to_bool(const std::string &s)
{
    if (s.empty() || s == "0" || s == "false")
        return false;
    return true;
}

} // namespace detail

namespace detail
{

#include "detail/Types.cpp"

} // namespace detail

} // namespace polygon4
