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
        if (cols[1]) configurationGood->goods.id = std::stoi(cols[1]);
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
        if (goods.find(configurationGood->goods.id) != goods.end())
            configurationGood->goods.ptr = goods[configurationGood->goods.id];
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
        query += "'" + std::to_string(configurationGood->goods.id) + "',";
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
        if (cols[4]) coordinate->yaw = std::stof(cols[4]);
        if (cols[5]) coordinate->pitch = std::stof(cols[5]);
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
        query += "'" + std::to_string(coordinate.second->yaw) + "',";
        query += "'" + std::to_string(coordinate.second->pitch) + "',";
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
        if (cols[1]) mapBuildingGood->goods.id = std::stoi(cols[1]);
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
        if (goods.find(mapBuildingGood->goods.id) != goods.end())
            mapBuildingGood->goods.ptr = goods[mapBuildingGood->goods.id];
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
        query += "'" + std::to_string(mapBuildingGood->goods.id) + "',";
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
        if (cols[4]) mapBuilding->coordinate.id = std::stoi(cols[4]);

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
        if (coordinates.find(mapBuilding.second->coordinate.id) != coordinates.end())
            mapBuilding.second->coordinate.ptr = coordinates[mapBuilding.second->coordinate.id];
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
        query += "'" + std::to_string(mapBuilding.second->coordinate.id) + "',";
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
        if (cols[1]) mapObject->map.id = std::stoi(cols[1]);
        if (cols[2]) mapObject->object.id = std::stoi(cols[2]);
        if (cols[3]) mapObject->coordinate.id = std::stoi(cols[3]);

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
        if (coordinates.find(mapObject.second->coordinate.id) != coordinates.end())
            mapObject.second->coordinate.ptr = coordinates[mapObject.second->coordinate.id];
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
        query += "'" + std::to_string(mapObject.second->map.id) + "',";
        query += "'" + std::to_string(mapObject.second->object.id) + "',";
        query += "'" + std::to_string(mapObject.second->coordinate.id) + "',";
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
                map.second->mapBuildings.push_back(mapBuilding.second);
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
        query.resize(query.size() - 1);
        query += "),\n";
    }
    query.resize(query.size() - 2);
    query += ";";
    db->execute(query.c_str(), 0, 0);
}

void StorageImpl::_loadMechanoidGroupMechanoids()
{
    auto callback = [](void *o, int ncols, char **cols, char **names)
    {
        Ptr<MechanoidGroupMechanoid> mechanoidGroupMechanoid = std::make_shared<MechanoidGroupMechanoid>();
        if (cols[0]) mechanoidGroupMechanoid->mechanoidGroup.id = std::stoi(cols[0]);
        if (cols[1]) mechanoidGroupMechanoid->mechanoid.id = std::stoi(cols[1]);

        Storage *storage = (Storage *)o;
        storage->mechanoidGroupMechanoids.push_back(mechanoidGroupMechanoid);
        return 0;
    };
    db->execute("select * from MechanoidGroupMechanoids;", this, callback);
}

void StorageImpl::_loadMechanoidGroupMechanoidsPtrs()
{
    for (auto &mechanoidGroupMechanoid : mechanoidGroupMechanoids)
    {
        if (mechanoidGroups.find(mechanoidGroupMechanoid->mechanoidGroup.id) != mechanoidGroups.end())
            mechanoidGroupMechanoid->mechanoidGroup.ptr = mechanoidGroups[mechanoidGroupMechanoid->mechanoidGroup.id];
        if (mechanoids.find(mechanoidGroupMechanoid->mechanoid.id) != mechanoids.end())
            mechanoidGroupMechanoid->mechanoid.ptr = mechanoids[mechanoidGroupMechanoid->mechanoid.id];
    }
}

void StorageImpl::_loadMechanoidGroupMechanoidsArrays()
{
}

