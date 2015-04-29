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

#include <string>

struct sqlite3;

namespace polygon4
{

struct DatabaseSchema;

class Database
{    
    typedef int(*DatabaseCallback)(void*, int, char**, char**);

public:
    Database(std::string dbname);
    ~Database();

    void loadDatabase(std::string dbname);
    bool isLoaded() const;
    void getSchema(DatabaseSchema *schema);
    std::string getName() const;

    void execute(const std::string &sql, void *object, DatabaseCallback callback);

private:
    sqlite3 *db = 0;
    std::string name;
};

}
