#ifndef CONFIGFILE_H
#define CONFIGFILE_H

#include "../utilities_global.h"
#include "commonmethod.h"
#include "configobject.h"
#include <QDateTime>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QJsonDocument>
#include <QObject>
#include <QTimer>

class UTILITIESSHARED_EXPORT ConfigFile : public QObject
{
    Q_OBJECT

public:
    ConfigFile(const QString &configBaseName, ConfigBase *configBase, const QString &fileName, bool encript = true);

    QString getFileName() const
    {
        return fileName;
    }

    bool populate(bool createEmptyFileIfFileNotExist = false);

    void setAutoSave(bool autoSave);

    void resetFileName(const QString &fileName);

public slots:
    void save();
    void requestSaveConfig();

private:
    bool populateImpl(bool createEmptyFileIfFileNotExist);

private:
    QString configBaseName;
    ConfigBase *configBase = nullptr;
    QString fileName;
    bool encript;
    int m_requestSaveConfigSlotIndex;
    QTimer saveConfigTimer;
};

#endif    // CONFIGFILE_H
