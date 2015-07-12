/* DO NOT EDIT! This is an autogenerated file. */

EObjectType getTableType(const std::string &table)
{
    static std::map<std::string, EObjectType> types =
    {
        { "Buildings", EObjectType::Building },
        { "ClanMechanoids", EObjectType::ClanMechanoid },
        { "ClanReputations", EObjectType::ClanReputation },
        { "Clans", EObjectType::Clan },
        { "ConfigurationEquipments", EObjectType::ConfigurationEquipment },
        { "ConfigurationGoods", EObjectType::ConfigurationGood },
        { "ConfigurationProjectiles", EObjectType::ConfigurationProjectile },
        { "ConfigurationWeapons", EObjectType::ConfigurationWeapon },
        { "Configurations", EObjectType::Configuration },
        { "Equipments", EObjectType::Equipment },
        { "Gliders", EObjectType::Glider },
        { "Goods", EObjectType::Good },
        { "GroupMechanoids", EObjectType::GroupMechanoid },
        { "Groups", EObjectType::Group },
        { "MapBuildingEquipments", EObjectType::MapBuildingEquipment },
        { "MapBuildingGliders", EObjectType::MapBuildingGlider },
        { "MapBuildingGoods", EObjectType::MapBuildingGood },
        { "MapBuildingModificators", EObjectType::MapBuildingModificator },
        { "MapBuildingProjectiles", EObjectType::MapBuildingProjectile },
        { "MapBuildingWeapons", EObjectType::MapBuildingWeapon },
        { "MapBuildings", EObjectType::MapBuilding },
        { "MapGoods", EObjectType::MapGood },
        { "MapObjects", EObjectType::MapObject },
        { "Maps", EObjectType::Map },
        { "MechanoidQuests", EObjectType::MechanoidQuest },
        { "Mechanoids", EObjectType::Mechanoid },
        { "ModificationClans", EObjectType::ModificationClan },
        { "ModificationMaps", EObjectType::ModificationMap },
        { "ModificationMechanoids", EObjectType::ModificationMechanoid },
        { "Modifications", EObjectType::Modification },
        { "Modificators", EObjectType::Modificator },
        { "Objects", EObjectType::Object },
        { "Players", EObjectType::Player },
        { "Projectiles", EObjectType::Projectile },
        { "QuestRewardEquipments", EObjectType::QuestRewardEquipment },
        { "QuestRewardGliders", EObjectType::QuestRewardGlider },
        { "QuestRewardGoods", EObjectType::QuestRewardGood },
        { "QuestRewardModificators", EObjectType::QuestRewardModificator },
        { "QuestRewardProjectiles", EObjectType::QuestRewardProjectile },
        { "QuestRewardReputations", EObjectType::QuestRewardReputation },
        { "QuestRewardWeapons", EObjectType::QuestRewardWeapon },
        { "QuestRewards", EObjectType::QuestReward },
        { "Quests", EObjectType::Quest },
        { "ScriptVariables", EObjectType::ScriptVariable },
        { "Settings", EObjectType::Setting },
        { "Strings", EObjectType::String },
        { "Tables", EObjectType::Table },
        { "Weapons", EObjectType::Weapon },
        { "AnyTable", EObjectType::Any },
    };
    return types[table];
};

