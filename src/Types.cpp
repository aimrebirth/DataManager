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

#include <Polygon4/Types.h>

#include <sstream>

#ifdef USE_QT
#include <qcoreapplication.h>
#include <qtreewidget.h>
#endif

namespace polygon4
{

int gCurrentLocalizationId = 0;

namespace detail
{
    
inline Text to_string(const Coordinate &c)
{
    std::wstringstream ss;
    ss <<
#ifdef USE_QT
        QCoreApplication::translate("DB", "Point").toStdWString()
#else
        "Point"
#endif
        << " (" <<
        c.x << ", " <<
        c.y << ", " <<
        c.z << "), " <<
#ifdef USE_QT
        QCoreApplication::translate("DB", "Rotation").toStdWString()
#else
        "Rotation"
#endif
        << " (" <<
        c.pitch << ", " <<
        c.yaw << ", " <<
        c.roll << ")";
    return ss.str();
}

#include "detail/Types.cpp"

} // namespace detail

} // namespace polygon4
