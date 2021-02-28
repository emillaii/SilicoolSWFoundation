#include "visionconfigdir.h"

VisionConfigDir &VisionConfigDir::getIns()
{
    static VisionConfigDir instance;
    return instance;
}

void VisionConfigDir::setDutRelatedConfigDir(QString dir)
{
    m_dutRelatedConfigDir = dir;
    QDir d(dir);
    d.mkpath(PrDirName);
    m_prConfigDir = QFileInfo(d.filePath(PrDirName)).absoluteFilePath() + "/";
}
