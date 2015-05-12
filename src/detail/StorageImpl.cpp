void StorageImpl::_loadBuildings()
{
    auto callback = [](void *o, int ncols, char **cols, char **names)
    {
        Ptr<Building> building = std::make_shared<Building>();
        if (cols[0]) building->id = std::stoi(cols[0]);
        if (cols[1]) building->text_id = cols[1];
        if (cols[2]) building->resource = cols[2];
        if (cols[3]) building->name.id = std::stoi(cols[3]);

        Storage *storage = (Storage *)o;
        storage->buildings[building->id] = building;
        return 0;
    };
    db->execute("select * from Buildings;", this, callback);
}

void StorageImpl::_loadBuildingsPtrs()
{
    for (auto &building : buildings)
    {
        if (strings.find(building.second->name.id) != strings.end())
            building.second->name.ptr = strings[building.second->name.id];
    }
}

void StorageImpl::_loadBuildingsArrays()
{
}

void StorageImpl::_saveBuildings()
{
    std::string query;
    query += "delete from Buildings;";
    db->execute(query.c_str(), 0, 0);
    query.clear();
    if (buildings.empty())
        return;
    query += "insert or replace into Buildings values\n";
    for (auto &building : buildings)
    {
        query += "(";
        query += "'" + std::to_string(building.second->id) + "',";
        query += "'" + building.second->text_id.string() + "',";
        query += "'" + building.second->resource.string() + "',";
        query += "'" + std::to_string(building.second->name.id) + "',";
        query.resize(query.size() - 1);
        query += "),\n";
    }
    query.resize(query.size() - 2);
    query += ";";
    db->execute(query.c_str(), 0, 0);
}

void StorageImpl::_loadClanReputations()
{
    auto callback = [](void *o, int ncols, char **cols, char **names)
    {
        Ptr<ClanReputation> clanReputation = std::make_shared<ClanReputation>();
        if (cols[0]) clanReputation->clan.id = std::stoi(cols[0]);
        if (cols[1]) clanReputation->clan2.id = std::stoi(cols[1]);
        if (cols[2]) clanReputation->reputation = std::stof(cols[2]);

        Storage *storage = (Storage *)o;
        storage->clanReputations.push_back(clanReputation);
        return 0;
    };
    db->execute("select * from ClanReputations;", this, callback);
}

void StorageImpl::_loadClanReputationsPtrs()
{
    for (auto &clanReputation : clanReputations)
    {
        if (clans.find(clanReputation->clan.id) != clans.end())
            clanReputation->clan.ptr = clans[clanReputation->clan.id];
        if (clans.find(clanReputation->clan2.id) != clans.end())
            clanReputation->clan2.ptr = clans[clanReputation->clan2.id];
    }
}

void StorageImpl::_loadClanReputationsArrays()
{
}

void StorageImpl::_saveClanReputations()
{
    std::string query;
    query += "delete from ClanReputations;";
    db->execute(query.c_str(), 0, 0);
    query.clear();
    if (clanReputations.empty())
        return;
    query += "insert or replace into ClanReputations values\n";
    for (auto &clanReputation : clanReputations)
    {
        query += "(";
        query += "'" + std::to_string(clanReputation->clan.id) + "',";
        query += "'" + std::to_string(clanReputation->clan2.id) + "',";
        query += "'" + std::to_string(clanReputation->reputation) + "',";
        query.resize(query.size() - 1);
        query += "),\n";
    }
    query.resize(query.size() - 2);
    query += ";";
    db->execute(query.c_str(), 0, 0);
}

void StorageImpl::_loadClans()
{
    auto callback = [](void *o, int ncols, char **cols, char **names)
    {
        Ptr<Clan> clan = std::make_shared<Clan>();
        if (cols[0]) clan->id = std::stoi(cols[0]);
        if (cols[1]) clan->text_id = cols[1];
        if (cols[2]) clan->resource = cols[2];
        if (cols[3]) clan->name.id = std::stoi(cols[3]);

        Storage *storage = (Storage *)o;
        storage->clans[clan->id] = clan;
        return 0;
    };
    db->execute("select * from Clans;", this, callback);
}

void StorageImpl::_loadClansPtrs()
{
    for (auto &clan : clans)
    {
        if (strings.find(clan.second->name.id) != strings.end())
            clan.second->name.ptr = strings[clan.second->name.id];
    }
}

void StorageImpl::_loadClansArrays()
{
    for (auto &clan : clans)
    {
        for (auto &clanReputation : clanReputations)
            if (clan.first == clanReputation->clan.id)
                clan.second->reputations.push_back(clanReputation);
    }
}

void StorageImpl::_saveClans()
{
    std::string query;
    query += "delete from Clans;";
    db->execute(query.c_str(), 0, 0);
    query.clear();
    if (clans.empty())
        return;
    query += "insert or replace into Clans values\n";
    for (auto &clan : clans)
    {
        query += "(";
        query += "'" + std::to_string(clan.second->id) + "',";
        query += "'" + clan.second->text_id.string() + "',";
        query += "'" + clan.second->resource.string() + "',";
        query += "'" + std::to_string(clan.second->name.id) + "',";
        query.resize(query.size() - 1);
        query += "),\n";
    }
    query.resize(query.size() - 2);
    query += ";";
    db->execute(query.c_str(), 0, 0);
}

void StorageImpl::_loadConfigurationEquipments()
{
    auto callback = [](void *o, int ncols, char **cols, char **names)
    {
        Ptr<ConfigurationEquipment> configurationEquipment = std::make_shared<ConfigurationEquipment>();
        if (cols[0]) configurationEquipment->configuration.id = std::stoi(cols[0]);
        if (cols[1]) configurationEquipment->equipment.id = std::stoi(cols[1]);
        if (cols[2]) configurationEquipment->quantity = std::stoi(cols[2]);

        Storage *storage = (Storage *)o;
        storage->configurationEquipments.push_back(configurationEquipment);
        return 0;
    };
    db->execute("select * from ConfigurationEquipments;", this, callback);
}

void StorageImpl::_loadConfigurationEquipmentsPtrs()
{
    for (auto &configurationEquipment : configurationEquipments)
    {
        if (configurations.find(configurationEquipment->configuration.id) != configurations.end())
            configurationEquipment->configuration.ptr = configurations[configurationEquipment->configuration.id];
        if (equipments.find(configurationEquipment->equipment.id) != equipments.end())
            configurationEquipment->equipment.ptr = equipments[configurationEquipment->equipment.id];
    }
}

void StorageImpl::_loadConfigurationEquipmentsArrays()
{
}

void StorageImpl::_saveConfigurationEquipments()
{
    std::string query;
    query += "delete from ConfigurationEquipments;";
    db->execute(query.c_str(), 0, 0);
    query.clear();
    if (configurationEquipments.empty())
        return;
    query += "insert or replace into ConfigurationEquipments values\n";
    for (auto &configurationEquipment : configurationEquipments)
    {
        query += "(";
        query += "'" + std::to_string(configurationEquipment->configuration.id) + "',";
        query += "'" + std::to_string(configurationEquipment->equipment.id) + "',";
        query += "'" + std::to_string(configurationEquipment->quantity) + "',";
        query.resize(query.size() - 1);
        query += "),\n";
    }
    query.resize(query.size() - 2);
    query += ";";
    db->execute(query.c_str(), 0, 0);
}

void StorageImpl::_loadConfigurationGoods()
{
    auto callback = [](void *o, int ncols, char **cols, char **names)
    {
        Ptr<ConfigurationGood> configurationGood = std::make_shared<ConfigurationGood>();
        if (cols[0]) configurationGood->configuration.id = std::stoi(cols[0]);
        if (cols[1]) configurationGood->good.id = std::stoi(cols[1]);
        if (cols[2]) configurationGood->quantity = std::stoi(cols[2]);

        Storage *storage = (Storage *)o;
        storage->configurationGoods.push_back(configurationGood);
        return 0;
    };
    db->execute("select * from ConfigurationGoods;", this, callback);
}

void StorageImpl::_loadConfigurationGoodsPtrs()
{
    for (auto &configurationGood : configurationGoods)
    {
        if (configurations.find(configurationGood->configuration.id) != configurations.end())
            configurationGood->configuration.ptr = configurations[configurationGood->configuration.id];
        if (goods.find(configurationGood->good.id) != goods.end())
            configurationGood->good.ptr = goods[configurationGood->good.id];
    }
}

void StorageImpl::_loadConfigurationGoodsArrays()
{
}

void StorageImpl::_saveConfigurationGoods()
{
    std::string query;
    query += "delete from ConfigurationGoods;";
    db->execute(query.c_str(), 0, 0);
    query.clear();
    if (configurationGoods.empty())
        return;
    query += "insert or replace into ConfigurationGoods values\n";
    for (auto &configurationGood : configurationGoods)
    {
        query += "(";
        query += "'" + std::to_string(configurationGood->configuration.id) + "',";
        query += "'" + std::to_string(configurationGood->good.id) + "',";
        query += "'" + std::to_string(configurationGood->quantity) + "',";
        query.resize(query.size() - 1);
        query += "),\n";
    }
    query.resize(query.size() - 2);
    query += ";";
    db->execute(query.c_str(), 0, 0);
}

void StorageImpl::_loadConfigurationProjectiles()
{
    auto callback = [](void *o, int ncols, char **cols, char **names)
    {
        Ptr<ConfigurationProjectile> configurationProjectile = std::make_shared<ConfigurationProjectile>();
        if (cols[0]) configurationProjectile->configuration.id = std::stoi(cols[0]);
        if (cols[1]) configurationProjectile->projectile.id = std::stoi(cols[1]);
        if (cols[2]) configurationProjectile->quantity = std::stoi(cols[2]);

        Storage *storage = (Storage *)o;
        storage->configurationProjectiles.push_back(configurationProjectile);
        return 0;
    };
    db->execute("select * from ConfigurationProjectiles;", this, callback);
}

void StorageImpl::_loadConfigurationProjectilesPtrs()
{
    for (auto &configurationProjectile : configurationProjectiles)
    {
        if (configurations.find(configurationProjectile->configuration.id) != configurations.end())
            configurationProjectile->configuration.ptr = configurations[configurationProjectile->configuration.id];
        if (projectiles.find(configurationProjectile->projectile.id) != projectiles.end())
            configurationProjectile->projectile.ptr = projectiles[configurationProjectile->projectile.id];
    }
}

void StorageImpl::_loadConfigurationProjectilesArrays()
{
}

void StorageImpl::_saveConfigurationProjectiles()
{
    std::string query;
    query += "delete from ConfigurationProjectiles;";
    db->execute(query.c_str(), 0, 0);
    query.clear();
    if (configurationProjectiles.empty())
        return;
    query += "insert or replace into ConfigurationProjectiles values\n";
    for (auto &configurationProjectile : configurationProjectiles)
    {
        query += "(";
        query += "'" + std::to_string(configurationProjectile->configuration.id) + "',";
        query += "'" + std::to_string(configurationProjectile->projectile.id) + "',";
        query += "'" + std::to_string(configurationProjectile->quantity) + "',";
        query.resize(query.size() - 1);
        query += "),\n";
    }
    query.resize(query.size() - 2);
    query += ";";
    db->execute(query.c_str(), 0, 0);
}

void StorageImpl::_loadConfigurationWeapons()
{
    auto callback = [](void *o, int ncols, char **cols, char **names)
    {
        Ptr<ConfigurationWeapon> configurationWeapon = std::make_shared<ConfigurationWeapon>();
        if (cols[0]) configurationWeapon->configuration.id = std::stoi(cols[0]);
        if (cols[1]) configurationWeapon->weapon.id = std::stoi(cols[1]);
        if (cols[2]) configurationWeapon->quantity = std::stoi(cols[2]);

        Storage *storage = (Storage *)o;
        storage->configurationWeapons.push_back(configurationWeapon);
        return 0;
    };
    db->execute("select * from ConfigurationWeapons;", this, callback);
}

void StorageImpl::_loadConfigurationWeaponsPtrs()
{
    for (auto &configurationWeapon : configurationWeapons)
    {
        if (configurations.find(configurationWeapon->configuration.id) != configurations.end())
            configurationWeapon->configuration.ptr = configurations[configurationWeapon->configuration.id];
        if (weapons.find(configurationWeapon->weapon.id) != weapons.end())
            configurationWeapon->weapon.ptr = weapons[configurationWeapon->weapon.id];
    }
}

void StorageImpl::_loadConfigurationWeaponsArrays()
{
}

void StorageImpl::_saveConfigurationWeapons()
{
    std::string query;
    query += "delete from ConfigurationWeapons;";
    db->execute(query.c_str(), 0, 0);
    query.clear();
    if (configurationWeapons.empty())
        return;
    query += "insert or replace into ConfigurationWeapons values\n";
    for (auto &configurationWeapon : configurationWeapons)
    {
        query += "(";
        query += "'" + std::to_string(configurationWeapon->configuration.id) + "',";
        query += "'" + std::to_string(configurationWeapon->weapon.id) + "',";
        query += "'" + std::to_string(configurationWeapon->quantity) + "',";
        query.resize(query.size() - 1);
        query += "),\n";
    }
    query.resize(query.size() - 2);
    query += ";";
    db->execute(query.c_str(), 0, 0);
}

void StorageImpl::_loadConfigurations()
{
    auto callback = [](void *o, int ncols, char **cols, char **names)
    {
        Ptr<Configuration> configuration = std::make_shared<Configuration>();
        if (cols[0]) configuration->id = std::stoi(cols[0]);
        if (cols[1]) configuration->text_id = cols[1];
        if (cols[2]) configuration->name.id = std::stoi(cols[2]);
        if (cols[3]) configuration->glider.id = std::stoi(cols[3]);

        Storage *storage = (Storage *)o;
        storage->configurations[configuration->id] = configuration;
        return 0;
    };
    db->execute("select * from Configurations;", this, callback);
}

void StorageImpl::_loadConfigurationsPtrs()
{
    for (auto &configuration : configurations)
    {
        if (strings.find(configuration.second->name.id) != strings.end())
            configuration.second->name.ptr = strings[configuration.second->name.id];
        if (gliders.find(configuration.second->glider.id) != gliders.end())
            configuration.second->glider.ptr = gliders[configuration.second->glider.id];
    }
}

void StorageImpl::_loadConfigurationsArrays()
{
    for (auto &configuration : configurations)
    {
        for (auto &configurationEquipment : configurationEquipments)
            if (configuration.first == configurationEquipment->configuration.id)
                configuration.second->equipments.push_back(configurationEquipment);
        for (auto &configurationGood : configurationGoods)
            if (configuration.first == configurationGood->configuration.id)
                configuration.second->goods.push_back(configurationGood);
        for (auto &configurationProjectile : configurationProjectiles)
            if (configuration.first == configurationProjectile->configuration.id)
                configuration.second->projectiles.push_back(configurationProjectile);
        for (auto &configurationWeapon : configurationWeapons)
            if (configuration.first == configurationWeapon->configuration.id)
                configuration.second->weapons.push_back(configurationWeapon);
    }
}

void StorageImpl::_saveConfigurations()
{
    std::string query;
    query += "delete from Configurations;";
    db->execute(query.c_str(), 0, 0);
    query.clear();
    if (configurations.empty())
        return;
    query += "insert or replace into Configurations values\n";
    for (auto &configuration : configurations)
    {
        query += "(";
        query += "'" + std::to_string(configuration.second->id) + "',";
        query += "'" + configuration.second->text_id.string() + "',";
        query += "'" + std::to_string(configuration.second->name.id) + "',";
        query += "'" + std::to_string(configuration.second->glider.id) + "',";
        query.resize(query.size() - 1);
        query += "),\n";
    }
    query.resize(query.size() - 2);
    query += ";";
    db->execute(query.c_str(), 0, 0);
}

void StorageImpl::_loadCoordinates()
{
    auto callback = [](void *o, int ncols, char **cols, char **names)
    {
        Ptr<Coordinate> coordinate = std::make_shared<Coordinate>();
        if (cols[0]) coordinate->id = std::stoi(cols[0]);
        if (cols[1]) coordinate->x = std::stof(cols[1]);
        if (cols[2]) coordinate->y = std::stof(cols[2]);
        if (cols[3]) coordinate->z = std::stof(cols[3]);
        if (cols[4]) coordinate->pitch = std::stof(cols[4]);
        if (cols[5]) coordinate->yaw = std::stof(cols[5]);
        if (cols[6]) coordinate->roll = std::stof(cols[6]);

        Storage *storage = (Storage *)o;
        storage->coordinates[coordinate->id] = coordinate;
        return 0;
    };
    db->execute("select * from Coordinates;", this, callback);
}

void StorageImpl::_loadCoordinatesPtrs()
{
}

void StorageImpl::_loadCoordinatesArrays()
{
}

void StorageImpl::_saveCoordinates()
{
    std::string query;
    query += "delete from Coordinates;";
    db->execute(query.c_str(), 0, 0);
    query.clear();
    if (coordinates.empty())
        return;
    query += "insert or replace into Coordinates values\n";
    for (auto &coordinate : coordinates)
    {
        query += "(";
        query += "'" + std::to_string(coordinate.second->id) + "',";
        query += "'" + std::to_string(coordinate.second->x) + "',";
        query += "'" + std::to_string(coordinate.second->y) + "',";
        query += "'" + std::to_string(coordinate.second->z) + "',";
        query += "'" + std::to_string(coordinate.second->pitch) + "',";
        query += "'" + std::to_string(coordinate.second->yaw) + "',";
        query += "'" + std::to_string(coordinate.second->roll) + "',";
        query.resize(query.size() - 1);
        query += "),\n";
    }
    query.resize(query.size() - 2);
    query += ";";
    db->execute(query.c_str(), 0, 0);
}

void StorageImpl::_loadEquipments()
{
    auto callback = [](void *o, int ncols, char **cols, char **names)
    {
        Ptr<Equipment> equipment = std::make_shared<Equipment>();
        if (cols[0]) equipment->id = std::stoi(cols[0]);
        if (cols[1]) equipment->text_id = cols[1];
        if (cols[2]) equipment->resource = cols[2];
        if (cols[3]) equipment->name.id = std::stoi(cols[3]);
        if (cols[4]) equipment->type = std::stoi(cols[4]);
        if (cols[5]) equipment->standard = std::stoi(cols[5]);
        if (cols[6]) equipment->weight = std::stof(cols[6]);
        if (cols[7]) equipment->durability = std::stof(cols[7]);
        if (cols[8]) equipment->power = std::stof(cols[8]);
        if (cols[9]) equipment->value1 = std::stof(cols[9]);
        if (cols[10]) equipment->value2 = std::stof(cols[10]);
        if (cols[11]) equipment->value3 = std::stof(cols[11]);
        if (cols[12]) equipment->manual = std::stoi(cols[12]);
        if (cols[13]) equipment->price = std::stof(cols[13]);
        if (cols[14]) equipment->notrade = std::stoi(cols[14]);

        Storage *storage = (Storage *)o;
        storage->equipments[equipment->id] = equipment;
        return 0;
    };
    db->execute("select * from Equipments;", this, callback);
}

void StorageImpl::_loadEquipmentsPtrs()
{
    for (auto &equipment : equipments)
    {
        if (strings.find(equipment.second->name.id) != strings.end())
            equipment.second->name.ptr = strings[equipment.second->name.id];
    }
}

void StorageImpl::_loadEquipmentsArrays()
{
}

void StorageImpl::_saveEquipments()
{
    std::string query;
    query += "delete from Equipments;";
    db->execute(query.c_str(), 0, 0);
    query.clear();
    if (equipments.empty())
        return;
    query += "insert or replace into Equipments values\n";
    for (auto &equipment : equipments)
    {
        query += "(";
        query += "'" + std::to_string(equipment.second->id) + "',";
        query += "'" + equipment.second->text_id.string() + "',";
        query += "'" + equipment.second->resource.string() + "',";
        query += "'" + std::to_string(equipment.second->name.id) + "',";
        query += "'" + std::to_string(equipment.second->type) + "',";
        query += "'" + std::to_string(equipment.second->standard) + "',";
        query += "'" + std::to_string(equipment.second->weight) + "',";
        query += "'" + std::to_string(equipment.second->durability) + "',";
        query += "'" + std::to_string(equipment.second->power) + "',";
        query += "'" + std::to_string(equipment.second->value1) + "',";
        query += "'" + std::to_string(equipment.second->value2) + "',";
        query += "'" + std::to_string(equipment.second->value3) + "',";
        query += "'" + std::to_string(equipment.second->manual) + "',";
        query += "'" + std::to_string(equipment.second->price) + "',";
        query += "'" + std::to_string(equipment.second->notrade) + "',";
        query.resize(query.size() - 1);
        query += "),\n";
    }
    query.resize(query.size() - 2);
    query += ";";
    db->execute(query.c_str(), 0, 0);
}

void StorageImpl::_loadGliders()
{
    auto callback = [](void *o, int ncols, char **cols, char **names)
    {
        Ptr<Glider> glider = std::make_shared<Glider>();
        if (cols[0]) glider->id = std::stoi(cols[0]);
        if (cols[1]) glider->text_id = cols[1];
        if (cols[2]) glider->resource = cols[2];
        if (cols[3]) glider->name.id = std::stoi(cols[3]);
        if (cols[4]) glider->standard = std::stoi(cols[4]);
        if (cols[5]) glider->weight = std::stof(cols[5]);
        if (cols[6]) glider->maxweight = std::stof(cols[6]);
        if (cols[7]) glider->rotatespeed = std::stof(cols[7]);
        if (cols[8]) glider->armor = std::stof(cols[8]);
        if (cols[9]) glider->price = std::stoi(cols[9]);
        if (cols[10]) glider->restore = std::stof(cols[10]);
        if (cols[11]) glider->power = std::stof(cols[11]);
        if (cols[12]) glider->special = std::stoi(cols[12]);

        Storage *storage = (Storage *)o;
        storage->gliders[glider->id] = glider;
        return 0;
    };
    db->execute("select * from Gliders;", this, callback);
}

void StorageImpl::_loadGlidersPtrs()
{
    for (auto &glider : gliders)
    {
        if (strings.find(glider.second->name.id) != strings.end())
            glider.second->name.ptr = strings[glider.second->name.id];
    }
}

