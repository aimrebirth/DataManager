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

#include "ParserDriver.h"

#include <algorithm>
#include <fstream>

// Prevent using <unistd.h> because of bug in flex.
#define YY_NO_UNISTD_H 1
#define YY_DECL 1
#include <lexer.h>
extern yy::parser::symbol_type yylex(yyscan_t yyscanner, yy::location &loc);

yy::parser::symbol_type convert(const Token &t)
{
    yy::parser::symbol_type s;
    s.type = t.token;
    switch (t.type)
    {
    case Token::Integer:
        s.value.build<int>(boost::get<int>(t.value));
        break;
    case Token::String:
        s.value.build<std::string>(boost::get<std::string>(t.value));
        break;
    }
    return s;
}

Token convert(const yy::parser::symbol_type &tok)
{
    Token t;
    t.token = tok.type;

    yy::parser::by_type bt;
    bt.type = tok.type;
    switch (bt.token())
    {
    case yy::parser::token::INTEGER:
        t.type = Token::Integer;
        t.value = tok.value.as<int>();
        break;
    case yy::parser::token::STRING:
        t.type = Token::String;
        t.value = tok.value.as<std::string>();
        break;
    default:
        t.type = Token::None;
        break;
    }
    return t;
}

ParserDriver::ParserDriver()
{
    debug = false;
}

yy::parser::symbol_type ParserDriver::lex()
{
    if (parseMode == Mode::String)
    {
        auto ret = yylex(scanner, location);
        if (tokensWrite)
            tokensWrite->push_back(convert(ret));
        return ret;
    }
    else
    {
        return convert(*readIterator++);
    }
}

int ParserDriver::parse(const std::string &s, Tokens *tokens)
{
    parseMode = Mode::String;
    tokensWrite = tokens;

    yylex_init(&scanner);
    yy_scan_string(s.c_str(), scanner);
    auto res = parse();
    yylex_destroy(scanner);

    return res;
}

int ParserDriver::parse(const Tokens &tokens)
{
    parseMode = Mode::Tokens;
    tokensRead = &tokens;
    readIterator = tokensRead->cbegin();
    auto res = parse();
    return res;
}

int ParserDriver::parse()
{
    yy::parser parser(*this);
    parser.set_debug_level(debug);
    int res = parser.parse();
    return res;
}

void ParserDriver::error(const yy::location &l, const std::string &m)
{
    std::cerr << l << " " << m << std::endl;
}

void ParserDriver::error(const std::string& m)
{
    std::cerr << m << std::endl;
}
