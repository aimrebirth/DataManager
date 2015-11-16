#pragma once

#include <string>
#include <vector>

#include "Schema.h"
#include "Token.h"

Schema parse(const Tokens &tokens);
Schema parse_string(const std::string &s, Tokens *tokens = nullptr);
Schema parse_file(const std::string &filename, Tokens *tokens = nullptr);
