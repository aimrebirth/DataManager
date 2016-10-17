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

#include <assert.h>

#include <algorithm>
#include <list>
#include <map>
#include <set>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>

#include "../Dll.h"

#include "Context.h"
#include "Types.h"

DataType dataTypeFromName(const Name &name);

namespace ast
{
    class Schema;
}

class Schema;

class ObjectWithFlags
{
public:
    const ObjectFlags &getFlags() const { return flags; }

    bool hasFlags(const std::vector<ObjectFlag> &in_flags, bool revert = false) const
    {
        ObjectFlags flags_new;
        for (auto &f : in_flags)
            flags_new.set(f);
        ObjectFlags r = this->flags;
        if (revert)
            r = r ^ flags_new;
        r = r & flags_new;
        r = r ^ flags_new;
        return r.none();
    }
    bool hasFlags(const std::initializer_list<ObjectFlag> &in_flags, bool revert = false) const
    {
        return hasFlags(std::vector<ObjectFlag>(in_flags), revert);
    }
    bool hasFlags(ObjectFlags in_flags, bool revert = false) const
    {
        if (revert)
            in_flags.flip();
        return in_flags == flags;
    }

    void setFlags(const ObjectFlags &flags_in) { flags = flags_in; }

protected:
    ObjectFlags flags;
};

template <typename Container>
class ObjectArray : public Container
{
public:
    using Parent = Container;

    using value_type = typename Parent::value_type;
    using iterator = typename Parent::iterator;
    using const_iterator = typename Parent::const_iterator;

public:
    template <class T>
    ObjectArray filter(const T &flags, bool revert = false, bool sort = true) const
    {
        ObjectArray objects;
        for (auto &c : *this)
        {
            if (c.hasFlags(flags, revert))
                objects.push_back(c);
        }
        auto f = { fFirst };
        auto l = { fLast };
        if (sort)
        {
            this->sort(objects, [&](const auto &c1, const auto &c2)
            {
                if (c1.hasFlags(f))
                {
                    if (c2.hasFlags(f))
                        return c1 < c2;
                    return true;
                }
                if (c2.hasFlags(f))
                    return false;
                if (c1.hasFlags(l))
                {
                    if (c2.hasFlags(l))
                        return c1 < c2;
                    return false;
                }
                if (c2.hasFlags(l))
                    return true;
                return c1 < c2;
            });
        }
        return objects;
    }

    ObjectArray operator()(const std::initializer_list<ObjectFlag> &flags, bool revert = false, bool sort = true) const
    {
        return filter(std::vector<ObjectFlag>(flags), revert, sort);
    }
    ObjectArray operator()(const std::vector<ObjectFlag> &flags, bool revert = false, bool sort = true) const
    {
        return filter<std::vector<ObjectFlag>>(flags, revert, sort);
    }
    ObjectArray operator()(ObjectFlags flags, bool revert = false, bool sort = true) const
    {
        return filter<ObjectFlags>(flags, revert, sort);
    }

    template <typename F>
    static void sort(std::vector<value_type> &v, F f)
    {
        std::sort(v.begin(), v.end(), f);
    }
    template <typename F>
    static void sort(std::list<value_type> &v, F f)
    {
        v.sort(f);
    }

    iterator find(const Name &name)
    {
        return std::find_if(Parent::begin(), Parent::end(), [&name](const auto &v) { return name == v.getName(); });
    }
    const_iterator find(const Name &name) const
    {
        return std::find_if(Parent::begin(), Parent::end(), [&name](const auto &v) { return name == v.getName(); });
    }

    bool has(const Name &name) const
    {
        return find(name) != Parent::end();
    }
};

class DLL_EXPORT Type : public ObjectWithFlags
{
public:
    Name getName() const
    {
        return name;
    }
    Name getCppName() const
    {
        return getName();
    }
    Name getCppArrayName() const
    {
        return name + "s";
    }
    Name getCppArrayVariableName() const
    {
        auto n = getCppArrayName();
        n[0] = tolower(n[0]);
        return n;
    }
    Name getCppVariableName() const
    {
        auto n = name;
        n[0] = tolower(n[0]);
        return n;
    }
    Name getHumanName() const
    {
        return humanName.empty() ? getCppName() : humanName;
    }
    Name getSqlName() const { return getCppArrayName(); }
    virtual Type *getParent() const { return nullptr; }
    virtual Type *getChild() const { return nullptr; }
    DataType getDataType() const { return dataType; }

