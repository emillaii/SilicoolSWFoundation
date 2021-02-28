#ifndef PROFFSET_H
#define PROFFSET_H

#include "scvision_global.h"
#include <QList>

struct SCVISIONSHARED_EXPORT PrOffset
{
public:
    PrOffset();

    PrOffset(double x, double y, double theta);

    void ReSet();

    QString toString() const;

    QString toShortString() const;

    QString toPureString() const;

    double X;
    double Y;
    double Theta;
    double W;
    double H;
    double O_X;
    double O_Y;
};

PrOffset SCVISIONSHARED_EXPORT operator-(const PrOffset &left, const PrOffset &right);
PrOffset SCVISIONSHARED_EXPORT operator+(const PrOffset &left, const PrOffset &right);
PrOffset SCVISIONSHARED_EXPORT operator*(const PrOffset &offset, double multiplier);
PrOffset SCVISIONSHARED_EXPORT average(const QList<PrOffset> &offsets);

#endif    // PROFFSET_H
