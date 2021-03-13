#ifndef HIKVISIONRESULTIMAGEINFO_H
#define HIKVISIONRESULTIMAGEINFO_H

#include "scvision.h"
#include "hikvision_global.h"

class HIKVISIONSHARED_EXPORT HikVisionResultImageInfo : public PRResultImageInfo
{
    Q_OBJECT

public:
    HikVisionResultImageInfo(QObject *parent = nullptr) : PRResultImageInfo(parent) {}

public:
};

#endif    // HIKVISIONRESULTIMAGEINFO_H
