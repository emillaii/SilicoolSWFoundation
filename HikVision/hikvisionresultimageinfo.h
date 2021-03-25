#ifndef HIKVISIONRESULTIMAGEINFO_H
#define HIKVISIONRESULTIMAGEINFO_H

#include "hikvision_global.h"
#include "scvision.h"

class HIKVISIONSHARED_EXPORT HikVisionResultImageInfo : public PRResultImageInfo
{
public:
    QPointF m_point;

    double m_radius;

    QRectF m_rect;

    QLineF m_line;

    QString m_text;

    QList<QPointF> m_mpolygon;

    QPointF m_center;

    QPolygonF m_points;

    QList<QLineF> m_lines;
    Q_OBJECT

public:
    HikVisionResultImageInfo(QObject *parent = nullptr) : PRResultImageInfo(parent) {}

public:
};

#endif    // HIKVISIONRESULTIMAGEINFO_H