std::string getTableNameByType(EObjectType type)
{
    static std::map<EObjectType, std::string> tables =
    {
        { EObjectType::Building, "Buildings" },
        { EObjectType::ClanMechanoid, "ClanMechanoids" },
        { EObjectType::ClanReputation, "ClanReputations" },
        { EObjectType::Clan, "Clans" },
        { EObjectType::ConfigurationEquipment, "ConfigurationEquipments" },
        { EObjectType::ConfigurationGood, "ConfigurationGoods" },
        { EObjectType::ConfigurationProjectile, "ConfigurationProjectiles" },
        { EObjectType::ConfigurationWeapon, "ConfigurationWeapons" },
        { EObjectType::Configuration, "Configurations" },
        { EObjectType::Equipment, "Equipments" },
        { EObjectType::Glider, "Gliders" },
        { EObjectType::Good, "Goods" },
        { EObjectType::GroupMechanoid, "GroupMechanoids" },
        { EObjectType::Group, "Groups" },
        { EObjectType::MapBuildingEquipment, "MapBuildingEquipments" },
        { EObjectType::MapBuildingGlider, "MapBuildingGliders" },
        { EObjectType::MapBuildingGood, "MapBuildingGoods" },
        { EObjectType::MapBuildingModificator, "MapBuildingModificators" },
        { EObjectType::MapBuildingProjectile, "MapBuildingProjectiles" },
        { EObjectType::MapBuildingWeapon, "MapBuildingWeapons" },
        { EObjectType::MapBuilding, "MapBuildings" },
        { EObjectType::MapGood, "MapGoods" },
        { EObjectType::MapObject, "MapObjects" },
        { EObjectType::Map, "Maps" },
        { EObjectType::MechanoidQuest, "MechanoidQuests" },
        { EObjectType::Mechanoid, "Mechanoids" },
        { EObjectType::ModificationClan, "ModificationClans" },
        { EObjectType::ModificationMap, "ModificationMaps" },
        { EObjectType::ModificationMechanoid, "ModificationMechanoids" },
        { EObjectType::Modification, "Modifications" },
        { EObjectType::Modificator, "Modificators" },
        { EObjectType::Object, "Objects" },
        { EObjectType::Player, "Players" },
        { EObjectType::Projectile, "Projectiles" },
        { EObjectType::QuestRewardEquipment, "QuestRewardEquipments" },
        { EObjectType::QuestRewardGlider, "QuestRewardGliders" },
        { EObjectType::QuestRewardGood, "QuestRewardGoods" },
        { EObjectType::QuestRewardModificator, "QuestRewardModificators" },
        { EObjectType::QuestRewardProjectile, "QuestRewardProjectiles" },
        { EObjectType::QuestRewardReputation, "QuestRewardReputations" },
        { EObjectType::QuestRewardWeapon, "QuestRewardWeapons" },
        { EObjectType::QuestReward, "QuestRewards" },
        { EObjectType::Quest, "Quests" },
        { EObjectType::ScriptVariable, "ScriptVariables" },
        { EObjectType::Setting, "Settings" },
        { EObjectType::String, "Strings" },
        { EObjectType::Table, "Tables" },
        { EObjectType::Weapon, "Weapons" },
        { EObjectType::Any, "AnyTable" },
    };
    return tables[type];
};

#ifdef USE_QT
typedef std::map<std::string, QString> FieldNames;

FieldNames fieldNames;

