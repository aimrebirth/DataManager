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

#include <Polygon4/DataManager/Storage.h>

#include <Polygon4/DataManager/Database.h>
#include <Polygon4/DataManager/Localization.h>
#include <Polygon4/DataManager/StorageImpl.h>
#include <Polygon4/DataManager/Types.h>

#include "Logger.h"
DECLARE_STATIC_LOGGER(logger, "storage");

namespace polygon4
{

namespace detail
{

#include "detail/Storage.cpp"

} // namespace detail

std::shared_ptr<Storage> initStorage(const path &filename)
{
    std::shared_ptr<Database> db = std::make_shared<Database>(filename);
    return initStorage(db);
}

std::shared_ptr<Storage> initStorage(const std::shared_ptr<Database> &db)
{
    if (!detail::schema)
    {
        LOG_DEBUG(logger, "Initializing schema");
        detail::schema = new Schema(detail::getSchema());
    }
    initTranslator();
    LOG_DEBUG(logger, "Initializing storage");
    return std::make_shared<detail::StorageImpl>(db);
}

} // namespace polygon4
