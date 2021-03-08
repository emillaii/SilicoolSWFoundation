#ifndef EXTERNEXELAUCHER_H
#define EXTERNEXELAUCHER_H

#include "configManager/configfile.h"
#include "configManager/configobjectarray.h"
#include "uiHelper/uioperation.h"
#include "utilities_global.h"
#include <QFileInfo>
#include <QObject>
#include <QProcess>
#include <qmap.h>

class UTILITIESSHARED_EXPORT ExternExeConfig : public ConfigObject
{
    Q_OBJECT

    Q_PROPERTY(QString exeName READ exeName WRITE setExeName NOTIFY exeNameChanged)
    Q_PROPERTY(QString exeFileFullName READ exeFileFullName WRITE setExeFileFullName NOTIFY exeFileFullNameChanged)

    QString m_exeName;
    QString m_exeFileFullName;

public:
    Q_INVOKABLE ExternExeConfig(QObject *parent = nullptr) : ConfigObject(parent)
    {
        setSelectFileProperty("exeFileFullName", false);
        init();
    }
    QString exeName() const
    {
        return m_exeName;
    }
    QString exeFileFullName() const
    {
        return m_exeFileFullName;
    }

public slots:
    void setExeName(QString exeName)
    {
        if (m_exeName == exeName)
            return;

        m_exeName = exeName;
        emit exeNameChanged(m_exeName);
    }
    void setExeFileFullName(QString exeFileFullName)
    {
        if (m_exeFileFullName == exeFileFullName)
            return;

        m_exeFileFullName = exeFileFullName;
        emit exeFileFullNameChanged(m_exeFileFullName);
    }

signals:
    void exeNameChanged(QString exeName);
    void exeFileFullNameChanged(QString exeFileFullName);
};

class UTILITIESSHARED_EXPORT ExternExeLaucher : public QObject
{
    Q_OBJECT
public:
    explicit ExternExeLaucher(QObject *parent = nullptr);

    Q_INVOKABLE QStringList externExeNames() const;

    Q_INVOKABLE void open(QString exeName);

    ConfigObjectArray *getExternExeConfigs() const;

private:
    ConfigObjectArray *externExeConfigs;
    ConfigFile *externExeConfigsFile;
    QMap<QString, ExternExeConfig *> externExes;
};

#endif    // EXTERNEXELAUCHER_H
