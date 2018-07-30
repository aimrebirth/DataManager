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

#include "Schema.h"
#include "Token.h"

#include <primitives/filesystem.h>

#include <string>
#include <vector>

SCHEMA_API
Schema parse(const Tokens &tokens);
SCHEMA_API
Schema parse_string(const std::string &s, Tokens *tokens = nullptr);
SCHEMA_API
Schema parse_file(const path &filename, Tokens *tokens = nullptr);
