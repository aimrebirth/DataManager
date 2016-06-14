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

#include <algorithm>

#include <Polygon4/DataManager/Storage.h>
#include <Polygon4/DataManager/Table.h>

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
            s += L" - ";
    }
    s += tr(name);
    int cc = child_count();
    if (cc)
        s += L" [" + std::to_wstring(cc) + L"]";
    replace_all<::polygon4::String>(s, L"\n", L" ");

    static const size_t max_size = 50;
    if (s.size() > max_size)
        s.resize(max_size);
    return s;
}

bool to_bool(const std::string &s)
{
    if (s.empty() || s == "0" || s == "false")
        return false;
    return true;
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

} // namespace detail

namespace detail
{

#include "detail/Types.cpp"

} // namespace detail

} // namespace polygon4
