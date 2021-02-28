#ifndef VISIONCONFIGDIR_H
#define VISIONCONFIGDIR_H

#include "scvision_global.h"
#include <QDir>
#include <QFileInfo>
#include <QString>

class SCVISIONSHARED_EXPORT VisionConfigDir
{
private:
    VisionConfigDir() {}

public:
    static VisionConfigDir &getIns();

    void setDutRelatedConfigDir(QString dir);

    QString dutRelatedConfigDir() const
    {
        return m_dutRelatedConfigDir;
    }

    QString prConfigDir() const
    {
        return m_prConfigDir;
    }

private:
    const QString PrDirName = "Pr";
    QString m_dutRelatedConfigDir;
    QString m_prConfigDir;
};

#endif    // VISIONCONFIGDIR_H
