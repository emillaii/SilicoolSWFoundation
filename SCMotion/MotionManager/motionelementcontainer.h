#ifndef MOTIONELEMENTCONTAINER_H
#define MOTIONELEMENTCONTAINER_H

#include "errorHandling/silicolerror.h"
#include "loghelper.h"
#include "loging/Logger.h"
#include "motionutility.h"
#include "scmotion_global.h"
#include <QMap>
#include <QObject>

class SCMOTIONSHARED_EXPORT MotionElementContainer
{
private:
    MotionElementContainer() {}

public:
    static MotionElementContainer *getIns()
    {
        static MotionElementContainer instance;
        return &instance;
    }

    template <typename T>
    void registerItem(QString name, T *item)
    {
        QString typedName = nominate(T::elementType(), name);
        if (items.contains(typedName))
        {
            throw SilicolAbort(QObject::tr("Item %1 already existed!").arg(typedName), EX_LOCATION);
        }
        items[typedName] = item;
    }

    void unregisterItem(MotionElement::Type elementType, QString name)
    {
        QString typedName = nominate(elementType, name);
        if (items.contains(typedName))
        {
            items.remove(typedName);
        }
    }

    QObject *getItem(MotionElement::Type elementType, QString name)
    {
        QString typedName = nominate(elementType, name);
        if (items.contains(typedName))
        {
            return items[typedName];
        }
        else
        {
            throw SilicolAbort(QObject::tr("Item %1 has not registered!").arg(typedName), EX_LOCATION);
        }
    }

    template <typename T>
    T *getItem(QString name)
    {
        return qobject_cast<T *>(getItem(T::elementType(), name));
    }

private:
    QMap<QString, QObject *> items;
};

#endif    // MOTIONELEMENTCONTAINER_H
