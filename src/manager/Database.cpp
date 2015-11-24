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

#include <Polygon4/DataManager/Database.h>

#include <algorithm>

#include <sqlite3/sqlite3.h>

#include <Polygon4/DataManager/Schema.h>
#include <Polygon4/DataManager/Storage.h>
#include <Polygon4/DataManager/StorageImpl.h>
#include <Polygon4/DataManager/Exception.h>

#include "Logger.h"
DECLARE_STATIC_LOGGER(logger, "db");

#define MAX_ERROR_SQL_LENGTH 200

namespace polygon4
{

Database::Database(const std::string &dbname)
{
    loadDatabase(dbname);
    name = dbname.substr(std::max((int)dbname.rfind("/"), (int)dbname.rfind("\\")) + 1);
    fullName = dbname;
}

Database::~Database()
{
    sqlite3_close(db);
    db = 0;
}

void Database::loadDatabase(const std::string &dbname)
{
    if (db)
        return;
    if (sqlite3_open(dbname.c_str(), &db))
    {
        std::string error = "Can't open database file: " + dbname + " error: " + sqlite3_errmsg(db);
        LOG_ERROR(logger, error);
        throw EXCEPTION(error);
    }
    execute("PRAGMA cache_size = 8000;"); // cache size (N * page size)
    execute("PRAGMA page_size = 2048;"); // page size bytes (N * page size)
    execute("PRAGMA journal_mode = OFF;"); // set to no journal
    execute("PRAGMA synchronous = 1;"); // set to wait for OS sync (0 - no wait, 1 - wait OS, 2 - wait all)
    execute("PRAGMA foreign_keys = OFF;"); // this can be turned on in the future
}

bool Database::isLoaded() const
{
    return db != 0;
}

bool Database::execute(const std::string &sql, void *object, Sqlite3Callback callback, bool nothrow, std::string *err) const
{
    LOG_TRACE(logger, "Executing sql statement: " << sql);
    char *errmsg;
    std::string error;
    sqlite3_exec(db, sql.c_str(), callback, object, &errmsg);
    if (errmsg)
    {
        auto s = sql.substr(0, MAX_ERROR_SQL_LENGTH);
        if (sql.size() > MAX_ERROR_SQL_LENGTH)
            s += "...";
        error = "Error executing sql statement:\n" + s + "\nError: " + errmsg;
        sqlite3_free(errmsg);
        LOG_ERROR(logger, error);
        if (nothrow)
        {
            if (errmsg)
                *err = error;
        }
        else
            throw EXCEPTION(error);
    }
    return error.empty();
}

bool Database::execute(const std::string &sql, DatabaseCallback callback, bool nothrow, std::string *err) const
{
    LOG_TRACE(logger, "Executing sql statement: " << sql);
    char *errmsg;
    std::string error;
    auto cb = [](void *o, int ncols, char **cols, char **names)
    {
        DatabaseCallback *f = (DatabaseCallback *)o;
        if (*f)
            return (*f)(ncols, cols, names);
        return 0;
    };
    sqlite3_exec(db, sql.c_str(), cb, &callback, &errmsg);
    if (errmsg)
    {
        auto s = sql.substr(0, MAX_ERROR_SQL_LENGTH);
        if (sql.size() > MAX_ERROR_SQL_LENGTH)
            s += "...";
        error = "Error executing sql statement:\n" + s + "\nError: " + errmsg;
        sqlite3_free(errmsg);
        LOG_ERROR(logger, error);
        if (nothrow)
        {
            if (errmsg)
                *err = error;
        }
        else
            throw EXCEPTION(error);
    }
    return error.empty();
}

std::string Database::getName() const
{
    return name;
}

std::string Database::getFullName() const
{
    return fullName;
}

sqlite3 *Database::getDb() const
{
    return db;
}

}
