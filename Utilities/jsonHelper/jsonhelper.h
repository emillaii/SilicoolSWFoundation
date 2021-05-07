#ifndef JSONHELPER_H
#define JSONHELPER_H

#include <QDebug>
#include <QJsonArray>
#include <QJsonObject>
#include <QMetaObject>
#include <QMetaProperty>
#include <QObject>

class JsonHelper
{
public:
    JsonHelper() = default;

    void addFilteredPropertyName(const QString &propertyName);

    void addFilteredPropertyNames(const QStringList &propertyNames);

    template <typename T>
    QJsonObject toJson(T *obj, bool useFilter = true)
    {
        QJsonObject jsonObj;
        for (int i = 0; i < obj->metaObject()->propertyCount(); i++)
        {
            auto prop = obj->metaObject()->property(i);
            if (useFilter && m_filteredPropertyName.contains(prop.name()))
            {
                continue;
            }
            jsonObj[prop.name()] = obj->property(prop.name()).toJsonValue();
        }
        return jsonObj;
    }

    template <typename T>
    T *fromJson(const QJsonObject &jsonObj, bool useFilter = true)
    {
        QObject *obj = T::staticMetaObject.newInstance();
        if (obj != nullptr)
        {
            foreach (auto key, jsonObj.keys())
            {
                if (useFilter && m_filteredPropertyName.contains(key))
                {
                    continue;
                }
                obj->setProperty(key.toUtf8(), jsonObj[key].toVariant());
            }
        }
        else
        {
            qCritical() << QString("Create object failed! ClassName: %1").arg(T::staticMetaObject.className());
        }
        return qobject_cast<T *>(obj);
    }

    template <typename T>
    QJsonArray toJson(const QList<T *> objs, bool useFilter = true)
    {
        QJsonArray jArray;
        foreach (auto obj, objs)
        {
            jArray.append(toJson<T>(obj, useFilter));
        }
        return jArray;
    }

    template <typename T>
    QList<T *> fromJson(const QJsonArray &jsonArray, bool useFilter = true)
    {
        QList<T *> objs;
        for (int i = 0; i < jsonArray.count(); i++)
        {
            objs.append(fromJson<T>(jsonArray[i].toObject(), useFilter));
        }
        return objs;
    }

private:
    QStringList m_filteredPropertyName;
};

#endif    // JSONHELPER_H
