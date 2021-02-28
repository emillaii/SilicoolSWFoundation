#include "calibration.h"

Calibration::Calibration(CalibrationConfig *config, VisionLocation *location, QObject *parent) : QObject(parent), m_config(config), location(location)
{
}

QString Calibration::calibrationName() const
{
    return m_config->calibrationName();
}

void Calibration::performCalibration()
{
    qCInfo(visionCate()) << "Performing calibration" << calibrationName();

    QVector<QPointF> pixelPoints;
    QVector<QPointF> motorPoints;
    if (m_config->calibrationFunction() == CalibrationConfig::Custom)
    {
        SC_ASSERT(customCaliFunc != nullptr);
        customCaliFunc(m_config, pixelPoints, motorPoints);
    }
    else
    {
        getCorrespondingPixelMotorPoints(pixelPoints, motorPoints);
    }

    calcAffineTransform(pixelPoints, motorPoints);
}

void Calibration::performPRResult(PrOffset offset)
{
    if (m_config->calibrationFunction() == CalibrationConfig::Custom)
    {
        SC_ASSERT(customPerformPrResultFunc != nullptr);
        customPerformPrResultFunc(offset);
    }
    else
    {
        QVariantList args1;
        args1 << offset.X * -1;
        MotionManager::getIns().executeInstruction(MotionElement::Axis, m_config->motorXName(), "relMove", args1);

        QVariantList args2;
        args2 << offset.Y * -1;
        MotionManager::getIns().executeInstruction(MotionElement::Axis, m_config->motorYName(), "relMove", args2);
    }
}

void Calibration::performPROriginResult(PrOffset offset)
{
    QVariantList args1;
    args1 << offset.O_X * -1;
    MotionManager::getIns().executeInstruction(MotionElement::Axis, m_config->motorXName(), "relMove", args1);

    QVariantList args2;
    args2 << offset.O_Y * -1;
    MotionManager::getIns().executeInstruction(MotionElement::Axis, m_config->motorYName(), "relMove", args2);
}

QPointF Calibration::getOnePxielDistance()
{
    auto caliResult = m_config->calibrationResult();
    if (!caliResult->isValid())
    {
        return QPointF();
    }
    else
    {
        return caliResult->pixelToMech(1, 1) - caliResult->pixelToMech(0, 0);
    }
}

QPointF Calibration::getOneXPxielDistance()
{
    auto caliResult = m_config->calibrationResult();
    if (!caliResult->isValid())
    {
        return QPointF();
    }
    else
    {
        return caliResult->pixelToMech(1, 0) - caliResult->pixelToMech(0, 0);
    }
}

QPointF Calibration::getOneYPxielDistance()
{
    auto caliResult = m_config->calibrationResult();
    if (!caliResult->isValid())
    {
        return QPointF();
    }
    else
    {
        return caliResult->pixelToMech(0, 1) - caliResult->pixelToMech(0, 0);
    }
}

void Calibration::setCustomCaliFunc(CustomCaliFunc customCaliFunc, CustomPerformPrResultFunc customPerformPrResultFunc)
{
    this->customCaliFunc = customCaliFunc;
    this->customPerformPrResultFunc = customPerformPrResultFunc;
}

void Calibration::setCustomGetPixelPointFunc(CustomGetPixelPointFunc customGetPixelPointFunc)
{
    this->customGetPixelPointFunc = customGetPixelPointFunc;
}

QPointF Calibration::getPixelPoint()
{
    if (customGetPixelPointFunc != nullptr)
    {
        return customGetPixelPointFunc();
    }
    else
    {
        PRResultStruct prResult;
        if (!location->performPR(prResult))
        {
            throw SilicolAbort(tr("%1 perform PR failed!").arg(location->locationName()));
        }
        return QPointF(prResult.x, prResult.y);
    }
}

