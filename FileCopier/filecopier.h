#ifndef FILECOPIER_H
#define FILECOPIER_H

#include "configarray.h"
#include "configfile.h"
#include "configobject.h"
#include <QFileInfo>
#include <QObject>

class FileCopier : public ConfigObject
{
    Q_OBJECT

    Q_PROPERTY(ConfigArray *sourceFiles READ sourceFiles)
    Q_PROPERTY(ConfigArray *destDirs READ destDirs)

public:
    explicit FileCopier(QObject *parent = nullptr);

    ConfigArray *destDirs() const
    {
        return m_destDirs;
    }

    ConfigArray *sourceFiles() const
    {
        return m_sourceFiles;
    }

signals:
    void pubMsg(QString msg);

public slots:
    void copy();

private:
    ConfigArray *m_destDirs;
    ConfigFile *configFile;
    ConfigArray *m_sourceFiles;
};

#endif    // FILECOPIER_H
