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

#include <Polygon4/DataManager/Storage.h>

#include <Polygon4/DataManager/Database.h>
#include <Polygon4/DataManager/Localization.h>
#include <Polygon4/DataManager/StorageImpl.h>

namespace polygon4
{

namespace detail
{

#include "detail/Storage.cpp"

} // namespace detail

std::shared_ptr<Storage> initStorage(std::string filename)
{
    std::shared_ptr<Database> db = std::make_shared<Database>(filename);
    return initStorage(db);
}

std::shared_ptr<Storage> initStorage(std::shared_ptr<Database> db)
{
    if (!detail::schema)
        detail::schema = new Schema(detail::getSchema());
    initTranslator();
    return std::make_shared<detail::StorageImpl>(db);
}

} // namespace polygon4