void Calibration::getCorrespondingPixelMotorPoints(QVector<QPointF> &pixelPoints, QVector<QPointF> &motorPoints)
{
    QString firstAxisName = m_config->moveXFirst() ? m_config->motorXName() : m_config->motorYName();
    QString anotherAxisName = m_config->moveXFirst() ? m_config->motorYName() : m_config->motorXName();
    double firstAxisStep = m_config->moveXFirst() ? m_config->calibrationXStep() : m_config->calibrationYStep();
    double anotherAxisStep = m_config->moveXFirst() ? m_config->calibrationYStep() : m_config->calibrationXStep();

    QStringList xyNames;
    xyNames << m_config->motorXName() << m_config->motorYName();
    QList<double> stepBeforeCali;

    QList<CalibrationMoveInfo> caliMoveInfos;
    if (m_config->calibrationFunction() == CalibrationConfig::ThreePoints || m_config->calibrationFunction() == CalibrationConfig::FourPoints)
    {
        stepBeforeCali << -m_config->calibrationXStep() / 2 << -m_config->calibrationYStep() / 2;

        caliMoveInfos.append(CalibrationMoveInfo(firstAxisName, firstAxisStep));
        caliMoveInfos.append(CalibrationMoveInfo(anotherAxisName, anotherAxisStep));
        if (m_config->calibrationFunction() == CalibrationConfig::FourPoints)
        {
            caliMoveInfos.append(CalibrationMoveInfo(firstAxisName, -firstAxisStep));
        }
    }
    else if (m_config->calibrationFunction() == CalibrationConfig::NinePoints)
    {
        stepBeforeCali << -m_config->calibrationXStep() << -m_config->calibrationYStep();

        caliMoveInfos.append(CalibrationMoveInfo(firstAxisName, firstAxisStep));
        caliMoveInfos.append(CalibrationMoveInfo(firstAxisName, firstAxisStep));
        caliMoveInfos.append(CalibrationMoveInfo(anotherAxisName, anotherAxisStep));
        caliMoveInfos.append(CalibrationMoveInfo(firstAxisName, -firstAxisStep));
        caliMoveInfos.append(CalibrationMoveInfo(firstAxisName, -firstAxisStep));
        caliMoveInfos.append(CalibrationMoveInfo(anotherAxisName, anotherAxisStep));
        caliMoveInfos.append(CalibrationMoveInfo(firstAxisName, firstAxisStep));
        caliMoveInfos.append(CalibrationMoveInfo(firstAxisName, firstAxisStep));
    }
    else
    {
        throw SilicolAbort(
            tr("Unknown calibration function: %1").arg(CalibrationConfig::calibrationFunctionEnumInfo().enumToName(m_config->calibrationFunction())));
    }

    QVariantList velAcc;
    velAcc << m_config->calibrationVel() << m_config->calibrationAcc();
    MotionManager::getIns().runInstruction(MotionElement::Axis, m_config->motorXName(), "setNextMoveVelAcc", velAcc);
    MotionManager::getIns().runInstruction(MotionElement::Axis, m_config->motorYName(), "setNextMoveVelAcc", velAcc);

    AutoExecute ae([this] {
        MotionManager::getIns().runInstruction(MotionElement::Axis, m_config->motorXName(), "resetMaxVel");
        MotionManager::getIns().runInstruction(MotionElement::Axis, m_config->motorXName(), "resetMaxAcc");
        MotionManager::getIns().runInstruction(MotionElement::Axis, m_config->motorYName(), "resetMaxVel");
        MotionManager::getIns().runInstruction(MotionElement::Axis, m_config->motorYName(), "resetMaxAcc");
    });

    MotionManager::getIns().moveMultiAxes(xyNames, stepBeforeCali, false);
    if (m_config->calibrationDelay() > 0)
    {
        QThread::msleep(m_config->calibrationDelay());
    }

    pixelPoints.clear();
    motorPoints.clear();

    for (int i = -1; i < caliMoveInfos.count(); i++)
    {
        if (i >= 0)
        {
            QVariantList args;
            args << caliMoveInfos[i].step;
            MotionManager::getIns().runInstruction(MotionElement::Axis, caliMoveInfos[i].axisName, "relMove", args);
            if (m_config->calibrationDelay() > 0)
            {
                QThread::msleep(m_config->calibrationDelay());
            }
        }
        pixelPoints.append(getPixelPoint(m_config->prCountForEachPoint(), m_config->prErrorLimit()));
        motorPoints.append(QPointF(MotionManager::getIns().getAxisCurrentPos(m_config->motorXName()),
                                   MotionManager::getIns().getAxisCurrentPos(m_config->motorYName())));
    }
}