    virtual bool isSimple() const { return true; }
    bool isComplex() const { return !isSimple(); }
    bool isText() const { return getCppName() == "Text"; }

    virtual void printVariables(ModuleContext &mc) const {}

    bool operator<(const Type &rhs) const
    {
        return name < rhs.name;
    }

protected:
    Name name;
    Name humanName;
    DataType dataType;

    friend Schema convert(const ast::Schema &schema);
};

using Types = std::set<Type>;

struct Database
{
    Name name;
    Types types;
};

using Databases = std::vector<Database>;

class DLL_EXPORT Variable : public ObjectWithFlags
{
public:
    using variable_ptr = std::shared_ptr<Variable>;
    using DepVariable = variable_ptr;
    using DepVariables = std::vector<DepVariable>;
    using InitialValues = std::map<Variable, std::string>;

public:
    std::string print() const;
    std::string printSet() const;
    std::string printSetPtr() const;
    void printLoadSqlite3(Context &context, const std::string &var) const;
    void printSaveSqlite3(Context &context, const std::string &var) const;

public:
    Name getName() const
    {
        return name;
    }
    Name getSqlName() const
    {
        if (type->getDataType() == DataType::Complex)
            return getName() + "_id";
        return getName();
    }
    Name getNameWithCaptitalLetter() const
    {
        auto n = name;
        n[0] = toupper(n[0]);
        if (prefix.empty())
            return n;
        return prefix + "." + n;
    }
    Name getPrefixOrName() const
    {
        if (prefix.empty())
            return getName();
        return prefix;
    }
    Name getPrefixedName() const
    {
        if (prefix.empty())
            return getName();
        return prefix + "." + getName();
    }
    Name getEnumTypeName() const { return enumTypeName; }
    Name getDisplayName() const
    {
        if (displayName.empty())
            return getName();
        return displayName;
    }
    Name getObjectName() const { return objectName; }

    bool empty() const { return name.empty(); }
    const Type *getType() const { return type; }
    DataType getDataType() const
    {
        auto dt = type->getDataType();
        if (flags[fArray])
        {
            return dt == DataType::Complex ? DataType::ComplexArray : DataType::Array;
        }
        return dt;
    }
    int getId() const { return id; }
    void setId(int id_in) { id = id_in; }
    std::string getDefaultValue() const;
    std::string getRawDefaultValue() const { return defaultValue; }
    std::string getGetOrderedObjectMap() const { return getOrderedObjectMap; }
    void setPrefix(const Name &prefix_in) { prefix = prefix_in; }

    DepVariable getMasterVariable() const { return masterVariable; }
    const DepVariables &getSlaveVariables() const { return slaveVariables; }

    bool isId() const
    {
        bool is_id = name == "id";
        if (is_id)
        {
            assert(id == 0 && "Id should be on the first position in class!");
            assert(type->getDataType() == DataType::Integer && "Id should have Integer type!");
        }
        return is_id;
    }
    bool isFk() const { return type->isComplex() && type->hasFlags({ fInline, fPrefixed }, true); }

    bool operator<(const Variable &rhs) const
    {
        return getName() < rhs.getName();
    }
    operator bool() const { return !getName().empty(); }

    const InitialValues &getInitialValues() const { return initialValues; }
    const variable_ptr &getArrayKey() const { return arrayKey; }

    int getFromVersion() const { return from_version; }
    int getToVersion() const { return to_version; }

private:
    int id;
    Name name;
    Name prefix;
    Name displayName;
    Type *type;
    std::string defaultValue;
    std::string getOrderedObjectMap;
    Name enumTypeName;
    Name objectName;
    variable_ptr arrayKey;
    InitialValues initialValues;

    DepVariable masterVariable;
    DepVariables slaveVariables;

    int from_version = 0;
    int to_version = 0;

    friend Schema convert(const ast::Schema &schema);
};

using Variables = ObjectArray<std::vector<Variable>>;

