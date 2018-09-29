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

#include <primitives/filesystem.h>

#include <functional>
#include <memory>
#include <string>

struct sqlite3;

namespace polygon4
{

class DATA_MANAGER_API Database
{
    typedef int(*Sqlite3Callback)(void*, int, char**, char**);
    typedef std::function<int(int, char**, char**)> DatabaseCallback;

public:
    Database(const path &dbname);
    ~Database();

    void loadDatabase(const path &dbname);
    void save() const;

    bool isLoaded() const;
    sqlite3 *getDb() const;

    std::string getName() const;
    path getFullName() const;

    bool execute(const std::string &sql, void *object, Sqlite3Callback callback, bool nothrow = false, std::string *errmsg = 0) const;
    bool execute(const std::string &sql, DatabaseCallback callback = DatabaseCallback(), bool nothrow = false, std::string *errmsg = 0) const;

private:
    sqlite3 *db = 0;
    std::string name;
    path fullName;
};

}
