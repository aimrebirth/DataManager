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

#include <Polygon4/DataManager/Schema/Parser.h>

#include <algorithm>
#include <fstream>

#include <Polygon4/DataManager/Schema/Context.h>

#include "ParserDriver.h"

Schema convert(const ast::Schema &ast)
{
    Schema s;
    std::unordered_map<Name, const ast::Class*> astClasses;

    auto find_type = [ast](Name name)
    {
        while (1)
        {
            auto i = ast.types.find(name);
            if (i == ast.types.end() || name == i->second)
                break;
            name = i->second;
        }
        return name;
    };

    // COPY
    s.version = ast.version;

    // types
    for (auto &t : ast.types)
    {
        Type type;
        type.name = find_type(t.second);
        type.dataType = dataTypeFromName(type.name);
        s.types.insert(type);
    }

    // classes
    for (auto &c : ast.classes)
    {
        Class class_;
        class_.name = c.name;
        class_.humanName = c.getHumanName();
        class_.enumName = c.getEnumName();
        class_.dataType = dataTypeFromName(c.name);
        class_.subtreeItem = c.getSubtreeItem();
        auto no = c.getNamesOrder();
        if (!no.empty())
        {
            std::set<ObjectName> names(class_.namesOrder.begin(), class_.namesOrder.end());
            for (auto &n : no)
                names.erase(n);
            for (auto &n : class_.namesOrder)
                if (names.count(n))
                    no.push_back(n);
            class_.namesOrder = no;
        }
        class_.objectName = c.getObjectName();
        class_.setFlags(c.flags());
        s.classes.push_back(class_);
    }

    // databases
    for (auto &d : ast.databases)
    {
        Database db;
        db.name = d.name;
        for (auto &t : d.types)
        {
            Type type;
            type.name = find_type(t.second);
            type.dataType = dataTypeFromName(type.name);
            db.types.insert(type);
        }
        s.databases.push_back(db);
    }

    // enums
    for (auto &e : ast.enums)
    {
        Enum enum_;
        enum_.name = e.name;
        enum_.flags = e.flags();
        enum_.ending = e.getEnding();
        for (auto &i : e.items)
        {
            EnumItem ei;
            ei.name = i.name;
            ei.id = i.id;
            ei.flags = i.flags();
            enum_.items.push_back(ei);
        }
        enum_.dataType = DataType::Enum;
        s.enums.push_back(enum_);
    }

    // create pointers
    for (auto &t : ast.types)
    {
        auto i = std::find_if(s.types.begin(), s.types.end(), [n = find_type(t.second)](const auto &t)
        { return t.name == n; });
        s.typePtrs[t.second] = const_cast<Type *>(&*i);
    }
    for (auto &c : ast.classes)
    {
        s.typePtrs[c.name] = (Type *)&s.getClass(c.name);
        astClasses[c.name] = &c;
    }
    for (auto &e : s.enums)
    {
        s.typePtrs[e.name] = &e;
    }

    // init variables
    for (auto &c : s.classes)
    {
        auto &ac = *astClasses[c.name];
        int id = 0;
        for (auto &av : ac.variables)
        {
            Variable v;
            v.id = id++;
            v.name = av.name;
            v.defaultValue = av.defaultValue;
            v.getOrderedObjectMap = av.getPropertyValue("getOrderedObjectMap");
            v.enumTypeName = av.getPropertyValue("enum_type");
            v.displayName = av.getPropertyValue("display_name");
            v.objectName = av.getPropertyValue("object_name");
            {
                auto f = av.getPropertyValue("from_version");
                v.from_version = f.empty() ? 0 : std::stoi(f);
                auto t = av.getPropertyValue("to_version");
                v.to_version = t.empty() ? 0 : std::stoi(t);
            }
            auto tn = find_type(av.type);
            auto t = s.typePtrs.find(tn);
            if (t == s.typePtrs.end())
            {
                printf("Type not found! '%s'\n", tn.c_str());
            }
            assert(t != s.typePtrs.end() && "Type not found!");
            v.type = s.typePtrs[tn];
            v.flags = av.flags();
            c.variables.push_back(v);
        }
    }
    // init classes
    for (auto &c : s.classes)
        c.initialize();
    // dependencies, array keys, initial values
    for (auto &c : s.classes)
    {
        auto &ac = *astClasses[c.name];
        if (ac.flags()[fSplitBy])
        {
            c.splitBy = *c.variables.find(ac.getPropertyValue("split_by"));
        }
        for (auto &av : ac.variables)
        {
            // deps
            auto dep_name = av.getPropertyValue("dependsOn");
            if (!dep_name.empty())
            {
                auto master = c.variables.find(dep_name);
                auto slave = c.variables.find(av.name);
                master->slaveVariables.push_back(std::make_shared<Variable>(*slave));
                slave->masterVariable = std::make_shared<Variable>(*master);
            }
            // init values
            auto initialValues = av.getProperty("initial_value").properties;
            if (initialValues)
            {
                auto v = c.variables.find(av.name);
                auto vc = (Class *)v->getType();
                for (auto &p : *initialValues)
                    v->initialValues[vc->getVariable(p.second.key)] = p.second.value;
            }
            // array keys
            if (av.flags()[fArray])
            {
                auto ak = av.getPropertyValue("array_key");
                if (ak.empty())
                    ak = "id";
                auto v = c.variables.find(av.name);
                assert(v != c.variables.end() && "Variable not found");
                auto vc = (Class *)v->getType();
                v->arrayKey = std::make_shared<Variable>(vc->getVariable(ak));
            }
        }
    }
    // parents and childs
    for (auto &c : s.classes)
    {
        auto &ac = *astClasses[c.name];
        auto pn = ac.getParentName();
        auto pvn = ac.getParentVarName();
        if (!pn.empty() || !pvn.empty())
        {
            assert(!(!pn.empty() && !pvn.empty()) && "Only one of 'parent' and 'parent_var' should be specified");
            if (!pvn.empty())
            {
                auto v = c.variables.find(pvn);
                c.parentVariable = *v;
                auto t = (Class *)v->getType();
                auto pn = t->getCppName();
                auto i = s.typePtrs.find(pn);
                assert(i != s.typePtrs.end());
                c.parent = (Class *)i->second;
                c.parent->children.push_back(&c);
            }
            else if (!pn.empty())
            {
                auto i = s.typePtrs.find(pn);
                if (i == s.typePtrs.end())
                {
                    printf("Check spelling! '%s'\n", pn.c_str());
                }
                assert(i != s.typePtrs.end() && "Check spelling!");
                c.parent = (Class *)i->second;
                c.parent->children.push_back(&c);
                for (auto &v : c.getVariables())
                {
                    if (v.getType()->getCppName() == c.parent->getCppName())
                    {
                        c.parentVariable = v;
                        break;
                    }
                }
            }
        }
        auto cn = ac.getChildName();
        auto cvn = ac.getChildVarName();
        if (!cn.empty() || !cvn.empty())
        {
            assert(!(!cn.empty() && !cvn.empty()) && "Only one of 'child' and 'child_var' should be specified");
            if (!cvn.empty())
            {
                auto v = c.variables.find(cvn);
                c.childVariable = *v;
                auto t = (Class *)v->getType();
                auto cn = t->getCppName();
                auto i = s.typePtrs.find(cn);
                assert(i != s.typePtrs.end());
                c.child = (Class *)i->second;
            }
            else if (!cn.empty())
            {
                auto i = s.typePtrs.find(cn);
                assert(i != s.typePtrs.end());
                c.child = (Class *)i->second;
                for (auto &v : c.getVariables())
                {
                    if (v.getType()->getCppName() == c.child->getCppName())
                    {
                        c.childVariable = v;
                        break;
                    }
                }
            }
        }
    }

    s.initialize();
    return s;
}

std::string read_file(const std::string &filename)
{
    std::ifstream ifile(filename);
    if (!ifile)
        throw std::runtime_error("Cannot open file " + filename);
    std::string f, s;
    while (std::getline(ifile, s))
        f += s + "\n";
    return f;
}

Schema parse(const Tokens &tokens)
{
    ParserDriver driver;
    int ret = driver.parse(tokens);
    if (ret)
        throw std::runtime_error("Error during parsing file");

    return convert(driver.getSchema());
}

Schema parse_string(const std::string &s, Tokens *tokens)
{
    if (tokens)
    {
        tokens->clear();
        tokens->reserve(10000);
    }

    ParserDriver driver;
    int ret = driver.parse(s, tokens);
    if (ret)
        throw std::runtime_error("Error during parsing file");

    return convert(driver.getSchema());
}

Schema parse_file(const std::string &filename, Tokens *tokens)
{
    auto s = read_file(filename);
    return parse_string(s, tokens);
}