void StorageImpl::_loadGlidersArrays()
{
}

void StorageImpl::_saveGliders()
{
    std::string query;
    query += "delete from Gliders;";
    db->execute(query.c_str(), 0, 0);
    query.clear();
    if (gliders.empty())
        return;
    query += "insert or replace into Gliders values\n";
    for (auto &glider : gliders)
    {
        query += "(";
        query += "'" + std::to_string(glider.second->id) + "',";
        query += "'" + glider.second->text_id.string() + "',";
        query += "'" + glider.second->resource.string() + "',";
        query += "'" + std::to_string(glider.second->name.id) + "',";
        query += "'" + std::to_string(glider.second->standard) + "',";
        query += "'" + std::to_string(glider.second->weight) + "',";
        query += "'" + std::to_string(glider.second->maxweight) + "',";
        query += "'" + std::to_string(glider.second->rotatespeed) + "',";
        query += "'" + std::to_string(glider.second->armor) + "',";
        query += "'" + std::to_string(glider.second->price) + "',";
        query += "'" + std::to_string(glider.second->restore) + "',";
        query += "'" + std::to_string(glider.second->power) + "',";
        query += "'" + std::to_string(glider.second->special) + "',";
        query.resize(query.size() - 1);
        query += "),\n";
    }
    query.resize(query.size() - 2);
    query += ";";
    db->execute(query.c_str(), 0, 0);
}

void StorageImpl::_loadGoods()
{
    auto callback = [](void *o, int ncols, char **cols, char **names)
    {
        Ptr<Good> good = std::make_shared<Good>();
        if (cols[0]) good->id = std::stoi(cols[0]);
        if (cols[1]) good->text_id = cols[1];
        if (cols[2]) good->resource = cols[2];
        if (cols[3]) good->name.id = std::stoi(cols[3]);
        if (cols[4]) good->price = std::stoi(cols[4]);
        if (cols[5]) good->notrade = std::stoi(cols[5]);
        if (cols[6]) good->weight = std::stof(cols[6]);

        Storage *storage = (Storage *)o;
        storage->goods[good->id] = good;
        return 0;
    };
    db->execute("select * from Goods;", this, callback);
}

void StorageImpl::_loadGoodsPtrs()
{
    for (auto &good : goods)
    {
        if (strings.find(good.second->name.id) != strings.end())
            good.second->name.ptr = strings[good.second->name.id];
    }
}

void StorageImpl::_loadGoodsArrays()
{
}

void StorageImpl::_saveGoods()
{
    std::string query;
    query += "delete from Goods;";
    db->execute(query.c_str(), 0, 0);
    query.clear();
    if (goods.empty())
        return;
    query += "insert or replace into Goods values\n";
    for (auto &good : goods)
    {
        query += "(";
        query += "'" + std::to_string(good.second->id) + "',";
        query += "'" + good.second->text_id.string() + "',";
        query += "'" + good.second->resource.string() + "',";
        query += "'" + std::to_string(good.second->name.id) + "',";
        query += "'" + std::to_string(good.second->price) + "',";
        query += "'" + std::to_string(good.second->notrade) + "',";
        query += "'" + std::to_string(good.second->weight) + "',";
        query.resize(query.size() - 1);
        query += "),\n";
    }
    query.resize(query.size() - 2);
    query += ";";
    db->execute(query.c_str(), 0, 0);
}

void StorageImpl::_loadMapBuildingEquipments()
{
    auto callback = [](void *o, int ncols, char **cols, char **names)
    {
        Ptr<MapBuildingEquipment> mapBuildingEquipment = std::make_shared<MapBuildingEquipment>();
        if (cols[0]) mapBuildingEquipment->mapBuilding.id = std::stoi(cols[0]);
        if (cols[1]) mapBuildingEquipment->equipment.id = std::stoi(cols[1]);
        if (cols[2]) mapBuildingEquipment->quantity = std::stoi(cols[2]);

        Storage *storage = (Storage *)o;
        storage->mapBuildingEquipments.push_back(mapBuildingEquipment);
        return 0;
    };
    db->execute("select * from MapBuildingEquipments;", this, callback);
}

void StorageImpl::_loadMapBuildingEquipmentsPtrs()
{
    for (auto &mapBuildingEquipment : mapBuildingEquipments)
    {
        if (mapBuildings.find(mapBuildingEquipment->mapBuilding.id) != mapBuildings.end())
            mapBuildingEquipment->mapBuilding.ptr = mapBuildings[mapBuildingEquipment->mapBuilding.id];
        if (equipments.find(mapBuildingEquipment->equipment.id) != equipments.end())
            mapBuildingEquipment->equipment.ptr = equipments[mapBuildingEquipment->equipment.id];
    }
}

void StorageImpl::_loadMapBuildingEquipmentsArrays()
{
}

void StorageImpl::_saveMapBuildingEquipments()
{
    std::string query;
    query += "delete from MapBuildingEquipments;";
    db->execute(query.c_str(), 0, 0);
    query.clear();
    if (mapBuildingEquipments.empty())
        return;
    query += "insert or replace into MapBuildingEquipments values\n";
    for (auto &mapBuildingEquipment : mapBuildingEquipments)
    {
        query += "(";
        query += "'" + std::to_string(mapBuildingEquipment->mapBuilding.id) + "',";
        query += "'" + std::to_string(mapBuildingEquipment->equipment.id) + "',";
        query += "'" + std::to_string(mapBuildingEquipment->quantity) + "',";
        query.resize(query.size() - 1);
        query += "),\n";
    }
    query.resize(query.size() - 2);
    query += ";";
    db->execute(query.c_str(), 0, 0);
}

void StorageImpl::_loadMapBuildingGliders()
{
    auto callback = [](void *o, int ncols, char **cols, char **names)
    {
        Ptr<MapBuildingGlider> mapBuildingGlider = std::make_shared<MapBuildingGlider>();
        if (cols[0]) mapBuildingGlider->mapBuilding.id = std::stoi(cols[0]);
        if (cols[1]) mapBuildingGlider->glider.id = std::stoi(cols[1]);
        if (cols[2]) mapBuildingGlider->quantity = std::stoi(cols[2]);

        Storage *storage = (Storage *)o;
        storage->mapBuildingGliders.push_back(mapBuildingGlider);
        return 0;
    };
    db->execute("select * from MapBuildingGliders;", this, callback);
}

void StorageImpl::_loadMapBuildingGlidersPtrs()
{
    for (auto &mapBuildingGlider : mapBuildingGliders)
    {
        if (mapBuildings.find(mapBuildingGlider->mapBuilding.id) != mapBuildings.end())
            mapBuildingGlider->mapBuilding.ptr = mapBuildings[mapBuildingGlider->mapBuilding.id];
        if (gliders.find(mapBuildingGlider->glider.id) != gliders.end())
            mapBuildingGlider->glider.ptr = gliders[mapBuildingGlider->glider.id];
    }
}

void StorageImpl::_loadMapBuildingGlidersArrays()
{
}

void StorageImpl::_saveMapBuildingGliders()
{
    std::string query;
    query += "delete from MapBuildingGliders;";
    db->execute(query.c_str(), 0, 0);
    query.clear();
    if (mapBuildingGliders.empty())
        return;
    query += "insert or replace into MapBuildingGliders values\n";
    for (auto &mapBuildingGlider : mapBuildingGliders)
    {
        query += "(";
        query += "'" + std::to_string(mapBuildingGlider->mapBuilding.id) + "',";
        query += "'" + std::to_string(mapBuildingGlider->glider.id) + "',";
        query += "'" + std::to_string(mapBuildingGlider->quantity) + "',";
        query.resize(query.size() - 1);
        query += "),\n";
    }
    query.resize(query.size() - 2);
    query += ";";
    db->execute(query.c_str(), 0, 0);
}

void StorageImpl::_loadMapBuildingGoods()
{
    auto callback = [](void *o, int ncols, char **cols, char **names)
    {
        Ptr<MapBuildingGood> mapBuildingGood = std::make_shared<MapBuildingGood>();
        if (cols[0]) mapBuildingGood->mapBuilding.id = std::stoi(cols[0]);
        if (cols[1]) mapBuildingGood->good.id = std::stoi(cols[1]);
        if (cols[2]) mapBuildingGood->quantity = std::stoi(cols[2]);

        Storage *storage = (Storage *)o;
        storage->mapBuildingGoods.push_back(mapBuildingGood);
        return 0;
    };
    db->execute("select * from MapBuildingGoods;", this, callback);
}

void StorageImpl::_loadMapBuildingGoodsPtrs()
{
    for (auto &mapBuildingGood : mapBuildingGoods)
    {
        if (mapBuildings.find(mapBuildingGood->mapBuilding.id) != mapBuildings.end())
            mapBuildingGood->mapBuilding.ptr = mapBuildings[mapBuildingGood->mapBuilding.id];
        if (goods.find(mapBuildingGood->good.id) != goods.end())
            mapBuildingGood->good.ptr = goods[mapBuildingGood->good.id];
    }
}

void StorageImpl::_loadMapBuildingGoodsArrays()
{
}

void StorageImpl::_saveMapBuildingGoods()
{
    std::string query;
    query += "delete from MapBuildingGoods;";
    db->execute(query.c_str(), 0, 0);
    query.clear();
    if (mapBuildingGoods.empty())
        return;
    query += "insert or replace into MapBuildingGoods values\n";
    for (auto &mapBuildingGood : mapBuildingGoods)
    {
        query += "(";
        query += "'" + std::to_string(mapBuildingGood->mapBuilding.id) + "',";
        query += "'" + std::to_string(mapBuildingGood->good.id) + "',";
        query += "'" + std::to_string(mapBuildingGood->quantity) + "',";
        query.resize(query.size() - 1);
        query += "),\n";
    }
    query.resize(query.size() - 2);
    query += ";";
    db->execute(query.c_str(), 0, 0);
}

void StorageImpl::_loadMapBuildingModificators()
{
    auto callback = [](void *o, int ncols, char **cols, char **names)
    {
        Ptr<MapBuildingModificator> mapBuildingModificator = std::make_shared<MapBuildingModificator>();
        if (cols[0]) mapBuildingModificator->mapBuilding.id = std::stoi(cols[0]);
        if (cols[1]) mapBuildingModificator->modificator.id = std::stoi(cols[1]);
        if (cols[2]) mapBuildingModificator->quantity = std::stoi(cols[2]);

        Storage *storage = (Storage *)o;
        storage->mapBuildingModificators.push_back(mapBuildingModificator);
        return 0;
    };
    db->execute("select * from MapBuildingModificators;", this, callback);
}

void StorageImpl::_loadMapBuildingModificatorsPtrs()
{
    for (auto &mapBuildingModificator : mapBuildingModificators)
    {
        if (mapBuildings.find(mapBuildingModificator->mapBuilding.id) != mapBuildings.end())
            mapBuildingModificator->mapBuilding.ptr = mapBuildings[mapBuildingModificator->mapBuilding.id];
        if (modificators.find(mapBuildingModificator->modificator.id) != modificators.end())
            mapBuildingModificator->modificator.ptr = modificators[mapBuildingModificator->modificator.id];
    }
}

void StorageImpl::_loadMapBuildingModificatorsArrays()
{
}

void StorageImpl::_saveMapBuildingModificators()
{
    std::string query;
    query += "delete from MapBuildingModificators;";
    db->execute(query.c_str(), 0, 0);
    query.clear();
    if (mapBuildingModificators.empty())
        return;
    query += "insert or replace into MapBuildingModificators values\n";
    for (auto &mapBuildingModificator : mapBuildingModificators)
    {
        query += "(";
        query += "'" + std::to_string(mapBuildingModificator->mapBuilding.id) + "',";
        query += "'" + std::to_string(mapBuildingModificator->modificator.id) + "',";
        query += "'" + std::to_string(mapBuildingModificator->quantity) + "',";
        query.resize(query.size() - 1);
        query += "),\n";
    }
    query.resize(query.size() - 2);
    query += ";";
    db->execute(query.c_str(), 0, 0);
}

void StorageImpl::_loadMapBuildingProjectiles()
{
    auto callback = [](void *o, int ncols, char **cols, char **names)
    {
        Ptr<MapBuildingProjectile> mapBuildingProjectile = std::make_shared<MapBuildingProjectile>();
        if (cols[0]) mapBuildingProjectile->mapBuilding.id = std::stoi(cols[0]);
        if (cols[1]) mapBuildingProjectile->projectile.id = std::stoi(cols[1]);
        if (cols[2]) mapBuildingProjectile->quantity = std::stoi(cols[2]);

        Storage *storage = (Storage *)o;
        storage->mapBuildingProjectiles.push_back(mapBuildingProjectile);
        return 0;
    };
    db->execute("select * from MapBuildingProjectiles;", this, callback);
}

void StorageImpl::_loadMapBuildingProjectilesPtrs()
{
    for (auto &mapBuildingProjectile : mapBuildingProjectiles)
    {
        if (mapBuildings.find(mapBuildingProjectile->mapBuilding.id) != mapBuildings.end())
            mapBuildingProjectile->mapBuilding.ptr = mapBuildings[mapBuildingProjectile->mapBuilding.id];
        if (projectiles.find(mapBuildingProjectile->projectile.id) != projectiles.end())
            mapBuildingProjectile->projectile.ptr = projectiles[mapBuildingProjectile->projectile.id];
    }
}

void StorageImpl::_loadMapBuildingProjectilesArrays()
{
}

void StorageImpl::_saveMapBuildingProjectiles()
{
    std::string query;
    query += "delete from MapBuildingProjectiles;";
    db->execute(query.c_str(), 0, 0);
    query.clear();
    if (mapBuildingProjectiles.empty())
        return;
    query += "insert or replace into MapBuildingProjectiles values\n";
    for (auto &mapBuildingProjectile : mapBuildingProjectiles)
    {
        query += "(";
        query += "'" + std::to_string(mapBuildingProjectile->mapBuilding.id) + "',";
        query += "'" + std::to_string(mapBuildingProjectile->projectile.id) + "',";
        query += "'" + std::to_string(mapBuildingProjectile->quantity) + "',";
        query.resize(query.size() - 1);
        query += "),\n";
    }
    query.resize(query.size() - 2);
    query += ";";
    db->execute(query.c_str(), 0, 0);
}

void StorageImpl::_loadMapBuildingWeapons()
{
    auto callback = [](void *o, int ncols, char **cols, char **names)
    {
        Ptr<MapBuildingWeapon> mapBuildingWeapon = std::make_shared<MapBuildingWeapon>();
        if (cols[0]) mapBuildingWeapon->mapBuilding.id = std::stoi(cols[0]);
        if (cols[1]) mapBuildingWeapon->weapon.id = std::stoi(cols[1]);
        if (cols[2]) mapBuildingWeapon->quantity = std::stoi(cols[2]);

        Storage *storage = (Storage *)o;
        storage->mapBuildingWeapons.push_back(mapBuildingWeapon);
        return 0;
    };
    db->execute("select * from MapBuildingWeapons;", this, callback);
}

void StorageImpl::_loadMapBuildingWeaponsPtrs()
{
    for (auto &mapBuildingWeapon : mapBuildingWeapons)
    {
        if (mapBuildings.find(mapBuildingWeapon->mapBuilding.id) != mapBuildings.end())
            mapBuildingWeapon->mapBuilding.ptr = mapBuildings[mapBuildingWeapon->mapBuilding.id];
        if (weapons.find(mapBuildingWeapon->weapon.id) != weapons.end())
            mapBuildingWeapon->weapon.ptr = weapons[mapBuildingWeapon->weapon.id];
    }
}

void StorageImpl::_loadMapBuildingWeaponsArrays()
{
}

void StorageImpl::_saveMapBuildingWeapons()
{
    std::string query;
    query += "delete from MapBuildingWeapons;";
    db->execute(query.c_str(), 0, 0);
    query.clear();
    if (mapBuildingWeapons.empty())
        return;
    query += "insert or replace into MapBuildingWeapons values\n";
    for (auto &mapBuildingWeapon : mapBuildingWeapons)
    {
        query += "(";
        query += "'" + std::to_string(mapBuildingWeapon->mapBuilding.id) + "',";
        query += "'" + std::to_string(mapBuildingWeapon->weapon.id) + "',";
        query += "'" + std::to_string(mapBuildingWeapon->quantity) + "',";
        query.resize(query.size() - 1);
        query += "),\n";
    }
    query.resize(query.size() - 2);
    query += ";";
    db->execute(query.c_str(), 0, 0);
}

void StorageImpl::_loadMapBuildings()
{
    auto callback = [](void *o, int ncols, char **cols, char **names)
    {
        Ptr<MapBuilding> mapBuilding = std::make_shared<MapBuilding>();
        if (cols[0]) mapBuilding->id = std::stoi(cols[0]);
        if (cols[1]) mapBuilding->text_id = cols[1];
        if (cols[2]) mapBuilding->map.id = std::stoi(cols[2]);
        if (cols[3]) mapBuilding->building.id = std::stoi(cols[3]);
        if (cols[4]) mapBuilding->x = std::stof(cols[4]);
        if (cols[5]) mapBuilding->y = std::stof(cols[5]);
        if (cols[6]) mapBuilding->z = std::stof(cols[6]);
        if (cols[7]) mapBuilding->pitch = std::stof(cols[7]);
        if (cols[8]) mapBuilding->yaw = std::stof(cols[8]);
        if (cols[9]) mapBuilding->roll = std::stof(cols[9]);

        Storage *storage = (Storage *)o;
        storage->mapBuildings[mapBuilding->id] = mapBuilding;
        return 0;
    };
    db->execute("select * from MapBuildings;", this, callback);
}

void StorageImpl::_loadMapBuildingsPtrs()
{
    for (auto &mapBuilding : mapBuildings)
    {
        if (maps.find(mapBuilding.second->map.id) != maps.end())
            mapBuilding.second->map.ptr = maps[mapBuilding.second->map.id];
        if (buildings.find(mapBuilding.second->building.id) != buildings.end())
            mapBuilding.second->building.ptr = buildings[mapBuilding.second->building.id];
    }
}

void StorageImpl::_loadMapBuildingsArrays()
{
    for (auto &mapBuilding : mapBuildings)
    {
        for (auto &mapBuildingEquipment : mapBuildingEquipments)
            if (mapBuilding.first == mapBuildingEquipment->mapBuilding.id)
                mapBuilding.second->equipments.push_back(mapBuildingEquipment);
        for (auto &mapBuildingGlider : mapBuildingGliders)
            if (mapBuilding.first == mapBuildingGlider->mapBuilding.id)
                mapBuilding.second->gliders.push_back(mapBuildingGlider);
        for (auto &mapBuildingGood : mapBuildingGoods)
            if (mapBuilding.first == mapBuildingGood->mapBuilding.id)
                mapBuilding.second->goods.push_back(mapBuildingGood);
        for (auto &mapBuildingModificator : mapBuildingModificators)
            if (mapBuilding.first == mapBuildingModificator->mapBuilding.id)
                mapBuilding.second->modificators.push_back(mapBuildingModificator);
        for (auto &mapBuildingProjectile : mapBuildingProjectiles)
            if (mapBuilding.first == mapBuildingProjectile->mapBuilding.id)
                mapBuilding.second->projectiles.push_back(mapBuildingProjectile);
        for (auto &mapBuildingWeapon : mapBuildingWeapons)
            if (mapBuilding.first == mapBuildingWeapon->mapBuilding.id)
                mapBuilding.second->weapons.push_back(mapBuildingWeapon);
    }
}

void StorageImpl::_saveMapBuildings()
{
    std::string query;
    query += "delete from MapBuildings;";
    db->execute(query.c_str(), 0, 0);
    query.clear();
    if (mapBuildings.empty())
        return;
    query += "insert or replace into MapBuildings values\n";
    for (auto &mapBuilding : mapBuildings)
    {
        query += "(";
        query += "'" + std::to_string(mapBuilding.second->id) + "',";
        query += "'" + mapBuilding.second->text_id.string() + "',";
        query += "'" + std::to_string(mapBuilding.second->map.id) + "',";
        query += "'" + std::to_string(mapBuilding.second->building.id) + "',";
        query += "'" + std::to_string(mapBuilding.second->x) + "',";
        query += "'" + std::to_string(mapBuilding.second->y) + "',";
        query += "'" + std::to_string(mapBuilding.second->z) + "',";
        query += "'" + std::to_string(mapBuilding.second->pitch) + "',";
        query += "'" + std::to_string(mapBuilding.second->yaw) + "',";
        query += "'" + std::to_string(mapBuilding.second->roll) + "',";
        query.resize(query.size() - 1);
        query += "),\n";
    }
    query.resize(query.size() - 2);
    query += ";";
    db->execute(query.c_str(), 0, 0);
}

void StorageImpl::_loadMapObjects()
{
    auto callback = [](void *o, int ncols, char **cols, char **names)
    {
        Ptr<MapObject> mapObject = std::make_shared<MapObject>();
        if (cols[0]) mapObject->id = std::stoi(cols[0]);
        if (cols[1]) mapObject->text_id = cols[1];
        if (cols[2]) mapObject->map.id = std::stoi(cols[2]);
        if (cols[3]) mapObject->object.id = std::stoi(cols[3]);
        if (cols[4]) mapObject->x = std::stof(cols[4]);
        if (cols[5]) mapObject->y = std::stof(cols[5]);
        if (cols[6]) mapObject->z = std::stof(cols[6]);
        if (cols[7]) mapObject->pitch = std::stof(cols[7]);
        if (cols[8]) mapObject->yaw = std::stof(cols[8]);
        if (cols[9]) mapObject->roll = std::stof(cols[9]);

        Storage *storage = (Storage *)o;
        storage->mapObjects[mapObject->id] = mapObject;
        return 0;
    };
    db->execute("select * from MapObjects;", this, callback);
}

void StorageImpl::_loadMapObjectsPtrs()
{
    for (auto &mapObject : mapObjects)
    {
        if (maps.find(mapObject.second->map.id) != maps.end())
            mapObject.second->map.ptr = maps[mapObject.second->map.id];
        if (objects.find(mapObject.second->object.id) != objects.end())
            mapObject.second->object.ptr = objects[mapObject.second->object.id];
    }
}