void Calibration::calcAffineTransform(const QVector<QPointF> &pixelPoints, const QVector<QPointF> &motorPoints)
{
    QMatrix qMat = affineTransformCalc.calcAffineTransform(pixelPoints, motorPoints);
    auto caliResult = m_config->calibrationResult();
    caliResult->fromQMatrix(qMat);
    caliResult->setIsValid(true);
    qCInfo(visionCate()) << tr("%1 calibration successful!").arg(m_config->calibrationName()) << "Pixel points:" << pixelPoints
                         << "Motor points:" << motorPoints;

    QPointF convertedPixelPoints;
    double sumX = 0, sumY = 0;
    for (int i = 0; i < pixelPoints.size(); i++)
    {
        convertedPixelPoints = caliResult->pixelToMech(pixelPoints[i]);
        sumX += pow(motorPoints[i].x() - convertedPixelPoints.x(), 2);
        sumY += pow(motorPoints[i].y() - convertedPixelPoints.y(), 2);
    }
    double rmsX, rmsY;
    rmsX = sqrt(sumX / motorPoints.size());
    rmsY = sqrt(sumY / motorPoints.size());

    UIOperation::getIns()->showTip(tr("%1 calibration successful!\r\nStdEv x: %2, y: %3").arg(m_config->calibrationName()).arg(rmsX).arg(rmsY));
}

bool Calibration::calculateMatrixAttribute(QVector<QPointF> p, QVector<QPointF> m, double &scaleX, double &scaleY, double &closestAngle)
{
    double diff_py10 = p[1].y() - p[0].y();
    double diff_px10 = p[1].x() - p[0].x();
    double diff_py30 = p[3].y() - p[0].y();
    double diff_px30 = p[3].x() - p[0].x();
    double diff_mx10 = m[1].x() - m[0].x();
    double diff_mx30 = m[3].x() - m[0].x();
    double diff_my10 = m[1].y() - m[0].y();
    double diff_my30 = m[3].y() - m[0].y();
    double a_11 = (diff_py10 * diff_mx30 - diff_py30 * diff_mx10) / (diff_px30 * diff_py10 - diff_py30 * diff_px10);
    double a_12 = (diff_mx10 - diff_px10 * a_11) / diff_py10;
    double a_13 = (m[0].x() - p[0].x() * a_11 - p[0].y() * a_12);

    double a_21 = (diff_py10 * diff_my30 - diff_py30 * diff_my10) / (diff_px30 * diff_py10 - diff_py30 * diff_px10);
    double a_22 = (diff_my10 - diff_px10 * a_21) / diff_py10;
    double a_23 = (m[0].y() - p[0].x() * a_21 - p[0].y() * a_22);

    double s_x = sqrt(a_11 * a_11 + a_21 * a_21);
    double s_y = sqrt(a_12 * a_12 + a_22 * a_22);
    if (s_x == 0 || s_y == 0)
    {
        return false;
    }
    scaleX = 1 / s_x;
    scaleY = 1 / s_y;
    double angle = acos(a_11 / s_x) * 180;
    double steppedAngle = angle - 360 * ((int)((int)angle / 360));
    closestAngle = 0;
    if (steppedAngle <= 45)
    {
        closestAngle = 0;
    }
    else if (steppedAngle <= 135)
    {
        closestAngle = 90;
    }
    else if (steppedAngle <= 225)
    {
        closestAngle = 180;
    }
    else if (steppedAngle <= 315)
    {
        closestAngle = 270;
    }
    return true;
}

QPointF Calibration::getPixelPoint(int times, double errorLimit)
{
    QList<double> lpx;
    QList<double> lpy;
    QPointF pixelPoint;
    for (int i = 0; i < times; i++)
    {
        pixelPoint = getPixelPoint();
        lpx.append(pixelPoint.x());
        lpy.append(pixelPoint.y());
    }
    if (myMax(lpx) - myMin(lpx) > qAbs(errorLimit) || myMax(lpy) - myMin(lpy) > qAbs(errorLimit))
    {
        throw SilicolAbort("Pr result error exceeded limit!");
    }
    return QPointF(myAve(lpx), myAve(lpy));
}
