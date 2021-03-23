#ifndef PATHEDITORCONFIG_H
#define PATHEDITORCONFIG_H

#include "configManager/configobject.h"
#include "utilities_global.h"

class UTILITIESSHARED_EXPORT PathEditorConfig : public ConfigObject
{
    Q_OBJECT

    Q_PROPERTY(QString imageDir READ imageDir WRITE setImageDir NOTIFY imageDirChanged)
    Q_PROPERTY(QString pathDir READ pathDir WRITE setPathDir NOTIFY pathDirChanged)

public:
    PathEditorConfig(QObject *parent = nullptr) : ConfigObject(parent)
    {
        init();
    }

    QString imageDir() const
    {
        return m_imageDir;
    }

    QString pathDir() const
    {
        return m_pathDir;
    }

public slots:
    void setImageDir(QString imageDir)
    {
        if (m_imageDir == imageDir)
            return;

        m_imageDir = imageDir;
        emit imageDirChanged(m_imageDir);
    }

    void setPathDir(QString pathDir)
    {
        if (m_pathDir == pathDir)
            return;

        m_pathDir = pathDir;
        emit pathDirChanged(m_pathDir);
    }

signals:
    void imageDirChanged(QString imageDir);

    void pathDirChanged(QString pathDir);

private:
    QString m_imageDir;
    QString m_pathDir;
};

#endif    // PATHEDITORCONFIG_H