void retranslateFieldNames()
{
    FieldNames names =
    {
        { "T", QCoreApplication::translate("DB", "T") },
        { "Volatile", QCoreApplication::translate("DB", "Volatile") },
        { "angle", QCoreApplication::translate("DB", "angle") },
        { "armor", QCoreApplication::translate("DB", "armor") },
        { "author", QCoreApplication::translate("DB", "author") },
        { "bcolor", QCoreApplication::translate("DB", "bcolor") },
        { "bonusexp", QCoreApplication::translate("DB", "bonusexp") },
        { "bonusrepair", QCoreApplication::translate("DB", "bonusrepair") },
        { "bonustrade", QCoreApplication::translate("DB", "bonustrade") },
        { "building_id", QCoreApplication::translate("DB", "building_id") },
        { "bx", QCoreApplication::translate("DB", "bx") },
        { "by", QCoreApplication::translate("DB", "by") },
        { "careen", QCoreApplication::translate("DB", "careen") },
        { "clan_id", QCoreApplication::translate("DB", "clan_id") },
        { "clan_id2", QCoreApplication::translate("DB", "clan_id2") },
        { "color", QCoreApplication::translate("DB", "color") },
        { "comment", QCoreApplication::translate("DB", "comment") },
        { "configuration_id", QCoreApplication::translate("DB", "configuration_id") },
        { "cooperative_player_configuration_id", QCoreApplication::translate("DB", "cooperative_player_configuration_id") },
        { "damage", QCoreApplication::translate("DB", "damage") },
        { "damagetype", QCoreApplication::translate("DB", "damagetype") },
        { "date_created", QCoreApplication::translate("DB", "date_created") },
        { "date_modified", QCoreApplication::translate("DB", "date_modified") },
        { "delta_t", QCoreApplication::translate("DB", "delta_t") },
        { "description_id", QCoreApplication::translate("DB", "description_id") },
        { "detonation_delay", QCoreApplication::translate("DB", "detonation_delay") },
        { "directory", QCoreApplication::translate("DB", "directory") },
        { "distance_detonation", QCoreApplication::translate("DB", "distance_detonation") },
        { "durability", QCoreApplication::translate("DB", "durability") },
        { "en", QCoreApplication::translate("DB", "en") },
        { "equipment_id", QCoreApplication::translate("DB", "equipment_id") },
        { "firerate", QCoreApplication::translate("DB", "firerate") },
        { "fx", QCoreApplication::translate("DB", "fx") },
        { "fxmodeltime", QCoreApplication::translate("DB", "fxmodeltime") },
        { "fxtime", QCoreApplication::translate("DB", "fxtime") },
        { "gcolor", QCoreApplication::translate("DB", "gcolor") },
        { "generation", QCoreApplication::translate("DB", "generation") },
        { "glider_id", QCoreApplication::translate("DB", "glider_id") },
        { "good_id", QCoreApplication::translate("DB", "good_id") },
        { "group_id", QCoreApplication::translate("DB", "group_id") },
        { "h_max", QCoreApplication::translate("DB", "h_max") },
        { "h_min", QCoreApplication::translate("DB", "h_min") },
        { "helpness", QCoreApplication::translate("DB", "helpness") },
        { "id", QCoreApplication::translate("DB", "id") },
        { "inside_mul", QCoreApplication::translate("DB", "inside_mul") },
        { "inside_x", QCoreApplication::translate("DB", "inside_x") },
        { "inside_y", QCoreApplication::translate("DB", "inside_y") },
        { "inside_z", QCoreApplication::translate("DB", "inside_z") },
        { "interactive", QCoreApplication::translate("DB", "interactive") },
        { "k_param1", QCoreApplication::translate("DB", "k_param1") },
        { "k_param2", QCoreApplication::translate("DB", "k_param2") },
        { "k_price", QCoreApplication::translate("DB", "k_price") },
        { "kx", QCoreApplication::translate("DB", "kx") },
        { "ky", QCoreApplication::translate("DB", "ky") },
        { "life_time", QCoreApplication::translate("DB", "life_time") },
        { "manual", QCoreApplication::translate("DB", "manual") },
        { "mapBuilding_id", QCoreApplication::translate("DB", "mapBuilding_id") },
        { "map_id", QCoreApplication::translate("DB", "map_id") },
        { "mask", QCoreApplication::translate("DB", "mask") },
        { "maxdistance", QCoreApplication::translate("DB", "maxdistance") },
        { "maxweight", QCoreApplication::translate("DB", "maxweight") },
        { "mechanoid_id", QCoreApplication::translate("DB", "mechanoid_id") },
        { "member_name_id", QCoreApplication::translate("DB", "member_name_id") },
        { "modification_id", QCoreApplication::translate("DB", "modification_id") },
        { "modificator_id", QCoreApplication::translate("DB", "modificator_id") },
        { "money", QCoreApplication::translate("DB", "money") },
        { "name_id", QCoreApplication::translate("DB", "name_id") },
        { "noblivion", QCoreApplication::translate("DB", "noblivion") },
        { "notrade", QCoreApplication::translate("DB", "notrade") },
        { "numstate", QCoreApplication::translate("DB", "numstate") },
        { "object_id", QCoreApplication::translate("DB", "object_id") },
        { "pitch", QCoreApplication::translate("DB", "pitch") },
        { "player_id", QCoreApplication::translate("DB", "player_id") },
        { "player_mechanoid_id", QCoreApplication::translate("DB", "player_mechanoid_id") },
        { "playereffect", QCoreApplication::translate("DB", "playereffect") },
        { "power", QCoreApplication::translate("DB", "power") },
        { "price", QCoreApplication::translate("DB", "price") },
        { "probability", QCoreApplication::translate("DB", "probability") },
        { "projectile_id", QCoreApplication::translate("DB", "projectile_id") },
        { "quantity", QCoreApplication::translate("DB", "quantity") },
        { "questReward_id", QCoreApplication::translate("DB", "questReward_id") },
        { "quest_id", QCoreApplication::translate("DB", "quest_id") },
        { "rating", QCoreApplication::translate("DB", "rating") },
        { "rating_courier", QCoreApplication::translate("DB", "rating_courier") },
        { "rating_fight", QCoreApplication::translate("DB", "rating_fight") },
        { "rating_trade", QCoreApplication::translate("DB", "rating_trade") },
        { "rcolor", QCoreApplication::translate("DB", "rcolor") },
        { "reconstruction", QCoreApplication::translate("DB", "reconstruction") },
        { "reputation", QCoreApplication::translate("DB", "reputation") },
        { "resfront", QCoreApplication::translate("DB", "resfront") },
        { "resource", QCoreApplication::translate("DB", "resource") },
        { "resource_drop", QCoreApplication::translate("DB", "resource_drop") },
        { "resside", QCoreApplication::translate("DB", "resside") },
        { "restop", QCoreApplication::translate("DB", "restop") },
        { "restore", QCoreApplication::translate("DB", "restore") },
        { "roll", QCoreApplication::translate("DB", "roll") },
        { "rotate", QCoreApplication::translate("DB", "rotate") },
        { "rotatespeed", QCoreApplication::translate("DB", "rotatespeed") },
        { "ru", QCoreApplication::translate("DB", "ru") },
        { "scale", QCoreApplication::translate("DB", "scale") },
        { "scale_x", QCoreApplication::translate("DB", "scale_x") },
        { "scale_y", QCoreApplication::translate("DB", "scale_y") },
        { "scale_z", QCoreApplication::translate("DB", "scale_z") },
        { "script_language", QCoreApplication::translate("DB", "script_language") },
        { "script_main", QCoreApplication::translate("DB", "script_main") },
        { "shootscale", QCoreApplication::translate("DB", "shootscale") },
        { "shoottype", QCoreApplication::translate("DB", "shoottype") },
        { "spare", QCoreApplication::translate("DB", "spare") },
        { "special", QCoreApplication::translate("DB", "special") },
        { "speed", QCoreApplication::translate("DB", "speed") },
        { "stabfront", QCoreApplication::translate("DB", "stabfront") },
        { "stabside", QCoreApplication::translate("DB", "stabside") },
        { "standard", QCoreApplication::translate("DB", "standard") },
        { "state", QCoreApplication::translate("DB", "state") },
        { "strength", QCoreApplication::translate("DB", "strength") },
        { "subtype", QCoreApplication::translate("DB", "subtype") },
        { "table_id", QCoreApplication::translate("DB", "table_id") },
        { "text_id", QCoreApplication::translate("DB", "text_id") },
        { "tfire", QCoreApplication::translate("DB", "tfire") },
        { "time", QCoreApplication::translate("DB", "time") },
        { "title_id", QCoreApplication::translate("DB", "title_id") },
        { "turbulence", QCoreApplication::translate("DB", "turbulence") },
        { "type", QCoreApplication::translate("DB", "type") },
        { "typearms", QCoreApplication::translate("DB", "typearms") },
        { "unicum_id", QCoreApplication::translate("DB", "unicum_id") },
        { "value1", QCoreApplication::translate("DB", "value1") },
        { "value2", QCoreApplication::translate("DB", "value2") },
        { "value3", QCoreApplication::translate("DB", "value3") },
        { "value_blob", QCoreApplication::translate("DB", "value_blob") },
        { "value_float", QCoreApplication::translate("DB", "value_float") },
        { "value_int", QCoreApplication::translate("DB", "value_int") },
        { "value_text", QCoreApplication::translate("DB", "value_text") },
        { "variable", QCoreApplication::translate("DB", "variable") },
        { "version", QCoreApplication::translate("DB", "version") },
        { "vtype", QCoreApplication::translate("DB", "vtype") },
        { "weapon_id", QCoreApplication::translate("DB", "weapon_id") },
        { "weight", QCoreApplication::translate("DB", "weight") },
        { "x", QCoreApplication::translate("DB", "x") },
        { "xstate", QCoreApplication::translate("DB", "xstate") },
        { "y", QCoreApplication::translate("DB", "y") },
        { "yaw", QCoreApplication::translate("DB", "yaw") },
        { "z", QCoreApplication::translate("DB", "z") },
    };
    fieldNames = names;
};