void StorageImpl::_saveMechanoidGroupMechanoids()
{
    std::string query;
    query += "delete from MechanoidGroupMechanoids;";
    db->execute(query.c_str(), 0, 0);
    query.clear();
    if (mechanoidGroupMechanoids.empty())
        return;
    query += "insert or replace into MechanoidGroupMechanoids values\n";
    for (auto &mechanoidGroupMechanoid : mechanoidGroupMechanoids)
    {
        query += "(";
        query += "'" + std::to_string(mechanoidGroupMechanoid->mechanoidGroup.id) + "',";
        query += "'" + std::to_string(mechanoidGroupMechanoid->mechanoid.id) + "',";
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
    for (auto &mechanoidGroup : mechanoidGroups)
    {
        for (auto &mechanoidGroupMechanoid : mechanoidGroupMechanoids)
            if (mechanoidGroup.first == mechanoidGroupMechanoid->mechanoidGroup.id)
                mechanoidGroup.second->mechanoids.push_back(mechanoidGroupMechanoid);
    }
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
        if (cols[7]) mechanoid->clan.id = std::stoi(cols[7]);
        if (cols[8]) mechanoid->rating_fight = std::stof(cols[8]);
        if (cols[9]) mechanoid->rating_courier = std::stof(cols[9]);
        if (cols[10]) mechanoid->rating_trade = std::stof(cols[10]);
        if (cols[11]) mechanoid->map.id = std::stoi(cols[11]);
        if (cols[12]) mechanoid->map_building.id = std::stoi(cols[12]);
        if (cols[13]) mechanoid->coordinate.id = std::stoi(cols[13]);

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
        if (clans.find(mechanoid.second->clan.id) != clans.end())
            mechanoid.second->clan.ptr = clans[mechanoid.second->clan.id];
        if (maps.find(mechanoid.second->map.id) != maps.end())
            mechanoid.second->map.ptr = maps[mechanoid.second->map.id];
        if (mapBuildings.find(mechanoid.second->map_building.id) != mapBuildings.end())
            mechanoid.second->map_building.ptr = mapBuildings[mechanoid.second->map_building.id];
        if (coordinates.find(mechanoid.second->coordinate.id) != coordinates.end())
            mechanoid.second->coordinate.ptr = coordinates[mechanoid.second->coordinate.id];
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
        query += "'" + std::to_string(mechanoid.second->clan.id) + "',";
        query += "'" + std::to_string(mechanoid.second->rating_fight) + "',";
        query += "'" + std::to_string(mechanoid.second->rating_courier) + "',";
        query += "'" + std::to_string(mechanoid.second->rating_trade) + "',";
        query += "'" + std::to_string(mechanoid.second->map.id) + "',";
        query += "'" + std::to_string(mechanoid.second->map_building.id) + "',";
        query += "'" + std::to_string(mechanoid.second->coordinate.id) + "',";
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
                quest.second->questRewards.push_back(questReward.second);
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
        if (cols[3]) saveObject->coordinate.id = std::stoi(cols[3]);

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
        if (coordinates.find(saveObject->coordinate.id) != coordinates.end())
            saveObject->coordinate.ptr = coordinates[saveObject->coordinate.id];
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
        query += "'" + std::to_string(saveObject->coordinate.id) + "',";
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
                save.second->scriptVariables.push_back(scriptVariable);
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
    mechanoidGroupMechanoids.clear();
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

void StorageImpl::load()
{
    _loadBuildings();
    _loadClanReputations();
    _loadClans();
    _loadConfigurationEquipments();
    _loadConfigurationGoods();
    _loadConfigurationProjectiles();
    _loadConfigurationWeapons();
    _loadConfigurations();
    _loadCoordinates();
    _loadEquipments();
    _loadGliders();
    _loadGoods();
    _loadMapBuildingEquipments();
    _loadMapBuildingGliders();
    _loadMapBuildingGoods();
    _loadMapBuildingModificators();
    _loadMapBuildingProjectiles();
    _loadMapBuildingWeapons();
    _loadMapBuildings();
    _loadMapObjects();
    _loadMaps();
    _loadMechanoidGroupMechanoids();
    _loadMechanoidGroups();
    _loadMechanoids();
    _loadModificationClans();
    _loadModificationMaps();
    _loadModificationMechanoids();
    _loadModifications();
    _loadModificators();
    _loadObjects();
    _loadPlayers();
    _loadProjectiles();
    _loadQuestRewardEquipments();
    _loadQuestRewardGliders();
    _loadQuestRewardGoods();
    _loadQuestRewardModificators();
    _loadQuestRewardProjectiles();
    _loadQuestRewardReputations();
    _loadQuestRewardWeapons();
    _loadQuestRewards();
    _loadQuests();
    _loadSaveObjects();
    _loadSavePlayers();
    _loadSaveQuests();
    _loadSaves();
    _loadScriptVariables();
    _loadStrings();
    _loadWeapons();

    _loadBuildingsPtrs();
    _loadClanReputationsPtrs();
    _loadClansPtrs();
    _loadConfigurationEquipmentsPtrs();
    _loadConfigurationGoodsPtrs();
    _loadConfigurationProjectilesPtrs();
    _loadConfigurationWeaponsPtrs();
    _loadConfigurationsPtrs();
    _loadCoordinatesPtrs();
    _loadEquipmentsPtrs();
    _loadGlidersPtrs();
    _loadGoodsPtrs();
    _loadMapBuildingEquipmentsPtrs();
    _loadMapBuildingGlidersPtrs();
    _loadMapBuildingGoodsPtrs();
    _loadMapBuildingModificatorsPtrs();
    _loadMapBuildingProjectilesPtrs();
    _loadMapBuildingWeaponsPtrs();
    _loadMapBuildingsPtrs();
    _loadMapObjectsPtrs();
    _loadMapsPtrs();
    _loadMechanoidGroupMechanoidsPtrs();
    _loadMechanoidGroupsPtrs();
    _loadMechanoidsPtrs();
    _loadModificationClansPtrs();
    _loadModificationMapsPtrs();
    _loadModificationMechanoidsPtrs();
    _loadModificationsPtrs();
    _loadModificatorsPtrs();
    _loadObjectsPtrs();
    _loadPlayersPtrs();
    _loadProjectilesPtrs();
    _loadQuestRewardEquipmentsPtrs();
    _loadQuestRewardGlidersPtrs();
    _loadQuestRewardGoodsPtrs();
    _loadQuestRewardModificatorsPtrs();
    _loadQuestRewardProjectilesPtrs();
    _loadQuestRewardReputationsPtrs();
    _loadQuestRewardWeaponsPtrs();
    _loadQuestRewardsPtrs();
    _loadQuestsPtrs();
    _loadSaveObjectsPtrs();
    _loadSavePlayersPtrs();
    _loadSaveQuestsPtrs();
    _loadSavesPtrs();
    _loadScriptVariablesPtrs();
    _loadStringsPtrs();
    _loadWeaponsPtrs();

    _loadBuildingsArrays();
    _loadClanReputationsArrays();
    _loadClansArrays();
    _loadConfigurationEquipmentsArrays();
    _loadConfigurationGoodsArrays();
    _loadConfigurationProjectilesArrays();
    _loadConfigurationWeaponsArrays();
    _loadConfigurationsArrays();
    _loadCoordinatesArrays();
    _loadEquipmentsArrays();
    _loadGlidersArrays();
    _loadGoodsArrays();
    _loadMapBuildingEquipmentsArrays();
    _loadMapBuildingGlidersArrays();
    _loadMapBuildingGoodsArrays();
    _loadMapBuildingModificatorsArrays();
    _loadMapBuildingProjectilesArrays();
    _loadMapBuildingWeaponsArrays();
    _loadMapBuildingsArrays();
    _loadMapObjectsArrays();
    _loadMapsArrays();
    _loadMechanoidGroupMechanoidsArrays();
    _loadMechanoidGroupsArrays();
    _loadMechanoidsArrays();
    _loadModificationClansArrays();
    _loadModificationMapsArrays();
    _loadModificationMechanoidsArrays();
    _loadModificationsArrays();
    _loadModificatorsArrays();
    _loadObjectsArrays();
    _loadPlayersArrays();
    _loadProjectilesArrays();
    _loadQuestRewardEquipmentsArrays();
    _loadQuestRewardGlidersArrays();
    _loadQuestRewardGoodsArrays();
    _loadQuestRewardModificatorsArrays();
    _loadQuestRewardProjectilesArrays();
    _loadQuestRewardReputationsArrays();
    _loadQuestRewardWeaponsArrays();
    _loadQuestRewardsArrays();
    _loadQuestsArrays();
    _loadSaveObjectsArrays();
    _loadSavePlayersArrays();
    _loadSaveQuestsArrays();
    _loadSavesArrays();
    _loadScriptVariablesArrays();
    _loadStringsArrays();
    _loadWeaponsArrays();
}

void StorageImpl::save()
{
    _saveStrings();
    _saveQuestRewards();
    _saveConfigurations();
    _saveMaps();
    _saveClans();
    _saveCoordinates();
    _saveModifications();
    _saveProjectiles();
    _saveSaves();
    _saveEquipments();
    _saveGliders();
    _saveGoods();
    _saveMechanoids();
    _saveWeapons();
    _saveModificators();
    _saveObjects();
    _saveQuests();
    _saveBuildings();
    _saveMapBuildings();
    _saveMechanoidGroups();
    _savePlayers();
    _saveClanReputations();
    _saveConfigurationEquipments();
    _saveConfigurationGoods();
    _saveConfigurationProjectiles();
    _saveConfigurationWeapons();
    _saveMapBuildingEquipments();
    _saveMapBuildingGliders();
    _saveMapBuildingGoods();
    _saveMapBuildingModificators();
    _saveMapBuildingProjectiles();
    _saveMapBuildingWeapons();
    _saveMapObjects();
    _saveMechanoidGroupMechanoids();
    _saveModificationClans();
    _saveModificationMaps();
    _saveModificationMechanoids();
    _saveQuestRewardEquipments();
    _saveQuestRewardGliders();
    _saveQuestRewardGoods();
    _saveQuestRewardModificators();
    _saveQuestRewardProjectiles();
    _saveQuestRewardReputations();
    _saveQuestRewardWeapons();
    _saveSaveObjects();
    _saveSavePlayers();
    _saveSaveQuests();
    _saveScriptVariables();
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

    item = new QTreeWidgetItem(root, QStringList(QCoreApplication::translate("DB", "Quest Rewards")));
    item->setData(0, Qt::UserRole, static_cast<int>(EObjectType::QuestReward));
    auto questRewards = getOrderedMap(EObjectType::QuestReward);
    for (auto &questReward : questRewards)
        questReward.second->printQtTreeView(item);

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
    switch (type)
    {
    case EObjectType::Building:
    {
        int id = 1;
        if (!buildings.empty())
            id = buildings.rbegin()->first + 1;
        auto v = std::make_shared<Building>();
        v->id = id;
        buildings[id] = v;
        return v->printQtTreeView(item);
    }
    case EObjectType::Clan:
    {
        int id = 1;
        if (!clans.empty())
            id = clans.rbegin()->first + 1;
        auto v = std::make_shared<Clan>();
        v->id = id;
        clans[id] = v;
        return v->printQtTreeView(item);
    }
    case EObjectType::Configuration:
    {
        int id = 1;
        if (!configurations.empty())
            id = configurations.rbegin()->first + 1;
        auto v = std::make_shared<Configuration>();
        v->id = id;
        configurations[id] = v;
        return v->printQtTreeView(item);
    }
    case EObjectType::Equipment:
    {
        int id = 1;
        if (!equipments.empty())
            id = equipments.rbegin()->first + 1;
        auto v = std::make_shared<Equipment>();
        v->id = id;
        equipments[id] = v;
        return v->printQtTreeView(item);
    }
    case EObjectType::Glider:
    {
        int id = 1;
        if (!gliders.empty())
            id = gliders.rbegin()->first + 1;
        auto v = std::make_shared<Glider>();
        v->id = id;
        gliders[id] = v;
        return v->printQtTreeView(item);
    }
    case EObjectType::Good:
    {
        int id = 1;
        if (!goods.empty())
            id = goods.rbegin()->first + 1;
        auto v = std::make_shared<Good>();
        v->id = id;
        goods[id] = v;
        return v->printQtTreeView(item);
    }
    case EObjectType::Map:
    {
        int id = 1;
        if (!maps.empty())
            id = maps.rbegin()->first + 1;
        auto v = std::make_shared<Map>();
        v->id = id;
        maps[id] = v;
        return v->printQtTreeView(item);
    }
    case EObjectType::Mechanoid:
    {
        int id = 1;
        if (!mechanoids.empty())
            id = mechanoids.rbegin()->first + 1;
        auto v = std::make_shared<Mechanoid>();
        v->id = id;
        mechanoids[id] = v;
        return v->printQtTreeView(item);
    }
    case EObjectType::Modification:
    {
        int id = 1;
        if (!modifications.empty())
            id = modifications.rbegin()->first + 1;
        auto v = std::make_shared<Modification>();
        v->id = id;
        modifications[id] = v;
        return v->printQtTreeView(item);
    }
    case EObjectType::Modificator:
    {
        int id = 1;
        if (!modificators.empty())
            id = modificators.rbegin()->first + 1;
        auto v = std::make_shared<Modificator>();
        v->id = id;
        modificators[id] = v;
        return v->printQtTreeView(item);
    }
    case EObjectType::Object:
    {
        int id = 1;
        if (!objects.empty())
            id = objects.rbegin()->first + 1;
        auto v = std::make_shared<Object>();
        v->id = id;
        objects[id] = v;
        return v->printQtTreeView(item);
    }
    case EObjectType::Player:
    {
        int id = 1;
        if (!players.empty())
            id = players.rbegin()->first + 1;
        auto v = std::make_shared<Player>();
        v->id = id;
        players[id] = v;
        return v->printQtTreeView(item);
    }
    case EObjectType::Projectile:
    {
        int id = 1;
        if (!projectiles.empty())
            id = projectiles.rbegin()->first + 1;
        auto v = std::make_shared<Projectile>();
        v->id = id;
        projectiles[id] = v;
        return v->printQtTreeView(item);
    }
    case EObjectType::QuestReward:
    {
        int id = 1;
        if (!questRewards.empty())
            id = questRewards.rbegin()->first + 1;
        auto v = std::make_shared<QuestReward>();
        v->id = id;
        questRewards[id] = v;
        return v->printQtTreeView(item);
    }
    case EObjectType::Quest:
    {
        int id = 1;
        if (!quests.empty())
            id = quests.rbegin()->first + 1;
        auto v = std::make_shared<Quest>();
        v->id = id;
        quests[id] = v;
        return v->printQtTreeView(item);
    }
    case EObjectType::Save:
    {
        int id = 1;
        if (!saves.empty())
            id = saves.rbegin()->first + 1;
        auto v = std::make_shared<Save>();
        v->id = id;
        saves[id] = v;
        return v->printQtTreeView(item);
    }
    case EObjectType::String:
    {
        int id = 1;
        if (!strings.empty())
            id = strings.rbegin()->first + 1;
        auto v = std::make_shared<String>();
        v->id = id;
        strings[id] = v;
        return v->printQtTreeView(item);
    }
    case EObjectType::Weapon:
    {
        int id = 1;
        if (!weapons.empty())
            id = weapons.rbegin()->first + 1;
        auto v = std::make_shared<Weapon>();
        v->id = id;
        weapons[id] = v;
        return v->printQtTreeView(item);
    }
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
    {
        Building *v = (Building *)item->data(0, Qt::UserRole).toULongLong();
        buildings.erase(v->id);
        item->parent()->removeChild(item);
        break;
    }
    case EObjectType::Clan:
    {
        Clan *v = (Clan *)item->data(0, Qt::UserRole).toULongLong();
        clans.erase(v->id);
        item->parent()->removeChild(item);
        break;
    }
    case EObjectType::Configuration:
    {
        Configuration *v = (Configuration *)item->data(0, Qt::UserRole).toULongLong();
        configurations.erase(v->id);
        item->parent()->removeChild(item);
        break;
    }
    case EObjectType::Equipment:
    {
        Equipment *v = (Equipment *)item->data(0, Qt::UserRole).toULongLong();
        equipments.erase(v->id);
        item->parent()->removeChild(item);
        break;
    }
    case EObjectType::Glider:
    {
        Glider *v = (Glider *)item->data(0, Qt::UserRole).toULongLong();
        gliders.erase(v->id);
        item->parent()->removeChild(item);
        break;
    }
    case EObjectType::Good:
    {
        Good *v = (Good *)item->data(0, Qt::UserRole).toULongLong();
        goods.erase(v->id);
        item->parent()->removeChild(item);
        break;
    }
    case EObjectType::Map:
    {
        Map *v = (Map *)item->data(0, Qt::UserRole).toULongLong();
        maps.erase(v->id);
        item->parent()->removeChild(item);
        break;
    }
    case EObjectType::Mechanoid:
    {
        Mechanoid *v = (Mechanoid *)item->data(0, Qt::UserRole).toULongLong();
        mechanoids.erase(v->id);
        item->parent()->removeChild(item);
        break;
    }
    case EObjectType::Modification:
    {
        Modification *v = (Modification *)item->data(0, Qt::UserRole).toULongLong();
        modifications.erase(v->id);
        item->parent()->removeChild(item);
        break;
    }
    case EObjectType::Modificator:
    {
        Modificator *v = (Modificator *)item->data(0, Qt::UserRole).toULongLong();
        modificators.erase(v->id);
        item->parent()->removeChild(item);
        break;
    }
    case EObjectType::Object:
    {
        Object *v = (Object *)item->data(0, Qt::UserRole).toULongLong();
        objects.erase(v->id);
        item->parent()->removeChild(item);
        break;
    }
    case EObjectType::Player:
    {
        Player *v = (Player *)item->data(0, Qt::UserRole).toULongLong();
        players.erase(v->id);
        item->parent()->removeChild(item);
        break;
    }
    case EObjectType::Projectile:
    {
        Projectile *v = (Projectile *)item->data(0, Qt::UserRole).toULongLong();
        projectiles.erase(v->id);
        item->parent()->removeChild(item);
        break;
    }
    case EObjectType::QuestReward:
    {
        QuestReward *v = (QuestReward *)item->data(0, Qt::UserRole).toULongLong();
        questRewards.erase(v->id);
        item->parent()->removeChild(item);
        break;
    }
    case EObjectType::Quest:
    {
        Quest *v = (Quest *)item->data(0, Qt::UserRole).toULongLong();
        quests.erase(v->id);
        item->parent()->removeChild(item);
        break;
    }
    case EObjectType::Save:
    {
        Save *v = (Save *)item->data(0, Qt::UserRole).toULongLong();
        saves.erase(v->id);
        item->parent()->removeChild(item);
        break;
    }
    case EObjectType::String:
    {
        String *v = (String *)item->data(0, Qt::UserRole).toULongLong();
        strings.erase(v->id);
        item->parent()->removeChild(item);
        break;
    }
    case EObjectType::Weapon:
    {
        Weapon *v = (Weapon *)item->data(0, Qt::UserRole).toULongLong();
        weapons.erase(v->id);
        item->parent()->removeChild(item);
        break;
    }
    default:
        break;
    }
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
    case EObjectType::MechanoidGroupMechanoid:
        return ::getOrderedMap(mechanoidGroupMechanoids);
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
