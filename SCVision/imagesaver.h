#ifndef IMAGESAVER_H
#define IMAGESAVER_H

#include <QObject>
#include <QImage>
#include <QThread>
#include <QMutex>
#include <QMutexLocker>
#include <QQueue>
#include <QDir>
#include <QImageWriter>
#include <QDebug>
#include "scvision_global.h"
#include "loging/Logger.h"

SILICOOL_DECLARE_LOGGING_CATEGORY(visionCate, SCVISIONSHARED_EXPORT)

struct ImageSaveInfo
{
public:
    ImageSaveInfo(QImage& img, QString dir, QString name):img(img), dir(dir), name(name)
    {

    }

    ImageSaveInfo()
    {

    }

    QImage img;
    QString dir;
    QString name;
};

class SCVISIONSHARED_EXPORT ImageSaver : public QThread
{
    Q_OBJECT

    explicit ImageSaver(QObject *parent = nullptr);

public:
    static ImageSaver& getIns()
    {
        static ImageSaver instance;
        return instance;
    }

    void saveImage(QImage& image, QString dir, QString name);

    // QThread interface
protected:
    void run();

private:
    QQueue<ImageSaveInfo> imgQueue;
    QMutex locker;
};

#endif // IMAGESAVER_H
