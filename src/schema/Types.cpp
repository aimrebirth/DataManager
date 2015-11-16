#include "types.h"

#include <assert.h>

#ifdef USE_QT
#include <qcoreapplication.h>
#include <qstring.h>
#endif

#ifdef USE_QT
QString getColumnTypeString(DataType type)
{
    switch (type)
    {
    case DataType::Integer:
        return QCoreApplication::translate("DB Types", "INTEGER");
    case DataType::Real:
        return QCoreApplication::translate("DB Types", "REAL");
    case DataType::Text:
        return QCoreApplication::translate("DB Types", "TEXT");
    case DataType::Bool:
        return QCoreApplication::translate("DB Types", "BOOL");
    case DataType::Blob:
        return QCoreApplication::translate("DB Types", "BLOB");
    case DataType::Complex:
        return "";
    default:
        return "Unknown type";
    }
}
#endif