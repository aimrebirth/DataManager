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

#include <map>
#include <set>
#include <string>
#include <tuple>
#include <unordered_map>

#include <boost/algorithm/string.hpp>

#include <Polygon4/DataManager/Common.h>
#include <Polygon4/DataManager/Schema/Types.h>

namespace ast
{

using Type = std::pair<Key, Value>;
using Types = std::unordered_map<Key, Value>;

struct Database
{
    Name name;
    Types types;
};

using Databases = std::vector<Database>;

struct Properties;

struct Property
{
    Key key;
    Value value;
    std::shared_ptr<Properties> properties;
};

struct Properties : public std::map<Key, struct Property>
{
    using base = std::map<Key, struct Property>;
    using iterator = base::iterator;
    using const_iterator = base::const_iterator;

    std::tuple<bool, Value> getPropertyValue(const Key &key) const
    {
        for (auto &p : *this)
        {
            if (p.first == key)
                return std::make_tuple(true, p.second.value);
            if (p.second.properties)
            {
                auto t = p.second.properties->getPropertyValue(key);
                if (std::get<bool>(t))
                    return t;
            }
        }
        return std::make_tuple(false, Value());
    }

    ObjectFlags flags(bool assert = true) const
    {
        ObjectFlags f;
        for (auto &p : *this)
        {
#define SET_PROPERTY(name, id) else if (p.first == name) f[id] = true
            if (0);
            SET_PROPERTY("enum_only", fEnumOnly);
            SET_PROPERTY("first", fFirst);
            SET_PROPERTY("last", fLast);
            SET_PROPERTY("inline", fInline);
            SET_PROPERTY("service", fService);
            SET_PROPERTY("create_enum", fCreateEnum);
            SET_PROPERTY("proxy", fProxy);
            SET_PROPERTY("tree_view", fTreeView);
            SET_PROPERTY("name", fName);
            SET_PROPERTY("object_name", fObjectName);
            SET_PROPERTY("enum_name", fEnumName);
            SET_PROPERTY("prefixed", fPrefixed);
            SET_PROPERTY("names_order", fNamesOrder);
            SET_PROPERTY("parent", fParent);
            SET_PROPERTY("child", fChild);
            SET_PROPERTY("parent_var", fParent);
            SET_PROPERTY("child_var", fChild);
            SET_PROPERTY("subtree_item", fSubtreeItem);
            SET_PROPERTY("split_by", fSplitBy);
            SET_PROPERTY("default", fDefault);
            SET_PROPERTY("not_in_table", fNotInTable);
            else if (assert)
                assert(false && "No such property");
#undef SET_PROPERTY
            if (p.second.properties)
            {
                bool assert = true;
                if (p.first == "subtree_items")
                    assert = false;
                f |= p.second.properties->flags(assert);
            }
        }
        return f;
    }
};

struct Variable
{
    std::string type;
    Name name;
    std::string defaultValue;
    Properties properties;

    ObjectFlags flags() const
    {
        ObjectFlags f;
        f[fPrimaryKey] = hasProperty("pk");
        f[fEnumItem] = hasProperty("enum_item");
        f[fContainer] = hasProperty("container");
        f[fGetOrderedObjectMap] = hasProperty("getOrderedObjectMap");
        f[fEnumType] = hasProperty("enum_type");
        f[fBigEdit] = hasProperty("big_edit");
        f[fDisplayName] = hasProperty("display_name");
        f[fReadOnly] = hasProperty("read_only");
        f[fNoChildren] = hasProperty("no_children");
        f[fObjectName] = hasProperty("object_name");
        f[fInline] = hasProperty("inline");
        f[fArray] = hasProperty("array");
        f[fArrayKey] = hasProperty("array_key");
        f[fInitialValue] = hasProperty("initial_value");

        // post effects
        if (f[fArray])
            f[fInline] = true;
        return f;
    }

    Property getProperty(const Key &key) const
    {
        auto i = properties.find(key);
        if (i != properties.end())
            return i->second;
        return Property();
    }

    bool hasProperty(const Key &key) const
    {
        if (properties.find(key) != properties.end())
            return true;
        return false;
    }

    Value getPropertyValue(const Key &key) const
    {
        bool found;
        Value v;
        std::tie(found, v) = properties.getPropertyValue(key);
        return v;
    }

    bool operator<(const Variable &rhs) const
    {
        return name < rhs.name;
    }
};

using Variables = std::vector<Variable>;

struct Class
{
    std::string name;
    Properties properties;
    Variables variables;

    Value getPropertyValue(const Key &key) const
    {
        bool found;
        Value v;
        std::tie(found, v) = properties.getPropertyValue(key);
        if (found)
            replace_all(v, "\\\"", "\"");
        return v;
    }

    Name getHumanName() const { return getPropertyValue("name"); }
    Name getEnumName() const { return getPropertyValue("enum_name"); }
    Name getParentName() const { return getPropertyValue("parent"); }
    Name getParentVarName() const { return getPropertyValue("parent_var"); }
    Name getChildName() const { return getPropertyValue("child"); }
    Name getChildVarName() const { return getPropertyValue("child_var"); }
    Name getObjectName() const { return getPropertyValue("object_name"); }
    Name getSubtreeItem() const { return getPropertyValue("subtree_item"); }

    ObjectFlags flags() const { return properties.flags(); }

    std::vector<ObjectName> getNamesOrder() const
    {
        std::vector<ObjectName> namesOrder;
        auto p = getPropertyValue("names_order");
        if (p.empty())
            return namesOrder;
        std::vector<std::string> names;
        boost::algorithm::split(names, p, boost::is_any_of(" "));
        for (auto &n : names)
        {
            if (n == "text")
                namesOrder.push_back(ObjectName::Text);
            else if (n == "name")
                namesOrder.push_back(ObjectName::Name);
            else if (n == "custom")
                namesOrder.push_back(ObjectName::Custom);
            else if (n == "parent")
                namesOrder.push_back(ObjectName::Parent);
            else if (n == "none")
                namesOrder.push_back(ObjectName::None);
        }
        return namesOrder;
    }
    
    bool operator<(const Class &rhs) const
    {
        return name < rhs.name;
    }
};

using Classes = std::set<Class>;

struct EnumItem
{
    Name name;
    int id;
    Properties properties;

    static const int default_id = -1;

    ObjectFlags flags() const
    {
        ObjectFlags f;
        f[fNotInTable] = hasProperty("not_in_table");
        f[fDefault] = hasProperty("default");      
        return f;
    }

    bool hasProperty(const Key &key) const
    {
        bool e;
        Value v;
        std::tie(e, v) = properties.getPropertyValue(key);
        return e;
    }

    Value getPropertyValue(const Key &key) const
    {
        bool found;
        Value v;
        std::tie(found, v) = properties.getPropertyValue(key);
        return v;
    }
};

using EnumItems = std::vector<EnumItem>;

struct Enum
{
    std::string name;
    Properties properties;
    EnumItems items;

    ObjectFlags flags() const
    {
        ObjectFlags f;
        f[fEnding] = hasProperty("ending");
        return f;
    }

    bool hasProperty(const Key &key) const
    {
        bool e;
        Value v;
        std::tie(e, v) = properties.getPropertyValue(key);
        return e;
    }

    std::string getEnding() const
    {
        bool e;
        Value v;
        std::tie(e, v) = properties.getPropertyValue("ending");
        return v;
    }
};

using Enums = std::vector<Enum>;

struct Schema
{
    Version version;
    Types types;
    Databases databases;
    Enums enums;
    Classes classes;
};

} // namespace ast
