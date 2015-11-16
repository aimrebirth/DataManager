#pragma once

#include <memory>
#include <vector>

#include <boost/variant.hpp>

#include "Context.h"

union YYSTYPE;
struct YYLTYPE;

using BisonToken = int;

struct Token
{
    enum Type
    {
        Integer,
        Float,
        String,
        None,
    };

    using Value = boost::variant<int, double, std::string>;

    BisonToken token;
    Value value;

    void print(ModuleContext &mc);
};
using Tokens = std::vector<Token>;

ModuleContext print(Tokens &tokens);
