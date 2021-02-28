#ifndef ADAPTVISIONPRRESULTIMAGEINFO_H
#define ADAPTVISIONPRRESULTIMAGEINFO_H

#include "adaptvision_global.h"
#include "scvision.h"

class ADAPTVISIONSHARED_EXPORT AdaptVisionPRResultImageInfo : public PRResultImageInfo
{
    Q_OBJECT

public:
    AdaptVisionPRResultImageInfo()
        : PRResultImageInfo(nullptr),
          rect_x(0),
          rect_y(0),
          rect_ori_x(0),
          rect_ori_y(0),
          rect_width(0),
          rect_height(0),
          rect_angle(0),
          result_ori_x(0),
          result_ori_y(0),
          result_x(0),
          result_y(0),
          resultString(""),
          radius(0),
          prType(0)
    {
    }

    // x y Rectangle upper left point pixel coordinate
    double rect_x;
    double rect_y;

    // ori x y Rectangle Mass center of the rectangle
    double rect_ori_x;
    double rect_ori_y;

    // Rectangle width height in pixel unit
    double rect_width;
    double rect_height;

    // Angle of the rectangle
    double rect_angle;

    // PR Result Original Point in pixel unit (without offset)

    double result_ori_x;
    double result_ori_y;

    // PR Result with user defined offset

    double result_x;
    double result_y;

    // Circle Related

    double radius;

    // PrType

    int prType;

    // Any result string
    QString resultString;
};

#endif    // ADAPTVISIONPRRESULTIMAGEINFO_H
