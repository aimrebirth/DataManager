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

#include <memory>
#include <string>
#include <stack>
#include <vector>

class Context
{
public:
    using Text = std::string;

private:
    struct Line
    {
        Text text;
        int n_indents = 0;

        Line() {}
        Line(const Text &t, int n = 0)
            : text(t), n_indents(n)
        {}

        Line& operator+=(const Text &t)
        {
            text += t;
            return *this;
        }
    };

public:
    using Lines = std::vector<Line>;

public:
    Context(const Text &indent = "    ", const Text &newline = "\n");

    void addLine(const Text &s = Text());
    void addNoNewLine(const Text &s);
    void addLineNoSpace(const Text &s);
    void addText(const Text &s);

    void decreaseIndent();
    void increaseIndent();

    void beginBlock(const Text &s = "", bool indent = true);
    void endBlock(bool semicolon = false);
    void beginFunction(const Text &s = "");
    void endFunction();
    void beginNamespace(const Text &s);
    void endNamespace(const Text &s = Text());

    void ifdef(const Text &s);
    void endif();

    void trimEnd(size_t n);

    Text getText() const;
    Lines getLines() const;

    Context &before()
    {
        if (!before_)
            before_ = std::make_shared<Context>();
        return *before_;
    }
    Context &after()
    {
        if (!after_)
            after_ = std::make_shared<Context>();
        return *after_;
    }

    void emptyLines(int n);

    // add with "as is" indent
    Context &operator+=(const Context &rhs);
    // add with relative indent
    void addWithRelativeIndent(const Context &rhs);

private:
    Lines lines;
    std::shared_ptr<Context> before_;
    std::shared_ptr<Context> after_;

    int n_indents = 0;
    Text indent;
    Text newline;
    std::stack<Text> namespaces;
};

struct ModuleContext
{
    Context cpp;
    Context hpp;

    ModuleContext() {}
    ModuleContext &operator+=(const ModuleContext &rhs)
    {
        cpp += rhs.cpp;
        hpp += rhs.hpp;
        return *this;
    }
};