void StorageImpl::_loadMapObjectsArrays()
{
}

void StorageImpl::_saveMapObjects()
{
    std::string query;
    query += "delete from MapObjects;";
    db->execute(query.c_str(), 0, 0);
    query.clear();
    if (mapObjects.empty())
        return;
    query += "insert or replace into MapObjects values\n";
    for (auto &mapObject : mapObjects)
    {
        query += "(";
        query += "'" + std::to_string(mapObject.second->id) + "',";
        query += "'" + mapObject.second->text_id.string() + "',";
        query += "'" + std::to_string(mapObject.second->map.id) + "',";
        query += "'" + std::to_string(mapObject.second->object.id) + "',";
        query += "'" + std::to_string(mapObject.second->x) + "',";
        query += "'" + std::to_string(mapObject.second->y) + "',";
        query += "'" + std::to_string(mapObject.second->z) + "',";
        query += "'" + std::to_string(mapObject.second->pitch) + "',";
        query += "'" + std::to_string(mapObject.second->yaw) + "',";
        query += "'" + std::to_string(mapObject.second->roll) + "',";
        query.resize(query.size() - 1);
        query += "),\n";
    }
    query.resize(query.size() - 2);
    query += ";";
    db->execute(query.c_str(), 0, 0);
}

void StorageImpl::_loadMaps()
{
    auto callback = [](void *o, int ncols, char **cols, char **names)
    {
        Ptr<Map> map = std::make_shared<Map>();
        if (cols[0]) map->id = std::stoi(cols[0]);
        if (cols[1]) map->text_id = cols[1];
        if (cols[2]) map->resource = cols[2];
        if (cols[3]) map->name.id = std::stoi(cols[3]);
        if (cols[4]) map->x_k = std::stof(cols[4]);
        if (cols[5]) map->x_b = std::stof(cols[5]);
        if (cols[6]) map->y_k = std::stof(cols[6]);
        if (cols[7]) map->y_b = std::stof(cols[7]);
        if (cols[8]) map->z_k = std::stof(cols[8]);
        if (cols[9]) map->z_b = std::stof(cols[9]);

        Storage *storage = (Storage *)o;
        storage->maps[map->id] = map;
        return 0;
    };
    db->execute("select * from Maps;", this, callback);
}

void StorageImpl::_loadMapsPtrs()
{
    for (auto &map : maps)
    {
        if (strings.find(map.second->name.id) != strings.end())
            map.second->name.ptr = strings[map.second->name.id];
    }
}

void StorageImpl::_loadMapsArrays()
{
    for (auto &map : maps)
    {
        for (auto &mapBuilding : mapBuildings)
            if (map.first == mapBuilding.second->map.id)
                map.second->buildings.push_back(mapBuilding.second);
        for (auto &mapObject : mapObjects)
            if (map.first == mapObject.second->map.id)
                map.second->objects.push_back(mapObject.second);
    }
}

void StorageImpl::_saveMaps()
{
    std::string query;
    query += "delete from Maps;";
    db->execute(query.c_str(), 0, 0);
    query.clear();
    if (maps.empty())
        return;
    query += "insert or replace into Maps values\n";
    for (auto &map : maps)
    {
        query += "(";
        query += "'" + std::to_string(map.second->id) + "',";
        query += "'" + map.second->text_id.string() + "',";
        query += "'" + map.second->resource.string() + "',";
        query += "'" + std::to_string(map.second->name.id) + "',";
        query += "'" + std::to_string(map.second->x_k) + "',";
        query += "'" + std::to_string(map.second->x_b) + "',";
        query += "'" + std::to_string(map.second->y_k) + "',";
        query += "'" + std::to_string(map.second->y_b) + "',";
        query += "'" + std::to_string(map.second->z_k) + "',";
        query += "'" + std::to_string(map.second->z_b) + "',";
        query.resize(query.size() - 1);
        query += "),\n";
    }
    query.resize(query.size() - 2);
    query += ";";
    db->execute(query.c_str(), 0, 0);
}

void StorageImpl::_loadMechanoidGroups()
{
    auto callback = [](void *o, int ncols, char **cols, char **names)
    {
        Ptr<MechanoidGroup> mechanoidGroup = std::make_shared<MechanoidGroup>();
        if (cols[0]) mechanoidGroup->id = std::stoi(cols[0]);
        if (cols[1]) mechanoidGroup->text_id = cols[1];
        if (cols[2]) mechanoidGroup->name.id = std::stoi(cols[2]);

        Storage *storage = (Storage *)o;
        storage->mechanoidGroups[mechanoidGroup->id] = mechanoidGroup;
        return 0;
    };
    db->execute("select * from MechanoidGroups;", this, callback);
}

void StorageImpl::_loadMechanoidGroupsPtrs()
{
    for (auto &mechanoidGroup : mechanoidGroups)
    {
        if (strings.find(mechanoidGroup.second->name.id) != strings.end())
            mechanoidGroup.second->name.ptr = strings[mechanoidGroup.second->name.id];
    }
}

void StorageImpl::_loadMechanoidGroupsArrays()
{
}

void StorageImpl::_saveMechanoidGroups()
{
    std::string query;
    query += "delete from MechanoidGroups;";
    db->execute(query.c_str(), 0, 0);
    query.clear();
    if (mechanoidGroups.empty())
        return;
    query += "insert or replace into MechanoidGroups values\n";
    for (auto &mechanoidGroup : mechanoidGroups)
    {
        query += "(";
        query += "'" + std::to_string(mechanoidGroup.second->id) + "',";
        query += "'" + mechanoidGroup.second->text_id.string() + "',";
        query += "'" + std::to_string(mechanoidGroup.second->name.id) + "',";
        query.resize(query.size() - 1);
        query += "),\n";
    }
    query.resize(query.size() - 2);
    query += ";";
    db->execute(query.c_str(), 0, 0);
}

void StorageImpl::_loadMechanoids()
{
    auto callback = [](void *o, int ncols, char **cols, char **names)
    {
        Ptr<Mechanoid> mechanoid = std::make_shared<Mechanoid>();
        if (cols[0]) mechanoid->id = std::stoi(cols[0]);
        if (cols[1]) mechanoid->text_id = cols[1];
        if (cols[2]) mechanoid->name.id = std::stoi(cols[2]);
        if (cols[3]) mechanoid->generation = std::stoi(cols[3]);
        if (cols[4]) mechanoid->rating = std::stof(cols[4]);
        if (cols[5]) mechanoid->money = std::stof(cols[5]);
        if (cols[6]) mechanoid->configuration.id = std::stoi(cols[6]);
        if (cols[7]) mechanoid->mechanoidGroup.id = std::stoi(cols[7]);
        if (cols[8]) mechanoid->clan.id = std::stoi(cols[8]);
        if (cols[9]) mechanoid->rating_fight = std::stof(cols[9]);
        if (cols[10]) mechanoid->rating_courier = std::stof(cols[10]);
        if (cols[11]) mechanoid->rating_trade = std::stof(cols[11]);
        if (cols[12]) mechanoid->map.id = std::stoi(cols[12]);
        if (cols[13]) mechanoid->mapBuilding.id = std::stoi(cols[13]);
        if (cols[14]) mechanoid->x = std::stof(cols[14]);
        if (cols[15]) mechanoid->y = std::stof(cols[15]);
        if (cols[16]) mechanoid->z = std::stof(cols[16]);
        if (cols[17]) mechanoid->pitch = std::stof(cols[17]);
        if (cols[18]) mechanoid->yaw = std::stof(cols[18]);
        if (cols[19]) mechanoid->roll = std::stof(cols[19]);

        Storage *storage = (Storage *)o;
        storage->mechanoids[mechanoid->id] = mechanoid;
        return 0;
    };
    db->execute("select * from Mechanoids;", this, callback);
}

void StorageImpl::_loadMechanoidsPtrs()
{
    for (auto &mechanoid : mechanoids)
    {
        if (strings.find(mechanoid.second->name.id) != strings.end())
            mechanoid.second->name.ptr = strings[mechanoid.second->name.id];
        if (configurations.find(mechanoid.second->configuration.id) != configurations.end())
            mechanoid.second->configuration.ptr = configurations[mechanoid.second->configuration.id];
        if (mechanoidGroups.find(mechanoid.second->mechanoidGroup.id) != mechanoidGroups.end())
            mechanoid.second->mechanoidGroup.ptr = mechanoidGroups[mechanoid.second->mechanoidGroup.id];
        if (clans.find(mechanoid.second->clan.id) != clans.end())
            mechanoid.second->clan.ptr = clans[mechanoid.second->clan.id];
        if (maps.find(mechanoid.second->map.id) != maps.end())
            mechanoid.second->map.ptr = maps[mechanoid.second->map.id];
        if (mapBuildings.find(mechanoid.second->mapBuilding.id) != mapBuildings.end())
            mechanoid.second->mapBuilding.ptr = mapBuildings[mechanoid.second->mapBuilding.id];
    }
}

void StorageImpl::_loadMechanoidsArrays()
{
}

void StorageImpl::_saveMechanoids()
{
    std::string query;
    query += "delete from Mechanoids;";
    db->execute(query.c_str(), 0, 0);
    query.clear();
    if (mechanoids.empty())
        return;
    query += "insert or replace into Mechanoids values\n";
    for (auto &mechanoid : mechanoids)
    {
        query += "(";
        query += "'" + std::to_string(mechanoid.second->id) + "',";
        query += "'" + mechanoid.second->text_id.string() + "',";
        query += "'" + std::to_string(mechanoid.second->name.id) + "',";
        query += "'" + std::to_string(mechanoid.second->generation) + "',";
        query += "'" + std::to_string(mechanoid.second->rating) + "',";
        query += "'" + std::to_string(mechanoid.second->money) + "',";
        query += "'" + std::to_string(mechanoid.second->configuration.id) + "',";
        query += "'" + std::to_string(mechanoid.second->mechanoidGroup.id) + "',";
        query += "'" + std::to_string(mechanoid.second->clan.id) + "',";
        query += "'" + std::to_string(mechanoid.second->rating_fight) + "',";
        query += "'" + std::to_string(mechanoid.second->rating_courier) + "',";
        query += "'" + std::to_string(mechanoid.second->rating_trade) + "',";
        query += "'" + std::to_string(mechanoid.second->map.id) + "',";
        query += "'" + std::to_string(mechanoid.second->mapBuilding.id) + "',";
        query += "'" + std::to_string(mechanoid.second->x) + "',";
        query += "'" + std::to_string(mechanoid.second->y) + "',";
        query += "'" + std::to_string(mechanoid.second->z) + "',";
        query += "'" + std::to_string(mechanoid.second->pitch) + "',";
        query += "'" + std::to_string(mechanoid.second->yaw) + "',";
        query += "'" + std::to_string(mechanoid.second->roll) + "',";
        query.resize(query.size() - 1);
        query += "),\n";
    }
    query.resize(query.size() - 2);
    query += ";";
    db->execute(query.c_str(), 0, 0);
}

void StorageImpl::_loadModificationClans()
{
    auto callback = [](void *o, int ncols, char **cols, char **names)
    {
        Ptr<ModificationClan> modificationClan = std::make_shared<ModificationClan>();
        if (cols[0]) modificationClan->modification.id = std::stoi(cols[0]);
        if (cols[1]) modificationClan->clan.id = std::stoi(cols[1]);

        Storage *storage = (Storage *)o;
        storage->modificationClans.push_back(modificationClan);
        return 0;
    };
    db->execute("select * from ModificationClans;", this, callback);
}

void StorageImpl::_loadModificationClansPtrs()
{
    for (auto &modificationClan : modificationClans)
    {
        if (modifications.find(modificationClan->modification.id) != modifications.end())
            modificationClan->modification.ptr = modifications[modificationClan->modification.id];
        if (clans.find(modificationClan->clan.id) != clans.end())
            modificationClan->clan.ptr = clans[modificationClan->clan.id];
    }
}

void StorageImpl::_loadModificationClansArrays()
{
}

void StorageImpl::_saveModificationClans()
{
    std::string query;
    query += "delete from ModificationClans;";
    db->execute(query.c_str(), 0, 0);
    query.clear();
    if (modificationClans.empty())
        return;
    query += "insert or replace into ModificationClans values\n";
    for (auto &modificationClan : modificationClans)
    {
        query += "(";
        query += "'" + std::to_string(modificationClan->modification.id) + "',";
        query += "'" + std::to_string(modificationClan->clan.id) + "',";
        query.resize(query.size() - 1);
        query += "),\n";
    }
    query.resize(query.size() - 2);
    query += ";";
    db->execute(query.c_str(), 0, 0);
}

void StorageImpl::_loadModificationMaps()
{
    auto callback = [](void *o, int ncols, char **cols, char **names)
    {
        Ptr<ModificationMap> modificationMap = std::make_shared<ModificationMap>();
        if (cols[0]) modificationMap->modification.id = std::stoi(cols[0]);
        if (cols[1]) modificationMap->map.id = std::stoi(cols[1]);

        Storage *storage = (Storage *)o;
        storage->modificationMaps.push_back(modificationMap);
        return 0;
    };
    db->execute("select * from ModificationMaps;", this, callback);
}

void StorageImpl::_loadModificationMapsPtrs()
{
    for (auto &modificationMap : modificationMaps)
    {
        if (modifications.find(modificationMap->modification.id) != modifications.end())
            modificationMap->modification.ptr = modifications[modificationMap->modification.id];
        if (maps.find(modificationMap->map.id) != maps.end())
            modificationMap->map.ptr = maps[modificationMap->map.id];
    }
}

void StorageImpl::_loadModificationMapsArrays()
{
}

void StorageImpl::_saveModificationMaps()
{
    std::string query;
    query += "delete from ModificationMaps;";
    db->execute(query.c_str(), 0, 0);
    query.clear();
    if (modificationMaps.empty())
        return;
    query += "insert or replace into ModificationMaps values\n";
    for (auto &modificationMap : modificationMaps)
    {
        query += "(";
        query += "'" + std::to_string(modificationMap->modification.id) + "',";
        query += "'" + std::to_string(modificationMap->map.id) + "',";
        query.resize(query.size() - 1);
        query += "),\n";
    }
    query.resize(query.size() - 2);
    query += ";";
    db->execute(query.c_str(), 0, 0);
}

void StorageImpl::_loadModificationMechanoids()
{
    auto callback = [](void *o, int ncols, char **cols, char **names)
    {
        Ptr<ModificationMechanoid> modificationMechanoid = std::make_shared<ModificationMechanoid>();
        if (cols[0]) modificationMechanoid->modification.id = std::stoi(cols[0]);
        if (cols[1]) modificationMechanoid->mechanoid.id = std::stoi(cols[1]);

        Storage *storage = (Storage *)o;
        storage->modificationMechanoids.push_back(modificationMechanoid);
        return 0;
    };
    db->execute("select * from ModificationMechanoids;", this, callback);
}

void StorageImpl::_loadModificationMechanoidsPtrs()
{
    for (auto &modificationMechanoid : modificationMechanoids)
    {
        if (modifications.find(modificationMechanoid->modification.id) != modifications.end())
            modificationMechanoid->modification.ptr = modifications[modificationMechanoid->modification.id];
        if (mechanoids.find(modificationMechanoid->mechanoid.id) != mechanoids.end())
            modificationMechanoid->mechanoid.ptr = mechanoids[modificationMechanoid->mechanoid.id];
    }
}

void StorageImpl::_loadModificationMechanoidsArrays()
{
}

void StorageImpl::_saveModificationMechanoids()
{
    std::string query;
    query += "delete from ModificationMechanoids;";
    db->execute(query.c_str(), 0, 0);
    query.clear();
    if (modificationMechanoids.empty())
        return;
    query += "insert or replace into ModificationMechanoids values\n";
    for (auto &modificationMechanoid : modificationMechanoids)
    {
        query += "(";
        query += "'" + std::to_string(modificationMechanoid->modification.id) + "',";
        query += "'" + std::to_string(modificationMechanoid->mechanoid.id) + "',";
        query.resize(query.size() - 1);
        query += "),\n";
    }
    query.resize(query.size() - 2);
    query += ";";
    db->execute(query.c_str(), 0, 0);
}

void StorageImpl::_loadModifications()
{
    auto callback = [](void *o, int ncols, char **cols, char **names)
    {
        Ptr<Modification> modification = std::make_shared<Modification>();
        if (cols[0]) modification->id = std::stoi(cols[0]);
        if (cols[1]) modification->name.id = std::stoi(cols[1]);
        if (cols[2]) modification->directory = cols[2];
        if (cols[3]) modification->author = cols[3];
        if (cols[4]) modification->date_created = cols[4];
        if (cols[5]) modification->date_modified = cols[5];
        if (cols[6]) modification->comment = cols[6];
        if (cols[7]) modification->version = std::stof(cols[7]);
        if (cols[8]) modification->script_language = cols[8];
        if (cols[9]) modification->script_main = cols[9];
        if (cols[10]) modification->player_mechanoid.id = std::stoi(cols[10]);
        if (cols[11]) modification->cooperative_player_configuration.id = std::stoi(cols[11]);

        Storage *storage = (Storage *)o;
        storage->modifications[modification->id] = modification;
        return 0;
    };
    db->execute("select * from Modifications;", this, callback);
}

void StorageImpl::_loadModificationsPtrs()
{
    for (auto &modification : modifications)
    {
        if (strings.find(modification.second->name.id) != strings.end())
            modification.second->name.ptr = strings[modification.second->name.id];
        if (mechanoids.find(modification.second->player_mechanoid.id) != mechanoids.end())
            modification.second->player_mechanoid.ptr = mechanoids[modification.second->player_mechanoid.id];
        if (configurations.find(modification.second->cooperative_player_configuration.id) != configurations.end())
            modification.second->cooperative_player_configuration.ptr = configurations[modification.second->cooperative_player_configuration.id];
    }
}

void StorageImpl::_loadModificationsArrays()
{
    for (auto &modification : modifications)
    {
        for (auto &modificationClan : modificationClans)
            if (modification.first == modificationClan->modification.id)
                modification.second->clans.push_back(modificationClan);
        for (auto &modificationMap : modificationMaps)
            if (modification.first == modificationMap->modification.id)
                modification.second->maps.push_back(modificationMap);
        for (auto &modificationMechanoid : modificationMechanoids)
            if (modification.first == modificationMechanoid->modification.id)
                modification.second->mechanoids.push_back(modificationMechanoid);
    }
}

void StorageImpl::_saveModifications()
{
    std::string query;
    query += "delete from Modifications;";
    db->execute(query.c_str(), 0, 0);
    query.clear();
    if (modifications.empty())
        return;
    query += "insert or replace into Modifications values\n";
    for (auto &modification : modifications)
    {
        query += "(";
        query += "'" + std::to_string(modification.second->id) + "',";
        query += "'" + std::to_string(modification.second->name.id) + "',";
        query += "'" + modification.second->directory.string() + "',";
        query += "'" + modification.second->author.string() + "',";
        query += "'" + modification.second->date_created.string() + "',";
        query += "'" + modification.second->date_modified.string() + "',";
        query += "'" + modification.second->comment.string() + "',";
        query += "'" + std::to_string(modification.second->version) + "',";
        query += "'" + modification.second->script_language.string() + "',";
        query += "'" + modification.second->script_main.string() + "',";
        query += "'" + std::to_string(modification.second->player_mechanoid.id) + "',";
        query += "'" + std::to_string(modification.second->cooperative_player_configuration.id) + "',";
        query.resize(query.size() - 1);
        query += "),\n";
    }
    query.resize(query.size() - 2);
    query += ";";
    db->execute(query.c_str(), 0, 0);
}

void StorageImpl::_loadModificators()
{
    auto callback = [](void *o, int ncols, char **cols, char **names)
    {
        Ptr<Modificator> modificator = std::make_shared<Modificator>();
        if (cols[0]) modificator->id = std::stoi(cols[0]);
        if (cols[1]) modificator->text_id = cols[1];
        if (cols[2]) modificator->resource = cols[2];
        if (cols[3]) modificator->name.id = std::stoi(cols[3]);
        if (cols[4]) modificator->probability = std::stof(cols[4]);
        if (cols[5]) modificator->price = std::stof(cols[5]);
        if (cols[6]) modificator->k_price = std::stof(cols[6]);
        if (cols[7]) modificator->k_param1 = std::stof(cols[7]);
        if (cols[8]) modificator->k_param2 = std::stof(cols[8]);

        Storage *storage = (Storage *)o;
        storage->modificators[modificator->id] = modificator;
        return 0;
    };
    db->execute("select * from Modificators;", this, callback);
}

void StorageImpl::_loadModificatorsPtrs()
{
    for (auto &modificator : modificators)
    {
        if (strings.find(modificator.second->name.id) != strings.end())
            modificator.second->name.ptr = strings[modificator.second->name.id];
    }
}

void StorageImpl::_loadModificatorsArrays()
{
}

void StorageImpl::_saveModificators()
{
    std::string query;
    query += "delete from Modificators;";
    db->execute(query.c_str(), 0, 0);
    query.clear();
    if (modificators.empty())
        return;
    query += "insert or replace into Modificators values\n";
    for (auto &modificator : modificators)
    {
        query += "(";
        query += "'" + std::to_string(modificator.second->id) + "',";
        query += "'" + modificator.second->text_id.string() + "',";
        query += "'" + modificator.second->resource.string() + "',";
        query += "'" + std::to_string(modificator.second->name.id) + "',";
        query += "'" + std::to_string(modificator.second->probability) + "',";
        query += "'" + std::to_string(modificator.second->price) + "',";
        query += "'" + std::to_string(modificator.second->k_price) + "',";
        query += "'" + std::to_string(modificator.second->k_param1) + "',";
        query += "'" + std::to_string(modificator.second->k_param2) + "',";
        query.resize(query.size() - 1);
        query += "),\n";
    }
    query.resize(query.size() - 2);
    query += ";";
    db->execute(query.c_str(), 0, 0);
}

