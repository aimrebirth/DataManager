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
        ObjectFlags flags;
        for (auto &f : in_flags)
            flags.set(f);
        ObjectFlags r = this->flags;
        if (revert)
            r = r ^ flags;
        r = r & flags;
        r = r ^ flags;
        return r.none();
    }
    bool hasFlags(const std::initializer_list<ObjectFlag> &in_flags, bool revert = false) const
    {
        return hasFlags(std::vector<ObjectFlag>(in_flags), revert);
    }
    bool hasFlags(ObjectFlags flags, bool revert = false) const
    {
        if (revert)
            flags.flip();
        return flags == this->flags;
    }

    void setFlags(const ObjectFlags &flags) { this->flags = flags; }

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
        return std::find_if(Parent::begin(), Parent::end(), [&name](const auto &v) { return name == v.getCppName(); });
    }
    const_iterator find(const Name &name) const
    {
        return std::find_if(Parent::begin(), Parent::end(), [&name](const auto &v) { return name == v.getCppName(); });
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
        return name;
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

    const Type *getType() const { return type; }
    DataType getDataType() const { return type->getDataType(); }
    int getId() const { return id; }
    void setId(int id) { this->id = id; }
    std::string getDefaultValue() const;
    std::string getRawDefaultValue() const { return defaultValue; }
    void setPrefix(const Name &prefix) { this->prefix = prefix; }

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

    std::string print() const;
    std::string printSet() const;
    std::string printSetPtr() const;
    std::string printLoadSqlite3(std::string var) const;
    std::string printSaveSqlite3(std::string var) const;

private:
    int id;
    Name name;
    Name prefix;
    Type *type;
    std::string defaultValue;

    friend Schema convert(const ast::Schema &schema);
};

using Variables = ObjectArray<std::vector<Variable>>;

class DLL_EXPORT Class : public Type
{
public:
    void addVariable(Variable v);    
    Variables getVariables(bool container = false) const;
    Name getEnumName() const { return enumName; }
    virtual Class *getParent() const override { return parent; }
    virtual Class *getChild() const override { return child; }
    
    Variable getVariable(const std::string &name) const
    {
        auto variables = getVariables();
        Variables::const_iterator i;
        i = find_if(variables.begin(), variables.end(), [name](const auto &p)
        {
            return p.getName() == name;
        });
        if (i != variables.end())
            return *i;
        i = find_if(variables.begin(), variables.end(), [name](const auto &p)
        {
            return p.getName() == name + "_id";
        });
        if (i != variables.end())
            return *i;
        i = find_if(variables.begin(), variables.end(), [name](const auto &p)
        {
            return p.getName().find(name) != -1;
        });
        if (i != variables.end())
            return *i;
        return Variable();
    }
    Variable getNameVariable() const { return getVariable("name"); }
    Variable getTextVariable() const { return getVariable("text"); }

    virtual bool isSimple() const override { return false; }

    std::string printSql() const;
    virtual void printVariables(ModuleContext &mc) const override;

    ModuleContext print() const;
    ModuleContext printIo() const;
    ModuleContext printAddDeleteRecordVirtual() const;
    ModuleContext printAddDeleteRecord() const;
    
private:
    Variables variables;
    Name enumName;
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
    Class *child = nullptr;
    std::vector<Class *> children;
    bool hasIdField = false;
    bool hasFks = false;

    friend Schema convert(const ast::Schema &schema);
};

using Classes = ObjectArray<std::list<Class>>;

struct EnumItem
{
    Name name;
    int id;
    bool not_in_table;
};

using EnumItems = std::vector<EnumItem>;

class Enum : public Type
{
public:
    ModuleContext print() const;
    ModuleContext printTableRecord() const;

private:
    EnumItems items;

    friend Schema convert(const ast::Schema &schema);
};

using Enums = ObjectArray<std::list<Enum>>;

class DLL_EXPORT Schema
{
public:
    const Class &getClass(const Name &name) const
    {
        auto i = classes.find(name);
        if (i == classes.end())
            throw std::runtime_error("No class with such name: " + name);
        return *i;
    }
    Classes getClasses() const
    {
        return classes({ fService, fInline, fEnumOnly }, true);
    }

    ModuleContext printObjectInterfaces() const;
    ModuleContext printTypes() const;
    ModuleContext printStorage() const;
    ModuleContext printStorageImplementation() const;
    ModuleContext printEnums() const;

private:
    Version version;
    Types types;
    Classes classes;
    Databases databases;
    Enums enums;

    std::unordered_map<Name, Type*> typePtrs;

    friend Schema convert(const ast::Schema &schema);
};
