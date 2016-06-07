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

#define POLYGON4_MECHANOID_INTERFACE

class DLL_EXPORT IMechanoid : public detail::IObjectBase
{
public:
    virtual bool spawn() { return false; }

    virtual ModificationPlayer *getPlayer() const { return nullptr; }
    virtual void setPlayer(ModificationPlayer *in_player) {}
    virtual bool isPlayer() const { return false; }

    virtual void enterBuilding(MapBuilding *building) {}

    virtual Configuration *getConfiguration() { return nullptr; }

    virtual void addMoney(float m) { }
    virtual float getMoney() const { return 0.0f; }
    virtual bool hasMoney(float m) const { return false; }
    virtual void setMoney(float m) { }

    virtual float getRating(RatingType type = RatingType::Normal) const { return 0.0f; }
    virtual bool hasRating(float rating, RatingType type = RatingType::Normal) const { return false; }
    virtual void setRating(float rating, RatingType type = RatingType::Normal) { }

    virtual int getRatingLevel(RatingType type = RatingType::Normal) const { return 0; }
    virtual bool hasRatingLevel(int level, RatingType type = RatingType::Normal) const { return false; }
    virtual void setRatingLevel(int level, RatingType type = RatingType::Normal) { }

    virtual bool buy(float money) { return false; }
    virtual void sell(float money) { }

    virtual bool setName(const Text &name) { return false; }
};
