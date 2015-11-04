/* DO NOT EDIT! This is an autogenerated file. */

#pragma once

#include "../Table.h"

class Storage
{
public:
    CTable<Building> buildings;
    CTable<Clan> clans;
    CTable<ClanReputation> clanReputations;
    CTable<Configuration> configurations;
    CTable<ConfigurationEquipment> configurationEquipments;
    CTable<ConfigurationGood> configurationGoods;
    CTable<ConfigurationProjectile> configurationProjectiles;
    CTable<ConfigurationWeapon> configurationWeapons;
    CTable<Equipment> equipments;
    CTable<Glider> gliders;
    CTable<Good> goods;
    CTable<Group> groups;
    CTable<GroupMechanoid> groupMechanoids;
    CTable<Map> maps;
    CTable<MapBuilding> mapBuildings;
    CTable<MapBuildingEquipment> mapBuildingEquipments;
    CTable<MapBuildingGlider> mapBuildingGliders;
    CTable<MapBuildingGood> mapBuildingGoods;
    CTable<MapBuildingModificator> mapBuildingModificators;
    CTable<MapBuildingProjectile> mapBuildingProjectiles;
    CTable<MapBuildingWeapon> mapBuildingWeapons;
    CTable<MapGood> mapGoods;
    CTable<MapObject> mapObjects;
    CTable<Mechanoid> mechanoids;
    CTable<MechanoidQuest> mechanoidQuests;
    CTable<Modification> modifications;
    CTable<ModificationMap> modificationMaps;
    CTable<Modificator> modificators;
    CTable<Object> objects;
    CTable<Player> players;
    CTable<Projectile> projectiles;
    CTable<Quest> quests;
    CTable<QuestReward> questRewards;
    CTable<QuestRewardEquipment> questRewardEquipments;
    CTable<QuestRewardGlider> questRewardGliders;
    CTable<QuestRewardGood> questRewardGoods;
    CTable<QuestRewardModificator> questRewardModificators;
    CTable<QuestRewardProjectile> questRewardProjectiles;
    CTable<QuestRewardReputation> questRewardReputations;
    CTable<QuestRewardWeapon> questRewardWeapons;
    CTable<ScriptVariable> scriptVariables;
    CTable<Setting> settings;
    CTable<String> strings;
    CTable<Table> tables;
    CTable<Weapon> weapons;

public:
    Storage();
    virtual ~Storage();

    virtual void create() const = 0;
    virtual void clear() = 0;
    virtual void load(ProgressCallback callback = ProgressCallback()) = 0;
    virtual void save(ProgressCallback callback = ProgressCallback()) const = 0;

    virtual Ptr<TreeItem> printTree() const = 0;
    virtual Ptr<TreeItem> addRecord(TreeItem *item) = 0;
    virtual void deleteRecord(TreeItem *item) = 0;

    virtual OrderedObjectMap getOrderedMap(EObjectType type, std::function<bool(IObjectBase *)> f = std::function<bool(IObjectBase *)>()) const = 0;

