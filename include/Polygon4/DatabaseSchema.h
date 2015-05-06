/*
 * Polygon-4 Engine
 * Copyright (C) 2015 lzwdgc
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

#include <map>
#include <string>

namespace polygon4
{

enum class ColumnType
{
    Integer,
    Real,
    Text,
    Blob
};
std::string getColumnTypeString(ColumnType type);
ColumnType getColumnType(const std::string &s);

struct ForeignKey
{
    std::string table_name;
    std::string column_name;
};
typedef std::map<std::string, ForeignKey> ForeignKeys;

struct Column
{
    int id;
    std::string name;
    ColumnType type;
    ForeignKey *fk = 0;
};
typedef std::map<std::string, Column> Columns;

struct Table
{
    int id;
    std::string name;
    Columns columns;
};
typedef std::map<std::string, Table> Tables;

struct DatabaseSchema
{
    std::string name;
    Tables tables;
};

}
