#include "commonmethod.h"

QString combineString(const QString &seperator, const QList<QString> &stringList)
{
    QString returnString;
    for (int i = 0; i < stringList.count(); i++)
    {
        returnString += stringList[i];
        if (i != stringList.count() - 1)
        {
            returnString += seperator;
        }
    }
    return returnString;
}

QString combineVariant(const QString &seperator, const QList<QVariant> &variantList)
{
    QString returnString;
    for (int i = 0; i < variantList.count(); i++)
    {
        returnString += variantList[i].toString();
        if (i != variantList.count() - 1)
        {
            returnString += seperator;
        }
    }
    return returnString;
}

bool silicoolWait(int timeout, std::function<bool()> checkFunc, int checkInterval)
{
    bool _isInMainThread = isInMainThread();
    QElapsedTimer timer;
    timer.start();
    while (true)
    {
        if (checkFunc())
        {
            return true;
        }
        if (timeout != -1 && timer.elapsed() > timeout)
        {
            return false;
        }
        if (_isInMainThread)
        {
            QCoreApplication::processEvents(QEventLoop::AllEvents, checkInterval);
        }
        else
        {
            QThread::msleep(checkInterval);
        }
    }
}

int UTILITIESSHARED_EXPORT getMethodIndex(const QMetaObject *metaObj,
                                          QMetaMethod::MethodType methodType,
                                          const QString &methodName)
{
    for (int i = 0; i < metaObj->methodCount(); i++)
    {
        QMetaMethod metaMethod = metaObj->method(i);
        if (metaMethod.methodType() == methodType && metaMethod.name() == methodName)
        {
            return i;
        }
    }
    return -1;
}

Qt::HANDLE mainThreadID = QThread::currentThreadId();

bool isInMainThread()
{
    return QThread::currentThreadId() == mainThreadID;
}

bool fitCircle(const QVector<QPointF> &points, QPointF &center, double &radius)
{
    center.setX(0);
    center.setY(0);
    radius = 0.0;
    if (points.size() < 3)
    {
        return false;
    }

    double sum_x = 0.0, sum_y = 0.0;
    double sum_x2 = 0.0, sum_y2 = 0.0;
    double sum_x3 = 0.0, sum_y3 = 0.0;
    double sum_xy = 0.0, sum_x1y2 = 0.0, sum_x2y1 = 0.0;

    int N = points.size();
    for (int i = 0; i < N; i++)
    {
        double x = points[i].x();
        double y = points[i].y();
        double x2 = x * x;
        double y2 = y * y;
        sum_x += x;
        sum_y += y;
        sum_x2 += x2;
        sum_y2 += y2;
        sum_x3 += x2 * x;
        sum_y3 += y2 * y;
        sum_xy += x * y;
        sum_x1y2 += x * y2;
        sum_x2y1 += x2 * y;
    }

    double C, D, E, G, H;
    double a, b, c;

    C = N * sum_x2 - sum_x * sum_x;
    D = N * sum_xy - sum_x * sum_y;
    E = N * sum_x3 + N * sum_x1y2 - (sum_x2 + sum_y2) * sum_x;
    G = N * sum_y2 - sum_y * sum_y;
    H = N * sum_x2y1 + N * sum_y3 - (sum_x2 + sum_y2) * sum_y;
    a = (H * D - E * G) / (C * G - D * D);
    b = (H * C - E * D) / (D * D - G * C);
    c = -(a * sum_x + b * sum_y + sum_x2 + sum_y2) / N;

    center.setX(a / (-2));
    center.setY(b / (-2));
    radius = sqrt(a * a + b * b - 4 * c) / 2;
    return true;
}

bool fitLine(const QVector<QPointF> &points, double &a, double &b, double &c)
{
    int size = points.size();
    if (size < 2)
    {
        a = 0;
        b = 0;
        c = 0;
        return false;
    }
    double x_mean = 0;
    double y_mean = 0;

    for (int i = 0; i < size; i++)
    {
        x_mean += points[i].x();
        y_mean += points[i].y();
    }
    x_mean /= size;
    y_mean /= size;

    double Dxx = 0, Dxy = 0, Dyy = 0;

    for (int i = 0; i < size; i++)
    {
        Dxx += (points[i].x() - x_mean) * (points[i].x() - x_mean);
        Dxy += (points[i].x() - x_mean) * (points[i].y() - y_mean);
        Dyy += (points[i].y() - y_mean) * (points[i].y() - y_mean);
    }

    double lambda = ((Dxx + Dyy) - sqrt((Dxx - Dyy) * (Dxx - Dyy) + 4 * Dxy * Dxy)) / 2.0;
    double den = sqrt(Dxy * Dxy + (lambda - Dxx) * (lambda - Dxx));

    if (fabs(den) < 1e-5)
    {
        if (fabs(Dxx / Dyy - 1) < 1e-5)
        {
            return false;
        }
        else
        {
            a = 1;
            b = 0;
            c = -x_mean;
        }
    }
    else
    {
        a = Dxy / den;
        b = (lambda - Dxx) / den;
        c = -a * x_mean - b * y_mean;
    }
    return true;
}
