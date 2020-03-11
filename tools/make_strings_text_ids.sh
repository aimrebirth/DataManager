#!/bin/bash

LIST="Buildings
Equipments Gliders Goods Projectiles Weapons Modificators
Quests Clans Messages
ModificationMap
ModificationMapBuilding
"

LIST_NAME_ONLY="Maps Objects Modifications
"

for i in $LIST $LIST_NAME_ONLY; do
    echo "update strings set text_id = (select text_id from $i where name_id = strings.id) || '.NAME_ID'"
    echo "where exists(select 1 from $i where name_id = strings.id);"
done

for i in $LIST; do
    echo "update strings set text_id = (select text_id from $i where description_id = strings.id) || '.DESCRIPTION_ID'"
    echo "where exists(select 1 from $i where description_id = strings.id);"
done

# different field names
echo "update strings set text_id = (select text_id from Messages where title_id = strings.id) || '.DESCRIPTION_ID'"
echo "where exists(select 1 from Messages where title_id = strings.id);"
echo "update strings set text_id = (select text_id from Messages where txt_id = strings.id) || '.DESCRIPTION_ID'"
echo "where exists(select 1 from Messages where txt_id = strings.id);"
# also quests
echo "update strings set text_id = (select text_id from Quests where title_id = strings.id) || '.DESCRIPTION_ID'"
echo "where exists(select 1 from Quests where title_id = strings.id);"