void StorageImpl::_loadObjects()
{
    auto callback = [](void *o, int ncols, char **cols, char **names)
    {
        Ptr<Object> object = std::make_shared<Object>();
        if (cols[0]) object->id = std::stoi(cols[0]);
        if (cols[1]) object->text_id = cols[1];
        if (cols[2]) object->resource = cols[2];
        if (cols[3]) object->name.id = std::stoi(cols[3]);
        if (cols[4]) object->type = std::stoi(cols[4]);

        Storage *storage = (Storage *)o;
        storage->objects[object->id] = object;
        return 0;
    };
    db->execute("select * from Objects;", this, callback);
}

void StorageImpl::_loadObjectsPtrs()
{
    for (auto &object : objects)
    {
        if (strings.find(object.second->name.id) != strings.end())
            object.second->name.ptr = strings[object.second->name.id];
    }
}

void StorageImpl::_loadObjectsArrays()
{
}

void StorageImpl::_saveObjects()
{
    std::string query;
    query += "delete from Objects;";
    db->execute(query.c_str(), 0, 0);
    query.clear();
    if (objects.empty())
        return;
    query += "insert or replace into Objects values\n";
    for (auto &object : objects)
    {
        query += "(";
        query += "'" + std::to_string(object.second->id) + "',";
        query += "'" + object.second->text_id.string() + "',";
        query += "'" + object.second->resource.string() + "',";
        query += "'" + std::to_string(object.second->name.id) + "',";
        query += "'" + std::to_string(object.second->type) + "',";
        query.resize(query.size() - 1);
        query += "),\n";
    }
    query.resize(query.size() - 2);
    query += ";";
    db->execute(query.c_str(), 0, 0);
}

void StorageImpl::_loadPlayers()
{
    auto callback = [](void *o, int ncols, char **cols, char **names)
    {
        Ptr<Player> player = std::make_shared<Player>();
        if (cols[0]) player->id = std::stoi(cols[0]);
        if (cols[1]) player->mechanoid.id = std::stoi(cols[1]);

        Storage *storage = (Storage *)o;
        storage->players[player->id] = player;
        return 0;
    };
    db->execute("select * from Players;", this, callback);
}

void StorageImpl::_loadPlayersPtrs()
{
    for (auto &player : players)
    {
        if (mechanoids.find(player.second->mechanoid.id) != mechanoids.end())
            player.second->mechanoid.ptr = mechanoids[player.second->mechanoid.id];
    }
}

void StorageImpl::_loadPlayersArrays()
{
}

void StorageImpl::_savePlayers()
{
    std::string query;
    query += "delete from Players;";
    db->execute(query.c_str(), 0, 0);
    query.clear();
    if (players.empty())
        return;
    query += "insert or replace into Players values\n";
    for (auto &player : players)
    {
        query += "(";
        query += "'" + std::to_string(player.second->id) + "',";
        query += "'" + std::to_string(player.second->mechanoid.id) + "',";
        query.resize(query.size() - 1);
        query += "),\n";
    }
    query.resize(query.size() - 2);
    query += ";";
    db->execute(query.c_str(), 0, 0);
}

void StorageImpl::_loadProjectiles()
{
    auto callback = [](void *o, int ncols, char **cols, char **names)
    {
        Ptr<Projectile> projectile = std::make_shared<Projectile>();
        if (cols[0]) projectile->id = std::stoi(cols[0]);
        if (cols[1]) projectile->text_id = cols[1];
        if (cols[2]) projectile->resource = cols[2];
        if (cols[3]) projectile->name.id = std::stoi(cols[3]);
        if (cols[4]) projectile->type = std::stoi(cols[4]);
        if (cols[5]) projectile->weight = std::stof(cols[5]);
        if (cols[6]) projectile->damage = std::stof(cols[6]);
        if (cols[7]) projectile->speed = std::stof(cols[7]);
        if (cols[8]) projectile->scale = std::stof(cols[8]);
        if (cols[9]) projectile->notrade = std::stoi(cols[9]);

        Storage *storage = (Storage *)o;
        storage->projectiles[projectile->id] = projectile;
        return 0;
    };
    db->execute("select * from Projectiles;", this, callback);
}

void StorageImpl::_loadProjectilesPtrs()
{
    for (auto &projectile : projectiles)
    {
        if (strings.find(projectile.second->name.id) != strings.end())
            projectile.second->name.ptr = strings[projectile.second->name.id];
    }
}

void StorageImpl::_loadProjectilesArrays()
{
}

void StorageImpl::_saveProjectiles()
{
    std::string query;
    query += "delete from Projectiles;";
    db->execute(query.c_str(), 0, 0);
    query.clear();
    if (projectiles.empty())
        return;
    query += "insert or replace into Projectiles values\n";
    for (auto &projectile : projectiles)
    {
        query += "(";
        query += "'" + std::to_string(projectile.second->id) + "',";
        query += "'" + projectile.second->text_id.string() + "',";
        query += "'" + projectile.second->resource.string() + "',";
        query += "'" + std::to_string(projectile.second->name.id) + "',";
        query += "'" + std::to_string(projectile.second->type) + "',";
        query += "'" + std::to_string(projectile.second->weight) + "',";
        query += "'" + std::to_string(projectile.second->damage) + "',";
        query += "'" + std::to_string(projectile.second->speed) + "',";
        query += "'" + std::to_string(projectile.second->scale) + "',";
        query += "'" + std::to_string(projectile.second->notrade) + "',";
        query.resize(query.size() - 1);
        query += "),\n";
    }
    query.resize(query.size() - 2);
    query += ";";
    db->execute(query.c_str(), 0, 0);
}

void StorageImpl::_loadQuestRewardEquipments()
{
    auto callback = [](void *o, int ncols, char **cols, char **names)
    {
        Ptr<QuestRewardEquipment> questRewardEquipment = std::make_shared<QuestRewardEquipment>();
        if (cols[0]) questRewardEquipment->questReward.id = std::stoi(cols[0]);
        if (cols[1]) questRewardEquipment->equipment.id = std::stoi(cols[1]);
        if (cols[2]) questRewardEquipment->quantity = std::stoi(cols[2]);

        Storage *storage = (Storage *)o;
        storage->questRewardEquipments.push_back(questRewardEquipment);
        return 0;
    };
    db->execute("select * from QuestRewardEquipments;", this, callback);
}

void StorageImpl::_loadQuestRewardEquipmentsPtrs()
{
    for (auto &questRewardEquipment : questRewardEquipments)
    {
        if (questRewards.find(questRewardEquipment->questReward.id) != questRewards.end())
            questRewardEquipment->questReward.ptr = questRewards[questRewardEquipment->questReward.id];
        if (equipments.find(questRewardEquipment->equipment.id) != equipments.end())
            questRewardEquipment->equipment.ptr = equipments[questRewardEquipment->equipment.id];
    }
}

void StorageImpl::_loadQuestRewardEquipmentsArrays()
{
}

void StorageImpl::_saveQuestRewardEquipments()
{
    std::string query;
    query += "delete from QuestRewardEquipments;";
    db->execute(query.c_str(), 0, 0);
    query.clear();
    if (questRewardEquipments.empty())
        return;
    query += "insert or replace into QuestRewardEquipments values\n";
    for (auto &questRewardEquipment : questRewardEquipments)
    {
        query += "(";
        query += "'" + std::to_string(questRewardEquipment->questReward.id) + "',";
        query += "'" + std::to_string(questRewardEquipment->equipment.id) + "',";
        query += "'" + std::to_string(questRewardEquipment->quantity) + "',";
        query.resize(query.size() - 1);
        query += "),\n";
    }
    query.resize(query.size() - 2);
    query += ";";
    db->execute(query.c_str(), 0, 0);
}

void StorageImpl::_loadQuestRewardGliders()
{
    auto callback = [](void *o, int ncols, char **cols, char **names)
    {
        Ptr<QuestRewardGlider> questRewardGlider = std::make_shared<QuestRewardGlider>();
        if (cols[0]) questRewardGlider->questReward.id = std::stoi(cols[0]);
        if (cols[1]) questRewardGlider->glider.id = std::stoi(cols[1]);
        if (cols[2]) questRewardGlider->quantity = std::stoi(cols[2]);

        Storage *storage = (Storage *)o;
        storage->questRewardGliders.push_back(questRewardGlider);
        return 0;
    };
    db->execute("select * from QuestRewardGliders;", this, callback);
}

void StorageImpl::_loadQuestRewardGlidersPtrs()
{
    for (auto &questRewardGlider : questRewardGliders)
    {
        if (questRewards.find(questRewardGlider->questReward.id) != questRewards.end())
            questRewardGlider->questReward.ptr = questRewards[questRewardGlider->questReward.id];
        if (gliders.find(questRewardGlider->glider.id) != gliders.end())
            questRewardGlider->glider.ptr = gliders[questRewardGlider->glider.id];
    }
}

void StorageImpl::_loadQuestRewardGlidersArrays()
{
}

void StorageImpl::_saveQuestRewardGliders()
{
    std::string query;
    query += "delete from QuestRewardGliders;";
    db->execute(query.c_str(), 0, 0);
    query.clear();
    if (questRewardGliders.empty())
        return;
    query += "insert or replace into QuestRewardGliders values\n";
    for (auto &questRewardGlider : questRewardGliders)
    {
        query += "(";
        query += "'" + std::to_string(questRewardGlider->questReward.id) + "',";
        query += "'" + std::to_string(questRewardGlider->glider.id) + "',";
        query += "'" + std::to_string(questRewardGlider->quantity) + "',";
        query.resize(query.size() - 1);
        query += "),\n";
    }
    query.resize(query.size() - 2);
    query += ";";
    db->execute(query.c_str(), 0, 0);
}

void StorageImpl::_loadQuestRewardGoods()
{
    auto callback = [](void *o, int ncols, char **cols, char **names)
    {
        Ptr<QuestRewardGood> questRewardGood = std::make_shared<QuestRewardGood>();
        if (cols[0]) questRewardGood->questReward.id = std::stoi(cols[0]);
        if (cols[1]) questRewardGood->good.id = std::stoi(cols[1]);
        if (cols[2]) questRewardGood->quantity = std::stoi(cols[2]);

        Storage *storage = (Storage *)o;
        storage->questRewardGoods.push_back(questRewardGood);
        return 0;
    };
    db->execute("select * from QuestRewardGoods;", this, callback);
}

void StorageImpl::_loadQuestRewardGoodsPtrs()
{
    for (auto &questRewardGood : questRewardGoods)
    {
        if (questRewards.find(questRewardGood->questReward.id) != questRewards.end())
            questRewardGood->questReward.ptr = questRewards[questRewardGood->questReward.id];
        if (goods.find(questRewardGood->good.id) != goods.end())
            questRewardGood->good.ptr = goods[questRewardGood->good.id];
    }
}

void StorageImpl::_loadQuestRewardGoodsArrays()
{
}

void StorageImpl::_saveQuestRewardGoods()
{
    std::string query;
    query += "delete from QuestRewardGoods;";
    db->execute(query.c_str(), 0, 0);
    query.clear();
    if (questRewardGoods.empty())
        return;
    query += "insert or replace into QuestRewardGoods values\n";
    for (auto &questRewardGood : questRewardGoods)
    {
        query += "(";
        query += "'" + std::to_string(questRewardGood->questReward.id) + "',";
        query += "'" + std::to_string(questRewardGood->good.id) + "',";
        query += "'" + std::to_string(questRewardGood->quantity) + "',";
        query.resize(query.size() - 1);
        query += "),\n";
    }
    query.resize(query.size() - 2);
    query += ";";
    db->execute(query.c_str(), 0, 0);
}

void StorageImpl::_loadQuestRewardModificators()
{
    auto callback = [](void *o, int ncols, char **cols, char **names)
    {
        Ptr<QuestRewardModificator> questRewardModificator = std::make_shared<QuestRewardModificator>();
        if (cols[0]) questRewardModificator->questReward.id = std::stoi(cols[0]);
        if (cols[1]) questRewardModificator->modificator.id = std::stoi(cols[1]);
        if (cols[2]) questRewardModificator->quantity = std::stoi(cols[2]);

        Storage *storage = (Storage *)o;
        storage->questRewardModificators.push_back(questRewardModificator);
        return 0;
    };
    db->execute("select * from QuestRewardModificators;", this, callback);
}

void StorageImpl::_loadQuestRewardModificatorsPtrs()
{
    for (auto &questRewardModificator : questRewardModificators)
    {
        if (questRewards.find(questRewardModificator->questReward.id) != questRewards.end())
            questRewardModificator->questReward.ptr = questRewards[questRewardModificator->questReward.id];
        if (modificators.find(questRewardModificator->modificator.id) != modificators.end())
            questRewardModificator->modificator.ptr = modificators[questRewardModificator->modificator.id];
    }
}

void StorageImpl::_loadQuestRewardModificatorsArrays()
{
}

void StorageImpl::_saveQuestRewardModificators()
{
    std::string query;
    query += "delete from QuestRewardModificators;";
    db->execute(query.c_str(), 0, 0);
    query.clear();
    if (questRewardModificators.empty())
        return;
    query += "insert or replace into QuestRewardModificators values\n";
    for (auto &questRewardModificator : questRewardModificators)
    {
        query += "(";
        query += "'" + std::to_string(questRewardModificator->questReward.id) + "',";
        query += "'" + std::to_string(questRewardModificator->modificator.id) + "',";
        query += "'" + std::to_string(questRewardModificator->quantity) + "',";
        query.resize(query.size() - 1);
        query += "),\n";
    }
    query.resize(query.size() - 2);
    query += ";";
    db->execute(query.c_str(), 0, 0);
}

void StorageImpl::_loadQuestRewardProjectiles()
{
    auto callback = [](void *o, int ncols, char **cols, char **names)
    {
        Ptr<QuestRewardProjectile> questRewardProjectile = std::make_shared<QuestRewardProjectile>();
        if (cols[0]) questRewardProjectile->questReward.id = std::stoi(cols[0]);
        if (cols[1]) questRewardProjectile->projectile.id = std::stoi(cols[1]);
        if (cols[2]) questRewardProjectile->quantity = std::stoi(cols[2]);

        Storage *storage = (Storage *)o;
        storage->questRewardProjectiles.push_back(questRewardProjectile);
        return 0;
    };
    db->execute("select * from QuestRewardProjectiles;", this, callback);
}

void StorageImpl::_loadQuestRewardProjectilesPtrs()
{
    for (auto &questRewardProjectile : questRewardProjectiles)
    {
        if (questRewards.find(questRewardProjectile->questReward.id) != questRewards.end())
            questRewardProjectile->questReward.ptr = questRewards[questRewardProjectile->questReward.id];
        if (projectiles.find(questRewardProjectile->projectile.id) != projectiles.end())
            questRewardProjectile->projectile.ptr = projectiles[questRewardProjectile->projectile.id];
    }
}

void StorageImpl::_loadQuestRewardProjectilesArrays()
{
}

void StorageImpl::_saveQuestRewardProjectiles()
{
    std::string query;
    query += "delete from QuestRewardProjectiles;";
    db->execute(query.c_str(), 0, 0);
    query.clear();
    if (questRewardProjectiles.empty())
        return;
    query += "insert or replace into QuestRewardProjectiles values\n";
    for (auto &questRewardProjectile : questRewardProjectiles)
    {
        query += "(";
        query += "'" + std::to_string(questRewardProjectile->questReward.id) + "',";
        query += "'" + std::to_string(questRewardProjectile->projectile.id) + "',";
        query += "'" + std::to_string(questRewardProjectile->quantity) + "',";
        query.resize(query.size() - 1);
        query += "),\n";
    }
    query.resize(query.size() - 2);
    query += ";";
    db->execute(query.c_str(), 0, 0);
}

void StorageImpl::_loadQuestRewardReputations()
{
    auto callback = [](void *o, int ncols, char **cols, char **names)
    {
        Ptr<QuestRewardReputation> questRewardReputation = std::make_shared<QuestRewardReputation>();
        if (cols[0]) questRewardReputation->questReward.id = std::stoi(cols[0]);
        if (cols[1]) questRewardReputation->clan.id = std::stoi(cols[1]);
        if (cols[2]) questRewardReputation->reputation = std::stof(cols[2]);

        Storage *storage = (Storage *)o;
        storage->questRewardReputations.push_back(questRewardReputation);
        return 0;
    };
    db->execute("select * from QuestRewardReputations;", this, callback);
}

void StorageImpl::_loadQuestRewardReputationsPtrs()
{
    for (auto &questRewardReputation : questRewardReputations)
    {
        if (questRewards.find(questRewardReputation->questReward.id) != questRewards.end())
            questRewardReputation->questReward.ptr = questRewards[questRewardReputation->questReward.id];
        if (clans.find(questRewardReputation->clan.id) != clans.end())
            questRewardReputation->clan.ptr = clans[questRewardReputation->clan.id];
    }
}

void StorageImpl::_loadQuestRewardReputationsArrays()
{
}

void StorageImpl::_saveQuestRewardReputations()
{
    std::string query;
    query += "delete from QuestRewardReputations;";
    db->execute(query.c_str(), 0, 0);
    query.clear();
    if (questRewardReputations.empty())
        return;
    query += "insert or replace into QuestRewardReputations values\n";
    for (auto &questRewardReputation : questRewardReputations)
    {
        query += "(";
        query += "'" + std::to_string(questRewardReputation->questReward.id) + "',";
        query += "'" + std::to_string(questRewardReputation->clan.id) + "',";
        query += "'" + std::to_string(questRewardReputation->reputation) + "',";
        query.resize(query.size() - 1);
        query += "),\n";
    }
    query.resize(query.size() - 2);
    query += ";";
    db->execute(query.c_str(), 0, 0);
}

void StorageImpl::_loadQuestRewardWeapons()
{
    auto callback = [](void *o, int ncols, char **cols, char **names)
    {
        Ptr<QuestRewardWeapon> questRewardWeapon = std::make_shared<QuestRewardWeapon>();
        if (cols[0]) questRewardWeapon->questReward.id = std::stoi(cols[0]);
        if (cols[1]) questRewardWeapon->weapon.id = std::stoi(cols[1]);
        if (cols[2]) questRewardWeapon->quantity = std::stoi(cols[2]);

        Storage *storage = (Storage *)o;
        storage->questRewardWeapons.push_back(questRewardWeapon);
        return 0;
    };
    db->execute("select * from QuestRewardWeapons;", this, callback);
}

void StorageImpl::_loadQuestRewardWeaponsPtrs()
{
    for (auto &questRewardWeapon : questRewardWeapons)
    {
        if (questRewards.find(questRewardWeapon->questReward.id) != questRewards.end())
            questRewardWeapon->questReward.ptr = questRewards[questRewardWeapon->questReward.id];
        if (weapons.find(questRewardWeapon->weapon.id) != weapons.end())
            questRewardWeapon->weapon.ptr = weapons[questRewardWeapon->weapon.id];
    }
}

void StorageImpl::_loadQuestRewardWeaponsArrays()
{
}

void StorageImpl::_saveQuestRewardWeapons()
{
    std::string query;
    query += "delete from QuestRewardWeapons;";
    db->execute(query.c_str(), 0, 0);
    query.clear();
    if (questRewardWeapons.empty())
        return;
    query += "insert or replace into QuestRewardWeapons values\n";
    for (auto &questRewardWeapon : questRewardWeapons)
    {
        query += "(";
        query += "'" + std::to_string(questRewardWeapon->questReward.id) + "',";
        query += "'" + std::to_string(questRewardWeapon->weapon.id) + "',";
        query += "'" + std::to_string(questRewardWeapon->quantity) + "',";
        query.resize(query.size() - 1);
        query += "),\n";
    }
    query.resize(query.size() - 2);
    query += ";";
    db->execute(query.c_str(), 0, 0);
}

void StorageImpl::_loadQuestRewards()
{
    auto callback = [](void *o, int ncols, char **cols, char **names)
    {
        Ptr<QuestReward> questReward = std::make_shared<QuestReward>();
        if (cols[0]) questReward->id = std::stoi(cols[0]);
        if (cols[1]) questReward->quest.id = std::stoi(cols[1]);
        if (cols[2]) questReward->text_id = cols[2];
        if (cols[3]) questReward->money = std::stoi(cols[3]);
        if (cols[4]) questReward->rating = std::stof(cols[4]);

        Storage *storage = (Storage *)o;
        storage->questRewards[questReward->id] = questReward;
        return 0;
    };
    db->execute("select * from QuestRewards;", this, callback);
}

void StorageImpl::_loadQuestRewardsPtrs()
{
    for (auto &questReward : questRewards)
    {
        if (quests.find(questReward.second->quest.id) != quests.end())
            questReward.second->quest.ptr = quests[questReward.second->quest.id];
    }
}

void StorageImpl::_loadQuestRewardsArrays()
{
    for (auto &questReward : questRewards)
    {
        for (auto &questRewardEquipment : questRewardEquipments)
            if (questReward.first == questRewardEquipment->questReward.id)
                questReward.second->equipments.push_back(questRewardEquipment);
        for (auto &questRewardGlider : questRewardGliders)
            if (questReward.first == questRewardGlider->questReward.id)
                questReward.second->gliders.push_back(questRewardGlider);
        for (auto &questRewardGood : questRewardGoods)
            if (questReward.first == questRewardGood->questReward.id)
                questReward.second->goods.push_back(questRewardGood);
        for (auto &questRewardModificator : questRewardModificators)
            if (questReward.first == questRewardModificator->questReward.id)
                questReward.second->modificators.push_back(questRewardModificator);
        for (auto &questRewardProjectile : questRewardProjectiles)
            if (questReward.first == questRewardProjectile->questReward.id)
                questReward.second->projectiles.push_back(questRewardProjectile);
        for (auto &questRewardReputation : questRewardReputations)
            if (questReward.first == questRewardReputation->questReward.id)
                questReward.second->reputations.push_back(questRewardReputation);
        for (auto &questRewardWeapon : questRewardWeapons)
            if (questReward.first == questRewardWeapon->questReward.id)
                questReward.second->weapons.push_back(questRewardWeapon);
    }
}

