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
    return firstNonEmpty();
}

LocalizedString::string_type LocalizedString::firstNonEmpty() const
{
	for (auto &s : *this)
    {
        if (!s.empty())
            return s;
    }
    return LocalizedString::string_type();
}

String LocalizationInfo::str() const
{
	String s = data;
	if (!s.empty())
		return s;
	return key;
}

LocalizationInfo::operator String() const
{
	return str();
}

String Translator::tr(const key_type &key, const context_type &context)
{
	auto c = translator.find(context);
	if (c == translator.end())
	{
		// make empty entry
		translator[context];
		c = translator.find(context);
	}
	auto s = c->second.find(key);
	if (s == c->second.end())
	{
		c->second[key] = { key };
		s = c->second.find(key);
	}
	return s->second.str();
}

void Translator::init(const translator_type &t)
{
	if (initialized)
		return;
	initialized = true;
	translator = t;
}

void Translator::add(const key_type &key, const LocalizedString &ls)
{
	translator[""][key] = ls;
}

void Translator::add(const key_type &key, const context_type &context, const LocalizedString &ls)
{
	translator[context][key] = ls;
}

void Translator::add(const context_type &context, const dictionary_type &d)
{
	translator[context].insert(d.begin(), d.end());
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

String tr(const String &key, const String &context)
{
    return translator.tr(key, context);
}

const Translator::translator_type translator_data =
    {
        {
            // pair < context, dict >
            L"", // context
            {
                // dict start

                // languages
                {L"en", {L"English", L"English"}},
                {L"ru", {L"Русский", L"Русский"}},
                {L"de", {L"Deutsch", L"Deutsch"}},
                {L"fr", {L"Français", L"Français"}},
                {L"es", {L"Español", L"Español"}},
                {L"et", {L"Eesti", L"Eesti"}},
                {L"cs", {L"Čeština", L"Čeština"}},

                // table names (singular)
                {L"None", {L"Отсутствует", L"None"}},
                {L"Building", {L"Здание", L"Building"}},
                {L"Clan", {L"Клан", L"Clan"}},
                {L"ClanReputation", {L"Репутация клана", L"Clan Reputation"}},
                {L"Configuration", {L"Конфигурация", L"Configuration"}},
                {L"ConfigurationEquipment", {L"Оборудование конфигурации", L"Configuration Equipment"}},
                {L"ConfigurationGood", {L"Товар конфигурации", L"Configuration Good"}},
                {L"ConfigurationProjectile", {L"Снаряд конфигурации", L"Configuration Projectile"}},
                {L"ConfigurationWeapon", {L"Оружие конфигурации", L"Configuration Weapon"}},
                {L"Equipment", {L"Оборудование", L"Equipment"}},
                {L"Glider", {L"Глайдер", L"Glider"}},
                {L"Good", {L"Товар", L"Good"}},
                {L"Group", {L"Группа", L"Group"}},
                {L"GroupMechanoid", {L"Механоид группы", L"Group Mechanoid"}},
                {L"Map", {L"Карта", L"Map"}},
                {L"MapBuilding", {L"Здание карты", L"Map Building"}},
                {L"MapBuildingEquipment", {L"Оборудование в здании", L"Map Building Equipment"}},
                {L"MapBuildingGlider", {L"Глайдер в здании", L"Map Building Glider"}},
                {L"MapBuildingGood", {L"Товар в здании", L"Map Building Good"}},
                {L"MapBuildingModificator", {L"Модификатор в здании", L"Map Building Modificator"}},
                {L"MapBuildingProjectile", {L"Снаряд в здании", L"Map Building Projectile"}},
                {L"MapBuildingWeapon", {L"Оружие в здании", L"Map Building Weapon"}},
                {L"MapGood", {L"Товар на карте", L"Map Good"}},
                {L"MapObject", {L"Объект на карте", L"Map Object"}},
                {L"Mechanoid", {L"Механоид", L"Mechanoid"}},
                {L"MechanoidQuest", {L"Задание механоида", L"Mechanoid Quest"}},
                {L"Modification", {L"Мод", L"Modification"}},
                {L"ModificationMap", {L"Карта мода", L"Modification Map"}},
                {L"Modificator", {L"Модификатор", L"Modificator"}},
                {L"Object", {L"Объект", L"Object"}},
                {L"Player", {L"Игрок", L"Player"}},
                {L"Projectile", {L"Снаряд", L"Projectile"}},
                {L"Quest", {L"Задание", L"Quest"}},
                {L"QuestReward", {L"Награда за задание", L"Quest Reward"}},
                {L"Reward", {L"Награда за задание", L"Quest Reward"}},
                {L"QuestRewardEquipment", {L"Оборудование за задание", L"Quest Reward Equipment"}},
                {L"QuestRewardGlider", {L"Глайдер за задание", L"Quest Reward Glider"}},
                {L"QuestRewardGood", {L"Товар за задание", L"Quest Reward Good"}},
                {L"QuestRewardModificator", {L"Модификатор за задание", L"Quest Reward Modificator"}},
                {L"QuestRewardProjectile", {L"Снаряд за задание", L"Quest Reward Projectile"}},
                {L"QuestRewardReputation", {L"Репутация за задание", L"Quest Reward Reputation"}},
                {L"QuestRewardWeapon", {L"Оружие за задание", L"Quest Reward Weapon"}},
                {L"ScriptVariable", {L"Переменная скрипта", L"Script Variable"}},
                {L"Setting", {L"Настройка", L"Setting"}},
                {L"String", {L"Строка", L"String"}},
                {L"Weapon", {L"Оружие", L"Weapon"}},
                {L"Any", {L"Любой объект", L"Any Object"}},
                {L"Anys", {L"Любой объект", L"Any Object"}},

                // table names (plural, words)
                {L"Buildings", {L"Здания", L"Buildings"}},
                {L"Clans", {L"Кланы", L"Clans"}},
                {L"Clan Reputations", {L"Отношения", L"Clan Reputations"}},
                {L"Configurations", {L"Конфигурации", L"Configurations"}},
                {L"Configuration Equipments", {L"Оборудование конфигурации", L"Configuration Equipments"}},
                {L"Configuration Goods", {L"Товары конфигурации", L"Configuration Goods"}},
                {L"Configuration Projectiles", {L"Снаряды конфигурации", L"Configuration Projectiles"}},
                {L"Configuration Weapons", {L"Оружие конфигурации", L"Configuration Weapons"}},
                {L"Equipments", {L"Оборудование", L"Equipments"}},
                {L"Gliders", {L"Глайдеры", L"Gliders"}},
                {L"Goods", {L"Товары", L"Goods"}},
                {L"Groups", {L"Группы", L"Groups"}},
                {L"Group Mechanoids", {L"Механоиды группы", L"Group Mechanoids"}},
                {L"Inline variables", {L"Переменные", L"Inline variables"}},
                {L"Maps", {L"Карты", L"Maps"}},
                {L"Map Buildings", {L"Здания карты", L"Map Buildings"}},
                {L"Map Building Equipments", {L"Оборудование в здании", L"Map Building Equipments"}},
                {L"Map Building Gliders", {L"Глайдеры в здании", L"Map Building Gliders"}},
                {L"Map Building Goods", {L"Товары в здании", L"Map Building Goods"}},
                {L"Map Building Modificators", {L"Модификаторы в здании", L"Map Building Modificators"}},
                {L"Map Building Projectiles", {L"Снаряды в здании", L"Map Building Projectiles"}},
                {L"Map Building Weapons", {L"Оружие в здании", L"Map Building Weapons"}},
                {L"Map Goods", {L"Товары на карте", L"Map Goods"}},
                {L"Map Objects", {L"Объекты на карте", L"Map Objects"}},
                {L"Mechanoids", {L"Механоиды", L"Mechanoids"}},
                {L"Mechanoid Quests", {L"Задания механоида", L"Mechanoid Quests"}},
                {L"Modifications", {L"Моды", L"Modifications"}},
                {L"Modification Maps", {L"Карты мода", L"Modification Maps"}},
                {L"Modificators", {L"Модификаторы", L"Modificators"}},
                {L"Objects", {L"Объекты", L"Objects"}},
                {L"Players", {L"Игроки", L"Players"}},
                {L"Projectiles", {L"Снаряды", L"Projectiles"}},
                {L"Quests", {L"Задания", L"Quests"}},
                {L"Quest Rewards", {L"Награды за задание", L"Quest Rewards"}},
                {L"Rewards", {L"Награды", L"Rewards"}},
                {L"Quest Reward Equipments", {L"Оборудование за задание", L"Quest Reward Equipments"}},
                {L"Quest Reward Gliders", {L"Глайдеры за задание", L"Quest Reward Gliders"}},
                {L"Quest Reward Goods", {L"Товары за задание", L"Quest Reward Goods"}},
                {L"Quest Reward Modificators", {L"Модификаторы за задание", L"Quest Reward Modificators"}},
                {L"Quest Reward Projectiles", {L"Снаряды за задание", L"Quest Reward Projectiles"}},
                {L"Quest Reward Reputations", {L"Репутация за задание", L"Quest Reward Reputations"}},
                {L"Quest Reward Weapons", {L"Оружие за задание", L"Quest Reward Weapons"}},
                {L"Script Variables", {L"Переменные скрипта", L"Script Variables"}},
                {L"Settings", {L"Настройки", L"Settings"}},
                {L"Strings", {L"Строки", L"Strings"}},
                {L"Weapons", {L"Оружие", L"Weapons"}},

                // table names (plural, single word)
                {L"Buildings", {L"Здания", L"Buildings"}},
                {L"Clans", {L"Кланы", L"Clans"}},
                {L"ClanReputations", {L"Отношения", L"Clan Reputations"}},
                {L"Configurations", {L"Конфигурации", L"Configurations"}},
                {L"ConfigurationEquipments", {L"Оборудование конфигурации", L"Configuration Equipments"}},
                {L"ConfigurationGoods", {L"Товары конфигурации", L"Configuration Goods"}},
                {L"ConfigurationProjectiles", {L"Снаряды конфигурации", L"Configuration Projectiles"}},
                {L"ConfigurationWeapons", {L"Оружие конфигурации", L"Configuration Weapons"}},
                {L"Equipments", {L"Оборудование", L"Equipments"}},
                {L"Gliders", {L"Глайдеры", L"Gliders"}},
                {L"Goods", {L"Товары", L"Goods"}},
                {L"Groups", {L"Группы", L"Groups"}},
                {L"GroupMechanoids", {L"Механоиды группы", L"Group Mechanoids"}},
                {L"Maps", {L"Карты", L"Maps"}},
                {L"MapBuildings", {L"Здания карты", L"Map Buildings"}},
                {L"MapBuildingEquipments", {L"Оборудование в здании", L"Map Building Equipments"}},
                {L"MapBuildingGliders", {L"Глайдеры в здании", L"Map Building Gliders"}},
                {L"MapBuildingGoods", {L"Товары в здании", L"Map Building Goods"}},
                {L"MapBuildingModificators", {L"Модификаторы в здании", L"Map Building Modificators"}},
                {L"MapBuildingProjectiles", {L"Снаряды в здании", L"Map Building Projectiles"}},
                {L"MapBuildingWeapons", {L"Оружие в здании", L"Map Building Weapons"}},
                {L"MapGoods", {L"Товары на карте", L"Map Goods"}},
                {L"MapObjects", {L"Объекты на карте", L"Map Objects"}},
                {L"Mechanoids", {L"Механоиды", L"Mechanoids"}},
                {L"MechanoidQuests", {L"Задания механоида", L"Mechanoid Quests"}},
                {L"Messages", {L"Сообщения", L"Messages"}},
                {L"Modifications", {L"Моды", L"Modifications"}},
                {L"ModificationMaps", {L"Карты мода", L"Modification Maps"}},
                {L"Modificators", {L"Модификаторы", L"Modificators"}},
                {L"Objects", {L"Объекты", L"Objects"}},
                {L"Players", {L"Игроки", L"Players"}},
                {L"Projectiles", {L"Снаряды", L"Projectiles"}},
                {L"Quests", {L"Задания", L"Quests"}},
                {L"QuestRewards", {L"Награды за задание", L"Quest Rewards"}},
                {L"QuestRewardEquipments", {L"Оборудование за задание", L"Quest Reward Equipments"}},
                {L"QuestRewardGliders", {L"Глайдеры за задание", L"Quest Reward Gliders"}},
                {L"QuestRewardGoods", {L"Товары за задание", L"Quest Reward Goods"}},
                {L"QuestRewardModificators", {L"Модификаторы за задание", L"Quest Reward Modificators"}},
                {L"QuestRewardProjectiles", {L"Снаряды за задание", L"Quest Reward Projectiles"}},
                {L"QuestRewardReputations", {L"Репутация за задание", L"Quest Reward Reputations"}},
                {L"QuestRewardWeapons", {L"Оружие за задание", L"Quest Reward Weapons"}},
                {L"Reputations", {L"Отношения", L"Reputations"}},
                {L"ScriptVariables", {L"Переменные скрипта", L"Script Variables"}},
                {L"Settings", {L"Настройки", L"Settings"}},
                {L"Strings", {L"Строки", L"Strings"}},
                {L"Weapons", {L"Оружие", L"Weapons"}},

                // field names
                {L"T", {L"", L"T"}},
                {L"Volatile", {L"", L"Volatile"}},
                {L"angle", {L"Угол", L"angle"}},
                {L"armor", {L"Броня", L"armor"}},
                {L"author", {L"Автор", L"author"}},
                {L"bcolor", {L"", L"bcolor"}},
                {L"blueprint", {L"Блюпринт", L"Blueprint"}},
                {L"bonusexp", {L"Бонус к опыту", L"bonusexp"}},
                {L"bonusrepair", {L"Бонус к починке", L"bonusrepair"}},
                {L"bonustrade", {L"Бонус к торговле", L"bonustrade"}},
                {L"brief", {L"Краткое", L"brief"}},
                {L"building", {L"Здание", L"building"}},
                {L"bx", {L"", L"bx"}},
                {L"by", {L"", L"by"}},
                {L"careen", {L"", L"careen"}},
                {L"clan", {L"Клан", L"clan"}},
                {L"clan1", {L"", L"clan1"}},
                {L"clan2", {L"", L"clan2"}},
                {L"color", {L"Цвет", L"color"}},
                {L"comment", {L"Комментарий", L"comment"}},
                {L"complete", {L"Выполнено", L"complete"}},
                {L"configuration", {L"Конфигурация", L"configuration"}},
                {L"cooperative_player_configuration", {L"Конфигурация сетевого игрока", L"cooperative_player_configuration"}},
                {L"damage", {L"Урон", L"damage"}},
                {L"damagetype", {L"Тип урона", L"damagetype"}},
                {L"date_created", {L"Дата создания", L"date_created"}},
                {L"date_modified", {L"Дата изменения", L"date_modified"}},
                {L"delta_t", {L"", L"delta_t"}},
                {L"description", {L"Описание", L"description"}},
                {L"detonation_delay", {L"Задержка детонации", L"detonation_delay"}},
                {L"directory", {L"Директория", L"directory"}},
                {L"distance_detonation", {L"Дистанция детонации", L"distance_detonation"}},
                {L"durability", {L"Прочность", L"durability"}},
                {L"equipment", {L"Оборудование", L"equipment"}},
                {L"fail", {L"Провалено", L"fail"}},
                {L"firerate", {L"Частота стрельбы", L"firerate"}},
                {L"fx", {L"", L"fx"}},
                {L"fxmodeltime", {L"", L"fxmodeltime"}},
                {L"fxtime", {L"", L"fxtime"}},
                {L"gcolor", {L"", L"gcolor"}},
                {L"generation", {L"Поколение", L"generation"}},
                {L"get", {L"Получено", L"get"}},
                {L"glider", {L"Глайдер", L"glider"}},
                {L"good", {L"Товар", L"good"}},
                {L"group", {L"Группа", L"group"}},
                {L"h_max", {L"", L"h_max"}},
                {L"h_min", {L"", L"h_min"}},
                {L"helpness", {L"Полезность", L"helpness"}},
                {L"id", {L"Номер", L"id"}},
                {L"in_progress", {L"В процессе", L"in_progress"}},
                {L"inside_mul", {L"", L"inside_mul"}},
                {L"inside_x", {L"", L"inside_x"}},
                {L"inside_y", {L"", L"inside_y"}},
                {L"inside_z", {L"", L"inside_z"}},
                {L"interactive", {L"Интерактивный", L"interactive"}},
                {L"k_param1", {L"Коэффициент 1", L"k_param1"}},
                {L"k_param2", {L"Коэффициент 2", L"k_param2"}},
                {L"k_price", {L"Коэффициент цены", L"k_price"}},
                {L"kx", {L"", L"kx"}},
                {L"ky", {L"", L"ky"}},
                {L"life_time", {L"Время жизни", L"life_time"}},
                {L"manual", {L"Ручная активация", L"manual"}},
                {L"map", {L"Карта", L"map"}},
                {L"map_building", {L"Здание карты", L"map_building"}},
                {L"mask", {L"Маска", L"mask"}},
                {L"maxdistance", {L"Максимальное расстояние", L"maxdistance"}},
                {L"maxweight", {L"Максимальная масса", L"maxweight"}},
                {L"mechanoid", {L"Механоид", L"mechanoid"}},
                {L"member_name", {L"Имя члена", L"member_name"}},
                {L"modification", {L"Мод", L"modification"}},
                {L"modificator", {L"Модификатор", L"modificator"}},
                {L"money", {L"Деньги", L"money"}},
                {L"name", {L"Имя", L"name"}},
                {L"name_ending", {L"Окончание имени", L"name_ending"}},
                {L"nickname", {L"Псевдоним", L"Nickname"}},
                {L"noblivion", {L"", L"noblivion"}},
                {L"notrade", {L"Не продаётся", L"notrade"}},
                {L"numstate", {L"", L"numstate"}},
                {L"object", {L"Объект", L"object"}},
                {L"other_strings", {L"Другие строки", L"other_strings"}},
                {L"pitch", {L"", L"pitch"}},
                {L"player", {L"Игрок", L"player"}},
                {L"player_mechanoid", {L"Механоид игрока", L"player_mechanoid"}},
                {L"playereffect", {L"", L"playereffect"}},
                {L"power", {L"Мощность", L"power"}},
                {L"price", {L"Цена", L"price"}},
                {L"probability", {L"Вероятность", L"probability"}},
                {L"projectile", {L"Снаряд", L"projectile"}},
                {L"quantity", {L"Количество", L"quantity"}},
                {L"quest", {L"Задание", L"quest"}},
                {L"quest_reward", {L"Награда за задание", L"quest_reward"}},
                {L"rating", {L"Рейтинг", L"rating"}},
                {L"rating_courier", {L"Рейтинг курьерский", L"rating_courier"}},
                {L"rating_fight", {L"Рейтинг боевой", L"rating_fight"}},
                {L"rating_trade", {L"Рейтинг торговый", L"rating_trade"}},
                {L"rcolor", {L"", L"rcolor"}},
                {L"reconstruction", {L"Восстановление", L"reconstruction"}},
                {L"reputation", {L"Репутация", L"reputation"}},
                {L"resfront", {L"Сопротивление спереди", L"resfront"}},
                {L"resource", {L"Ресурс", L"resource"}},
                {L"resource_drop", {L"Ресурс при выпадании", L"resource_drop"}},
                {L"resside", {L"Сопротивление сбоку", L"resside"}},
                {L"restop", {L"Сопротивление сверху", L"restop"}},
                {L"restore", {L"Восстановление", L"restore"}},
                {L"roll", {L"", L"roll"}},
                {L"rotate", {L"Поворот", L"rotate"}},
                {L"rotatespeed", {L"Скорость поворота", L"rotatespeed"}},
                {L"scale", {L"Масштаб", L"scale"}},
                {L"scale_x", {L"Масштаб по X", L"scale_x"}},
                {L"scale_y", {L"Масштаб по Y", L"scale_y"}},
                {L"scale_z", {L"Масштаб по Z", L"scale_z"}},
                {L"script_language", {L"Язык скрипта", L"script_language"}},
                {L"script_main", {L"Главный скрипт", L"script_main"}},
                {L"shootscale", {L"", L"shootscale"}},
                {L"shoottype", {L"Тип выстрела", L"shoottype"}},
                {L"spare", {L"Вместимость", L"spare"}},
                {L"special", {L"Специальный", L"special"}},
                {L"speed", {L"Скорость", L"speed"}},
                {L"stabfront", {L"Стабилизация спереди", L"stabfront"}},
                {L"stabside", {L"Стабилизация сбоку", L"stabside"}},
                {L"standard", {L"Стандарт", L"standard"}},
                {L"state", {L"Состояние", L"state"}},
                {L"strength", {L"Сила", L"strength"}},
                {L"subtype", {L"Подтип", L"subtype"}},
                {L"text_id", {L"", L"Text ID"}},
                {L"tfire", {L"", L"tfire"}},
                {L"time", {L"Время", L"time"}},
                {L"title", {L"Название", L"title"}},
                {L"turbulence", {L"Турбулентность", L"turbulence"}},
                {L"txt", {L"Текст", L"Text"}},
                {L"type", {L"Тип", L"type"}},
                {L"typearms", {L"", L"typearms"}},
                {L"unicum", {L"", L"unicum_id"}},
                {L"value1", {L"Значение 1", L"value1"}},
                {L"value2", {L"Значение 2", L"value2"}},
                {L"value3", {L"Значение 3", L"value3"}},
                {L"value_blob", {L"Значение бинарное", L"value_blob"}},
                {L"value_float", {L"Значение вещественное", L"value_float"}},
                {L"value_int", {L"Значение целое", L"value_int"}},
                {L"value_text", {L"Значение текстовое", L"value_text"}},
                {L"variable", {L"Переменная", L"variable"}},
                {L"version", {L"Версия", L"version"}},
                {L"vtype", {L"", L"vtype"}},
                {L"weapon", {L"Оружие", L"weapon"}},
                {L"weight", {L"Масса", L"weight"}},
                {L"x", {L"", L"x"}},
                {L"xstate", {L"", L"xstate"}},
                {L"y", {L"", L"y"}},
                {L"yaw", {L"", L"yaw"}},
                {L"z", {L"", L"z"}},

                // enum fields

                // EObjectType Descriptions
                {L"BuildingDescriptions", {L"Описания зданий", L"Building Descriptions"}},
                {L"ClanDescriptions", {L"Описания кланов", L"Clan Descriptions"}},
                {L"ClanMechanoids", {L"Механоиды клана", L"Clan Mechanoids"}},
                {L"ClanMemberNames", {L"Имена членов кланов", L"Clan Member Names"}},
                {L"EquipmentDescriptions", {L"Описания оборудования", L"Equipment Descriptions"}},
                {L"GliderDescriptions", {L"Описания глайдеров", L"Glider Descriptions"}},
                {L"GoodDescriptions", {L"Описания товаров", L"Good Descriptions"}},
                {L"MapBuildingDescriptions", {L"Описания зданий карт", L"Map Building Descriptions"}},
                {L"MapBuildingNameEndings", {L"Описания окончаний имён зданий карт", L"Map Building Name Endings"}},
                {L"MessageTitleDescriptions", {L"Заголовки сообщений", L"Message Title Descriptions"}},
                {L"MessageTextDescriptions", {L"Тексты сообщений", L"Message Text Descriptions"}},
                {L"ModificationMapDescriptions", {L"Описания карт модификаций", L"Modification Map Descriptions"}},
                {L"ModificatorDescriptions", {L"Описания модификаторов", L"Modificator Descriptions"}},
                {L"ModificationPlayers", {L"Игроки модификаций", L"Modification Players"}},
                {L"ProjectileDescriptions", {L"Описания снарядов", L"Projectile Descriptions"}},
                {L"QuestDescriptions", {L"Описания заданий", L"Quest Descriptions"}},
                {L"QuestTexts", {L"Тексты заданий", L"Quest Texts"}},
                {L"QuestTitles", {L"Названия заданий", L"Quest Titles"}},
                {L"WeaponDescriptions", {L"Описания оружия", L"Weapon Descriptions"}},
                //
                {L"BuildingDescription", {L"Описание зданий", L"Building Description"}},
                {L"ClanDescription", {L"Описание кланов", L"Clan Description"}},
                {L"ClanMechanoid", {L"Механоид клана", L"Clan Mechanoid"}},
                {L"ClanMemberName", {L"Имя члена клана", L"Clan Member Name"}},
                {L"EquipmentDescription", {L"Описание оборудования", L"Equipment Description"}},
                {L"GliderDescription", {L"Описание глайдеров", L"Glider Description"}},
                {L"GoodDescription", {L"Описание товаров", L"Good Description"}},
                {L"MapBuildingDescription", {L"Описание зданий карт", L"Map Building Description"}},
                {L"MapBuildingNameEnding", {L"Описание окончаний имён зданий карт", L"Map Building Name Ending"}},
                {L"MessageTitleDescription", {L"Заголовки сообщений", L"Message Title Description"}},
                {L"MessageTextDescription", {L"Тексты сообщений", L"Message Text Description"}},
                {L"ModificationMapDescription", {L"Описание карт модификаций", L"Modification Map Description"}},
                {L"ModificatorDescription", {L"Описание модификаторов", L"Modificator Description"}},
                {L"ModificationPlayer", {L"Игрок модификации", L"Modification Player"}},
                {L"ProjectileDescription", {L"Описание снарядов", L"Projectile Description"}},
                {L"QuestDescription", {L"Описание заданий", L"Quest Description"}},
                {L"QuestText", {L"Текст задания", L"Quest Text"}},
                {L"QuestTitle", {L"Название заданий", L"Quest Title"}},
                {L"WeaponDescription", {L"Описание оружия", L"Weapon Description"}},

                // Standard
                {L"Standard1", {L"Стандарт 1", L"Standard 1"}},
                {L"Standard2", {L"Стандарт 2", L"Standard 2"}},
                {L"Standard3", {L"Стандарт 3", L"Standard 3"}},
                {L"Standard4", {L"Стандарт 4", L"Standard 4"}},
                {L"Universal", {L"Универсальный", L"Universal"}},
                {L"Other", {L"Другое", L"Other"}},

                // QuestState
                {L"Started", {L"Начато", L"Started"}},
                {L"Completed", {L"Завершено", L"Completed"}},
                {L"Abandoned", {L"Отменено", L"Abandoned"}},
                {L"Failed", {L"Провалено", L"Failed"}},

                // WeaponType
                {L"Light", {L"Лёгкое", L"Light"}},
                {L"Heavy", {L"Тяжёлое", L"Heavy"}},
                {L"RocketLauncher", {L"Ракетомёт", L"Rocket Launcher"}},
                {L"BombLauncher", {L"Бомбомёт", L"Bomb Launcher"}},

                // PlayerType
                {L"Internet", {L"Сетевой", L"Internet"}},
                {L"Local", {L"Локальный", L"Local"}},

                // MessageType
                {L"Info", {L"Информация", L"Info"}},
                {L"Text", {L"Текст", L"Text"}},
                {L"Message", {L"Сообщение", L"Message"}},
                {L"Packet", {L"Информпакет", L"Packet"}},

            } // dict end
        },    // pair end
};

}
