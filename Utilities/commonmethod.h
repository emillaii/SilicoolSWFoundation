#ifndef COMMONMETHOD_H
#define COMMONMETHOD_H

#include "utilities_global.h"
#include <QCoreApplication>
#include <QDateTime>
#include <QElapsedTimer>
#include <QMetaEnum>
#include <QThread>
#include <QVariantList>
#include <QtCore>

UTILITIESSHARED_EXPORT QString combineString(const QString &seperator, const QList<QString> &stringList);

UTILITIESSHARED_EXPORT QString combineVariant(const QString &seperator, const QList<QVariant> &variantList);

template <typename T>
QString combineNumber(const QString &seperator, const QList<T> &numberList)
{
    QString returnString;
    for (int i = 0; i < numberList.count(); i++)
    {
        returnString += QString::number(numberList[i]);
        if (i != numberList.count() - 1)
        {
            returnString += seperator;
        }
    }
    return returnString;
}

inline QString getCurrentDateTime()
{
    return QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz");
}

inline int getIntFromHexOrDecString(QString value)
{
    bool ok = false;
    uint result = 0;
    result = value.toInt(&ok, 16);       // hex == 255, ok == true
    if (ok) {
        return result;
    } else {
        result = value.toInt(&ok, 10);       // dec == 0, ok == false
    }
    return result;
}

template <typename T>
QVariantList toVariantList(QList<T> source)
{
    QVariantList qv;
    foreach (T item, source)
    {
        qv.append(item);
    }
    return qv;
}

template <typename T>
QList<T> unpackVariantList(QVariantList source)
{
    QList<T> list;
    foreach (const QVariant &qv, source)
    {
        list.append(qv.value<T>());
    }
    return list;
}

template <typename T>
T max(QList<T> &data)
{
    if (data.length() == 0)
    {
        qFatal("Can not get the max value for the reason that data length is zero!");
    }
    T maxData = data[0];
    for (int i = 1; i < data.length(); i++)
    {
        if (data[i] > maxData)
        {
            maxData = data[i];
        }
    }
    return maxData;
}

template <typename T>
T min(QList<T> &data)
{
    if (data.length() == 0)
    {
        qFatal("Can not get the min value for the reason that data length is zero!");
    }
    T minData = data[0];
    for (int i = 1; i < data.length(); i++)
    {
        if (data[i] < minData)
        {
            minData = data[i];
        }
    }
    return minData;
}

class AutoResetBool
{
public:
    AutoResetBool(bool *target, bool resetValue) : target(target), resetValue(resetValue) {}
    ~AutoResetBool()
    {
        *target = resetValue;
    }

private:
    bool *target;
    bool resetValue;
};

class AutoResetProperty
{
public:
    AutoResetProperty(QObject *target, QString property, QVariant value) : target(target), property(property), value(value) {}
    ~AutoResetProperty()
    {
        if (!target->setProperty(property.toUtf8(), value))
        {
            qCritical() << "Set" << QString("%1::%2").arg(target->metaObject()->className()).arg(property) << "to" << value << "failed!";
        }
    }

private:
    QObject *target;
    QString property;
    QVariant value;
};

typedef std::function<void()> AutoExecuteFunc;

class AutoExecute
{
public:
    AutoExecute(AutoExecuteFunc aef) : aef(aef) {}
    ~AutoExecute()
    {
        if (aef != nullptr)
        {
            aef();
        }
    }

private:
    AutoExecuteFunc aef = nullptr;
};

bool UTILITIESSHARED_EXPORT fitCircle(const QVector<QPointF> &points, QPointF &center, double &radius);

bool UTILITIESSHARED_EXPORT fitLine(const QVector<QPointF> &points, double &a, double &b, double &c);

bool UTILITIESSHARED_EXPORT silicoolWait(int timeout, std::function<bool()> checkFunc, int checkInterval = 20);

int UTILITIESSHARED_EXPORT getMethodIndex(const QMetaObject *metaObj, QMetaMethod::MethodType methodType, const QString &methodName);

extern Qt::HANDLE mainThreadID;
bool UTILITIESSHARED_EXPORT isInMainThread();

#define CHECK_POINTER(pointer)                                                                                                                       \
    if (pointer == nullptr)                                                                                                                          \
    {                                                                                                                                                \
        qFatal("%s is null!", #pointer);                                                                                                             \
    }

#define SILICOOL_DELETE(pointer)                                                                                                                     \
    if (pointer != nullptr)                                                                                                                          \
    {                                                                                                                                                \
        delete pointer;                                                                                                                              \
        pointer = nullptr;                                                                                                                           \
    }

#endif
