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

#include <functional>
#include <string>

struct sqlite3;

namespace polygon4
{

struct DatabaseSchema;

class Database
{    
    typedef int(*Sqlite3Callback)(void*, int, char**, char**);
    typedef std::function<int(int, char**, char**)> DatabaseCallback;

public:
    Database(std::string dbname);
    ~Database();

    void loadDatabase(std::string dbname);
    bool isLoaded() const;
    void getSchema(DatabaseSchema *schema) const;
    std::string getName() const;
    std::string Database::getFullName() const;

    bool execute(const std::string &sql, void *object, Sqlite3Callback callback, bool nothrow = false, std::string *errmsg = 0) const;
    bool execute(const std::string &sql, DatabaseCallback callback, bool nothrow = false, std::string *errmsg = 0) const;

private:
    sqlite3 *db = 0;
    std::string name;
    std::string fullName;
};

}
