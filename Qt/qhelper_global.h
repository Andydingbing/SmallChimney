#ifndef QHELPER_GLOBAL_H
#define QHELPER_GLOBAL_H

#include <QtGlobal>
#include <QMetaEnum>
#include <QObject>
#include <QStringList>

#ifdef QHELPER_DLL_EXPORT
    #define QHELPER_EXPORT Q_DECL_EXPORT
#else
    #define QHELPER_EXPORT Q_DECL_IMPORT
#endif

class Global_Enum : public QObject
{
    Q_OBJECT

#define ENUM_STRING_LIST(e) \
    Q_ENUM(e##_t) \
    static QStringList e() \
    { \
        QStringList list; \
        const char *str = nullptr; \
        for (int i = 0;;++i) { \
            if ((str = QMetaEnum::fromType<radio_system_t>().valueToKey(i)) == nullptr) { \
                break; \
            } \
            list.push_back(QString(str)); \
        } \
        return list; \
    }

public:
    #include "device_types.h"
    ENUM_STRING_LIST(radio_system)
};

#endif
