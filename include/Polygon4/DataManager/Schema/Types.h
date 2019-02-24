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

#include <bitset>
#include <string>
#include <vector>

using Name = std::string;
using Key = std::string;
using Value = std::string;

enum ObjectFlag
{
    fAll,

    // classes
    fFirst,
    fLast,
    fCreateEnum,
    fEnumOnly,
    fService,
    fInline,
    fProxy,
    fTreeView,
    fName,
    fObjectName,
    fParent,
    fChild,
    fEnumName,
    fPrefixed,
    fNamesOrder,
    fSubtreeItem,
    fSplitBy,
    fServiceDb,
    fNoSave,

    // fields
    fPrimaryKey,
    fEnumItem,
    fContainer,
    fGetOrderedObjectMap,
    fEnumType,
    fBigEdit,
    fDisplayName,
    fReadOnly,
    fNoChildren,
    fArray,
    fArrayKey,
    fInitialValue,

    // enum items
    fNotInTable,
    fDefault,

    // enums
    fEnding,

    fMaxFlag,
};

using ObjectFlags = std::bitset<fMaxFlag>;
//using ObjectFlags = std::bitset<64>;

using Version = std::string;

enum class DataType
{
    None,
    Bool,
    Integer,
    Real,
    Text,
    Blob,
    Array,
    Complex,
    ComplexArray,
    Enum,
};

enum class ObjectName
{
    None,
    Text,
    Name,
    Parent,
    Custom,
};