void StorageImpl::_saveQuestRewards()
{
    std::string query;
    query += "delete from QuestRewards;";
    db->execute(query.c_str(), 0, 0);
    query.clear();
    if (questRewards.empty())
        return;
    query += "insert or replace into QuestRewards values\n";
    for (auto &questReward : questRewards)
    {
        query += "(";
        query += "'" + std::to_string(questReward.second->id) + "',";
        query += "'" + std::to_string(questReward.second->quest.id) + "',";
        query += "'" + questReward.second->text_id.string() + "',";
        query += "'" + std::to_string(questReward.second->money) + "',";
        query += "'" + std::to_string(questReward.second->rating) + "',";
        query.resize(query.size() - 1);
        query += "),\n";
    }
    query.resize(query.size() - 2);
    query += ";";
    db->execute(query.c_str(), 0, 0);
}

void StorageImpl::_loadQuests()
{
    auto callback = [](void *o, int ncols, char **cols, char **names)
    {
        Ptr<Quest> quest = std::make_shared<Quest>();
        if (cols[0]) quest->id = std::stoi(cols[0]);
        if (cols[1]) quest->text_id = cols[1];
        if (cols[2]) quest->name.id = std::stoi(cols[2]);
        if (cols[3]) quest->title.id = std::stoi(cols[3]);
        if (cols[4]) quest->description.id = std::stoi(cols[4]);
        if (cols[5]) quest->time = std::stoi(cols[5]);

        Storage *storage = (Storage *)o;
        storage->quests[quest->id] = quest;
        return 0;
    };
    db->execute("select * from Quests;", this, callback);
}

void StorageImpl::_loadQuestsPtrs()
{
    for (auto &quest : quests)
    {
        if (strings.find(quest.second->name.id) != strings.end())
            quest.second->name.ptr = strings[quest.second->name.id];
        if (strings.find(quest.second->title.id) != strings.end())
            quest.second->title.ptr = strings[quest.second->title.id];
        if (strings.find(quest.second->description.id) != strings.end())
            quest.second->description.ptr = strings[quest.second->description.id];
    }
}

void StorageImpl::_loadQuestsArrays()
{
    for (auto &quest : quests)
    {
        for (auto &questReward : questRewards)
            if (quest.first == questReward.second->quest.id)
                quest.second->rewards.push_back(questReward.second);
    }
}

void StorageImpl::_saveQuests()
{
    std::string query;
    query += "delete from Quests;";
    db->execute(query.c_str(), 0, 0);
    query.clear();
    if (quests.empty())
        return;
    query += "insert or replace into Quests values\n";
    for (auto &quest : quests)
    {
        query += "(";
        query += "'" + std::to_string(quest.second->id) + "',";
        query += "'" + quest.second->text_id.string() + "',";
        query += "'" + std::to_string(quest.second->name.id) + "',";
        query += "'" + std::to_string(quest.second->title.id) + "',";
        query += "'" + std::to_string(quest.second->description.id) + "',";
        query += "'" + std::to_string(quest.second->time) + "',";
        query.resize(query.size() - 1);
        query += "),\n";
    }
    query.resize(query.size() - 2);
    query += ";";
    db->execute(query.c_str(), 0, 0);
}

void StorageImpl::_loadSaveObjects()
{
    auto callback = [](void *o, int ncols, char **cols, char **names)
    {
        Ptr<SaveObject> saveObject = std::make_shared<SaveObject>();
        if (cols[0]) saveObject->save.id = std::stoi(cols[0]);
        if (cols[1]) saveObject->object.id = std::stoi(cols[1]);
        if (cols[2]) saveObject->map.id = std::stoi(cols[2]);
        if (cols[3]) saveObject->x = std::stof(cols[3]);
        if (cols[4]) saveObject->y = std::stof(cols[4]);
        if (cols[5]) saveObject->z = std::stof(cols[5]);
        if (cols[6]) saveObject->pitch = std::stof(cols[6]);
        if (cols[7]) saveObject->yaw = std::stof(cols[7]);
        if (cols[8]) saveObject->roll = std::stof(cols[8]);

        Storage *storage = (Storage *)o;
        storage->saveObjects.push_back(saveObject);
        return 0;
    };
    db->execute("select * from SaveObjects;", this, callback);
}

void StorageImpl::_loadSaveObjectsPtrs()
{
    for (auto &saveObject : saveObjects)
    {
        if (saves.find(saveObject->save.id) != saves.end())
            saveObject->save.ptr = saves[saveObject->save.id];
        if (objects.find(saveObject->object.id) != objects.end())
            saveObject->object.ptr = objects[saveObject->object.id];
        if (maps.find(saveObject->map.id) != maps.end())
            saveObject->map.ptr = maps[saveObject->map.id];
    }
}

void StorageImpl::_loadSaveObjectsArrays()
{
}

void StorageImpl::_saveSaveObjects()
{
    std::string query;
    query += "delete from SaveObjects;";
    db->execute(query.c_str(), 0, 0);
    query.clear();
    if (saveObjects.empty())
        return;
    query += "insert or replace into SaveObjects values\n";
    for (auto &saveObject : saveObjects)
    {
        query += "(";
        query += "'" + std::to_string(saveObject->save.id) + "',";
        query += "'" + std::to_string(saveObject->object.id) + "',";
        query += "'" + std::to_string(saveObject->map.id) + "',";
        query += "'" + std::to_string(saveObject->x) + "',";
        query += "'" + std::to_string(saveObject->y) + "',";
        query += "'" + std::to_string(saveObject->z) + "',";
        query += "'" + std::to_string(saveObject->pitch) + "',";
        query += "'" + std::to_string(saveObject->yaw) + "',";
        query += "'" + std::to_string(saveObject->roll) + "',";
        query.resize(query.size() - 1);
        query += "),\n";
    }
    query.resize(query.size() - 2);
    query += ";";
    db->execute(query.c_str(), 0, 0);
}

void StorageImpl::_loadSavePlayers()
{
    auto callback = [](void *o, int ncols, char **cols, char **names)
    {
        Ptr<SavePlayer> savePlayer = std::make_shared<SavePlayer>();
        if (cols[0]) savePlayer->save.id = std::stoi(cols[0]);
        if (cols[1]) savePlayer->player.id = std::stoi(cols[1]);

        Storage *storage = (Storage *)o;
        storage->savePlayers.push_back(savePlayer);
        return 0;
    };
    db->execute("select * from SavePlayers;", this, callback);
}

void StorageImpl::_loadSavePlayersPtrs()
{
    for (auto &savePlayer : savePlayers)
    {
        if (saves.find(savePlayer->save.id) != saves.end())
            savePlayer->save.ptr = saves[savePlayer->save.id];
        if (players.find(savePlayer->player.id) != players.end())
            savePlayer->player.ptr = players[savePlayer->player.id];
    }
}

void StorageImpl::_loadSavePlayersArrays()
{
}

void StorageImpl::_saveSavePlayers()
{
    std::string query;
    query += "delete from SavePlayers;";
    db->execute(query.c_str(), 0, 0);
    query.clear();
    if (savePlayers.empty())
        return;
    query += "insert or replace into SavePlayers values\n";
    for (auto &savePlayer : savePlayers)
    {
        query += "(";
        query += "'" + std::to_string(savePlayer->save.id) + "',";
        query += "'" + std::to_string(savePlayer->player.id) + "',";
        query.resize(query.size() - 1);
        query += "),\n";
    }
    query.resize(query.size() - 2);
    query += ";";
    db->execute(query.c_str(), 0, 0);
}

void StorageImpl::_loadSaveQuests()
{
    auto callback = [](void *o, int ncols, char **cols, char **names)
    {
        Ptr<SaveQuest> saveQuest = std::make_shared<SaveQuest>();
        if (cols[0]) saveQuest->save.id = std::stoi(cols[0]);
        if (cols[1]) saveQuest->quest.id = std::stoi(cols[1]);
        if (cols[2]) saveQuest->state = std::stoi(cols[2]);

        Storage *storage = (Storage *)o;
        storage->saveQuests.push_back(saveQuest);
        return 0;
    };
    db->execute("select * from SaveQuests;", this, callback);
}

void StorageImpl::_loadSaveQuestsPtrs()
{
    for (auto &saveQuest : saveQuests)
    {
        if (saves.find(saveQuest->save.id) != saves.end())
            saveQuest->save.ptr = saves[saveQuest->save.id];
        if (quests.find(saveQuest->quest.id) != quests.end())
            saveQuest->quest.ptr = quests[saveQuest->quest.id];
    }
}

void StorageImpl::_loadSaveQuestsArrays()
{
}

void StorageImpl::_saveSaveQuests()
{
    std::string query;
    query += "delete from SaveQuests;";
    db->execute(query.c_str(), 0, 0);
    query.clear();
    if (saveQuests.empty())
        return;
    query += "insert or replace into SaveQuests values\n";
    for (auto &saveQuest : saveQuests)
    {
        query += "(";
        query += "'" + std::to_string(saveQuest->save.id) + "',";
        query += "'" + std::to_string(saveQuest->quest.id) + "',";
        query += "'" + std::to_string(saveQuest->state) + "',";
        query.resize(query.size() - 1);
        query += "),\n";
    }
    query.resize(query.size() - 2);
    query += ";";
    db->execute(query.c_str(), 0, 0);
}

void StorageImpl::_loadSaves()
{
    auto callback = [](void *o, int ncols, char **cols, char **names)
    {
        Ptr<Save> save = std::make_shared<Save>();
        if (cols[0]) save->id = std::stoi(cols[0]);
        if (cols[1]) save->modification.id = std::stoi(cols[1]);
        if (cols[2]) save->name = cols[2];
        if (cols[3]) save->date = cols[3];

        Storage *storage = (Storage *)o;
        storage->saves[save->id] = save;
        return 0;
    };
    db->execute("select * from Saves;", this, callback);
}

void StorageImpl::_loadSavesPtrs()
{
    for (auto &save : saves)
    {
        if (modifications.find(save.second->modification.id) != modifications.end())
            save.second->modification.ptr = modifications[save.second->modification.id];
    }
}

void StorageImpl::_loadSavesArrays()
{
    for (auto &save : saves)
    {
        for (auto &saveObject : saveObjects)
            if (save.first == saveObject->save.id)
                save.second->objects.push_back(saveObject);
        for (auto &savePlayer : savePlayers)
            if (save.first == savePlayer->save.id)
                save.second->players.push_back(savePlayer);
        for (auto &saveQuest : saveQuests)
            if (save.first == saveQuest->save.id)
                save.second->quests.push_back(saveQuest);
        for (auto &scriptVariable : scriptVariables)
            if (save.first == scriptVariable->save.id)
                save.second->ptVariables.push_back(scriptVariable);
    }
}

void StorageImpl::_saveSaves()
{
    std::string query;
    query += "delete from Saves;";
    db->execute(query.c_str(), 0, 0);
    query.clear();
    if (saves.empty())
        return;
    query += "insert or replace into Saves values\n";
    for (auto &save : saves)
    {
        query += "(";
        query += "'" + std::to_string(save.second->id) + "',";
        query += "'" + std::to_string(save.second->modification.id) + "',";
        query += "'" + save.second->name.string() + "',";
        query += "'" + save.second->date.string() + "',";
        query.resize(query.size() - 1);
        query += "),\n";
    }
    query.resize(query.size() - 2);
    query += ";";
    db->execute(query.c_str(), 0, 0);
}

void StorageImpl::_loadScriptVariables()
{
    auto callback = [](void *o, int ncols, char **cols, char **names)
    {
        Ptr<ScriptVariable> scriptVariable = std::make_shared<ScriptVariable>();
        if (cols[0]) scriptVariable->save.id = std::stoi(cols[0]);
        if (cols[1]) scriptVariable->variable = cols[1];
        if (cols[2]) scriptVariable->value = cols[2];

        Storage *storage = (Storage *)o;
        storage->scriptVariables.push_back(scriptVariable);
        return 0;
    };
    db->execute("select * from ScriptVariables;", this, callback);
}

void StorageImpl::_loadScriptVariablesPtrs()
{
    for (auto &scriptVariable : scriptVariables)
    {
        if (saves.find(scriptVariable->save.id) != saves.end())
            scriptVariable->save.ptr = saves[scriptVariable->save.id];
    }
}

void StorageImpl::_loadScriptVariablesArrays()
{
}

void StorageImpl::_saveScriptVariables()
{
    std::string query;
    query += "delete from ScriptVariables;";
    db->execute(query.c_str(), 0, 0);
    query.clear();
    if (scriptVariables.empty())
        return;
    query += "insert or replace into ScriptVariables values\n";
    for (auto &scriptVariable : scriptVariables)
    {
        query += "(";
        query += "'" + std::to_string(scriptVariable->save.id) + "',";
        query += "'" + scriptVariable->variable.string() + "',";
        query += "'" + scriptVariable->value.string() + "',";
        query.resize(query.size() - 1);
        query += "),\n";
    }
    query.resize(query.size() - 2);
    query += ";";
    db->execute(query.c_str(), 0, 0);
}

void StorageImpl::_loadStrings()
{
    auto callback = [](void *o, int ncols, char **cols, char **names)
    {
        Ptr<String> string = std::make_shared<String>();
        if (cols[0]) string->id = std::stoi(cols[0]);
        if (cols[1]) string->ru = cols[1];
        if (cols[2]) string->en = cols[2];

        Storage *storage = (Storage *)o;
        storage->strings[string->id] = string;
        return 0;
    };
    db->execute("select * from Strings;", this, callback);
}

void StorageImpl::_loadStringsPtrs()
{
}

void StorageImpl::_loadStringsArrays()
{
}

void StorageImpl::_saveStrings()
{
    std::string query;
    query += "delete from Strings;";
    db->execute(query.c_str(), 0, 0);
    query.clear();
    if (strings.empty())
        return;
    query += "insert or replace into Strings values\n";
    for (auto &string : strings)
    {
        query += "(";
        query += "'" + std::to_string(string.second->id) + "',";
        query += "'" + string.second->ru.string() + "',";
        query += "'" + string.second->en.string() + "',";
        query.resize(query.size() - 1);
        query += "),\n";
    }
    query.resize(query.size() - 2);
    query += ";";
    db->execute(query.c_str(), 0, 0);
}

void StorageImpl::_loadWeapons()
{
    auto callback = [](void *o, int ncols, char **cols, char **names)
    {
        Ptr<Weapon> weapon = std::make_shared<Weapon>();
        if (cols[0]) weapon->id = std::stoi(cols[0]);
        if (cols[1]) weapon->text_id = cols[1];
        if (cols[2]) weapon->resource = cols[2];
        if (cols[3]) weapon->name.id = std::stoi(cols[3]);
        if (cols[4]) weapon->type = std::stoi(cols[4]);
        if (cols[5]) weapon->standard = std::stoi(cols[5]);
        if (cols[6]) weapon->weight = std::stof(cols[6]);
        if (cols[7]) weapon->power = std::stof(cols[7]);
        if (cols[8]) weapon->firerate = std::stof(cols[8]);
        if (cols[9]) weapon->damage = std::stof(cols[9]);
        if (cols[10]) weapon->price = std::stof(cols[10]);
        if (cols[11]) weapon->projectile.id = std::stoi(cols[11]);

        Storage *storage = (Storage *)o;
        storage->weapons[weapon->id] = weapon;
        return 0;
    };
    db->execute("select * from Weapons;", this, callback);
}

void StorageImpl::_loadWeaponsPtrs()
{
    for (auto &weapon : weapons)
    {
        if (strings.find(weapon.second->name.id) != strings.end())
            weapon.second->name.ptr = strings[weapon.second->name.id];
        if (projectiles.find(weapon.second->projectile.id) != projectiles.end())
            weapon.second->projectile.ptr = projectiles[weapon.second->projectile.id];
    }
}

void StorageImpl::_loadWeaponsArrays()
{
}

void StorageImpl::_saveWeapons()
{
    std::string query;
    query += "delete from Weapons;";
    db->execute(query.c_str(), 0, 0);
    query.clear();
    if (weapons.empty())
        return;
    query += "insert or replace into Weapons values\n";
    for (auto &weapon : weapons)
    {
        query += "(";
        query += "'" + std::to_string(weapon.second->id) + "',";
        query += "'" + weapon.second->text_id.string() + "',";
        query += "'" + weapon.second->resource.string() + "',";
        query += "'" + std::to_string(weapon.second->name.id) + "',";
        query += "'" + std::to_string(weapon.second->type) + "',";
        query += "'" + std::to_string(weapon.second->standard) + "',";
        query += "'" + std::to_string(weapon.second->weight) + "',";
        query += "'" + std::to_string(weapon.second->power) + "',";
        query += "'" + std::to_string(weapon.second->firerate) + "',";
        query += "'" + std::to_string(weapon.second->damage) + "',";
        query += "'" + std::to_string(weapon.second->price) + "',";
        query += "'" + std::to_string(weapon.second->projectile.id) + "',";
        query.resize(query.size() - 1);
        query += "),\n";
    }
    query.resize(query.size() - 2);
    query += ";";
    db->execute(query.c_str(), 0, 0);
}

void StorageImpl::clear()
{
    buildings.clear();
    clanReputations.clear();
    clans.clear();
    configurationEquipments.clear();
    configurationGoods.clear();
    configurationProjectiles.clear();
    configurationWeapons.clear();
    configurations.clear();
    coordinates.clear();
    equipments.clear();
    gliders.clear();
    goods.clear();
    mapBuildingEquipments.clear();
    mapBuildingGliders.clear();
    mapBuildingGoods.clear();
    mapBuildingModificators.clear();
    mapBuildingProjectiles.clear();
    mapBuildingWeapons.clear();
    mapBuildings.clear();
    mapObjects.clear();
    maps.clear();
    mechanoidGroups.clear();
    mechanoids.clear();
    modificationClans.clear();
    modificationMaps.clear();
    modificationMechanoids.clear();
    modifications.clear();
    modificators.clear();
    objects.clear();
    players.clear();
    projectiles.clear();
    questRewardEquipments.clear();
    questRewardGliders.clear();
    questRewardGoods.clear();
    questRewardModificators.clear();
    questRewardProjectiles.clear();
    questRewardReputations.clear();
    questRewardWeapons.clear();
    questRewards.clear();
    quests.clear();
    saveObjects.clear();
    savePlayers.clear();
    saveQuests.clear();
    saves.clear();
    scriptVariables.clear();
    strings.clear();
    weapons.clear();
}

