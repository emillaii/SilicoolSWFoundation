#include "scdispenser.h"

SCDispenser::SCDispenser(const QLoggingCategory &logCate, DispenserConfig *dispenserConfig, QObject *parent)
    : QObject(parent), logCate(logCate), m_dispConfig(dispenserConfig)
{
}

void SCDispenser::init(SCAxis *xAxis, SCAxis *yAxis, SCAxis *zAxis, SCDO *shotGlueOut)
{
    this->xAxis = xAxis;
    this->yAxis = yAxis;
    this->zAxis = zAxis;
    this->shotGlueOut = shotGlueOut;
    isInit = true;
}

void SCDispenser::dispensePath(QVector<PathEndPoint> &path)
{
    checkIsInit();
    if (path.length() < 2)
    {
        throw SilicolAbort(QString("Dispense path length error. Len: %1").arg(path.length()), EX_LOCATION);
    }

    addExtraPath(path);
    setDispenseVel(path);
    setLineDisable(path);
    __dispensePath(path);
}

void SCDispenser::dispenseLine(QPointF startPos, QPointF endPos, double dispenseHeight, double zSafetyHeight)
{
    checkIsInit();
    moveToStartPos(startPos.x(), startPos.y(), dispenseHeight, zSafetyHeight);

    QVector<PathEndPoint> path;
    path.append(PathEndPoint(startPos.x(), startPos.y(), dispenseHeight, LinePath));
    path.append(PathEndPoint(endPos.x(), endPos.y(), dispenseHeight, LinePath));
    setDispenseVel(path);
    try
    {
        __dispensePath(path);
    }
    catch (SilicoolException &se)
    {
        zAxis->absMove(zSafetyHeight);
        throw se;
    }
    zAxis->absMove(zSafetyHeight);
}

void SCDispenser::dispenseRect(QPointF leftTopPos, double width, double height, double dispenseHeight, double zSafetyHeight)
{
    checkIsInit();
    moveToStartPos(leftTopPos.x(), leftTopPos.y(), dispenseHeight, zSafetyHeight);

    QVector<PathEndPoint> path;
    path.append(PathEndPoint(leftTopPos.x(), leftTopPos.y(), dispenseHeight, LinePath));
    path.append(PathEndPoint(leftTopPos.x() + width, leftTopPos.y(), dispenseHeight, LinePath));
    path.append(PathEndPoint(leftTopPos.x() + width, leftTopPos.y() + height, dispenseHeight, LinePath));
    path.append(PathEndPoint(leftTopPos.x(), leftTopPos.y() + height, dispenseHeight, LinePath));
    addExtraPath(path);
    setDispenseVel(path);
    setLineDisable(path);
    try
    {
        __dispensePath(path);
    }
    catch (SilicoolException &se)
    {
        zAxis->absMove(zSafetyHeight);
        throw se;
    }
    zAxis->absMove(zSafetyHeight);
}

void SCDispenser::moveToStartPos(double x, double y, double z, double zSafetyHeight)
{
    zAxis->absMove(zSafetyHeight);
    xAxis->absMove(x, false);
    yAxis->absMove(y);
    xAxis->waitProfilerStopped();
    zAxis->absMove(z);
}

void SCDispenser::__dispensePath(QVector<PathEndPoint> &path)
{
    xAxis->absMove(path[0].x, false);
    yAxis->absMove(path[0].y, false);
    zAxis->absMove(path[0].z);
    xAxis->waitProfilerStopped();
    yAxis->waitProfilerStopped();

    for (int i = 0; i < path.count(); i++)
    {
        qCInfo(logCate) << QString("Path %1, %2").arg(i).arg(path[i].toString());
    }
    enterInterpolationMode();
    try
    {
        executePath(path);
        QThread::msleep(50);
        waitDone();
    }
    catch (SilicoolException &se)
    {
        exitInterpolationMode();
        shotGlueOut->set(false);
        throw se;
    }
    exitInterpolationMode();
}

void SCDispenser::waitDone(int timeout)
{
    QElapsedTimer timer;
    timer.start();
    while (true)
    {
        if (isFinished())
        {
            break;
        }
        QThread::msleep(1);
        if (timer.elapsed() > timeout)
        {
            throw SilicolAbort("Wait dispensing done timeout!");
        }
    }
}

double SCDispenser::getGeneraDispVel(int index)
{
    if (m_dispConfig->generalDispenserConfig()->useLineSpeeds() && index < m_dispConfig->generalDispenserConfig()->lineSpeeds()->count())
    {
        return m_dispConfig->generalDispenserConfig()->lineSpeeds()->at(index).toDouble();
    }
    return m_dispConfig->generalDispenserConfig()->maxSpeed();
}

bool SCDispenser::getLineDisabled(int index)
{
    if (m_dispConfig->generalDispenserConfig()->useLineDisable() && index < m_dispConfig->generalDispenserConfig()->lineDisabled()->count())
    {
        return m_dispConfig->generalDispenserConfig()->lineDisabled()->at(index).toBool();
    }
    return false;
}

void SCDispenser::addExtraPath(QVector<PathEndPoint> &path)
{
    if (m_dispConfig->addPathToStartPos())
    {
        PathEndPoint startPos = path[0];
        path.append(startPos);
    }
    if (!m_dispConfig->useFlyDispensing() && m_dispConfig->generalDispenserConfig()->addAnExtraPath())
    {
        PathEndPoint firstPos = path[1];
        path.append(firstPos);
    }
}

void SCDispenser::setDispenseVel(QVector<PathEndPoint> &path)
{
    if (m_dispConfig->useFlyDispensing())
    {
        for (int i = 0; i < path.count(); i++)
        {
            path[i].maxVel = m_dispConfig->flyDispenserConfig()->maxSpeed();
            path[i].endVel = m_dispConfig->flyDispenserConfig()->maxSpeed();
        }
        path[path.count() - 1].endVel = 0;
    }
    else
    {
        for (int i = 0; i < path.count(); i++)
        {
            if (i == 0)
            {
                path[i].maxVel = m_dispConfig->generalDispenserConfig()->maxSpeed();
            }
            else
            {
                path[i].maxVel = getGeneraDispVel(i - 1);
            }
            if (i == path.count() - 1)
            {
                path[i].endVel = 0;
            }
            else
            {
                path[i].endVel = getGeneraDispVel(i);
            }
        }
    }
}

void SCDispenser::setLineDisable(QVector<PathEndPoint> &path)
{
    for (int i = 0; i < path.count(); i++)
    {
        if (i <= 1 || i == path.count() - 1)
        {
            path[i].isDisabled = false;
        }
        else
        {
            path[i].isDisabled = getLineDisabled(i - 1);
        }
    }
}

void SCDispenser::checkIsInit()
{
    if (!isInit)
    {
        throw SilicolAbort("SCDispenser is not inited!");
    }
}

void SCDispenser::generateConfig(int pointCount, ConfigArray *config, QVariant defaultValue)
{
    if (pointCount < 2)
    {
        return;
    }
    if (m_dispConfig->useFlyDispensing())
    {
        return;
    }
    if (m_dispConfig->addPathToStartPos())
    {
        pointCount++;
    }
    if (m_dispConfig->generalDispenserConfig()->addAnExtraPath())
    {
        pointCount++;
    }
    int lineCount = pointCount - 1;

    while (config->count() > lineCount)
    {
        config->remove(config->count() - 1);
    }
    while (config->count() < lineCount)
    {
        config->add(config->count());
    }
    for (int i = 0; i < config->count(); i++)
    {
        config->setConfig(i, defaultValue);
    }
}
