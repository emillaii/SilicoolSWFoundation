#ifndef DUTTYPEMANAGER_H
#define DUTTYPEMANAGER_H

#include "uiHelper/uioperation.h"
#include "utilities_global.h"
#include <QDir>
#include <QObject>
#include <QQmlListProperty>
#include <basicconfig.h>

class UTILITIESSHARED_EXPORT DutTypeManager : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QStringList dutTypes READ dutTypes NOTIFY dutTypesChanged)
    Q_PROPERTY(int launchResult READ launchResult WRITE setLaunchResult NOTIFY launchResultChanged)

public:
    explicit DutTypeManager(BasicConfig *basicConfig, QObject *parent = nullptr)
        : QObject(parent), basicConfig(basicConfig)
    {
        updateDutTypes();
    }

    QStringList dutTypes() const
    {
        return m_dutTypes;
    }

    int launchResult() const
    {
        return m_launchResult;
    }

signals:
    void dutTypesChanged(QStringList dutTypes);

    void launchResultChanged(int launchResult);

public slots:
    void addDutType(QString dutType, QString copyFromDutType);

    void removeDutType(QString dutType);

    void setLaunchResult(int launchResult)
    {
        if (m_launchResult == launchResult)
            return;

        m_launchResult = launchResult;
        emit launchResultChanged(m_launchResult);
    }

private:
    void copyDir(const QString &source, const QString &target);

    void removeDir(const QString &dirName);

    void updateDutTypes();

private:
    QStringList m_dutTypes;
    QString DefaultDut = "DefaultDut";
    int m_launchResult = -1;
    BasicConfig *basicConfig;
};

#endif    // DUTTYPEMANAGER_H