void StorageImpl::load(ProgressCallback callback)
{
    _loadBuildings();
    PROGRESS_CALLBACK(0.709220);
    _loadClanReputations();
    PROGRESS_CALLBACK(1.418440);
    _loadClans();
    PROGRESS_CALLBACK(2.127660);
    _loadConfigurationEquipments();
    PROGRESS_CALLBACK(2.836879);
    _loadConfigurationGoods();
    PROGRESS_CALLBACK(3.546099);
    _loadConfigurationProjectiles();
    PROGRESS_CALLBACK(4.255319);
    _loadConfigurationWeapons();
    PROGRESS_CALLBACK(4.964539);
    _loadConfigurations();
    PROGRESS_CALLBACK(5.673759);
    _loadCoordinates();
    PROGRESS_CALLBACK(6.382979);
    _loadEquipments();
    PROGRESS_CALLBACK(7.092199);
    _loadGliders();
    PROGRESS_CALLBACK(7.801418);
    _loadGoods();
    PROGRESS_CALLBACK(8.510638);
    _loadMapBuildingEquipments();
    PROGRESS_CALLBACK(9.219858);
    _loadMapBuildingGliders();
    PROGRESS_CALLBACK(9.929078);
    _loadMapBuildingGoods();
    PROGRESS_CALLBACK(10.638298);
    _loadMapBuildingModificators();
    PROGRESS_CALLBACK(11.347518);
    _loadMapBuildingProjectiles();
    PROGRESS_CALLBACK(12.056738);
    _loadMapBuildingWeapons();
    PROGRESS_CALLBACK(12.765957);
    _loadMapBuildings();
    PROGRESS_CALLBACK(13.475177);
    _loadMapObjects();
    PROGRESS_CALLBACK(14.184397);
    _loadMaps();
    PROGRESS_CALLBACK(14.893617);
    _loadMechanoidGroups();
    PROGRESS_CALLBACK(15.602837);
    _loadMechanoids();
    PROGRESS_CALLBACK(16.312057);
    _loadModificationClans();
    PROGRESS_CALLBACK(17.021277);
    _loadModificationMaps();
    PROGRESS_CALLBACK(17.730496);
    _loadModificationMechanoids();
    PROGRESS_CALLBACK(18.439716);
    _loadModifications();
    PROGRESS_CALLBACK(19.148936);
    _loadModificators();
    PROGRESS_CALLBACK(19.858156);
    _loadObjects();
    PROGRESS_CALLBACK(20.567376);
    _loadPlayers();
    PROGRESS_CALLBACK(21.276596);
    _loadProjectiles();
    PROGRESS_CALLBACK(21.985816);
    _loadQuestRewardEquipments();
    PROGRESS_CALLBACK(22.695035);
    _loadQuestRewardGliders();
    PROGRESS_CALLBACK(23.404255);
    _loadQuestRewardGoods();
    PROGRESS_CALLBACK(24.113475);
    _loadQuestRewardModificators();
    PROGRESS_CALLBACK(24.822695);
    _loadQuestRewardProjectiles();
    PROGRESS_CALLBACK(25.531915);
    _loadQuestRewardReputations();
    PROGRESS_CALLBACK(26.241135);
    _loadQuestRewardWeapons();
    PROGRESS_CALLBACK(26.950355);
    _loadQuestRewards();
    PROGRESS_CALLBACK(27.659574);
    _loadQuests();
    PROGRESS_CALLBACK(28.368794);
    _loadSaveObjects();
    PROGRESS_CALLBACK(29.078014);
    _loadSavePlayers();
    PROGRESS_CALLBACK(29.787234);
    _loadSaveQuests();
    PROGRESS_CALLBACK(30.496454);
    _loadSaves();
    PROGRESS_CALLBACK(31.205674);
    _loadScriptVariables();
    PROGRESS_CALLBACK(31.914894);
    _loadStrings();
    PROGRESS_CALLBACK(32.624113);
    _loadWeapons();
    PROGRESS_CALLBACK(33.333333);

    _loadBuildingsPtrs();
    PROGRESS_CALLBACK(34.042553);
    _loadClanReputationsPtrs();
    PROGRESS_CALLBACK(34.751773);
    _loadClansPtrs();
    PROGRESS_CALLBACK(35.460993);
    _loadConfigurationEquipmentsPtrs();
    PROGRESS_CALLBACK(36.170213);
    _loadConfigurationGoodsPtrs();
    PROGRESS_CALLBACK(36.879433);
    _loadConfigurationProjectilesPtrs();
    PROGRESS_CALLBACK(37.588652);
    _loadConfigurationWeaponsPtrs();
    PROGRESS_CALLBACK(38.297872);
    _loadConfigurationsPtrs();
    PROGRESS_CALLBACK(39.007092);
    _loadCoordinatesPtrs();
    PROGRESS_CALLBACK(39.716312);
    _loadEquipmentsPtrs();
    PROGRESS_CALLBACK(40.425532);
    _loadGlidersPtrs();
    PROGRESS_CALLBACK(41.134752);
    _loadGoodsPtrs();
    PROGRESS_CALLBACK(41.843972);
    _loadMapBuildingEquipmentsPtrs();
    PROGRESS_CALLBACK(42.553191);
    _loadMapBuildingGlidersPtrs();
    PROGRESS_CALLBACK(43.262411);
    _loadMapBuildingGoodsPtrs();
    PROGRESS_CALLBACK(43.971631);
    _loadMapBuildingModificatorsPtrs();
    PROGRESS_CALLBACK(44.680851);
    _loadMapBuildingProjectilesPtrs();
    PROGRESS_CALLBACK(45.390071);
    _loadMapBuildingWeaponsPtrs();
    PROGRESS_CALLBACK(46.099291);
    _loadMapBuildingsPtrs();
    PROGRESS_CALLBACK(46.808511);
    _loadMapObjectsPtrs();
    PROGRESS_CALLBACK(47.517730);
    _loadMapsPtrs();
    PROGRESS_CALLBACK(48.226950);
    _loadMechanoidGroupsPtrs();
    PROGRESS_CALLBACK(48.936170);
    _loadMechanoidsPtrs();
    PROGRESS_CALLBACK(49.645390);
    _loadModificationClansPtrs();
    PROGRESS_CALLBACK(50.354610);
    _loadModificationMapsPtrs();
    PROGRESS_CALLBACK(51.063830);
    _loadModificationMechanoidsPtrs();
    PROGRESS_CALLBACK(51.773050);
    _loadModificationsPtrs();
    PROGRESS_CALLBACK(52.482270);
    _loadModificatorsPtrs();
    PROGRESS_CALLBACK(53.191489);
    _loadObjectsPtrs();
    PROGRESS_CALLBACK(53.900709);
    _loadPlayersPtrs();
    PROGRESS_CALLBACK(54.609929);
    _loadProjectilesPtrs();
    PROGRESS_CALLBACK(55.319149);
    _loadQuestRewardEquipmentsPtrs();
    PROGRESS_CALLBACK(56.028369);
    _loadQuestRewardGlidersPtrs();
    PROGRESS_CALLBACK(56.737589);
    _loadQuestRewardGoodsPtrs();
    PROGRESS_CALLBACK(57.446809);
    _loadQuestRewardModificatorsPtrs();
    PROGRESS_CALLBACK(58.156028);
    _loadQuestRewardProjectilesPtrs();
    PROGRESS_CALLBACK(58.865248);
    _loadQuestRewardReputationsPtrs();
    PROGRESS_CALLBACK(59.574468);
    _loadQuestRewardWeaponsPtrs();
    PROGRESS_CALLBACK(60.283688);
    _loadQuestRewardsPtrs();
    PROGRESS_CALLBACK(60.992908);
    _loadQuestsPtrs();
    PROGRESS_CALLBACK(61.702128);
    _loadSaveObjectsPtrs();
    PROGRESS_CALLBACK(62.411348);
    _loadSavePlayersPtrs();
    PROGRESS_CALLBACK(63.120567);
    _loadSaveQuestsPtrs();
    PROGRESS_CALLBACK(63.829787);
    _loadSavesPtrs();
    PROGRESS_CALLBACK(64.539007);
    _loadScriptVariablesPtrs();
    PROGRESS_CALLBACK(65.248227);
    _loadStringsPtrs();
    PROGRESS_CALLBACK(65.957447);
    _loadWeaponsPtrs();
    PROGRESS_CALLBACK(66.666667);

    _loadBuildingsArrays();
    PROGRESS_CALLBACK(67.375887);
    _loadClanReputationsArrays();
    PROGRESS_CALLBACK(68.085106);
    _loadClansArrays();
    PROGRESS_CALLBACK(68.794326);
    _loadConfigurationEquipmentsArrays();
    PROGRESS_CALLBACK(69.503546);
    _loadConfigurationGoodsArrays();
    PROGRESS_CALLBACK(70.212766);
    _loadConfigurationProjectilesArrays();
    PROGRESS_CALLBACK(70.921986);
    _loadConfigurationWeaponsArrays();
    PROGRESS_CALLBACK(71.631206);
    _loadConfigurationsArrays();
    PROGRESS_CALLBACK(72.340426);
    _loadCoordinatesArrays();
    PROGRESS_CALLBACK(73.049645);
    _loadEquipmentsArrays();
    PROGRESS_CALLBACK(73.758865);
    _loadGlidersArrays();
    PROGRESS_CALLBACK(74.468085);
    _loadGoodsArrays();
    PROGRESS_CALLBACK(75.177305);
    _loadMapBuildingEquipmentsArrays();
    PROGRESS_CALLBACK(75.886525);
    _loadMapBuildingGlidersArrays();
    PROGRESS_CALLBACK(76.595745);
    _loadMapBuildingGoodsArrays();
    PROGRESS_CALLBACK(77.304965);
    _loadMapBuildingModificatorsArrays();
    PROGRESS_CALLBACK(78.014184);
    _loadMapBuildingProjectilesArrays();
    PROGRESS_CALLBACK(78.723404);
    _loadMapBuildingWeaponsArrays();
    PROGRESS_CALLBACK(79.432624);
    _loadMapBuildingsArrays();
    PROGRESS_CALLBACK(80.141844);
    _loadMapObjectsArrays();
    PROGRESS_CALLBACK(80.851064);
    _loadMapsArrays();
    PROGRESS_CALLBACK(81.560284);
    _loadMechanoidGroupsArrays();
    PROGRESS_CALLBACK(82.269504);
    _loadMechanoidsArrays();
    PROGRESS_CALLBACK(82.978723);
    _loadModificationClansArrays();
    PROGRESS_CALLBACK(83.687943);
    _loadModificationMapsArrays();
    PROGRESS_CALLBACK(84.397163);
    _loadModificationMechanoidsArrays();
    PROGRESS_CALLBACK(85.106383);
    _loadModificationsArrays();
    PROGRESS_CALLBACK(85.815603);
    _loadModificatorsArrays();
    PROGRESS_CALLBACK(86.524823);
    _loadObjectsArrays();
    PROGRESS_CALLBACK(87.234043);
    _loadPlayersArrays();
    PROGRESS_CALLBACK(87.943262);
    _loadProjectilesArrays();
    PROGRESS_CALLBACK(88.652482);
    _loadQuestRewardEquipmentsArrays();
    PROGRESS_CALLBACK(89.361702);
    _loadQuestRewardGlidersArrays();
    PROGRESS_CALLBACK(90.070922);
    _loadQuestRewardGoodsArrays();
    PROGRESS_CALLBACK(90.780142);
    _loadQuestRewardModificatorsArrays();
    PROGRESS_CALLBACK(91.489362);
    _loadQuestRewardProjectilesArrays();
    PROGRESS_CALLBACK(92.198582);
    _loadQuestRewardReputationsArrays();
    PROGRESS_CALLBACK(92.907801);
    _loadQuestRewardWeaponsArrays();
    PROGRESS_CALLBACK(93.617021);
    _loadQuestRewardsArrays();
    PROGRESS_CALLBACK(94.326241);
    _loadQuestsArrays();
    PROGRESS_CALLBACK(95.035461);
    _loadSaveObjectsArrays();
    PROGRESS_CALLBACK(95.744681);
    _loadSavePlayersArrays();
    PROGRESS_CALLBACK(96.453901);
    _loadSaveQuestsArrays();
    PROGRESS_CALLBACK(97.163121);
    _loadSavesArrays();
    PROGRESS_CALLBACK(97.872340);
    _loadScriptVariablesArrays();
    PROGRESS_CALLBACK(98.581560);
    _loadStringsArrays();
    PROGRESS_CALLBACK(99.290780);
    _loadWeaponsArrays();
    PROGRESS_CALLBACK(100.000000);
}

void StorageImpl::save(ProgressCallback callback)
{
    _saveStrings();
    PROGRESS_CALLBACK(2.127660);
    _saveQuestRewards();
    PROGRESS_CALLBACK(4.255319);
    _saveConfigurations();
    PROGRESS_CALLBACK(6.382979);
    _saveMaps();
    PROGRESS_CALLBACK(8.510638);
    _saveClans();
    PROGRESS_CALLBACK(10.638298);
    _saveModifications();
    PROGRESS_CALLBACK(12.765957);
    _saveProjectiles();
    PROGRESS_CALLBACK(14.893617);
    _saveSaves();
    PROGRESS_CALLBACK(17.021277);
    _saveEquipments();
    PROGRESS_CALLBACK(19.148936);
    _saveGliders();
    PROGRESS_CALLBACK(21.276596);
    _saveGoods();
    PROGRESS_CALLBACK(23.404255);
    _saveMechanoids();
    PROGRESS_CALLBACK(25.531915);
    _saveWeapons();
    PROGRESS_CALLBACK(27.659574);
    _saveModificators();
    PROGRESS_CALLBACK(29.787234);
    _saveObjects();
    PROGRESS_CALLBACK(31.914894);
    _saveQuests();
    PROGRESS_CALLBACK(34.042553);
    _saveBuildings();
    PROGRESS_CALLBACK(36.170213);
    _saveMapBuildings();
    PROGRESS_CALLBACK(38.297872);
    _saveMechanoidGroups();
    PROGRESS_CALLBACK(40.425532);
    _savePlayers();
    PROGRESS_CALLBACK(42.553191);
    _saveClanReputations();
    PROGRESS_CALLBACK(44.680851);
    _saveConfigurationEquipments();
    PROGRESS_CALLBACK(46.808511);
    _saveConfigurationGoods();
    PROGRESS_CALLBACK(48.936170);
    _saveConfigurationProjectiles();
    PROGRESS_CALLBACK(51.063830);
    _saveConfigurationWeapons();
    PROGRESS_CALLBACK(53.191489);
    _saveCoordinates();
    PROGRESS_CALLBACK(55.319149);
    _saveMapBuildingEquipments();
    PROGRESS_CALLBACK(57.446809);
    _saveMapBuildingGliders();
    PROGRESS_CALLBACK(59.574468);
    _saveMapBuildingGoods();
    PROGRESS_CALLBACK(61.702128);
    _saveMapBuildingModificators();
    PROGRESS_CALLBACK(63.829787);
    _saveMapBuildingProjectiles();
    PROGRESS_CALLBACK(65.957447);
    _saveMapBuildingWeapons();
    PROGRESS_CALLBACK(68.085106);
    _saveMapObjects();
    PROGRESS_CALLBACK(70.212766);
    _saveModificationClans();
    PROGRESS_CALLBACK(72.340426);
    _saveModificationMaps();
    PROGRESS_CALLBACK(74.468085);
    _saveModificationMechanoids();
    PROGRESS_CALLBACK(76.595745);
    _saveQuestRewardEquipments();
    PROGRESS_CALLBACK(78.723404);
    _saveQuestRewardGliders();
    PROGRESS_CALLBACK(80.851064);
    _saveQuestRewardGoods();
    PROGRESS_CALLBACK(82.978723);
    _saveQuestRewardModificators();
    PROGRESS_CALLBACK(85.106383);
    _saveQuestRewardProjectiles();
    PROGRESS_CALLBACK(87.234043);
    _saveQuestRewardReputations();
    PROGRESS_CALLBACK(89.361702);
    _saveQuestRewardWeapons();
    PROGRESS_CALLBACK(91.489362);
    _saveSaveObjects();
    PROGRESS_CALLBACK(93.617021);
    _saveSavePlayers();
    PROGRESS_CALLBACK(95.744681);
    _saveSaveQuests();
    PROGRESS_CALLBACK(97.872340);
    _saveScriptVariables();
    PROGRESS_CALLBACK(100.000000);
}

Ptr<Building> StorageImpl::addBuilding(IObject *parent)
{
    int id = 1;
    if (!buildings.empty())
        id = buildings.rbegin()->first + 1;
    auto v = std::make_shared<Building>();
    v->id = id;
    buildings[v->id] = v;
    return v;
}

void StorageImpl::deleteBuilding(Building *v)
{
    buildings.erase(v->id);
}

Ptr<ClanReputation> StorageImpl::addClanReputation(IObject *parent)
{
    auto v = std::make_shared<ClanReputation>();
    Clan *clan = (Clan *)parent;
    clan->reputations.push_back(v);
    clanReputations.push_back(v);
    v->clan = clans[clan->id];
    return v;
}

void StorageImpl::deleteClanReputation(ClanReputation *v)
{
    while (1)
    {
        auto i = find_if(clanReputations.begin(), clanReputations.end(), [v](const Ptr<ClanReputation> &p){ return p.get() == v; });
        if (i == clanReputations.end())
            break;
        clanReputations.erase(i);
    }
}

Ptr<Clan> StorageImpl::addClan(IObject *parent)
{
    int id = 1;
    if (!clans.empty())
        id = clans.rbegin()->first + 1;
    auto v = std::make_shared<Clan>();
    v->id = id;
    clans[v->id] = v;
    return v;
}

void StorageImpl::deleteClan(Clan *v)
{
    clans.erase(v->id);
}

Ptr<ConfigurationEquipment> StorageImpl::addConfigurationEquipment(IObject *parent)
{
    auto v = std::make_shared<ConfigurationEquipment>();
    Configuration *configuration = (Configuration *)parent;
    configuration->equipments.push_back(v);
    configurationEquipments.push_back(v);
    v->configuration = configurations[configuration->id];
    return v;
}

void StorageImpl::deleteConfigurationEquipment(ConfigurationEquipment *v)
{
    while (1)
    {
        auto i = find_if(configurationEquipments.begin(), configurationEquipments.end(), [v](const Ptr<ConfigurationEquipment> &p){ return p.get() == v; });
        if (i == configurationEquipments.end())
            break;
        configurationEquipments.erase(i);
    }
}

Ptr<ConfigurationGood> StorageImpl::addConfigurationGood(IObject *parent)
{
    auto v = std::make_shared<ConfigurationGood>();
    Configuration *configuration = (Configuration *)parent;
    configuration->goods.push_back(v);
    configurationGoods.push_back(v);
    v->configuration = configurations[configuration->id];
    return v;
}

void StorageImpl::deleteConfigurationGood(ConfigurationGood *v)
{
    while (1)
    {
        auto i = find_if(configurationGoods.begin(), configurationGoods.end(), [v](const Ptr<ConfigurationGood> &p){ return p.get() == v; });
        if (i == configurationGoods.end())
            break;
        configurationGoods.erase(i);
    }
}

Ptr<ConfigurationProjectile> StorageImpl::addConfigurationProjectile(IObject *parent)
{
    auto v = std::make_shared<ConfigurationProjectile>();
    Configuration *configuration = (Configuration *)parent;
    configuration->projectiles.push_back(v);
    configurationProjectiles.push_back(v);
    v->configuration = configurations[configuration->id];
    return v;
}

void StorageImpl::deleteConfigurationProjectile(ConfigurationProjectile *v)
{
    while (1)
    {
        auto i = find_if(configurationProjectiles.begin(), configurationProjectiles.end(), [v](const Ptr<ConfigurationProjectile> &p){ return p.get() == v; });
        if (i == configurationProjectiles.end())
            break;
        configurationProjectiles.erase(i);
    }
}

Ptr<ConfigurationWeapon> StorageImpl::addConfigurationWeapon(IObject *parent)
{
    auto v = std::make_shared<ConfigurationWeapon>();
    Configuration *configuration = (Configuration *)parent;
    configuration->weapons.push_back(v);
    configurationWeapons.push_back(v);
    v->configuration = configurations[configuration->id];
    return v;
}

void StorageImpl::deleteConfigurationWeapon(ConfigurationWeapon *v)
{
    while (1)
    {
        auto i = find_if(configurationWeapons.begin(), configurationWeapons.end(), [v](const Ptr<ConfigurationWeapon> &p){ return p.get() == v; });
        if (i == configurationWeapons.end())
            break;
        configurationWeapons.erase(i);
    }
}

Ptr<Configuration> StorageImpl::addConfiguration(IObject *parent)
{
    int id = 1;
    if (!configurations.empty())
        id = configurations.rbegin()->first + 1;
    auto v = std::make_shared<Configuration>();
    v->id = id;
    configurations[v->id] = v;
    return v;
}

void StorageImpl::deleteConfiguration(Configuration *v)
{
    configurations.erase(v->id);
}

Ptr<Coordinate> StorageImpl::addCoordinate(IObject *parent)
{
    int id = 1;
    if (!coordinates.empty())
        id = coordinates.rbegin()->first + 1;
    auto v = std::make_shared<Coordinate>();
    v->id = id;
    coordinates[v->id] = v;
    return v;
}

void StorageImpl::deleteCoordinate(Coordinate *v)
{
    coordinates.erase(v->id);
}

Ptr<Equipment> StorageImpl::addEquipment(IObject *parent)
{
    int id = 1;
    if (!equipments.empty())
        id = equipments.rbegin()->first + 1;
    auto v = std::make_shared<Equipment>();
    v->id = id;
    equipments[v->id] = v;
    return v;
}

void StorageImpl::deleteEquipment(Equipment *v)
{
    equipments.erase(v->id);
}

Ptr<Glider> StorageImpl::addGlider(IObject *parent)
{
    int id = 1;
    if (!gliders.empty())
        id = gliders.rbegin()->first + 1;
    auto v = std::make_shared<Glider>();
    v->id = id;
    gliders[v->id] = v;
    return v;
}

void StorageImpl::deleteGlider(Glider *v)
{
    gliders.erase(v->id);
}

Ptr<Good> StorageImpl::addGood(IObject *parent)
{
    int id = 1;
    if (!goods.empty())
        id = goods.rbegin()->first + 1;
    auto v = std::make_shared<Good>();
    v->id = id;
    goods[v->id] = v;
    return v;
}

void StorageImpl::deleteGood(Good *v)
{
    goods.erase(v->id);
}

Ptr<MapBuildingEquipment> StorageImpl::addMapBuildingEquipment(IObject *parent)
{
    auto v = std::make_shared<MapBuildingEquipment>();
    MapBuilding *mapBuilding = (MapBuilding *)parent;
    mapBuilding->equipments.push_back(v);
    mapBuildingEquipments.push_back(v);
    v->mapBuilding = mapBuildings[mapBuilding->id];
    return v;
}

void StorageImpl::deleteMapBuildingEquipment(MapBuildingEquipment *v)
{
    while (1)
    {
        auto i = find_if(mapBuildingEquipments.begin(), mapBuildingEquipments.end(), [v](const Ptr<MapBuildingEquipment> &p){ return p.get() == v; });
        if (i == mapBuildingEquipments.end())
            break;
        mapBuildingEquipments.erase(i);
    }
}

Ptr<MapBuildingGlider> StorageImpl::addMapBuildingGlider(IObject *parent)
{
    auto v = std::make_shared<MapBuildingGlider>();
    MapBuilding *mapBuilding = (MapBuilding *)parent;
    mapBuilding->gliders.push_back(v);
    mapBuildingGliders.push_back(v);
    v->mapBuilding = mapBuildings[mapBuilding->id];
    return v;
}

void StorageImpl::deleteMapBuildingGlider(MapBuildingGlider *v)
{
    while (1)
    {
        auto i = find_if(mapBuildingGliders.begin(), mapBuildingGliders.end(), [v](const Ptr<MapBuildingGlider> &p){ return p.get() == v; });
        if (i == mapBuildingGliders.end())
            break;
        mapBuildingGliders.erase(i);
    }
}

Ptr<MapBuildingGood> StorageImpl::addMapBuildingGood(IObject *parent)
{
    auto v = std::make_shared<MapBuildingGood>();
    MapBuilding *mapBuilding = (MapBuilding *)parent;
    mapBuilding->goods.push_back(v);
    mapBuildingGoods.push_back(v);
    v->mapBuilding = mapBuildings[mapBuilding->id];
    return v;
}

void StorageImpl::deleteMapBuildingGood(MapBuildingGood *v)
{
    while (1)
    {
        auto i = find_if(mapBuildingGoods.begin(), mapBuildingGoods.end(), [v](const Ptr<MapBuildingGood> &p){ return p.get() == v; });
        if (i == mapBuildingGoods.end())
            break;
        mapBuildingGoods.erase(i);
    }
}

Ptr<MapBuildingModificator> StorageImpl::addMapBuildingModificator(IObject *parent)
{
    auto v = std::make_shared<MapBuildingModificator>();
    MapBuilding *mapBuilding = (MapBuilding *)parent;
    mapBuilding->modificators.push_back(v);
    mapBuildingModificators.push_back(v);
    v->mapBuilding = mapBuildings[mapBuilding->id];
    return v;
}

void StorageImpl::deleteMapBuildingModificator(MapBuildingModificator *v)
{
    while (1)
    {
        auto i = find_if(mapBuildingModificators.begin(), mapBuildingModificators.end(), [v](const Ptr<MapBuildingModificator> &p){ return p.get() == v; });
        if (i == mapBuildingModificators.end())
            break;
        mapBuildingModificators.erase(i);
    }
}

Ptr<MapBuildingProjectile> StorageImpl::addMapBuildingProjectile(IObject *parent)
{
    auto v = std::make_shared<MapBuildingProjectile>();
    MapBuilding *mapBuilding = (MapBuilding *)parent;
    mapBuilding->projectiles.push_back(v);
    mapBuildingProjectiles.push_back(v);
    v->mapBuilding = mapBuildings[mapBuilding->id];
    return v;
}

