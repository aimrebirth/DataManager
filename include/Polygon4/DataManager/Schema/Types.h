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

    // fields
    fPrimaryKey,
    fEnumItem,
    fContainer,

    // enums
    fNotInTable,

    fMaxFlag,
};

using ObjectFlags = std::bitset<fMaxFlag>;

using Version = std::string;

enum class DataType
{
    None,
    Bool,
    Integer,
    Real,
    Text,
    Blob,
    Complex,
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
