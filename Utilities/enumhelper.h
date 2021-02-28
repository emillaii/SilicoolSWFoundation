#ifndef ENUMHELPER_H
#define ENUMHELPER_H

#include "errorHandling/silicolerror.h"
#include <QObject>

template <typename T>
class EnumHelper
{
public:
    EnumHelper(const QMetaObject &metaObj, const QString &enumTypeName)
        : className(metaObj.className()), enumTypeName(enumTypeName)
    {
        for (int i = 0; i < metaObj.enumeratorCount(); i++)
        {
            QMetaEnum qme = metaObj.enumerator(i);
            if (qme.name() == enumTypeName)
            {
                for (int j = 0; j < qme.keyCount(); j++)
                {
                    nameEnumMap[qme.key(j)] = static_cast<T>(qme.value(j));
                    enumNameMap[static_cast<T>(qme.value(j))] = qme.key(j);
                }
                return;
            }
        }
        qFatal("Did not find enum type '%s' in class: '%s'", enumTypeName.toUtf8().data(), metaObj.className());
    }

    QStringList names() const
    {
        return nameEnumMap.keys();
    }

    QString enumToName(T enumeration) const
    {
        return enumNameMap[enumeration];
    }

    T nameToEnum(QString name) const
    {
        if (!nameEnumMap.contains(name))
        {
            throw SilicolAbort(QObject::tr("Enum '%1' did not exist in enum type '%2' of class '%3'!")
                                   .arg(name)
                                   .arg(enumTypeName)
                                   .arg(className),
                               EX_LOCATION);
        }
        return static_cast<T>(nameEnumMap[name]);
    }

private:
    QString className;
    QString enumTypeName;
    QMap<QString, T> nameEnumMap;
    QMap<T, QString> enumNameMap;
};

#endif    // ENUMHELPER_H
