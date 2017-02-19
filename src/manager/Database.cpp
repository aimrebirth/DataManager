/*
 * Polygon-4 Data Manager
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

#include <Polygon4/DataManager/Exception.h>

#include <sqlite3.h>

#include <algorithm>

#include "Logger.h"
DECLARE_STATIC_LOGGER(logger, "db");

#define MAX_ERROR_SQL_LENGTH 200

/*
** This function is used to load the contents of a database file on disk
** into the "main" database of open database connection pInMemory, or
** to save the current contents of the database opened by pInMemory into
** a database file on disk. pInMemory is probably an in-memory database,
** but this function will also work fine if it is not.
**
** Parameter zFilename points to a nul-terminated string containing the
** name of the database file on disk to load from or save to. If parameter
** isSave is non-zero, then the contents of the file zFilename are
** overwritten with the contents of the database opened by pInMemory. If
** parameter isSave is zero, then the contents of the database opened by
** pInMemory are replaced by data loaded from the file zFilename.
**
** If the operation is successful, SQLITE_OK is returned. Otherwise, if
** an error occurs, an SQLite error code is returned.
*/
int loadOrSaveDb(sqlite3 *pInMemory, const char *zFilename, int isSave)
{
    int rc;                  /* Function return code */
    sqlite3 *pFile;          /* Database connection opened on zFilename */
    sqlite3_backup *pBackup; /* Backup object used to copy data */
    sqlite3 *pTo;            /* Database to copy to (pFile or pInMemory) */
    sqlite3 *pFrom;          /* Database to copy from (pFile or pInMemory) */

    /* Open the database file identified by zFilename. Exit early if this fails
                              ** for any reason. */
    rc = sqlite3_open(zFilename, &pFile);
    if (rc == SQLITE_OK)
    {

        /* If this is a 'load' operation (isSave==0), then data is copied
        ** from the database file just opened to database pInMemory.
        ** Otherwise, if this is a 'save' operation (isSave==1), then data
        ** is copied from pInMemory to pFile.  Set the variables pFrom and
        ** pTo accordingly. */
        pFrom = (isSave ? pInMemory : pFile);
        pTo = (isSave ? pFile : pInMemory);

        /* Set up the backup procedure to copy from the "main" database of
        ** connection pFile to the main database of connection pInMemory.
        ** If something goes wrong, pBackup will be set to NULL and an error
        ** code and  message left in connection pTo.
        **
        ** If the backup object is successfully created, call backup_step()
        ** to copy data from pFile to pInMemory. Then call backup_finish()
        ** to release resources associated with the pBackup object.  If an
        ** error occurred, then  an error code and message will be left in
        ** connection pTo. If no error occurred, then the error code belonging
        ** to pTo is set to SQLITE_OK.
        */
        pBackup = sqlite3_backup_init(pTo, "main", pFrom, "main");
        if (pBackup)
        {
            (void)sqlite3_backup_step(pBackup, -1);
            (void)sqlite3_backup_finish(pBackup);
        }
        rc = sqlite3_errcode(pTo);
    }

    /* Close the database connection opened on database file zFilename
    ** and return the result of this function. */
    (void)sqlite3_close(pFile);
    return rc;
}

sqlite3 *load_from_file(const std::string &fn)
{
    sqlite3 *db = nullptr;
    if (sqlite3_open(":memory:", &db))
    {
        std::string error = "Can't open database file: " + fn + " error: " + sqlite3_errmsg(db);
        LOG_ERROR(logger, error);
        throw EXCEPTION(error);
    }
    auto ret = loadOrSaveDb(db, fn.c_str(), 0);
    if (ret != SQLITE_OK)
    {
        std::string error = "Can't load database: " + fn + " error: " + sqlite3_errstr(ret);
        LOG_ERROR(logger, error);
        throw EXCEPTION(error);
    }
    return db;
}

void save_to_file(const std::string &fn, sqlite3 *db)
{
    auto ret = loadOrSaveDb(db, fn.c_str(), 1);
    if (ret != SQLITE_OK)
    {
        std::string error = "Can't save database: " + fn + " error: " + sqlite3_errstr(ret);
        LOG_ERROR(logger, error);
        throw EXCEPTION(error);
    }
}

namespace polygon4
{

Database::Database(const std::string &dbname)
{
    LOG_TRACE(logger, "Initializing database: " << dbname);
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
    LOG_TRACE(logger, "Opening database: " << dbname);
    db = load_from_file(dbname.c_str());
    execute("PRAGMA cache_size = -2000;"); // cache size (N * page size)
    execute("PRAGMA page_size = 4096;"); // page size bytes (N * page size)
    execute("PRAGMA journal_mode = OFF;"); // set to no journal
    //execute("PRAGMA synchronous = 1;"); // set to wait for OS sync (0 - no wait, 1 - wait OS, 2 - wait all)
    execute("PRAGMA foreign_keys = OFF;"); // this can be turned on in the future
}

void Database::save() const
{
    if (!db)
        return;
    save_to_file(fullName, db);
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
