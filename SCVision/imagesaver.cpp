#include "imagesaver.h"

SILICOOL_CREATE_LOGGING_CATEGORY(visionCate, "Vision")

ImageSaver::ImageSaver(QObject *parent) : QThread(parent)
{

}

void ImageSaver::saveImage(QImage& image, QString dir, QString name)
{
    {
        QMutexLocker tmpLocker(&locker);
        imgQueue.enqueue(ImageSaveInfo(image, dir, name));
    }
    start();
}

void ImageSaver::run()
{
    while (true)
    {
        ImageSaveInfo imgSaveInfo;
        {
            QMutexLocker tmpLocker(&locker);
            if(imgQueue.isEmpty())
            {
                return;
            }
            imgSaveInfo = imgQueue.dequeue();
        }

        QDir qDir;
        if (!qDir.exists(imgSaveInfo.dir))
        {
            qDir.mkpath(imgSaveInfo.dir);
        }

        auto colorImg = imgSaveInfo.img.convertToFormat(QImage::Format_RGB888);

        QImageWriter imgWritter(QDir(imgSaveInfo.dir).absoluteFilePath(imgSaveInfo.name));
        if (!imgWritter.write(colorImg))
        {
            qCCritical(visionCate()) << "Save image failed!" << imgWritter.errorString();
        }
    }
}

