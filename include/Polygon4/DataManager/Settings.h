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

#include <bitset>

#include <Polygon4/DataManager/String.h>

namespace polygon4
{

enum GameFlag
{
    // only autosaves are allowed
    gfIronWill,

    // player can bring <5 gen. mechs to other sectors
    gfCanTransferMechanoidsBetweenSectors,

    // db tool mode: prevents from executing unneded actions
    gfDbTool,

    //
    gfMaxFlag,
};

using GameFlags = std::bitset<gfMaxFlag>;

struct Settings
{
    struct Directories
    {
        String game;
        String mods;
        String saves;

        void setGameDir(const String &p)
        {
            game = p;
            mods = game + "/Mods";
            saves = game + "/Saves";
        }
    };

    Directories dirs;
    GameFlags flags;
    float playtime = 0.0f;
};

} // namespace polygon4
