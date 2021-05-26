#include "xtdispenser.h"

XtDispenser::XtDispenser(const QLoggingCategory &logCate, DispenserConfig *dispenserConfig, QObject *parent)
    : SCDispenser(logCate, dispenserConfig, parent)
{
}

void XtDispenser::shotGlueDotInLineCenter(QPointF xyStartPos,
                                          QPointF xyEndPos,
                                          double shotGlueDotZHeight,
                                          double safetyZHeight,
                                          double shotGlueTime,
                                          double shotGlueDelay)
{
    checkIsInit();
    if (xtFlyDispensingConfig == nullptr)
    {
        throw SilicolAbort("xtFlyDispensingConfig was not set!", EX_LOCATION);
    }

    moveToStartPos(xyStartPos.x(), xyStartPos.y(), shotGlueDotZHeight, safetyZHeight);
    XT_Controler_Extend::Clear_Exec(curveId);
    XT_Controler_Extend::Curve_Reset_Inv_Param(curveId);
    QVector<int> axis;
    setCurveParam(2, axis);
    double startPos[2] = { xyStartPos.x(), xyStartPos.y() };
    double endPos[2] = { xyEndPos.x(), xyEndPos.y() };
    int pointIndex = 0;
    XT_Controler_Extend::Append_Line_Pos(curveId, 2, axis.data(), startPos, xtFlyDispensingConfig->calibrateMaxVel(), 0,
                                         0, pointIndex);
    XT_Controler_Extend::Append_Line_Pos(curveId, 2, axis.data(), endPos, xtFlyDispensingConfig->calibrateMaxVel(), 0,
                                         0, pointIndex);
    double lenght = XT_Controler_Extend::Curve_Get_LengthPos(curveId, pointIndex);
    pointIndex = 0;
    XT_Controler_Extend::Curve_Set_Trig_Output_Pulse(curveId, pointIndex, lenght / 2, shotGlueDelay,
                                                     xtShotGlueOut->getId(), true, shotGlueTime);
    XT_Controler_Extend::Exec_Curve(curveId, threadCurve, threadTrig, 1);

    QThread::msleep(50);
    waitDone();
    xtZAxis->absMove(safetyZHeight);
}