    virtual Ptr<Building> addBuilding() = 0;
    virtual void deleteBuilding(IObjectBase *object) = 0;
    virtual Ptr<Clan> addClan() = 0;
    virtual void deleteClan(IObjectBase *object) = 0;
    virtual Ptr<ClanReputation> addClanReputation(IObjectBase *parent = nullptr) = 0;
    virtual void deleteClanReputation(IObjectBase *object) = 0;
    virtual Ptr<Configuration> addConfiguration() = 0;
    virtual void deleteConfiguration(IObjectBase *object) = 0;
    virtual Ptr<ConfigurationEquipment> addConfigurationEquipment(IObjectBase *parent = nullptr) = 0;
    virtual void deleteConfigurationEquipment(IObjectBase *object) = 0;
    virtual Ptr<ConfigurationGood> addConfigurationGood(IObjectBase *parent = nullptr) = 0;
    virtual void deleteConfigurationGood(IObjectBase *object) = 0;
    virtual Ptr<ConfigurationProjectile> addConfigurationProjectile(IObjectBase *parent = nullptr) = 0;
    virtual void deleteConfigurationProjectile(IObjectBase *object) = 0;
    virtual Ptr<ConfigurationWeapon> addConfigurationWeapon(IObjectBase *parent = nullptr) = 0;
    virtual void deleteConfigurationWeapon(IObjectBase *object) = 0;
    virtual Ptr<Equipment> addEquipment() = 0;
    virtual void deleteEquipment(IObjectBase *object) = 0;
    virtual Ptr<Glider> addGlider() = 0;
    virtual void deleteGlider(IObjectBase *object) = 0;
    virtual Ptr<Good> addGood() = 0;
    virtual void deleteGood(IObjectBase *object) = 0;
    virtual Ptr<Group> addGroup() = 0;
    virtual void deleteGroup(IObjectBase *object) = 0;
    virtual Ptr<GroupMechanoid> addGroupMechanoid(IObjectBase *parent = nullptr) = 0;
    virtual void deleteGroupMechanoid(IObjectBase *object) = 0;
    virtual Ptr<Map> addMap() = 0;
    virtual void deleteMap(IObjectBase *object) = 0;
    virtual Ptr<MapBuilding> addMapBuilding(IObjectBase *parent = nullptr) = 0;
    virtual void deleteMapBuilding(IObjectBase *object) = 0;
    virtual Ptr<MapBuildingEquipment> addMapBuildingEquipment(IObjectBase *parent = nullptr) = 0;
    virtual void deleteMapBuildingEquipment(IObjectBase *object) = 0;
    virtual Ptr<MapBuildingGlider> addMapBuildingGlider(IObjectBase *parent = nullptr) = 0;
    virtual void deleteMapBuildingGlider(IObjectBase *object) = 0;
    virtual Ptr<MapBuildingGood> addMapBuildingGood(IObjectBase *parent = nullptr) = 0;
    virtual void deleteMapBuildingGood(IObjectBase *object) = 0;
    virtual Ptr<MapBuildingModificator> addMapBuildingModificator(IObjectBase *parent = nullptr) = 0;
    virtual void deleteMapBuildingModificator(IObjectBase *object) = 0;
    virtual Ptr<MapBuildingProjectile> addMapBuildingProjectile(IObjectBase *parent = nullptr) = 0;
    virtual void deleteMapBuildingProjectile(IObjectBase *object) = 0;
    virtual Ptr<MapBuildingWeapon> addMapBuildingWeapon(IObjectBase *parent = nullptr) = 0;
    virtual void deleteMapBuildingWeapon(IObjectBase *object) = 0;
    virtual Ptr<MapGood> addMapGood(IObjectBase *parent = nullptr) = 0;
    virtual void deleteMapGood(IObjectBase *object) = 0;
    virtual Ptr<MapObject> addMapObject(IObjectBase *parent = nullptr) = 0;
    virtual void deleteMapObject(IObjectBase *object) = 0;
    virtual Ptr<Mechanoid> addMechanoid() = 0;
    virtual void deleteMechanoid(IObjectBase *object) = 0;
    virtual Ptr<MechanoidQuest> addMechanoidQuest(IObjectBase *parent = nullptr) = 0;
    virtual void deleteMechanoidQuest(IObjectBase *object) = 0;
    virtual Ptr<Modification> addModification() = 0;
    virtual void deleteModification(IObjectBase *object) = 0;
    virtual Ptr<ModificationMap> addModificationMap(IObjectBase *parent = nullptr) = 0;
    virtual void deleteModificationMap(IObjectBase *object) = 0;
    virtual Ptr<Modificator> addModificator() = 0;
    virtual void deleteModificator(IObjectBase *object) = 0;
    virtual Ptr<Object> addObject() = 0;
    virtual void deleteObject(IObjectBase *object) = 0;
    virtual Ptr<Player> addPlayer() = 0;
    virtual void deletePlayer(IObjectBase *object) = 0;
    virtual Ptr<Projectile> addProjectile() = 0;
    virtual void deleteProjectile(IObjectBase *object) = 0;
    virtual Ptr<Quest> addQuest() = 0;
    virtual void deleteQuest(IObjectBase *object) = 0;
    virtual Ptr<QuestReward> addQuestReward(IObjectBase *parent = nullptr) = 0;
    virtual void deleteQuestReward(IObjectBase *object) = 0;
    virtual Ptr<QuestRewardEquipment> addQuestRewardEquipment(IObjectBase *parent = nullptr) = 0;
    virtual void deleteQuestRewardEquipment(IObjectBase *object) = 0;
    virtual Ptr<QuestRewardGlider> addQuestRewardGlider(IObjectBase *parent = nullptr) = 0;
    virtual void deleteQuestRewardGlider(IObjectBase *object) = 0;
    virtual Ptr<QuestRewardGood> addQuestRewardGood(IObjectBase *parent = nullptr) = 0;
    virtual void deleteQuestRewardGood(IObjectBase *object) = 0;
    virtual Ptr<QuestRewardModificator> addQuestRewardModificator(IObjectBase *parent = nullptr) = 0;
    virtual void deleteQuestRewardModificator(IObjectBase *object) = 0;
    virtual Ptr<QuestRewardProjectile> addQuestRewardProjectile(IObjectBase *parent = nullptr) = 0;
    virtual void deleteQuestRewardProjectile(IObjectBase *object) = 0;
    virtual Ptr<QuestRewardReputation> addQuestRewardReputation(IObjectBase *parent = nullptr) = 0;
    virtual void deleteQuestRewardReputation(IObjectBase *object) = 0;
    virtual Ptr<QuestRewardWeapon> addQuestRewardWeapon(IObjectBase *parent = nullptr) = 0;
    virtual void deleteQuestRewardWeapon(IObjectBase *object) = 0;
    virtual Ptr<ScriptVariable> addScriptVariable() = 0;
    virtual void deleteScriptVariable(IObjectBase *object) = 0;
    virtual Ptr<Setting> addSetting() = 0;
    virtual void deleteSetting(IObjectBase *object) = 0;
    virtual Ptr<String> addString() = 0;
    virtual void deleteString(IObjectBase *object) = 0;
    virtual Ptr<Table> addTable() = 0;
    virtual void deleteTable(IObjectBase *object) = 0;
    virtual Ptr<Weapon> addWeapon() = 0;
    virtual void deleteWeapon(IObjectBase *object) = 0;

    virtual Ptr<IObjectBase> addRecord(IObjectBase *parent = nullptr) = 0;
    virtual void deleteRecord(IObjectBase *data) = 0;
};
