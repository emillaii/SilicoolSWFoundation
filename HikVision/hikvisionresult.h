#ifndef HIKVISIONRESULT_H
#define HIKVISIONRESULT_H

#include "hikvisionresultimageinfo.h"
#include <QMutex>
#include <QWaitCondition>
#include <qmap.h>

struct HikVisionResult
{
public:
    QString errString;
    int resultModuleId;
    double x = 0;
    double y = 0;
    double theta = 0;
    double width = 0;
    double height = 0;
    double radius = 0;
    double fsharpness = 0;
    HikVisionResultImageInfo *resultImageInfo;

    QWaitCondition waiter;
};

#endif    // HIKVISIONRESULT_H
