#include "proffset.h"

PrOffset::PrOffset()
{
    X = 0;
    Y = 0;
    O_X = 0;
    O_Y = 0;
    W = 0;
    H = 0;
    Theta = 0;
}

PrOffset::PrOffset(double x, double y, double theta)
{
    X = x;
    Y = y;
    Theta = theta;
}

void PrOffset::ReSet()
{
    X = 0;
    Y = 0;
    Theta = 0;
    O_X = 0;
    O_Y = 0;
    W = 0;
    H = 0;
}

QString PrOffset::toString() const
{
    return QString("x: %1, y: %2, theta: %3, w: %4, h: %5").arg(X).arg(Y).arg(Theta).arg(W).arg(H);
}

QString PrOffset::toShortString() const
{
    return QString("x: %1, y: %2, theta: %3").arg(X).arg(Y).arg(Theta);
}

QString PrOffset::toPureString() const
{
    return QString("%1,%2,%3").arg(X).arg(Y).arg(Theta);
}

PrOffset operator-(const PrOffset &left, const PrOffset &right)
{
    PrOffset prOffset;
    prOffset.X = left.X - right.X;
    prOffset.Y = left.Y - right.Y;
    prOffset.Theta = left.Theta - right.Theta;
    prOffset.O_X = left.O_X - right.O_X;
    prOffset.O_Y = left.O_Y - right.O_Y;
    return prOffset;
}

PrOffset operator+(const PrOffset &left, const PrOffset &right)
{
    PrOffset prOffset;
    prOffset.X = left.X + right.X;
    prOffset.Y = left.Y + right.Y;
    prOffset.Theta = left.Theta + right.Theta;
    prOffset.O_X = left.O_X + right.O_X;
    prOffset.O_Y = left.O_Y + right.O_Y;
    return prOffset;
}

PrOffset operator*(const PrOffset &offset, double multiplier)
{
    PrOffset newOffset;
    newOffset.X = offset.X * multiplier;
    newOffset.Y = offset.Y * multiplier;
    newOffset.O_X = offset.O_X * multiplier;
    newOffset.O_Y = offset.O_Y * multiplier;
    newOffset.Theta = offset.Theta;
    return newOffset;
}

PrOffset average(const QList<PrOffset> &offsets)
{
    if (offsets.count() == 0)
    {
        return PrOffset();
    }
    PrOffset sum;
    for (int i = 0; i < offsets.count(); i++)
    {
        sum = sum + offsets[i];
    }
    PrOffset ave = sum * (1.0 / offsets.count());
    ave.Theta /= offsets.count();
    return ave;
}
