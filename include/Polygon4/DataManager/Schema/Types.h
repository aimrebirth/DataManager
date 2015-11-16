#pragma once

#include <bitset>
#include <string>

#ifdef USE_QT
#include <qstring.h>
#endif

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

    // fields
    fPrimaryKey,
    fEnumItem,
    fContainer,

    fMaxFlag,
};

using ObjectFlags = std::bitset<fMaxFlag>;

using Version = std::string;

enum class DataType
{
    Bool,
    Integer,
    Real,
    Text,
    Blob,
    Complex,
};

enum class ObjectName
{
    None,
    Text,
    Name,
    Parent,
    Custom,
};

#ifdef USE_QT
QString getColumnTypeString(DataType type);
#endif;