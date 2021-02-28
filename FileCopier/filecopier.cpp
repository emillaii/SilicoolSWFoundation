#include "filecopier.h"

FileCopier::FileCopier(QObject *parent) : ConfigObject(parent)
{
    m_sourceFiles = new ConfigArray(ConfigElementInfo::Other);
    m_destDirs = new ConfigArray(ConfigElementInfo::Other);
    m_sourceFiles->setSelectFileProperty();
    m_destDirs->setSelectFolderProperty();
    init();

    configFile = new ConfigFile("", this, "./fileCopierConfig.json");
    configFile->populate(true);
}

void FileCopier::copy()
{
    if (destDirs()->count() == 0)
    {
        pubMsg("Empty dest dirs");
        return;
    }
    if (sourceFiles()->count() == 0)
    {
        pubMsg("Empty source files");
        return;
    }
    for (int i = 0; i < sourceFiles()->count(); i++)
    {
        auto file = sourceFiles()->at(i).toString();
        QFileInfo fInfo(file);
        if (!fInfo.exists())
        {
            pubMsg(QString("File not exist: %1").arg(file));
            continue;
        }
        for (int j = 0; j < destDirs()->count(); j++)
        {
            auto destDir = destDirs()->at(j).toString();
            QDir d(destDir);
            if (!d.exists())
            {
                d.mkpath(d.absolutePath());
                pubMsg(QString("Create new dir: %1").arg(d.absolutePath()));
            }

            QString destFile = d.absoluteFilePath(fInfo.fileName());
            QFileInfo destFileInfo(destFile);
            if (destFileInfo.exists())
            {
                QFile::remove(destFile);
            }
            if (!QFile::copy(fInfo.absoluteFilePath(), destFile))
            {
                pubMsg(QString("Copy file %1 to %2 failed").arg(fInfo.absoluteFilePath()).arg(destFile));
            }
        }
    }
    pubMsg("Copy completed");
}
