#ifndef CSVHELPER_H
#define CSVHELPER_H

#include "utilities_global.h"
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QMetaObject>
#include <QMetaProperty>
#include <QObject>

class UTILITIESSHARED_EXPORT CSVHelper : public QObject
{
    Q_OBJECT

public:
    enum LoadStrategy
    {
        ByIndex,
        ByName
    };

    explicit CSVHelper(const QMetaObject &metaObj, QObject *parent = nullptr);

    template <typename T>
    static void load(const QString &fileName, QList<T *> &objs, LoadStrategy loadStrategy = ByName, bool fileHasTitle = true)
    {
        if (loadStrategy == ByName && !fileHasTitle)
        {
            qCritical() << "Can not load by name if file did not have title!";
            return;
        }
        QFile f(fileName);
        if (f.exists())
        {
            f.open(QIODevice::ReadOnly | QIODevice::Text);
            QString contents = f.readAll();
            f.close();
            QStringList lines = contents.split("\n");
            int minLines = fileHasTitle ? 2 : 1;
            if (lines.count() < minLines)
            {
                qCritical() << "Empty file content!";
                return;
            }

            if (loadStrategy == ByName)
            {
                QStringList fileTitles = lines[0].split(",");
                for (int i = 1; i < lines.count(); i++)
                {
                    if (lines[i].isEmpty())
                    {
                        continue;
                    }
                    QStringList lineDatas = lines[i].split(",");
                    T *obj = qobject_cast<T *>(T::staticMetaObject.newInstance());
                    if (obj == nullptr)
                    {
                        qCritical() << "Create object failed! Object type:" << T::staticMetaObject.className();
                        return;
                    }
                    for (int j = 0; j < fileTitles.count(); j++)
                    {
                        if (j < lineDatas.count())
                        {
                            obj->setProperty(fileTitles[j].toUtf8(), lineDatas[j]);
                        }
                    }
                    objs.append(obj);
                }
            }
            else
            {
                int startIndex = fileHasTitle ? 1 : 0;
                for (int i = startIndex; i < lines.count(); i++)
                {
                    QStringList lineDatas = lines[i].split(",");
                    T *obj = qobject_cast<T *>(T::staticMetaObject.newInstance());
                    if (obj == nullptr)
                    {
                        qCritical() << "Create object failed! Object type:" << T::staticMetaObject.className();
                        return;
                    }
                    for (int j = 0; j < T::staticMetaObject.propertyCount(); j++)
                    {
                        if (lineDatas.count() > 0)
                        {
                            auto propName = T::staticMetaObject.property(j).name();
                            if (propName != "objectName")
                            {
                                T::staticMetaObject.property(j).write(obj, lineDatas.takeFirst());
                            }
                        }
                        else
                        {
                            break;
                        }
                    }
                    objs.append(obj);
                }
            }
        }
        else
        {
            qCritical() << "File not exist!" << fileName;
        }
    }

    template <typename T>
    static void save(const QString &fileName, const QList<T *> &objs)
    {
        if (objs.isEmpty())
        {
            return;
        }
        QFileInfo fileInfo(fileName);
        QDir().mkpath(fileInfo.absolutePath());
        QFile f(fileName);
        f.open(QIODevice::WriteOnly | QIODevice::Text);
        int objNameIndex;
        QString title = getTitle(&T::staticMetaObject, objNameIndex);
        title.append("\n");
        f.write(title.toUtf8());
        for (int i = 0; i < objs.count(); i++)
        {
            auto obj = objs[i];
            QString lineData;
            for (int j = 0; j < T::staticMetaObject.propertyCount(); j++)
            {
                if (j == objNameIndex)
                {
                    continue;
                }
                if (lineData.length() > 0)
                {
                    lineData.append(",");
                }
                lineData.append(T::staticMetaObject.property(j).read(obj).toString());
            }
            lineData.append("\n");
            f.write(lineData.toUtf8());
        }
        f.flush();
        f.close();
    }

    void append(const QString &fileName, const QObject *obj);

private:
    static QString getTitle(const QMetaObject *metaObj, int &objNameIndex);

private:
    const QMetaObject *metaObj;
    QString title;
    int objectNamePropIndex = -1;
};

#endif    // CSVHELPER_H
