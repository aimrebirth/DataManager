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

// Prevent using <unistd.h> because of bug in flex.
#include <grammar.hpp>
#define YY_NO_UNISTD_H 1
#define YY_DECL 1
#include <lexer.h>
extern int yylex(yyscan_t yyscanner, YYSTYPE &yylval, YYLTYPE &yylloc);

#include <Polygon4/DataManager/MemoryManager.h>
#include <Polygon4/DataManager/Schema/Context.h>

#include "Ast.h"

namespace ast
{
    parser_data *pd;
}
ast::Schema *schema;
MemoryManager *parserMemoryManager;

YYSTYPE convert(const Token &t)
{
    YYSTYPE yylval = { 0 };
    switch (t.value.which())
    {
    case Token::Integer:
        yylval.intVal = boost::get<int>(t.value);
        break;
    case Token::Float:
        yylval.doubleVal = boost::get<double>(t.value);
        break;
    case Token::String:
        yylval.rawStrVal = strdup(boost::get<std::string>(t.value).c_str());
        break;
    default:
        break;
    }
    return yylval;
}

Token convert(BisonToken token, YYSTYPE &yylval)
{
    Token t;
    t.token = token;
    switch (token)
    {
    case INTEGER:
        t.value = yylval.intVal;
        break;
    case STRING:
        t.value = yylval.rawStrVal;
        break;
    default:
        break;
    }
    return t;
}

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
        for (auto &av : ac.variables)
        {
            Variable v;
            v.id = av.id;
            v.name = av.name;
            v.defaultValue = av.defaultValue;
            v.getOrderedObjectMap = av.getPropertyValue("getOrderedObjectMap");
            v.enumTypeName = av.getPropertyValue("enum_type");
            auto tn = find_type(av.type);
            auto t = s.typePtrs.find(tn);
            assert(t != s.typePtrs.end());
            v.type = s.typePtrs[tn];
            v.flags = av.flags();
            c.variables.push_back(v);
        }
    }
    // dependencies
    for (auto &c : s.classes)
    {
        c.initialize();
        auto &ac = *astClasses[c.name];
        if (ac.flags()[fSplitBy])
            c.splitBy = *c.variables.find(ac.getPropertyValue("split_by"));
        for (auto &av : ac.variables)
        {
            auto dep_name = av.getPropertyValue("dependsOn");
            if (dep_name.empty())
                continue;
            auto master = c.variables.find(dep_name);
            auto slave = c.variables.find(av.name);
            master->slaveVariables.push_back(std::make_shared<Variable>(*slave));
            slave->masterVariable = std::make_shared<Variable>(*master);
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
                assert(i != s.typePtrs.end());
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
    ast::Schema astSchema;
    ::schema = &astSchema;

    MemoryManager mm;
    ::parserMemoryManager = &mm;

    int ret;
    YYSTYPE yylval = { 0 };
    YYLTYPE yylloc = { 1,1,1,1 };
    yydebug = 0;
    yypstate *ps = yypstate_new();
    ast::pd = new ast::parser_data;
    auto t = tokens.begin();
    do {
        yylval = convert(*t);
        ret = yypush_parse(ps, t->token, &yylval, &yylloc);
        t++;
    } while (ret == YYPUSH_MORE);
    delete ast::pd;
    yypstate_delete(ps);

    if (ret)
        throw std::runtime_error("Error during parsing file");

    return convert(astSchema);
}

Schema parse_string(const std::string &s, Tokens *tokens)
{
    if (tokens)
    {
        tokens->clear();
        tokens->reserve(10000);
    }

    ast::Schema astSchema;
    ::schema = &astSchema;

    MemoryManager mm;
    ::parserMemoryManager = &mm;

    int ret;
    YYSTYPE yylval = { 0 };
    YYLTYPE yylloc = { 1,1,1,1 };
    yyscan_t scanner;
    yylex_init(&scanner);
    yy_scan_string(s.c_str(), scanner);
    yydebug = 0;
    yypstate *ps = yypstate_new();
    ast::pd = new ast::parser_data;
    do
    {
        ret = yylex(scanner, yylval, yylloc);
        if (tokens)
            tokens->push_back(convert(ret, yylval));
        ret = yypush_parse(ps, ret, &yylval, &yylloc);
    } while (ret == YYPUSH_MORE);
    delete ast::pd;
    yypstate_delete(ps);
    yylex_destroy(scanner);

    if (ret)
        throw std::runtime_error("Error during parsing file");

    return convert(astSchema);
}

Schema parse_file(const std::string &filename, Tokens *tokens)
{
    auto s = read_file(filename);
    return parse_string(s, tokens);
}
