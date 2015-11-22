#pragma once

#include <map>
#include <set>
#include <string>
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

using Properties = std::unordered_map<Key, Value>;
using Specifier = Key;
using Specifiers = std::set<Specifier>;

struct Variable
{
    int id;
    std::string type;
    Name name;
    std::string defaultValue;
    Specifiers specifiers;
    Properties properties;

    ObjectFlags flags() const
    {
        ObjectFlags f;
        f[fPrimaryKey] = hasProperty("pk");
        f[fEnumItem] = hasProperty("enum_item");
        f[fContainer] = hasProperty("container");
        return f;
    }

    Value getProperty(const Key &key) const
    {
        auto i = properties.find(key);
        if (i != properties.end())
            return i->second;
        return Value();
    }

    bool hasProperty(const Key &key) const
    {
        if (specifiers.find(key) != specifiers.end())
            return true;
        if (properties.find(key) != properties.end())
            return true;
        return false;
    }

    bool operator<(const Variable &rhs) const
    {
        return name < rhs.name;
    }
};

using Variables = std::vector<Variable>;

struct ClassSpecifier
{
    std::string name;
    Specifiers specifiers;
};

using ClassSpecifiers = std::vector<ClassSpecifier>;

struct Class
{
    std::string name;
    std::vector<Properties> properties;
    Variables variables;
    ClassSpecifiers classSpecifiers;

    Value getProperty(const Key &key) const
    {
        Value v;
        for (auto &p : properties)
        {
            auto i = p.find(key);
            if (i != p.end())
            {
                v = i->second;
                break;
            }
        }
        replace_all(v, "\\\"", "\"");
        return v;
    }

    Name getHumanName() const
    {
        return getProperty("name");
    }

    Name getEnumName() const
    {
        return getProperty("enum_name");
    }

    Name getParentName() const
    {
        return getProperty("parent");
    }
    Name getChildName() const
    {
        return getProperty("child");
    }

    std::vector<ObjectName> getNamesOrder() const
    {
        std::vector<ObjectName> namesOrder;
        auto p = getProperty("names_order");
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

    std::string getObjectName() const
    {
        return getProperty("object_name");
    }

    ObjectFlags flags() const
    {
        ObjectFlags f;
        for (auto &props : properties)
        {
            for (auto &p : props)
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
                SET_PROPERTY("parent", fParent);
                SET_PROPERTY("enum_name", fEnumName);
                SET_PROPERTY("prefixed", fPrefixed);
                SET_PROPERTY("names_order", fNamesOrder);
                SET_PROPERTY("child", fChild);
                else
                    assert(false);
#undef SET_PROPERTY
            }
        }
        return f;
    }

    void merge(const Class &rhs)
    {
        if (name.empty())
            name = rhs.name;
        for (auto &v : rhs.properties)
            properties.push_back(v);
        for (auto &v : rhs.variables)
            variables.push_back(v);
        for (auto &v : rhs.classSpecifiers)
            classSpecifiers.push_back(v);
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
    Specifiers specifiers;

    ObjectFlags flags() const
    {
        ObjectFlags f;
        f[fNotInTable] = hasProperty("not_in_table");
        return f;
    }

    bool hasProperty(const Key &key) const
    {
        if (specifiers.find(key) != specifiers.end())
            return true;
        return false;
    }
};

using EnumItems = std::vector<EnumItem>;

struct Enum
{
    std::string name;
    EnumItems items;
};

using Enums = std::vector<Enum>;

struct Schema
{
    Version version;
    Types types;
    Databases databases;
    Classes classes;
    Enums enums;
};

struct parser_data
{
    Types types;
    Database database;
    Databases databases;
    Properties properties;
    Specifier specifier;
    Specifiers specifiers;
    Class class_;
    Classes classes;
    Enum enum_;
    Enums enums;
    int variable_id = 0;
    int enum_var_id = 0;
};

extern parser_data *pd;

} // namespace ast
