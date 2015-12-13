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

#include <string>
#include <vector>

#include <grammar.hpp>

#include "Ast.h"
#include "Token.h"

class ParserDriver
{
    enum class Mode
    {
        String,
        Tokens,
    };

public:
    ParserDriver();

    yy::parser::symbol_type lex();
    int parse(const std::string &s, Tokens *tokens = nullptr);
    int parse(const Tokens &tokens);

    void error(const yy::location &l, const std::string &m);
    void error(const std::string &m);

    void setSchema(const ast::Schema &schema) { this->schema = schema; }
    ast::Schema getSchema() const { return schema; }

    // lex & parse
private:
    void *scanner;
    yy::location location;
    Mode parseMode;
    bool debug;

    // data
private:
    ast::Schema schema;
    Tokens *tokensWrite;
    const Tokens *tokensRead;
    Tokens::const_iterator readIterator;
};