void StorageImpl::deleteMapBuildingProjectile(MapBuildingProjectile *v)
{
    while (1)
    {
        auto i = find_if(mapBuildingProjectiles.begin(), mapBuildingProjectiles.end(), [v](const Ptr<MapBuildingProjectile> &p){ return p.get() == v; });
        if (i == mapBuildingProjectiles.end())
            break;
        mapBuildingProjectiles.erase(i);
    }
}

Ptr<MapBuildingWeapon> StorageImpl::addMapBuildingWeapon(IObject *parent)
{
    auto v = std::make_shared<MapBuildingWeapon>();
    MapBuilding *mapBuilding = (MapBuilding *)parent;
    mapBuilding->weapons.push_back(v);
    mapBuildingWeapons.push_back(v);
    v->mapBuilding = mapBuildings[mapBuilding->id];
    return v;
}

void StorageImpl::deleteMapBuildingWeapon(MapBuildingWeapon *v)
{
    while (1)
    {
        auto i = find_if(mapBuildingWeapons.begin(), mapBuildingWeapons.end(), [v](const Ptr<MapBuildingWeapon> &p){ return p.get() == v; });
        if (i == mapBuildingWeapons.end())
            break;
        mapBuildingWeapons.erase(i);
    }
}

Ptr<MapBuilding> StorageImpl::addMapBuilding(IObject *parent)
{
    int id = 1;
    if (!mapBuildings.empty())
        id = mapBuildings.rbegin()->first + 1;
    auto v = std::make_shared<MapBuilding>();
    v->id = id;
    mapBuildings[v->id] = v;
    Map *map = (Map *)parent;
    map->buildings.push_back(v);
    v->map = maps[map->id];
    return v;
}

void StorageImpl::deleteMapBuilding(MapBuilding *v)
{
    mapBuildings.erase(v->id);
}

Ptr<MapObject> StorageImpl::addMapObject(IObject *parent)
{
    int id = 1;
    if (!mapObjects.empty())
        id = mapObjects.rbegin()->first + 1;
    auto v = std::make_shared<MapObject>();
    v->id = id;
    mapObjects[v->id] = v;
    Map *map = (Map *)parent;
    map->objects.push_back(v);
    v->map = maps[map->id];
    return v;
}

void StorageImpl::deleteMapObject(MapObject *v)
{
    mapObjects.erase(v->id);
}

Ptr<Map> StorageImpl::addMap(IObject *parent)
{
    int id = 1;
    if (!maps.empty())
        id = maps.rbegin()->first + 1;
    auto v = std::make_shared<Map>();
    v->id = id;
    maps[v->id] = v;
    return v;
}

void StorageImpl::deleteMap(Map *v)
{
    maps.erase(v->id);
}

Ptr<Mechanoid> StorageImpl::addMechanoid(IObject *parent)
{
    int id = 1;
    if (!mechanoids.empty())
        id = mechanoids.rbegin()->first + 1;
    auto v = std::make_shared<Mechanoid>();
    v->id = id;
    mechanoids[v->id] = v;
    return v;
}

void StorageImpl::deleteMechanoid(Mechanoid *v)
{
    mechanoids.erase(v->id);
}

Ptr<ModificationClan> StorageImpl::addModificationClan(IObject *parent)
{
    auto v = std::make_shared<ModificationClan>();
    Modification *modification = (Modification *)parent;
    modification->clans.push_back(v);
    modificationClans.push_back(v);
    v->modification = modifications[modification->id];
    return v;
}

void StorageImpl::deleteModificationClan(ModificationClan *v)
{
    while (1)
    {
        auto i = find_if(modificationClans.begin(), modificationClans.end(), [v](const Ptr<ModificationClan> &p){ return p.get() == v; });
        if (i == modificationClans.end())
            break;
        modificationClans.erase(i);
    }
}

Ptr<ModificationMap> StorageImpl::addModificationMap(IObject *parent)
{
    auto v = std::make_shared<ModificationMap>();
    Modification *modification = (Modification *)parent;
    modification->maps.push_back(v);
    modificationMaps.push_back(v);
    v->modification = modifications[modification->id];
    return v;
}

void StorageImpl::deleteModificationMap(ModificationMap *v)
{
    while (1)
    {
        auto i = find_if(modificationMaps.begin(), modificationMaps.end(), [v](const Ptr<ModificationMap> &p){ return p.get() == v; });
        if (i == modificationMaps.end())
            break;
        modificationMaps.erase(i);
    }
}

Ptr<ModificationMechanoid> StorageImpl::addModificationMechanoid(IObject *parent)
{
    auto v = std::make_shared<ModificationMechanoid>();
    Modification *modification = (Modification *)parent;
    modification->mechanoids.push_back(v);
    modificationMechanoids.push_back(v);
    v->modification = modifications[modification->id];
    return v;
}

void StorageImpl::deleteModificationMechanoid(ModificationMechanoid *v)
{
    while (1)
    {
        auto i = find_if(modificationMechanoids.begin(), modificationMechanoids.end(), [v](const Ptr<ModificationMechanoid> &p){ return p.get() == v; });
        if (i == modificationMechanoids.end())
            break;
        modificationMechanoids.erase(i);
    }
}

Ptr<Modification> StorageImpl::addModification(IObject *parent)
{
    int id = 1;
    if (!modifications.empty())
        id = modifications.rbegin()->first + 1;
    auto v = std::make_shared<Modification>();
    v->id = id;
    modifications[v->id] = v;
    return v;
}

void StorageImpl::deleteModification(Modification *v)
{
    modifications.erase(v->id);
}

Ptr<Modificator> StorageImpl::addModificator(IObject *parent)
{
    int id = 1;
    if (!modificators.empty())
        id = modificators.rbegin()->first + 1;
    auto v = std::make_shared<Modificator>();
    v->id = id;
    modificators[v->id] = v;
    return v;
}

void StorageImpl::deleteModificator(Modificator *v)
{
    modificators.erase(v->id);
}

Ptr<Object> StorageImpl::addObject(IObject *parent)
{
    int id = 1;
    if (!objects.empty())
        id = objects.rbegin()->first + 1;
    auto v = std::make_shared<Object>();
    v->id = id;
    objects[v->id] = v;
    return v;
}

void StorageImpl::deleteObject(Object *v)
{
    objects.erase(v->id);
}

Ptr<Player> StorageImpl::addPlayer(IObject *parent)
{
    int id = 1;
    if (!players.empty())
        id = players.rbegin()->first + 1;
    auto v = std::make_shared<Player>();
    v->id = id;
    players[v->id] = v;
    return v;
}

void StorageImpl::deletePlayer(Player *v)
{
    players.erase(v->id);
}

Ptr<Projectile> StorageImpl::addProjectile(IObject *parent)
{
    int id = 1;
    if (!projectiles.empty())
        id = projectiles.rbegin()->first + 1;
    auto v = std::make_shared<Projectile>();
    v->id = id;
    projectiles[v->id] = v;
    return v;
}

void StorageImpl::deleteProjectile(Projectile *v)
{
    projectiles.erase(v->id);
}

Ptr<QuestRewardEquipment> StorageImpl::addQuestRewardEquipment(IObject *parent)
{
    auto v = std::make_shared<QuestRewardEquipment>();
    QuestReward *questReward = (QuestReward *)parent;
    questReward->equipments.push_back(v);
    questRewardEquipments.push_back(v);
    v->questReward = questRewards[questReward->id];
    return v;
}

void StorageImpl::deleteQuestRewardEquipment(QuestRewardEquipment *v)
{
    while (1)
    {
        auto i = find_if(questRewardEquipments.begin(), questRewardEquipments.end(), [v](const Ptr<QuestRewardEquipment> &p){ return p.get() == v; });
        if (i == questRewardEquipments.end())
            break;
        questRewardEquipments.erase(i);
    }
}

Ptr<QuestRewardGlider> StorageImpl::addQuestRewardGlider(IObject *parent)
{
    auto v = std::make_shared<QuestRewardGlider>();
    QuestReward *questReward = (QuestReward *)parent;
    questReward->gliders.push_back(v);
    questRewardGliders.push_back(v);
    v->questReward = questRewards[questReward->id];
    return v;
}

void StorageImpl::deleteQuestRewardGlider(QuestRewardGlider *v)
{
    while (1)
    {
        auto i = find_if(questRewardGliders.begin(), questRewardGliders.end(), [v](const Ptr<QuestRewardGlider> &p){ return p.get() == v; });
        if (i == questRewardGliders.end())
            break;
        questRewardGliders.erase(i);
    }
}

Ptr<QuestRewardGood> StorageImpl::addQuestRewardGood(IObject *parent)
{
    auto v = std::make_shared<QuestRewardGood>();
    QuestReward *questReward = (QuestReward *)parent;
    questReward->goods.push_back(v);
    questRewardGoods.push_back(v);
    v->questReward = questRewards[questReward->id];
    return v;
}

void StorageImpl::deleteQuestRewardGood(QuestRewardGood *v)
{
    while (1)
    {
        auto i = find_if(questRewardGoods.begin(), questRewardGoods.end(), [v](const Ptr<QuestRewardGood> &p){ return p.get() == v; });
        if (i == questRewardGoods.end())
            break;
        questRewardGoods.erase(i);
    }
}

Ptr<QuestRewardModificator> StorageImpl::addQuestRewardModificator(IObject *parent)
{
    auto v = std::make_shared<QuestRewardModificator>();
    QuestReward *questReward = (QuestReward *)parent;
    questReward->modificators.push_back(v);
    questRewardModificators.push_back(v);
    v->questReward = questRewards[questReward->id];
    return v;
}

void StorageImpl::deleteQuestRewardModificator(QuestRewardModificator *v)
{
    while (1)
    {
        auto i = find_if(questRewardModificators.begin(), questRewardModificators.end(), [v](const Ptr<QuestRewardModificator> &p){ return p.get() == v; });
        if (i == questRewardModificators.end())
            break;
        questRewardModificators.erase(i);
    }
}

Ptr<QuestRewardProjectile> StorageImpl::addQuestRewardProjectile(IObject *parent)
{
    auto v = std::make_shared<QuestRewardProjectile>();
    QuestReward *questReward = (QuestReward *)parent;
    questReward->projectiles.push_back(v);
    questRewardProjectiles.push_back(v);
    v->questReward = questRewards[questReward->id];
    return v;
}

void StorageImpl::deleteQuestRewardProjectile(QuestRewardProjectile *v)
{
    while (1)
    {
        auto i = find_if(questRewardProjectiles.begin(), questRewardProjectiles.end(), [v](const Ptr<QuestRewardProjectile> &p){ return p.get() == v; });
        if (i == questRewardProjectiles.end())
            break;
        questRewardProjectiles.erase(i);
    }
}

Ptr<QuestRewardReputation> StorageImpl::addQuestRewardReputation(IObject *parent)
{
    auto v = std::make_shared<QuestRewardReputation>();
    QuestReward *questReward = (QuestReward *)parent;
    questReward->reputations.push_back(v);
    questRewardReputations.push_back(v);
    v->questReward = questRewards[questReward->id];
    return v;
}

void StorageImpl::deleteQuestRewardReputation(QuestRewardReputation *v)
{
    while (1)
    {
        auto i = find_if(questRewardReputations.begin(), questRewardReputations.end(), [v](const Ptr<QuestRewardReputation> &p){ return p.get() == v; });
        if (i == questRewardReputations.end())
            break;
        questRewardReputations.erase(i);
    }
}

Ptr<QuestRewardWeapon> StorageImpl::addQuestRewardWeapon(IObject *parent)
{
    auto v = std::make_shared<QuestRewardWeapon>();
    QuestReward *questReward = (QuestReward *)parent;
    questReward->weapons.push_back(v);
    questRewardWeapons.push_back(v);
    v->questReward = questRewards[questReward->id];
    return v;
}

void StorageImpl::deleteQuestRewardWeapon(QuestRewardWeapon *v)
{
    while (1)
    {
        auto i = find_if(questRewardWeapons.begin(), questRewardWeapons.end(), [v](const Ptr<QuestRewardWeapon> &p){ return p.get() == v; });
        if (i == questRewardWeapons.end())
            break;
        questRewardWeapons.erase(i);
    }
}

Ptr<QuestReward> StorageImpl::addQuestReward(IObject *parent)
{
    int id = 1;
    if (!questRewards.empty())
        id = questRewards.rbegin()->first + 1;
    auto v = std::make_shared<QuestReward>();
    v->id = id;
    questRewards[v->id] = v;
    Quest *quest = (Quest *)parent;
    quest->rewards.push_back(v);
    v->quest = quests[quest->id];
    return v;
}

void StorageImpl::deleteQuestReward(QuestReward *v)
{
    questRewards.erase(v->id);
}

Ptr<Quest> StorageImpl::addQuest(IObject *parent)
{
    int id = 1;
    if (!quests.empty())
        id = quests.rbegin()->first + 1;
    auto v = std::make_shared<Quest>();
    v->id = id;
    quests[v->id] = v;
    return v;
}

void StorageImpl::deleteQuest(Quest *v)
{
    quests.erase(v->id);
}

Ptr<SaveObject> StorageImpl::addSaveObject(IObject *parent)
{
    auto v = std::make_shared<SaveObject>();
    Save *save = (Save *)parent;
    save->objects.push_back(v);
    saveObjects.push_back(v);
    v->save = saves[save->id];
    return v;
}

void StorageImpl::deleteSaveObject(SaveObject *v)
{
    while (1)
    {
        auto i = find_if(saveObjects.begin(), saveObjects.end(), [v](const Ptr<SaveObject> &p){ return p.get() == v; });
        if (i == saveObjects.end())
            break;
        saveObjects.erase(i);
    }
}

Ptr<SavePlayer> StorageImpl::addSavePlayer(IObject *parent)
{
    auto v = std::make_shared<SavePlayer>();
    Save *save = (Save *)parent;
    save->players.push_back(v);
    savePlayers.push_back(v);
    v->save = saves[save->id];
    return v;
}

void StorageImpl::deleteSavePlayer(SavePlayer *v)
{
    while (1)
    {
        auto i = find_if(savePlayers.begin(), savePlayers.end(), [v](const Ptr<SavePlayer> &p){ return p.get() == v; });
        if (i == savePlayers.end())
            break;
        savePlayers.erase(i);
    }
}

Ptr<SaveQuest> StorageImpl::addSaveQuest(IObject *parent)
{
    auto v = std::make_shared<SaveQuest>();
    Save *save = (Save *)parent;
    save->quests.push_back(v);
    saveQuests.push_back(v);
    v->save = saves[save->id];
    return v;
}

void StorageImpl::deleteSaveQuest(SaveQuest *v)
{
    while (1)
    {
        auto i = find_if(saveQuests.begin(), saveQuests.end(), [v](const Ptr<SaveQuest> &p){ return p.get() == v; });
        if (i == saveQuests.end())
            break;
        saveQuests.erase(i);
    }
}

Ptr<Save> StorageImpl::addSave(IObject *parent)
{
    int id = 1;
    if (!saves.empty())
        id = saves.rbegin()->first + 1;
    auto v = std::make_shared<Save>();
    v->id = id;
    saves[v->id] = v;
    return v;
}

void StorageImpl::deleteSave(Save *v)
{
    saves.erase(v->id);
}

Ptr<String> StorageImpl::addString(IObject *parent)
{
    int id = 1;
    if (!strings.empty())
        id = strings.rbegin()->first + 1;
    auto v = std::make_shared<String>();
    v->id = id;
    strings[v->id] = v;
    return v;
}

void StorageImpl::deleteString(String *v)
{
    strings.erase(v->id);
}

Ptr<Weapon> StorageImpl::addWeapon(IObject *parent)
{
    int id = 1;
    if (!weapons.empty())
        id = weapons.rbegin()->first + 1;
    auto v = std::make_shared<Weapon>();
    v->id = id;
    weapons[v->id] = v;
    return v;
}

void StorageImpl::deleteWeapon(Weapon *v)
{
    weapons.erase(v->id);
}

Ptr<IObject> StorageImpl::addRecord(IObject *parent)
{
    EObjectType type = parent->getType();
    switch (type)
    {
    case EObjectType::Building:
        return addBuilding(parent);
    case EObjectType::ClanReputation:
        return addClanReputation(parent);
    case EObjectType::Clan:
        return addClan(parent);
    case EObjectType::ConfigurationEquipment:
        return addConfigurationEquipment(parent);
    case EObjectType::ConfigurationGood:
        return addConfigurationGood(parent);
    case EObjectType::ConfigurationProjectile:
        return addConfigurationProjectile(parent);
    case EObjectType::ConfigurationWeapon:
        return addConfigurationWeapon(parent);
    case EObjectType::Configuration:
        return addConfiguration(parent);
    case EObjectType::Coordinate:
        return addCoordinate(parent);
    case EObjectType::Equipment:
        return addEquipment(parent);
    case EObjectType::Glider:
        return addGlider(parent);
    case EObjectType::Good:
        return addGood(parent);
    case EObjectType::MapBuildingEquipment:
        return addMapBuildingEquipment(parent);
    case EObjectType::MapBuildingGlider:
        return addMapBuildingGlider(parent);
    case EObjectType::MapBuildingGood:
        return addMapBuildingGood(parent);
    case EObjectType::MapBuildingModificator:
        return addMapBuildingModificator(parent);
    case EObjectType::MapBuildingProjectile:
        return addMapBuildingProjectile(parent);
    case EObjectType::MapBuildingWeapon:
        return addMapBuildingWeapon(parent);
    case EObjectType::MapBuilding:
        return addMapBuilding(parent);
    case EObjectType::MapObject:
        return addMapObject(parent);
    case EObjectType::Map:
        return addMap(parent);
    case EObjectType::Mechanoid:
        return addMechanoid(parent);
    case EObjectType::ModificationClan:
        return addModificationClan(parent);
    case EObjectType::ModificationMap:
        return addModificationMap(parent);
    case EObjectType::ModificationMechanoid:
        return addModificationMechanoid(parent);
    case EObjectType::Modification:
        return addModification(parent);
    case EObjectType::Modificator:
        return addModificator(parent);
    case EObjectType::Object:
        return addObject(parent);
    case EObjectType::Player:
        return addPlayer(parent);
    case EObjectType::Projectile:
        return addProjectile(parent);
    case EObjectType::QuestRewardEquipment:
        return addQuestRewardEquipment(parent);
    case EObjectType::QuestRewardGlider:
        return addQuestRewardGlider(parent);
    case EObjectType::QuestRewardGood:
        return addQuestRewardGood(parent);
    case EObjectType::QuestRewardModificator:
        return addQuestRewardModificator(parent);
    case EObjectType::QuestRewardProjectile:
        return addQuestRewardProjectile(parent);
    case EObjectType::QuestRewardReputation:
        return addQuestRewardReputation(parent);
    case EObjectType::QuestRewardWeapon:
        return addQuestRewardWeapon(parent);
    case EObjectType::QuestReward:
        return addQuestReward(parent);
    case EObjectType::Quest:
        return addQuest(parent);
    case EObjectType::SaveObject:
        return addSaveObject(parent);
    case EObjectType::SavePlayer:
        return addSavePlayer(parent);
    case EObjectType::SaveQuest:
        return addSaveQuest(parent);
    case EObjectType::Save:
        return addSave(parent);
    case EObjectType::String:
        return addString(parent);
    case EObjectType::Weapon:
        return addWeapon(parent);
    default:
        return Ptr<IObject>(0);
    }
}

void StorageImpl::deleteRecord(IObject *data)
{
    EObjectType type = data->getType();
    switch (type)
    {
    case EObjectType::Building:
        deleteBuilding((Building *)data);
        break;
    case EObjectType::ClanReputation:
        deleteClanReputation((ClanReputation *)data);
        break;
    case EObjectType::Clan:
        deleteClan((Clan *)data);
        break;
    case EObjectType::ConfigurationEquipment:
        deleteConfigurationEquipment((ConfigurationEquipment *)data);
        break;
    case EObjectType::ConfigurationGood:
        deleteConfigurationGood((ConfigurationGood *)data);
        break;
    case EObjectType::ConfigurationProjectile:
        deleteConfigurationProjectile((ConfigurationProjectile *)data);
        break;
    case EObjectType::ConfigurationWeapon:
        deleteConfigurationWeapon((ConfigurationWeapon *)data);
        break;
    case EObjectType::Configuration:
        deleteConfiguration((Configuration *)data);
        break;
    case EObjectType::Coordinate:
        deleteCoordinate((Coordinate *)data);
        break;
    case EObjectType::Equipment:
        deleteEquipment((Equipment *)data);
        break;
    case EObjectType::Glider:
        deleteGlider((Glider *)data);
        break;
    case EObjectType::Good:
        deleteGood((Good *)data);
        break;
    case EObjectType::MapBuildingEquipment:
        deleteMapBuildingEquipment((MapBuildingEquipment *)data);
        break;
    case EObjectType::MapBuildingGlider:
        deleteMapBuildingGlider((MapBuildingGlider *)data);
        break;
    case EObjectType::MapBuildingGood:
        deleteMapBuildingGood((MapBuildingGood *)data);
        break;
    case EObjectType::MapBuildingModificator:
        deleteMapBuildingModificator((MapBuildingModificator *)data);
        break;
    case EObjectType::MapBuildingProjectile:
        deleteMapBuildingProjectile((MapBuildingProjectile *)data);
        break;
    case EObjectType::MapBuildingWeapon:
        deleteMapBuildingWeapon((MapBuildingWeapon *)data);
        break;
    case EObjectType::MapBuilding:
        deleteMapBuilding((MapBuilding *)data);
        break;
    case EObjectType::MapObject:
        deleteMapObject((MapObject *)data);
        break;
    case EObjectType::Map:
        deleteMap((Map *)data);
        break;
    case EObjectType::Mechanoid:
        deleteMechanoid((Mechanoid *)data);
        break;
    case EObjectType::ModificationClan:
        deleteModificationClan((ModificationClan *)data);
        break;
    case EObjectType::ModificationMap:
        deleteModificationMap((ModificationMap *)data);
        break;
    case EObjectType::ModificationMechanoid:
        deleteModificationMechanoid((ModificationMechanoid *)data);
        break;
    case EObjectType::Modification:
        deleteModification((Modification *)data);
        break;
    case EObjectType::Modificator:
        deleteModificator((Modificator *)data);
        break;
    case EObjectType::Object:
        deleteObject((Object *)data);
        break;
    case EObjectType::Player:
        deletePlayer((Player *)data);
        break;
    case EObjectType::Projectile:
        deleteProjectile((Projectile *)data);
        break;
    case EObjectType::QuestRewardEquipment:
        deleteQuestRewardEquipment((QuestRewardEquipment *)data);
        break;
    case EObjectType::QuestRewardGlider:
        deleteQuestRewardGlider((QuestRewardGlider *)data);
        break;
    case EObjectType::QuestRewardGood:
        deleteQuestRewardGood((QuestRewardGood *)data);
        break;
    case EObjectType::QuestRewardModificator:
        deleteQuestRewardModificator((QuestRewardModificator *)data);
        break;
    case EObjectType::QuestRewardProjectile:
        deleteQuestRewardProjectile((QuestRewardProjectile *)data);
        break;
    case EObjectType::QuestRewardReputation:
        deleteQuestRewardReputation((QuestRewardReputation *)data);
        break;
    case EObjectType::QuestRewardWeapon:
        deleteQuestRewardWeapon((QuestRewardWeapon *)data);
        break;
    case EObjectType::QuestReward:
        deleteQuestReward((QuestReward *)data);
        break;
    case EObjectType::Quest:
        deleteQuest((Quest *)data);
        break;
    case EObjectType::SaveObject:
        deleteSaveObject((SaveObject *)data);
        break;
    case EObjectType::SavePlayer:
        deleteSavePlayer((SavePlayer *)data);
        break;
    case EObjectType::SaveQuest:
        deleteSaveQuest((SaveQuest *)data);
        break;
    case EObjectType::Save:
        deleteSave((Save *)data);
        break;
    case EObjectType::String:
        deleteString((String *)data);
        break;
    case EObjectType::Weapon:
        deleteWeapon((Weapon *)data);
        break;
    default:
        break;
    }
}

