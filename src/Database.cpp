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

#include <algorithm>

#include <sqlite3/sqlite3.h>

#include <Polygon4/DatabaseSchema.h>
#include <Polygon4/Storage.h>
#include <Polygon4/StorageImpl.h>

#include "Logger.h"
DECLARE_STATIC_LOGGER(logger, "db");

namespace polygon4
{

std::shared_ptr<Storage> initStorage(std::string filename)
{
    std::shared_ptr<Database> db = std::make_shared<Database>(filename);
    return initStorage(db);
}

std::shared_ptr<Storage> initStorage(std::shared_ptr<Database> db)
{
    return std::make_shared<detail::StorageImpl>(db);
}

Database::Database(std::string dbname)
{
    loadDatabase(dbname);
    name = dbname.substr(std::max((int)dbname.rfind("/"), (int)dbname.rfind("\\")) + 1);
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
    execute("PRAGMA foreign_keys = OFF;", 0, 0); // this can be turned on in the future
}

bool Database::isLoaded() const
{
    return db != 0;
}

void Database::execute(const std::string &sql, void *object, DatabaseCallback callback, bool nothrow, std::string *err)
{
    LOG_TRACE(logger, "Executing sql statement: " << sql);
    char *errmsg;
    sqlite3_exec(db, sql.c_str(), callback, object, &errmsg);
    if (errmsg)
    {
        std::string error = "Error executing sql statement:\n" + sql + "\nError: " + errmsg;
        sqlite3_free(errmsg);
        LOG_ERROR(logger, error);
        if (nothrow)
        {
            if (errmsg)
                *err = error;
        }
        else
            throw std::exception(error.c_str());
    }
}

std::string Database::getName() const
{
    return name;
}

void Database::getSchema(DatabaseSchema *schema)
{
    if (schema == 0)
        return;

    auto callback = [](void *o, int ncols, char **cols, char **names)
    {
        DatabaseSchema *schema = (DatabaseSchema *)o;
        Table table;
        table.id = schema->tables.size();
        table.name = cols[1];
        schema->tables[table.name] = table;
        return 0;
    };
    execute("SELECT * FROM sqlite_master WHERE type='table';", schema, callback);
    
    auto callback2 = [](void *o, int ncols, char **cols, char **names)
    {
        Table *table = (Table *)o;
        Column column;
        column.id = table->columns.size();
        column.name = cols[1];
        column.type = getColumnType(cols[2]);
        table->columns[column.name] = column;
        return 0;
    };
    for (auto &tbl : schema->tables)
        execute("PRAGMA table_info(" + tbl.first + ");", &tbl.second, callback2);
    
    auto callback3 = [](void *o, int ncols, char **cols, char **names)
    {
        Table *table = (Table *)o;
        Column &column = table->columns[cols[3]];
        column.fk = new ForeignKey;
        column.fk->table_name = cols[2];
        column.fk->column_name = cols[4];
        return 0;
    };
    for (auto &tbl : schema->tables)
        execute("PRAGMA foreign_key_list(" + tbl.first + ");", &tbl.second, callback3);
}

ColumnType getColumnType(const std::string &s)
{
    if (s == "INTEGER")
        return ColumnType::Integer;
    else if (s == "REAL")
        return ColumnType::Real;
    else if (s == "TEXT")
        return ColumnType::Text;
    else if (s == "BLOB")
        return ColumnType::Blob;
    return ColumnType::Integer;
}

}
