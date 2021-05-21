#include "csvhelper.h"

CSVHelper::CSVHelper(const QMetaObject &metaObj, QObject *parent) : QObject(parent), metaObj(&metaObj)
{
    title = getTitle(&metaObj, objectNamePropIndex);
    title.append("\n");
}

void CSVHelper::append(const QString &fileName, const QObject *obj)
{
    if (qstrcmp(metaObj->className(), obj->metaObject()->className()) != 0)
    {
        qCritical() << "Uncompatible type!" << metaObj->className() << obj->metaObject()->className();
        return;
    }
    QFileInfo fileInfo(fileName);
    bool fileExist = fileInfo.exists();
    QDir().mkpath(fileInfo.absolutePath());
    QFile f(fileName);
    f.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append);
    if (!fileExist)
    {
        f.write(title.toUtf8());
    }

    QString lineData;
    for (int i = 0; i < metaObj->propertyCount(); i++)
    {
        if (i == objectNamePropIndex)
        {
            continue;
        }
        if (lineData.length() > 0)
        {
            lineData.append(",");
        }
        lineData.append(metaObj->property(i).read(obj).toString());
    }
    lineData.append("\n");
    f.write(lineData.toUtf8());
    f.flush();
    f.close();
}

void CSVHelper::resetAllProperties(QObject *obj, QVariant value)
{
    if (qstrcmp(metaObj->className(), obj->metaObject()->className()) != 0)
    {
        qCritical() << "Uncompatible type!" << metaObj->className() << obj->metaObject()->className();
        return;
    }
    for (int i = 0; i < metaObj->propertyCount(); i++)
    {
        if (i == objectNamePropIndex)
        {
            continue;
        }
        metaObj->property(i).write(obj, value);
    }
}

QString CSVHelper::getTitle(const QMetaObject *metaObj, int &objNameIndex)
{
    QString title;
    for (int i = 0; i < metaObj->propertyCount(); i++)
    {
        QString propName = metaObj->property(i).name();
        if (propName == "objectName")
        {
            objNameIndex = i;
            continue;
        }
        if (!title.isEmpty())
        {
            title.append(",");
        }
        title.append(propName);
    }
    return title;
}
