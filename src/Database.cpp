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

#include <Polygon4/Database.h>

#include <sqlite3/sqlite3.h>

#include <Polygon4/Storage.h>
#include <Polygon4/StorageImpl.h>

#include "Logger.h"
DECLARE_STATIC_LOGGER(logger, "db");

namespace polygon4
{

Database::Database(std::string dbname)
{
    loadDatabase(dbname);
}

Database::~Database()
{
    sqlite3_close(db);
    db = 0;
}

void Database::loadDatabase(std::string dbname)
{
    if (db)
        return;
    if (sqlite3_open(dbname.c_str(), &db))
    {
        std::string error = "Can't open database file: " + dbname + " error: " + sqlite3_errmsg(db);
        LOG_ERROR(logger, error);
        throw std::exception(error.c_str());
    }
}

bool Database::isLoaded() const
{
    return db != 0;
}

void Database::execute(const std::string &sql, void *object, DatabaseCallback callback)
{
    LOG_TRACE(logger, "Executing sql statement: " << sql);
    char *errmsg;
    sqlite3_exec(db, sql.c_str(), callback, object, &errmsg);
    if (errmsg)
    {
        std::string error = "Error executing sql statement: " + sql + " error: " + errmsg;
        sqlite3_free(errmsg);
        LOG_ERROR(logger, error);
        throw std::exception(error.c_str());
    }
}

std::shared_ptr<Storage> initStorage(std::string filename)
{
    std::shared_ptr<Database> db = std::make_shared<Database>(filename);
    std::shared_ptr<Storage> storage = std::make_shared<detail::StorageImpl>(db);
    return storage;
}

}
