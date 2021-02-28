#ifndef SCGRABER_H
#define SCGRABER_H

#include <QObject>
#include <opencv2/opencv.hpp>
class SCGraber : public QObject
{
    Q_OBJECT
public:
    explicit SCGraber(QObject *parent = nullptr);
protected:
    virtual void enumerateDevice() =0;
    virtual void releaseDevice() =0;
    virtual void openDevice() = 0;
    virtual void closeDevice() = 0;
    virtual void startCamera() = 0;
    virtual QImage grabImage() = 0;
    virtual cv::Mat grabImageCV() = 0;
signals:

public slots:
};

#endif // SCGRABER_H