void XtDispenser::init(SCAxis *xAxis, SCAxis *yAxis, SCAxis *zAxis, SCDO *shotGlueOut)
{
#define CHECK_XT_PRT(ptr)                                                                                              \
    if (ptr == nullptr)                                                                                                \
    {                                                                                                                  \
        throw SilicolAbort(QString("XtDispenser init failed! %1 is null!").arg(#ptr));                                 \
    }

    this->xtXAxis = qobject_cast<XtAxis *>(xAxis);
    this->xtYAxis = qobject_cast<XtAxis *>(yAxis);
    this->xtZAxis = qobject_cast<XtAxis *>(zAxis);
    this->xtShotGlueOut = qobject_cast<XtDO *>(shotGlueOut);

    CHECK_XT_PRT(xtXAxis);
    CHECK_XT_PRT(xtYAxis);
    CHECK_XT_PRT(xtZAxis);
    CHECK_XT_PRT(xtShotGlueOut);

    mallocResource();
    SCDispenser::init(xAxis, yAxis, zAxis, shotGlueOut);
}

void XtDispenser::executePath(QVector<PathEndPoint> &path)
{
    XT_Controler_Extend::Clear_Exec(curveId);
    if (dispenserConfig()->useFlyDispensing())
    {
        if (xtFlyDispensingConfig == nullptr)
        {
            throw SilicolAbort("xtFlyDispensingConfig was not set!", EX_LOCATION);
        }
        flyDispenseImpl(path);
    }
    else
    {
        generalDispenseImpl(path);
    }
}

void XtDispenser::mallocResource()
{
    if (!isMallocResource)
    {
        this->curveId = XtAxis::generateNewCalcSlotID();
        this->threadCurve = XtAxis::generateNewThreadID();
        this->threadTrig = XtAxis::generateNewThreadID();
        isMallocResource = true;
    }
}

void XtDispenser::setCurveParam(int axisCount, QVector<int> &axis)
{
    if (axisCount < 1 || axisCount > 3)
    {
        throw SilicolAbort(QString("Error axis count: %1").arg(axisCount), EX_LOCATION);
    }
    QVector<int> axisCombine;
    QVector<double> axis_max_vel;
    QVector<double> axis_max_acc;
    QVector<double> axis_max_jerk;
    XtAxis *motor;
    for (int i = 0; i < axisCount; i++)
    {
        if (i == 0)
        {
            motor = xtXAxis;
        }
        else if (i == 1)
        {
            motor = xtYAxis;
        }
        else
        {
            motor = xtZAxis;
        }

        axis.append(motor->axisId());
        axisCombine.append(1);
        axis_max_vel.append(motor->maxVelocity());
        axis_max_acc.append(motor->maxAcc());
        axis_max_jerk.append(motor->maxJerk());
    }

    if (1
        != XT_Controler_Extend::Set_Curve_Param(curveId, 0.1, axisCount, axis.data(), axis_max_vel.data(),
                                                axis_max_acc.data(), axis_max_jerk.data(), axisCombine.data()))
    {
        throw SilicolAbort("Set_Curve_Param failed!");
    }
}

void XtDispenser::generalDispenseImpl(QVector<PathEndPoint> &dispensePath)
{
    QVector<int> axis;
    setCurveParam(2, axis);
    int pointIndex = 0;
    int res = 0;
    auto generaDispenserConfig = dispenserConfig()->generalDispenserConfig();
    for (int i = 0; i < dispensePath.length(); i++)
    {
        if (dispensePath[i].pathType == ArcPath)
        {
            throw SilicolAbort("Unimplemented path type: ArcPath");
        }

        res = XT_Controler_Extend::Append_Line_Pos(curveId, 2, axis.data(), dispensePath[i].dataPtr(),
                                                   dispensePath[i].maxVel, dispensePath[i].endVel, 0, pointIndex);
        if (1 != res)
        {
            throw SilicolAbort(QString("Append_Line_Pos failed! Index: %1").arg(i));
        }

        if (i == 1)
        {
            double firstLineLen = XT_Controler_Extend::Curve_Get_LengthPos(curveId, pointIndex);
            if (generaDispenserConfig->openOffset() > firstLineLen)
            {
                qWarning() << "openOffset too large, be auto changed to" << firstLineLen;
                generaDispenserConfig->setOpenOffset(round(firstLineLen * 10000) / 10000);
            }
            if (generaDispenserConfig->openOffset() < -firstLineLen)
            {
                qWarning() << "openOffset too small, be auto changed to" << -firstLineLen;
                generaDispenserConfig->setOpenOffset(round(-firstLineLen * 10000) / 10000);
            }

            if (generaDispenserConfig->openOffset() >= 0)
            {
                res = XT_Controler_Extend::Curve_Set_Cur_Trig_Output(curveId, 0, generaDispenserConfig->openOffset(), 0,
                                                                     xtShotGlueOut->getId(), 1);
            }
            else
            {
                res = XT_Controler_Extend::Curve_Set_Cur_Trig_Output(curveId, 1, -generaDispenserConfig->openOffset(),
                                                                     0, xtShotGlueOut->getId(), 1);
            }

            if (1 != res)
            {
                throw SilicolAbort("Set_Cur_Trig_Output Failed!", EX_LOCATION);
            }
        }

        if (i == dispensePath.length() - 1)
        {
            double lastLineLen = XT_Controler_Extend::Curve_Get_LengthPos(curveId, pointIndex);
            if (generaDispenserConfig->closeOffset() > lastLineLen)
            {
                qWarning() << "closeOffset too large, be auto changed to" << lastLineLen;
                generaDispenserConfig->setCloseOffset(round(lastLineLen * 10000) / 10000);
            }
            if (generaDispenserConfig->closeOffset() < -lastLineLen)
            {
                qWarning() << "closeOffset too small, be auto changed to" << -lastLineLen;
                generaDispenserConfig->setCloseOffset(round(-lastLineLen * 10000) / 10000);
            }

            if (generaDispenserConfig->closeOffset() >= 0)
            {
                res = XT_Controler_Extend::Curve_Set_Cur_Trig_Output(curveId, 0, generaDispenserConfig->closeOffset(),
                                                                     0, xtShotGlueOut->getId(), 0);
            }
            else
            {
                res = XT_Controler_Extend::Curve_Set_Cur_Trig_Output(curveId, 1, -generaDispenserConfig->closeOffset(),
                                                                     0, xtShotGlueOut->getId(), 0);
            }

            if (1 != res)
            {
                throw SilicolAbort("Set_Cur_Trig_Output Failed!", EX_LOCATION);
            }
        }
    }

    res = XT_Controler_Extend::Exec_Curve(curveId, threadCurve, threadTrig, 1);
    if (1 != res)
    {
        throw SilicolAbort("Exec_Curve failed!");
    }
}

void XtDispenser::flyDispenseImpl(QVector<PathEndPoint> &dispensePath)
{
    QVector<int> axis;
    setCurveParam(2, axis);
    int pointIndex = 0;
    int res = 0;
    double allLineLen = 0;
    for (int i = 0; i < dispensePath.length(); i++)
    {
        if (dispensePath[i].pathType == ArcPath)
        {
            throw SilicolAbort("Unimplemented path type: ArcPath");
        }

        res = XT_Controler_Extend::Append_Line_Pos(curveId, 2, axis.data(), dispensePath[i].dataPtr(),
                                                   dispensePath[i].maxVel, dispensePath[i].endVel, 0, pointIndex);
        if (1 != res)
        {
            throw SilicolAbort(QString("Append_Line_Pos failed! Index: %1").arg(i));
        }

        allLineLen += XT_Controler_Extend::Curve_Get_LengthPos(curveId, pointIndex);
    }

    auto flyDispensingConfig = dispenserConfig()->flyDispenserConfig();
    int glueDotCount = allLineLen / flyDispensingConfig->shotGlueDotStep();
    double step = allLineLen / glueDotCount;
    for (int i = 0; i < glueDotCount; i++)
    {
        if (1
            != XT_Controler_Extend::Curve_Set_Trig_Output_Pulse(
                curveId, 0, i * step, xtFlyDispensingConfig->shotGlueDotDelay(), xtShotGlueOut->getId(), true,
                flyDispensingConfig->shotGlueDotTime()))
        {
            throw SilicolAbort(QString("Set_Trig_Output_Pulse failed! Index: %1").arg(i));
        }
    }
    res = XT_Controler_Extend::Exec_Curve(curveId, threadCurve, threadTrig, 1);
    if (1 != res)
    {
        throw SilicolAbort("Exec_Curve failed!");
    }
}