#ifdef USE_QT
void StorageImpl::printQtTreeView(QTreeWidgetItem *root) const
{
    QTreeWidgetItem *item;

    item = new QTreeWidgetItem(root, QStringList(QCoreApplication::translate("DB", "Buildings")));
    item->setData(0, Qt::UserRole, static_cast<int>(EObjectType::Building));
    auto buildings = getOrderedMap(EObjectType::Building);
    for (auto &building : buildings)
        building.second->printQtTreeView(item);

    item = new QTreeWidgetItem(root, QStringList(QCoreApplication::translate("DB", "Clans")));
    item->setData(0, Qt::UserRole, static_cast<int>(EObjectType::Clan));
    auto clans = getOrderedMap(EObjectType::Clan);
    for (auto &clan : clans)
        clan.second->printQtTreeView(item);

    item = new QTreeWidgetItem(root, QStringList(QCoreApplication::translate("DB", "Configurations")));
    item->setData(0, Qt::UserRole, static_cast<int>(EObjectType::Configuration));
    auto configurations = getOrderedMap(EObjectType::Configuration);
    for (auto &configuration : configurations)
        configuration.second->printQtTreeView(item);

    item = new QTreeWidgetItem(root, QStringList(QCoreApplication::translate("DB", "Coordinates")));
    item->setData(0, Qt::UserRole, static_cast<int>(EObjectType::Coordinate));
    auto coordinates = getOrderedMap(EObjectType::Coordinate);
    for (auto &coordinate : coordinates)
        coordinate.second->printQtTreeView(item);

    item = new QTreeWidgetItem(root, QStringList(QCoreApplication::translate("DB", "Equipments")));
    item->setData(0, Qt::UserRole, static_cast<int>(EObjectType::Equipment));
    auto equipments = getOrderedMap(EObjectType::Equipment);
    for (auto &equipment : equipments)
        equipment.second->printQtTreeView(item);

    item = new QTreeWidgetItem(root, QStringList(QCoreApplication::translate("DB", "Gliders")));
    item->setData(0, Qt::UserRole, static_cast<int>(EObjectType::Glider));
    auto gliders = getOrderedMap(EObjectType::Glider);
    for (auto &glider : gliders)
        glider.second->printQtTreeView(item);

    item = new QTreeWidgetItem(root, QStringList(QCoreApplication::translate("DB", "Goods")));
    item->setData(0, Qt::UserRole, static_cast<int>(EObjectType::Good));
    auto goods = getOrderedMap(EObjectType::Good);
    for (auto &good : goods)
        good.second->printQtTreeView(item);

    item = new QTreeWidgetItem(root, QStringList(QCoreApplication::translate("DB", "Maps")));
    item->setData(0, Qt::UserRole, static_cast<int>(EObjectType::Map));
    auto maps = getOrderedMap(EObjectType::Map);
    for (auto &map : maps)
        map.second->printQtTreeView(item);

    item = new QTreeWidgetItem(root, QStringList(QCoreApplication::translate("DB", "Mechanoids")));
    item->setData(0, Qt::UserRole, static_cast<int>(EObjectType::Mechanoid));
    auto mechanoids = getOrderedMap(EObjectType::Mechanoid);
    for (auto &mechanoid : mechanoids)
        mechanoid.second->printQtTreeView(item);

    item = new QTreeWidgetItem(root, QStringList(QCoreApplication::translate("DB", "Modifications")));
    item->setData(0, Qt::UserRole, static_cast<int>(EObjectType::Modification));
    auto modifications = getOrderedMap(EObjectType::Modification);
    for (auto &modification : modifications)
        modification.second->printQtTreeView(item);

    item = new QTreeWidgetItem(root, QStringList(QCoreApplication::translate("DB", "Modificators")));
    item->setData(0, Qt::UserRole, static_cast<int>(EObjectType::Modificator));
    auto modificators = getOrderedMap(EObjectType::Modificator);
    for (auto &modificator : modificators)
        modificator.second->printQtTreeView(item);

    item = new QTreeWidgetItem(root, QStringList(QCoreApplication::translate("DB", "Objects")));
    item->setData(0, Qt::UserRole, static_cast<int>(EObjectType::Object));
    auto objects = getOrderedMap(EObjectType::Object);
    for (auto &object : objects)
        object.second->printQtTreeView(item);

    item = new QTreeWidgetItem(root, QStringList(QCoreApplication::translate("DB", "Players")));
    item->setData(0, Qt::UserRole, static_cast<int>(EObjectType::Player));
    auto players = getOrderedMap(EObjectType::Player);
    for (auto &player : players)
        player.second->printQtTreeView(item);

    item = new QTreeWidgetItem(root, QStringList(QCoreApplication::translate("DB", "Projectiles")));
    item->setData(0, Qt::UserRole, static_cast<int>(EObjectType::Projectile));
    auto projectiles = getOrderedMap(EObjectType::Projectile);
    for (auto &projectile : projectiles)
        projectile.second->printQtTreeView(item);

    item = new QTreeWidgetItem(root, QStringList(QCoreApplication::translate("DB", "Quests")));
    item->setData(0, Qt::UserRole, static_cast<int>(EObjectType::Quest));
    auto quests = getOrderedMap(EObjectType::Quest);
    for (auto &quest : quests)
        quest.second->printQtTreeView(item);

    item = new QTreeWidgetItem(root, QStringList(QCoreApplication::translate("DB", "Saves")));
    item->setData(0, Qt::UserRole, static_cast<int>(EObjectType::Save));
    auto saves = getOrderedMap(EObjectType::Save);
    for (auto &save : saves)
        save.second->printQtTreeView(item);

    item = new QTreeWidgetItem(root, QStringList(QCoreApplication::translate("DB", "Strings")));
    item->setData(0, Qt::UserRole, static_cast<int>(EObjectType::String));
    auto strings = getOrderedMap(EObjectType::String);
    for (auto &string : strings)
        string.second->printQtTreeView(item);

    item = new QTreeWidgetItem(root, QStringList(QCoreApplication::translate("DB", "Weapons")));
    item->setData(0, Qt::UserRole, static_cast<int>(EObjectType::Weapon));
    auto weapons = getOrderedMap(EObjectType::Weapon);
    for (auto &weapon : weapons)
        weapon.second->printQtTreeView(item);
}

QTreeWidgetItem *StorageImpl::addRecord(QTreeWidgetItem *item)
{
    EObjectType type = static_cast<EObjectType>(item->data(0, Qt::UserRole).toInt());
    IObject *parent = 0;
    auto parentItem = item->parent();
    if (parentItem)
        parent = (IObject *)parentItem->data(0, Qt::UserRole).toULongLong();
    switch (type)
    {
    case EObjectType::Building:
        return addBuilding(parent)->printQtTreeView(item);
    case EObjectType::ClanReputation:
        return addClanReputation(parent)->printQtTreeView(item);
    case EObjectType::Clan:
        return addClan(parent)->printQtTreeView(item);
    case EObjectType::ConfigurationEquipment:
        return addConfigurationEquipment(parent)->printQtTreeView(item);
    case EObjectType::ConfigurationGood:
        return addConfigurationGood(parent)->printQtTreeView(item);
    case EObjectType::ConfigurationProjectile:
        return addConfigurationProjectile(parent)->printQtTreeView(item);
    case EObjectType::ConfigurationWeapon:
        return addConfigurationWeapon(parent)->printQtTreeView(item);
    case EObjectType::Configuration:
        return addConfiguration(parent)->printQtTreeView(item);
    case EObjectType::Coordinate:
        return addCoordinate(parent)->printQtTreeView(item);
    case EObjectType::Equipment:
        return addEquipment(parent)->printQtTreeView(item);
    case EObjectType::Glider:
        return addGlider(parent)->printQtTreeView(item);
    case EObjectType::Good:
        return addGood(parent)->printQtTreeView(item);
    case EObjectType::MapBuildingEquipment:
        return addMapBuildingEquipment(parent)->printQtTreeView(item);
    case EObjectType::MapBuildingGlider:
        return addMapBuildingGlider(parent)->printQtTreeView(item);
    case EObjectType::MapBuildingGood:
        return addMapBuildingGood(parent)->printQtTreeView(item);
    case EObjectType::MapBuildingModificator:
        return addMapBuildingModificator(parent)->printQtTreeView(item);
    case EObjectType::MapBuildingProjectile:
        return addMapBuildingProjectile(parent)->printQtTreeView(item);
    case EObjectType::MapBuildingWeapon:
        return addMapBuildingWeapon(parent)->printQtTreeView(item);
    case EObjectType::MapBuilding:
        return addMapBuilding(parent)->printQtTreeView(item);
    case EObjectType::MapObject:
        return addMapObject(parent)->printQtTreeView(item);
    case EObjectType::Map:
        return addMap(parent)->printQtTreeView(item);
    case EObjectType::Mechanoid:
        return addMechanoid(parent)->printQtTreeView(item);
    case EObjectType::ModificationClan:
        return addModificationClan(parent)->printQtTreeView(item);
    case EObjectType::ModificationMap:
        return addModificationMap(parent)->printQtTreeView(item);
    case EObjectType::ModificationMechanoid:
        return addModificationMechanoid(parent)->printQtTreeView(item);
    case EObjectType::Modification:
        return addModification(parent)->printQtTreeView(item);
    case EObjectType::Modificator:
        return addModificator(parent)->printQtTreeView(item);
    case EObjectType::Object:
        return addObject(parent)->printQtTreeView(item);
    case EObjectType::Player:
        return addPlayer(parent)->printQtTreeView(item);
    case EObjectType::Projectile:
        return addProjectile(parent)->printQtTreeView(item);
    case EObjectType::QuestRewardEquipment:
        return addQuestRewardEquipment(parent)->printQtTreeView(item);
    case EObjectType::QuestRewardGlider:
        return addQuestRewardGlider(parent)->printQtTreeView(item);
    case EObjectType::QuestRewardGood:
        return addQuestRewardGood(parent)->printQtTreeView(item);
    case EObjectType::QuestRewardModificator:
        return addQuestRewardModificator(parent)->printQtTreeView(item);
    case EObjectType::QuestRewardProjectile:
        return addQuestRewardProjectile(parent)->printQtTreeView(item);
    case EObjectType::QuestRewardReputation:
        return addQuestRewardReputation(parent)->printQtTreeView(item);
    case EObjectType::QuestRewardWeapon:
        return addQuestRewardWeapon(parent)->printQtTreeView(item);
    case EObjectType::QuestReward:
        return addQuestReward(parent)->printQtTreeView(item);
    case EObjectType::Quest:
        return addQuest(parent)->printQtTreeView(item);
    case EObjectType::SaveObject:
        return addSaveObject(parent)->printQtTreeView(item);
    case EObjectType::SavePlayer:
        return addSavePlayer(parent)->printQtTreeView(item);
    case EObjectType::SaveQuest:
        return addSaveQuest(parent)->printQtTreeView(item);
    case EObjectType::Save:
        return addSave(parent)->printQtTreeView(item);
    case EObjectType::String:
        return addString(parent)->printQtTreeView(item);
    case EObjectType::Weapon:
        return addWeapon(parent)->printQtTreeView(item);
    default:
        return 0;
    }
}

void StorageImpl::deleteRecord(QTreeWidgetItem *item)
{
    IObject *data = (IObject *)item->data(0, Qt::UserRole).toULongLong(); 
    EObjectType type = data->getType();
    switch (type)
    {
    case EObjectType::Building:
        deleteBuilding((Building *)data);
        break;
    case EObjectType::ClanReputation:
        deleteClanReputation((ClanReputation *)data);
        break;
    case EObjectType::Clan:
        deleteClan((Clan *)data);
        break;
    case EObjectType::ConfigurationEquipment:
        deleteConfigurationEquipment((ConfigurationEquipment *)data);
        break;
    case EObjectType::ConfigurationGood:
        deleteConfigurationGood((ConfigurationGood *)data);
        break;
    case EObjectType::ConfigurationProjectile:
        deleteConfigurationProjectile((ConfigurationProjectile *)data);
        break;
    case EObjectType::ConfigurationWeapon:
        deleteConfigurationWeapon((ConfigurationWeapon *)data);
        break;
    case EObjectType::Configuration:
        deleteConfiguration((Configuration *)data);
        break;
    case EObjectType::Coordinate:
        deleteCoordinate((Coordinate *)data);
        break;
    case EObjectType::Equipment:
        deleteEquipment((Equipment *)data);
        break;
    case EObjectType::Glider:
        deleteGlider((Glider *)data);
        break;
    case EObjectType::Good:
        deleteGood((Good *)data);
        break;
    case EObjectType::MapBuildingEquipment:
        deleteMapBuildingEquipment((MapBuildingEquipment *)data);
        break;
    case EObjectType::MapBuildingGlider:
        deleteMapBuildingGlider((MapBuildingGlider *)data);
        break;
    case EObjectType::MapBuildingGood:
        deleteMapBuildingGood((MapBuildingGood *)data);
        break;
    case EObjectType::MapBuildingModificator:
        deleteMapBuildingModificator((MapBuildingModificator *)data);
        break;
    case EObjectType::MapBuildingProjectile:
        deleteMapBuildingProjectile((MapBuildingProjectile *)data);
        break;
    case EObjectType::MapBuildingWeapon:
        deleteMapBuildingWeapon((MapBuildingWeapon *)data);
        break;
    case EObjectType::MapBuilding:
        deleteMapBuilding((MapBuilding *)data);
        break;
    case EObjectType::MapObject:
        deleteMapObject((MapObject *)data);
        break;
    case EObjectType::Map:
        deleteMap((Map *)data);
        break;
    case EObjectType::Mechanoid:
        deleteMechanoid((Mechanoid *)data);
        break;
    case EObjectType::ModificationClan:
        deleteModificationClan((ModificationClan *)data);
        break;
    case EObjectType::ModificationMap:
        deleteModificationMap((ModificationMap *)data);
        break;
    case EObjectType::ModificationMechanoid:
        deleteModificationMechanoid((ModificationMechanoid *)data);
        break;
    case EObjectType::Modification:
        deleteModification((Modification *)data);
        break;
    case EObjectType::Modificator:
        deleteModificator((Modificator *)data);
        break;
    case EObjectType::Object:
        deleteObject((Object *)data);
        break;
    case EObjectType::Player:
        deletePlayer((Player *)data);
        break;
    case EObjectType::Projectile:
        deleteProjectile((Projectile *)data);
        break;
    case EObjectType::QuestRewardEquipment:
        deleteQuestRewardEquipment((QuestRewardEquipment *)data);
        break;
    case EObjectType::QuestRewardGlider:
        deleteQuestRewardGlider((QuestRewardGlider *)data);
        break;
    case EObjectType::QuestRewardGood:
        deleteQuestRewardGood((QuestRewardGood *)data);
        break;
    case EObjectType::QuestRewardModificator:
        deleteQuestRewardModificator((QuestRewardModificator *)data);
        break;
    case EObjectType::QuestRewardProjectile:
        deleteQuestRewardProjectile((QuestRewardProjectile *)data);
        break;
    case EObjectType::QuestRewardReputation:
        deleteQuestRewardReputation((QuestRewardReputation *)data);
        break;
    case EObjectType::QuestRewardWeapon:
        deleteQuestRewardWeapon((QuestRewardWeapon *)data);
        break;
    case EObjectType::QuestReward:
        deleteQuestReward((QuestReward *)data);
        break;
    case EObjectType::Quest:
        deleteQuest((Quest *)data);
        break;
    case EObjectType::SaveObject:
        deleteSaveObject((SaveObject *)data);
        break;
    case EObjectType::SavePlayer:
        deleteSavePlayer((SavePlayer *)data);
        break;
    case EObjectType::SaveQuest:
        deleteSaveQuest((SaveQuest *)data);
        break;
    case EObjectType::Save:
        deleteSave((Save *)data);
        break;
    case EObjectType::String:
        deleteString((String *)data);
        break;
    case EObjectType::Weapon:
        deleteWeapon((Weapon *)data);
        break;
    default:
        break;
    }
    item->parent()->removeChild(item);
}
#endif

OrderedObjectMap StorageImpl::getOrderedMap(EObjectType type) const
{
    switch (type)
    {
    case EObjectType::Building:
        return ::getOrderedMap(buildings);
    case EObjectType::ClanReputation:
        return ::getOrderedMap(clanReputations);
    case EObjectType::Clan:
        return ::getOrderedMap(clans);
    case EObjectType::ConfigurationEquipment:
        return ::getOrderedMap(configurationEquipments);
    case EObjectType::ConfigurationGood:
        return ::getOrderedMap(configurationGoods);
    case EObjectType::ConfigurationProjectile:
        return ::getOrderedMap(configurationProjectiles);
    case EObjectType::ConfigurationWeapon:
        return ::getOrderedMap(configurationWeapons);
    case EObjectType::Configuration:
        return ::getOrderedMap(configurations);
    case EObjectType::Coordinate:
        return ::getOrderedMap(coordinates);
    case EObjectType::Equipment:
        return ::getOrderedMap(equipments);
    case EObjectType::Glider:
        return ::getOrderedMap(gliders);
    case EObjectType::Good:
        return ::getOrderedMap(goods);
    case EObjectType::MapBuildingEquipment:
        return ::getOrderedMap(mapBuildingEquipments);
    case EObjectType::MapBuildingGlider:
        return ::getOrderedMap(mapBuildingGliders);
    case EObjectType::MapBuildingGood:
        return ::getOrderedMap(mapBuildingGoods);
    case EObjectType::MapBuildingModificator:
        return ::getOrderedMap(mapBuildingModificators);
    case EObjectType::MapBuildingProjectile:
        return ::getOrderedMap(mapBuildingProjectiles);
    case EObjectType::MapBuildingWeapon:
        return ::getOrderedMap(mapBuildingWeapons);
    case EObjectType::MapBuilding:
        return ::getOrderedMap(mapBuildings);
    case EObjectType::MapObject:
        return ::getOrderedMap(mapObjects);
    case EObjectType::Map:
        return ::getOrderedMap(maps);
    case EObjectType::MechanoidGroup:
        return ::getOrderedMap(mechanoidGroups);
    case EObjectType::Mechanoid:
        return ::getOrderedMap(mechanoids);
    case EObjectType::ModificationClan:
        return ::getOrderedMap(modificationClans);
    case EObjectType::ModificationMap:
        return ::getOrderedMap(modificationMaps);
    case EObjectType::ModificationMechanoid:
        return ::getOrderedMap(modificationMechanoids);
    case EObjectType::Modification:
        return ::getOrderedMap(modifications);
    case EObjectType::Modificator:
        return ::getOrderedMap(modificators);
    case EObjectType::Object:
        return ::getOrderedMap(objects);
    case EObjectType::Player:
        return ::getOrderedMap(players);
    case EObjectType::Projectile:
        return ::getOrderedMap(projectiles);
    case EObjectType::QuestRewardEquipment:
        return ::getOrderedMap(questRewardEquipments);
    case EObjectType::QuestRewardGlider:
        return ::getOrderedMap(questRewardGliders);
    case EObjectType::QuestRewardGood:
        return ::getOrderedMap(questRewardGoods);
    case EObjectType::QuestRewardModificator:
        return ::getOrderedMap(questRewardModificators);
    case EObjectType::QuestRewardProjectile:
        return ::getOrderedMap(questRewardProjectiles);
    case EObjectType::QuestRewardReputation:
        return ::getOrderedMap(questRewardReputations);
    case EObjectType::QuestRewardWeapon:
        return ::getOrderedMap(questRewardWeapons);
    case EObjectType::QuestReward:
        return ::getOrderedMap(questRewards);
    case EObjectType::Quest:
        return ::getOrderedMap(quests);
    case EObjectType::SaveObject:
        return ::getOrderedMap(saveObjects);
    case EObjectType::SavePlayer:
        return ::getOrderedMap(savePlayers);
    case EObjectType::SaveQuest:
        return ::getOrderedMap(saveQuests);
    case EObjectType::Save:
        return ::getOrderedMap(saves);
    case EObjectType::ScriptVariable:
        return ::getOrderedMap(scriptVariables);
    case EObjectType::String:
        return ::getOrderedMap(strings);
    case EObjectType::Weapon:
        return ::getOrderedMap(weapons);
    default:
        return OrderedObjectMap();
    }
}
