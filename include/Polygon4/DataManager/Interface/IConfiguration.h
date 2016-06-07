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

#define POLYGON4_CONFIGURATION_INTERFACE

class DLL_EXPORT IConfiguration : public IObjectBase
{
public:
    virtual void addItem(IObjectBase *o, int quantity = 1) {}

    virtual void addEquipment(Equipment *e, int quantity = 1) {}
    virtual void addGlider(Glider *g) {}
    virtual void addGood(Good *g, int quantity = 1) {}
    virtual void addModificator(Modificator *m, int quantity = 1) {}
    virtual void addProjectile(Projectile *p, int quantity = 1) {}
    virtual void addWeapon(Weapon *w) {}

    virtual float getMass() const { return 0.0f; }
    virtual float getTotalMass() const { return 0.0f; }
    virtual float getCapacity() const { return 0.0f; }
};
