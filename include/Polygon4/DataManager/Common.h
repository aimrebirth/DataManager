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

template <typename T>
bool replaceAll(T &str, const T &from, const T &to)
{
    bool replaced = false;
    size_t start_pos = 0;
    while ((start_pos = str.find(from, start_pos)) != T::npos)
    {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length();
        replaced = true;
    }
    return replaced;
}

std::string removeId(std::string s)
{
    replaceAll(s, std::string("_id"), std::string());
    return s;
}