class DLL_EXPORT Class : public Type
{
public:
    void initialize();
    Variables getVariables(bool container = false) const;
    Name getEnumName() const { return enumName; }
    virtual Class *getParent() const override { return parent; }
    virtual Class *getChild() const override { return child; }

    Variable getVariable(const std::string &name_in) const
    {
        auto vars = getVariables();
        Variables::const_iterator i;
        i = find_if(vars.begin(), vars.end(), [&name_in](const auto &p)
        {
            return p.getName() == name_in;
        });
        if (i != vars.end())
            return *i;
        i = find_if(vars.begin(), vars.end(), [&name_in](const auto &p)
        {
            return p.getName() == name_in + "_id";
        });
        if (i != vars.end())
            return *i;
        i = find_if(vars.begin(), vars.end(), [&name_in](const auto &p)
        {
            return p.getName().find(name_in) != -1;
        });
        if (i != vars.end())
            return *i;
        return Variable();
    }
    Variable getNameVariable() const { return getVariable("name"); }
    Variable getTextVariable() const { return getVariable("text"); }
    Variable getParentVariable() const { return parentVariable; }
    Variable getChildVariable() const { return childVariable; }
    Variable getSplitByVariable() const { return splitBy; }

    virtual bool isSimple() const override { return false; }

    std::string printSql() const;
    virtual void printVariables(ModuleContext &mc) const override;

    ModuleContext print(const Schema &schema) const;
    ModuleContext printIo() const;
    ModuleContext printAddDeleteRecordVirtual() const;
    ModuleContext printAddDeleteRecord() const;

private:
    Variables variables;
    Name enumName;
    Name subtreeItem;
    std::vector<ObjectName> namesOrder =
    {
        ObjectName::Custom,
        ObjectName::Name,
        ObjectName::Text,
        ObjectName::Parent,
        ObjectName::None,
    };
    std::string objectName;
    Class *parent = nullptr;
    Variable parentVariable;
    Class *child = nullptr;
    Variable childVariable;
    std::vector<Class *> children;
    Variable splitBy;

    bool hasIdField = false;
    bool hasFks = false;

    friend Schema convert(const ast::Schema &schema);
};

using Classes = ObjectArray<std::list<Class>>;

struct EnumItem
{
    Name name;
    int id = -1;
    ObjectFlags flags;

    static const int default_id = -1;
};

using EnumItems = std::vector<EnumItem>;

class Enum : public Type
{
public:
    Name getTableName() const { return "table_" + getCppName(); }
    Name getExcludeTableName() const { return getTableName() + "_exclude"; }

    std::string getEnding() const { return ending; }
    EnumItems& getItems() { return items; }

    ModuleContext print() const;
    ModuleContext printTableRecord() const;

private:
    std::string ending;
    EnumItems items;

    friend Schema convert(const ast::Schema &schema);
};

using Enums = ObjectArray<std::list<Enum>>;

class DLL_EXPORT Schema
{
public:
    void initialize();

    const Class &getClass(const Name &name) const
    {
        auto i = classes.find(name);
        if (i == classes.end())
            throw std::runtime_error("No class with such name: " + name);
        return *i;
    }
    const Classes &getClasses() const
    {
        if (main.empty())
            main = classes({ fService, fInline, fEnumOnly }, true);
        return main;
    }
    const Classes &getServiceDbClasses() const
    {
        if (service_db.empty())
            service_db = classes({ fServiceDb });
        return service_db;
    }

    ModuleContext printForwardDeclarations() const;
    ModuleContext printObjectInterfaces() const;
    ModuleContext printTypes() const;
    ModuleContext printStorage() const;
    ModuleContext printStorageImplementation() const;
    ModuleContext printEnums() const;

    Version getVersion() const { return version; }

private:
    Version version;
    Types types;
    Classes classes;
    Databases databases;
    Enums enums;

    std::unordered_map<Name, Type*> typePtrs;

    mutable Classes main, service_db;

    template <typename F>
    void executeForClasses(F &&f, bool reverse = false) const
    {
        if (reverse)
        {
            std::forward<F>(f)(getServiceDbClasses());
            std::forward<F>(f)(getClasses());
        }
        else
        {
            std::forward<F>(f)(getClasses());
            std::forward<F>(f)(getServiceDbClasses());
        }
    }

    friend Schema convert(const ast::Schema &schema);
};
