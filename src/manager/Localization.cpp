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

#include <Polygon4/DataManager/Localization.h>

#include "Logger.h"
DECLARE_STATIC_LOGGER(logger, "tr");

#define TS(key, ...) { ENUM_NAME(key), { #key, { __VA_ARGS__ } } },

namespace polygon4
{

LocalizationType getCurrentLocalizationId(LocalizationType type)
{
    static LocalizationType loc = LocalizationType::ru;
    if (type != LocalizationType::max)
        loc = type;
    return loc;
}

Translator translator;
extern const Translator::translator_type translator_data;

}

namespace polygon4
{

LocalizedString::LocalizedString()
{
}

LocalizedString::LocalizedString(const std::initializer_list<string_type> &list)
{
    auto max = size();
    if (list.size() < max)
        max = list.size();
    auto i1 = begin();
    auto i2 = list.begin();
    for (size_t i = 0; i < max; i++)
        *i1++ = *i2++;
}

LocalizedString &LocalizedString::operator=(const string_type &s)
{
    auto type = getCurrentLocalizationId();
    (*this)[type] = s;
    return *this;
}

size_t LocalizedString::size() const
{
    return end() - begin();
}

LocalizedString::iterator LocalizedString::begin()
{
    return &__Begin + 1;
}

LocalizedString::iterator LocalizedString::end()
{
    return &__End;
}

LocalizedString::const_iterator LocalizedString::begin() const
{
    return &__Begin + 1;
}

LocalizedString::const_iterator LocalizedString::end() const
{
    return &__End;
}

LocalizedString::string_type &LocalizedString::operator[](LocalizationType type)
{
    return *(begin() + static_cast<int>(type));
}
const LocalizedString::string_type &LocalizedString::operator[](LocalizationType type) const
{
    return *(begin() + static_cast<int>(type));
}

LocalizedString::operator LocalizedString::string_type() const
{
    return str();
}

LocalizedString::string_type LocalizedString::str(LocalizationType type) const
{
    if (type == LocalizationType::max)
        type = getCurrentLocalizationId();
    auto &s = (*this)[type];
    if (!s.empty())
        return s;
    if (type == LocalizationType::en)
        return s;
    return firstNonEmpty();
}

LocalizedString::string_type LocalizedString::firstNonEmpty() const
{
    for (auto &s : *this)
        if (!s.empty())
            return s;
    return LocalizedString::string_type();
}

void initTranslator()
{
    LOG_TRACE(logger, "Initializing translator");
    translator.init(translator_data);
}

LocalizationTable<DataType> translator_DataType_data =
{
#define ENUM_NAME(x) DataType::x
    TS(Integer, "Целое", "Integer")
    TS(Real, "Вещественное", "Real")
    TS(Text, "Текст", "Text")
    TS(Bool, "Логическое", "Boolean")
    TS(Blob, "Двоичные данные", "Blob")
    TS(Enum, "Перечисление", "Enumeration")
    TS(Complex, "Объект", "Object")
    TS(None, "Неизвестный тип", "Unknown type")
#undef ENUM_NAME
};

String tr(DataType type)
{
    auto i = translator_DataType_data.find(type);
    if (i == translator_DataType_data.end())
        type = DataType::None;
    return translator_DataType_data[type].str();
}

String tr(const std::string &key, const std::string &context)
{
    return translator.tr(key, context);
}

const Translator::translator_type translator_data =
{
    { // pair < context, dict >
        "", // context
        { // dict start
            // table names (singular)
            { "None", { "Отсутствует", "None" } },
            { "Building", { "Здание",  "Building" } },
            { "Clan", { "Клан",  "Clan" } },
            { "ClanReputation", { "Репутация клана",  "Clan Reputation" } },
            { "Configuration", { "Конфигурация",  "Configuration" } },
            { "ConfigurationEquipment", { "Оборудование конфигурации",  "Configuration Equipment" } },
            { "ConfigurationGood", { "Товар конфигурации",  "Configuration Good" } },
            { "ConfigurationProjectile", { "Снаряд конфигурации",  "Configuration Projectile" } },
            { "ConfigurationWeapon", { "Оружие конфигурации",  "Configuration Weapon" } },
            { "Equipment", { "Оборудование",  "Equipment" } },
            { "Glider", { "Глайдер",  "Glider" } },
            { "Good", { "Товар",  "Good" } },
            { "Group", { "Группа",  "Group" } },
            { "GroupMechanoid", { "Механоид группы",  "Group Mechanoid" } },
            { "Map", { "Карта",  "Map" } },
            { "MapBuilding", { "Здание карты",  "Map Building" } },
            { "MapBuildingEquipment", { "Оборудование в здании",  "Map Building Equipment" } },
            { "MapBuildingGlider", { "Глайдер в здании",  "Map Building Glider" } },
            { "MapBuildingGood", { "Товар в здании",  "Map Building Good" } },
            { "MapBuildingModificator", { "Модификатор в здании",  "Map Building Modificator" } },
            { "MapBuildingProjectile", { "Снаряд в здании",  "Map Building Projectile" } },
            { "MapBuildingWeapon", { "Оружие в здании",  "Map Building Weapon" } },
            { "MapGood", { "Товар на карте",  "Map Good" } },
            { "MapObject", { "Объект на карте",  "Map Object" } },
            { "Mechanoid", { "Механоид",  "Mechanoid" } },
            { "MechanoidQuest", { "Задание механоида",  "Mechanoid Quest" } },
            { "Modification", { "Мод",  "Modification" } },
            { "ModificationMap", { "Карта мода",  "Modification Map" } },
            { "Modificator", { "Модификатор",  "Modificator" } },
            { "Object", { "Объект",  "Object" } },
            { "Player", { "Игрок",  "Player" } },
            { "Projectile", { "Снаряд",  "Projectile" } },
            { "Quest", { "Задание",  "Quest" } },
            { "QuestReward", { "Награда за задание",  "Quest Reward" } },
            { "Reward",{ "Награда за задание",  "Quest Reward" } },
            { "QuestRewardEquipment", { "Оборудование за задание",  "Quest Reward Equipment" } },
            { "QuestRewardGlider", { "Глайдер за задание",  "Quest Reward Glider" } },
            { "QuestRewardGood", { "Товар за задание",  "Quest Reward Good" } },
            { "QuestRewardModificator", { "Модификатор за задание",  "Quest Reward Modificator" } },
            { "QuestRewardProjectile", { "Снаряд за задание",  "Quest Reward Projectile" } },
            { "QuestRewardReputation", { "Репутация за задание",  "Quest Reward Reputation" } },
            { "QuestRewardWeapon", { "Оружие за задание",  "Quest Reward Weapon" } },
            { "ScriptVariable", { "Переменная скрипта",  "Script Variable" } },
            { "Setting", { "Настройка",  "Setting" } },
            { "String", { "Строка",  "String" } },
            { "Weapon", { "Оружие",  "Weapon" } },
            { "Any", { "Любой объект",  "Any Object" } },
            { "Anys",{ "Любой объект",  "Any Object" } },



            // table names (plural, words)
            { "Buildings",{ "Здания",  "Buildings" } },
            { "Clans",{ "Кланы",  "Clans" } },
            { "Clan Reputations",{ "Отношения",  "Clan Reputations" } },
            { "Configurations",{ "Конфигурации",  "Configurations" } },
            { "Configuration Equipments",{ "Оборудование конфигурации",  "Configuration Equipments" } },
            { "Configuration Goods",{ "Товары конфигурации",  "Configuration Goods" } },
            { "Configuration Projectiles",{ "Снаряды конфигурации",  "Configuration Projectiles" } },
            { "Configuration Weapons",{ "Оружие конфигурации",  "Configuration Weapons" } },
            { "Equipments",{ "Оборудование",  "Equipments" } },
            { "Gliders",{ "Глайдеры",  "Gliders" } },
            { "Goods",{ "Товары",  "Goods" } },
            { "Groups",{ "Группы",  "Groups" } },
            { "Group Mechanoids",{ "Механоиды группы",  "Group Mechanoids" } },
            { "Inline variables",{ "Переменные",  "Inline variables" } },
            { "Maps",{ "Карты",  "Maps" } },
            { "Map Buildings",{ "Здания карты",  "Map Buildings" } },
            { "Map Building Equipments",{ "Оборудование в здании",  "Map Building Equipments" } },
            { "Map Building Gliders",{ "Глайдеры в здании",  "Map Building Gliders" } },
            { "Map Building Goods",{ "Товары в здании",  "Map Building Goods" } },
            { "Map Building Modificators",{ "Модификаторы в здании",  "Map Building Modificators" } },
            { "Map Building Projectiles",{ "Снаряды в здании",  "Map Building Projectiles" } },
            { "Map Building Weapons",{ "Оружие в здании",  "Map Building Weapons" } },
            { "Map Goods",{ "Товары на карте",  "Map Goods" } },
            { "Map Objects",{ "Объекты на карте",  "Map Objects" } },
            { "Mechanoids",{ "Механоиды",  "Mechanoids" } },
            { "Mechanoid Quests",{ "Задания механоида",  "Mechanoid Quests" } },
            { "Modifications",{ "Моды",  "Modifications" } },
            { "Modification Maps",{ "Карты мода",  "Modification Maps" } },
            { "Modificators",{ "Модификаторы",  "Modificators" } },
            { "Objects",{ "Объекты",  "Objects" } },
            { "Players",{ "Игроки",  "Players" } },
            { "Projectiles",{ "Снаряды",  "Projectiles" } },
            { "Quests",{ "Задания",  "Quests" } },
            { "Quest Rewards",{ "Награды за задание",  "Quest Rewards" } },
            { "Rewards",{ "Награды",  "Rewards" } },
            { "Quest Reward Equipments",{ "Оборудование за задание",  "Quest Reward Equipments" } },
            { "Quest Reward Gliders",{ "Глайдеры за задание",  "Quest Reward Gliders" } },
            { "Quest Reward Goods",{ "Товары за задание",  "Quest Reward Goods" } },
            { "Quest Reward Modificators",{ "Модификаторы за задание",  "Quest Reward Modificators" } },
            { "Quest Reward Projectiles",{ "Снаряды за задание",  "Quest Reward Projectiles" } },
            { "Quest Reward Reputations",{ "Репутация за задание",  "Quest Reward Reputations" } },
            { "Quest Reward Weapons",{ "Оружие за задание",  "Quest Reward Weapons" } },
            { "Script Variables",{ "Переменные скрипта",  "Script Variables" } },
            { "Settings",{ "Настройки",  "Settings" } },
            { "Strings",{ "Строки",  "Strings" } },
            { "Weapons",{ "Оружие",  "Weapons" } },



            // table names (plural, single word)
            { "Buildings",{ "Здания",  "Buildings" } },
            { "Clans",{ "Кланы",  "Clans" } },
            { "ClanReputations",{ "Отношения",  "Clan Reputations" } },
            { "Configurations",{ "Конфигурации",  "Configurations" } },
            { "ConfigurationEquipments",{ "Оборудование конфигурации",  "Configuration Equipments" } },
            { "ConfigurationGoods",{ "Товары конфигурации",  "Configuration Goods" } },
            { "ConfigurationProjectiles",{ "Снаряды конфигурации",  "Configuration Projectiles" } },
            { "ConfigurationWeapons",{ "Оружие конфигурации",  "Configuration Weapons" } },
            { "Equipments",{ "Оборудование",  "Equipments" } },
            { "Gliders",{ "Глайдеры",  "Gliders" } },
            { "Goods",{ "Товары",  "Goods" } },
            { "Groups",{ "Группы",  "Groups" } },
            { "GroupMechanoids",{ "Механоиды группы",  "Group Mechanoids" } },
            { "Maps",{ "Карты",  "Maps" } },
            { "MapBuildings",{ "Здания карты",  "Map Buildings" } },
            { "MapBuildingEquipments",{ "Оборудование в здании",  "Map Building Equipments" } },
            { "MapBuildingGliders",{ "Глайдеры в здании",  "Map Building Gliders" } },
            { "MapBuildingGoods",{ "Товары в здании",  "Map Building Goods" } },
            { "MapBuildingModificators",{ "Модификаторы в здании",  "Map Building Modificators" } },
            { "MapBuildingProjectiles",{ "Снаряды в здании",  "Map Building Projectiles" } },
            { "MapBuildingWeapons",{ "Оружие в здании",  "Map Building Weapons" } },
            { "MapGoods",{ "Товары на карте",  "Map Goods" } },
            { "MapObjects",{ "Объекты на карте",  "Map Objects" } },
            { "Mechanoids",{ "Механоиды",  "Mechanoids" } },
            { "MechanoidQuests",{ "Задания механоида",  "Mechanoid Quests" } },
            { "Messages",{ "Сообщения",  "" } },
            { "Modifications",{ "Моды",  "Modifications" } },
            { "ModificationMaps",{ "Карты мода",  "Modification Maps" } },
            { "Modificators",{ "Модификаторы",  "Modificators" } },
            { "Objects",{ "Объекты",  "Objects" } },
            { "Players",{ "Игроки",  "Players" } },
            { "Projectiles",{ "Снаряды",  "Projectiles" } },
            { "Quests",{ "Задания",  "Quests" } },
            { "QuestRewards",{ "Награды за задание",  "Quest Rewards" } },
            { "QuestRewardEquipments",{ "Оборудование за задание",  "Quest Reward Equipments" } },
            { "QuestRewardGliders",{ "Глайдеры за задание",  "Quest Reward Gliders" } },
            { "QuestRewardGoods",{ "Товары за задание",  "Quest Reward Goods" } },
            { "QuestRewardModificators",{ "Модификаторы за задание",  "Quest Reward Modificators" } },
            { "QuestRewardProjectiles",{ "Снаряды за задание",  "Quest Reward Projectiles" } },
            { "QuestRewardReputations",{ "Репутация за задание",  "Quest Reward Reputations" } },
            { "QuestRewardWeapons",{ "Оружие за задание",  "Quest Reward Weapons" } },
            { "Reputations",{ "Отношения",  "Reputations" } },
            { "ScriptVariables",{ "Переменные скрипта",  "Script Variables" } },
            { "Settings",{ "Настройки",  "Settings" } },
            { "Strings",{ "Строки",  "Strings" } },
            { "Weapons",{ "Оружие",  "Weapons" } },



            // field names
            { "T", { "", "T"} },
            { "Volatile", { "", "Volatile"} },
            { "angle", { "Угол", "angle"} },
            { "armor", { "Броня", "armor"} },
            { "author", { "Автор", "author"} },
            { "bcolor", { "", "bcolor"} },
            { "blueprint",{ "Блюпринт", "Blueprint" } },
            { "bonusexp", { "Бонус к опыту", "bonusexp"} },
            { "bonusrepair", { "Бонус к починке", "bonusrepair"} },
            { "bonustrade", { "Бонус к торговле", "bonustrade"} },
            { "brief",{ "Краткое", "brief" } },
            { "building", { "Здание", "building"} },
            { "bx", { "", "bx"} },
            { "by", { "", "by"} },
            { "careen", { "", "careen"} },
            { "clan", { "Клан", "clan"} },
            { "clan1", { "", "clan1"} },
            { "clan2", { "", "clan2"} },
            { "color", { "Цвет", "color"} },
            { "comment", { "Комментарий", "comment"} },
            { "complete",{ "Выполнено", "complete" } },
            { "configuration", { "Конфигурация", "configuration"} },
            { "cooperative_player_configuration", { "Конфигурация сетевого игрока", "cooperative_player_configuration"} },
            { "damage", { "Урон", "damage"} },
            { "damagetype", { "Тип урона", "damagetype"} },
            { "date_created", { "Дата создания", "date_created"} },
            { "date_modified", { "Дата изменения", "date_modified"} },
            { "delta_t", { "", "delta_t"} },
            { "description", { "Описание", "description"} },
            { "detonation_delay", { "Задержка детонации", "detonation_delay"} },
            { "directory", { "Директория", "directory"} },
            { "distance_detonation", { "Дистанция детонации", "distance_detonation"} },
            { "durability", { "Прочность", "durability"} },
            { "en", { "English", "English"} },
            { "equipment", { "Оборудование", "equipment"} },
            { "fail",{ "Провалено", "fail" } },
            { "firerate", { "Частота стрельбы", "firerate"} },
            { "fx", { "", "fx"} },
            { "fxmodeltime", { "", "fxmodeltime"} },
            { "fxtime", { "", "fxtime"} },
            { "gcolor", { "", "gcolor"} },
            { "generation", { "Поколение", "generation"} },
            { "get",{ "Получено", "get" } },
            { "glider", { "Глайдер", "glider"} },
            { "good", { "Товар", "good"} },
            { "group", { "Группа", "group"} },
            { "h_max", { "", "h_max"} },
            { "h_min", { "", "h_min"} },
            { "helpness", { "Полезность", "helpness"} },
            { "id", { "Номер", "id"} },
            { "in_progress",{ "В процессе", "in_progress" } },
            { "inside_mul", { "", "inside_mul"} },
            { "inside_x", { "", "inside_x"} },
            { "inside_y", { "", "inside_y"} },
            { "inside_z", { "", "inside_z"} },
            { "interactive", { "Интерактивный", "interactive"} },
            { "k_param1", { "Коэффициент 1", "k_param1"} },
            { "k_param2", { "Коэффициент 2", "k_param2"} },
            { "k_price", { "Коэффициент цены", "k_price"} },
            { "kx", { "", "kx"} },
            { "ky", { "", "ky"} },
            { "life_time", { "Время жизни", "life_time"} },
            { "manual", { "Ручная активация", "manual"} },
            { "map", { "Карта", "map"} },
            { "map_building", { "Здание карты", "map_building"} },
            { "mask", { "Маска", "mask"} },
            { "maxdistance", { "Максимальное расстояние", "maxdistance"} },
            { "maxweight", { "Максимальная масса", "maxweight"} },
            { "mechanoid", { "Механоид", "mechanoid"} },
            { "member_name", { "Имя члена", "member_name"} },
            { "modification", { "Мод", "modification"} },
            { "modificator", { "Модификатор", "modificator"} },
            { "money", { "Деньги", "money"} },
            { "name", { "Имя", "name"} },
            { "name_ending",{ "Окончание имени", "name_ending" } },
            { "nickname",{ "Псевдоним", "" } },
            { "noblivion", { "", "noblivion"} },
            { "notrade", { "Не продаётся", "notrade"} },
            { "numstate", { "", "numstate"} },
            { "object", { "Объект", "object"} },
            { "other_strings",{ "Другие строки", "other_strings" } },
            { "pitch", { "", "pitch"} },
            { "player", { "Игрок", "player"} },
            { "player_mechanoid", { "Механоид игрока", "player_mechanoid"} },
            { "playereffect", { "", "playereffect"} },
            { "power", { "Мощность", "power"} },
            { "price", { "Цена", "price"} },
            { "probability", { "Вероятность", "probability"} },
            { "projectile", { "Снаряд", "projectile"} },
            { "quantity", { "Количество", "quantity"} },
            { "quest", { "Задание", "quest"} },
            { "quest_reward", { "Награда за задание", "quest_reward"} },
            { "rating", { "Рейтинг", "rating"} },
            { "rating_courier", { "Рейтинг курьерский", "rating_courier"} },
            { "rating_fight", { "Рейтинг боевой", "rating_fight"} },
            { "rating_trade", { "Рейтинг торговый", "rating_trade"} },
            { "rcolor", { "", "rcolor"} },
            { "reconstruction", { "Восстановление", "reconstruction"} },
            { "reputation", { "Репутация", "reputation"} },
            { "resfront", { "Сопротивление спереди", "resfront"} },
            { "resource", { "Ресурс", "resource"} },
            { "resource_drop", { "Ресурс при выпадании", "resource_drop"} },
            { "resside", { "Сопротивление сбоку", "resside"} },
            { "restop", { "Сопротивление сверху", "restop"} },
            { "restore", { "Восстановление", "restore"} },
            { "roll", { "", "roll"} },
            { "rotate", { "Поворот", "rotate"} },
            { "rotatespeed", { "Скорость поворота", "rotatespeed"} },
            { "ru", { "Русский", "Русский"} },
            { "scale", { "Масштаб", "scale"} },
            { "scale_x", { "Масштаб по X", "scale_x"} },
            { "scale_y", { "Масштаб по Y", "scale_y"} },
            { "scale_z", { "Масштаб по Z", "scale_z"} },
            { "script_language", { "Язык скрипта", "script_language"} },
            { "script_main", { "Главный скрипт", "script_main"} },
            { "shootscale", { "", "shootscale"} },
            { "shoottype", { "Тип выстрела", "shoottype"} },
            { "spare", { "Вместимость", "spare"} },
            { "special", { "Специальный", "special"} },
            { "speed", { "Скорость", "speed"} },
            { "stabfront", { "Стабилизация спереди", "stabfront"} },
            { "stabside", { "Стабилизация сбоку", "stabside"} },
            { "standard", { "Стандарт", "standard"} },
            { "state", { "Состояние", "state"} },
            { "strength", { "Сила", "strength"} },
            { "subtype", { "Подтип", "subtype"} },
            { "text_id", { "", "Text ID"} },
            { "tfire", { "", "tfire"} },
            { "time", { "Время", "time"} },
            { "title", { "Название", "title"} },
            { "turbulence", { "Турбулентность", "turbulence"} },
            { "txt",{ "Текст", "Text" } },
            { "type", { "Тип", "type"} },
            { "typearms", { "", "typearms"} },
            { "unicum", { "", "unicum_id"} },
            { "value1", { "Значение 1", "value1"} },
            { "value2", { "Значение 2", "value2"} },
            { "value3", { "Значение 3", "value3"} },
            { "value_blob", { "Значение бинарное", "value_blob"} },
            { "value_float", { "Значение вещественное", "value_float"} },
            { "value_int", { "Значение целое", "value_int"} },
            { "value_text", { "Значение текстовое", "value_text"} },
            { "variable", { "Переменная", "variable"} },
            { "version", { "Версия", "version"} },
            { "vtype", { "", "vtype"} },
            { "weapon", { "Оружие", "weapon"} },
            { "weight", { "Масса", "weight"} },
            { "x", { "", "x"} },
            { "xstate", { "", "xstate"} },
            { "y", { "", "y"} },
            { "yaw", { "", "yaw"} },
            { "z", { "", "z"} },



            // enum fields

            // EObjectType Descriptions
            { "BuildingDescriptions",{ "Описания зданий", "" } },
            { "ClanDescriptions",{ "Описания кланов", "" } },
            { "ClanMechanoids",{ "Механоиды клана", "" } },
            { "ClanMemberNames",{ "Имена членов кланов", "" } },
            { "EquipmentDescriptions",{ "Описания оборудования", "" } },
            { "GliderDescriptions",{ "Описания глайдеров", "" } },
            { "GoodDescriptions",{ "Описания товаров", "" } },
            { "MapBuildingDescriptions",{ "Описания зданий карт", "" } },
            { "MapBuildingNameEndings",{ "Описания окончаний имён зданий карт", "" } },
            { "MessageTitleDescriptions",{ "Заголовки сообщений", "" } },
            { "MessageTextDescriptions",{ "Тексты сообщений", "" } },
            { "ModificationMapDescriptions",{ "Описания карт модификаций", "" } },
            { "ModificatorDescriptions",{ "Описания модификаторов", "" } },
            { "ModificationPlayers",{ "Игроки модификаций", "" } },
            { "ProjectileDescriptions",{ "Описания снарядов", "" } },
            { "QuestDescriptions",{ "Описания заданий", "" } },
            { "QuestTexts",{ "Тексты заданий",  "Quest Texts" } },
            { "QuestTitles",{ "Названия заданий", "" } },
            { "WeaponDescriptions",{ "Описания оружия", "" } },
            //
            { "BuildingDescription",{ "Описание зданий", "" } },
            { "ClanDescription",{ "Описание кланов", "" } },
            { "ClanMechanoid",{ "Механоид клана", "" } },
            { "ClanMemberName",{ "Имя члена клана", "" } },
            { "EquipmentDescription",{ "Описание оборудования", "" } },
            { "GliderDescription",{ "Описание глайдеров", "" } },
            { "GoodDescription",{ "Описание товаров", "" } },
            { "MapBuildingDescription",{ "Описание зданий карт", "" } },
            { "MapBuildingNameEnding",{ "Описание окончаний имён зданий карт", "" } },
            { "MessageTitleDescription",{ "Заголовки сообщений", "" } },
            { "MessageTextDescription",{ "Тексты сообщений", "" } },
            { "ModificationMapDescription",{ "Описание карт модификаций", "" } },
            { "ModificatorDescription",{ "Описание модификаторов", "" } },
            { "ModificationPlayer",{ "Игрок модификации", "" } },
            { "ProjectileDescription",{ "Описание снарядов", "" } },
            { "QuestDescription",{ "Описание заданий", "" } },
            { "QuestText",{ "Текст задания",  "Quest Text" } },
            { "QuestTitle",{ "Название заданий", "" } },
            { "WeaponDescription",{ "Описание оружия", "" } },

            // Standard
            { "Standard1",{ "Стандарт 1", "Standard 1" } },
            { "Standard2",{ "Стандарт 2", "Standard 2" } },
            { "Standard3",{ "Стандарт 3", "Standard 3" } },
            { "Standard4",{ "Стандарт 4", "Standard 4" } },
            { "Universal",{ "Универсальный", "Universal" } },
            { "Other",{ "Другое", "Other" } },

            // QuestState
            { "Started",{ "Начато", "" } },
            { "Completed",{ "Завершено", "" } },
            { "Abandoned",{ "Отменено", "" } },
            { "Failed",{ "Провалено", "" } },

            // WeaponType
            { "Light",{ "Лёгкое", "" } },
            { "Heavy",{ "Тяжёлое", "" } },
            { "RocketLauncher",{ "Ракетомёт", "" } },
            { "BombLauncher",{ "Бомбомёт", "" } },

            // PlayerType
            { "Internet",{ "Сетевой", "" } },
            { "Local",{ "Локальный", "" } },

            // MessageType
            { "Info",{ "Информация", "" } },
            { "Text",{ "Текст", "" } },
            { "Message",{ "Сообщение", "" } },
            { "Packet",{ "Информпакет", "" } },

        } // dict end
    }, // pair end
};


}