QString getFieldName(const std::string &name)
{
    return fieldNames[name];
};

typedef std::map<std::string, QString> TableNames;

TableNames tableNames;

void retranslateTableNames()
{
    TableNames names =
    {
        { "Buildings", QCoreApplication::translate("DB", "Buildings") },
        { "ClanMechanoids", QCoreApplication::translate("DB", "Clan Mechanoids") },
        { "ClanReputations", QCoreApplication::translate("DB", "Clan Reputations") },
        { "Clans", QCoreApplication::translate("DB", "Clans") },
        { "ConfigurationEquipments", QCoreApplication::translate("DB", "Configuration Equipments") },
        { "ConfigurationGoods", QCoreApplication::translate("DB", "Configuration Goods") },
        { "ConfigurationProjectiles", QCoreApplication::translate("DB", "Configuration Projectiles") },
        { "ConfigurationWeapons", QCoreApplication::translate("DB", "Configuration Weapons") },
        { "Configurations", QCoreApplication::translate("DB", "Configurations") },
        { "Equipments", QCoreApplication::translate("DB", "Equipments") },
        { "Gliders", QCoreApplication::translate("DB", "Gliders") },
        { "Goods", QCoreApplication::translate("DB", "Goods") },
        { "GroupMechanoids", QCoreApplication::translate("DB", "Group Mechanoids") },
        { "Groups", QCoreApplication::translate("DB", "Groups") },
        { "MapBuildingEquipments", QCoreApplication::translate("DB", "Map Building Equipments") },
        { "MapBuildingGliders", QCoreApplication::translate("DB", "Map Building Gliders") },
        { "MapBuildingGoods", QCoreApplication::translate("DB", "Map Building Goods") },
        { "MapBuildingModificators", QCoreApplication::translate("DB", "Map Building Modificators") },
        { "MapBuildingProjectiles", QCoreApplication::translate("DB", "Map Building Projectiles") },
        { "MapBuildingWeapons", QCoreApplication::translate("DB", "Map Building Weapons") },
        { "MapBuildings", QCoreApplication::translate("DB", "Map Buildings") },
        { "MapGoods", QCoreApplication::translate("DB", "Map Goods") },
        { "MapObjects", QCoreApplication::translate("DB", "Map Objects") },
        { "Maps", QCoreApplication::translate("DB", "Maps") },
        { "MechanoidQuests", QCoreApplication::translate("DB", "Mechanoid Quests") },
        { "Mechanoids", QCoreApplication::translate("DB", "Mechanoids") },
        { "ModificationClans", QCoreApplication::translate("DB", "Modification Clans") },
        { "ModificationMaps", QCoreApplication::translate("DB", "Modification Maps") },
        { "ModificationMechanoids", QCoreApplication::translate("DB", "Modification Mechanoids") },
        { "Modifications", QCoreApplication::translate("DB", "Modifications") },
        { "Modificators", QCoreApplication::translate("DB", "Modificators") },
        { "Objects", QCoreApplication::translate("DB", "Objects") },
        { "Players", QCoreApplication::translate("DB", "Players") },
        { "Projectiles", QCoreApplication::translate("DB", "Projectiles") },
        { "QuestRewardEquipments", QCoreApplication::translate("DB", "Quest Reward Equipments") },
        { "QuestRewardGliders", QCoreApplication::translate("DB", "Quest Reward Gliders") },
        { "QuestRewardGoods", QCoreApplication::translate("DB", "Quest Reward Goods") },
        { "QuestRewardModificators", QCoreApplication::translate("DB", "Quest Reward Modificators") },
        { "QuestRewardProjectiles", QCoreApplication::translate("DB", "Quest Reward Projectiles") },
        { "QuestRewardReputations", QCoreApplication::translate("DB", "Quest Reward Reputations") },
        { "QuestRewardWeapons", QCoreApplication::translate("DB", "Quest Reward Weapons") },
        { "QuestRewards", QCoreApplication::translate("DB", "Quest Rewards") },
        { "Quests", QCoreApplication::translate("DB", "Quests") },
        { "ScriptVariables", QCoreApplication::translate("DB", "Script Variables") },
        { "Settings", QCoreApplication::translate("DB", "Settings") },
        { "Strings", QCoreApplication::translate("DB", "Strings") },
        { "Tables", QCoreApplication::translate("DB", "Tables") },
        { "Weapons", QCoreApplication::translate("DB", "Weapons") },
        { "AnyTable", QCoreApplication::translate("DB", "Any Table") },
    };
    tableNames = names;
};

QString getTableName(const std::string &name)
{
    return tableNames[name];
};
#endif
