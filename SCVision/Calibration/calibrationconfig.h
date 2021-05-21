#ifndef CALIBRATIONCONFIG_H
#define CALIBRATIONCONFIG_H

#include "configManager/configobject.h"
#include "renameManager/renamemanager.h"
#include "sccameraconfig.h"
#include "scvision_global.h"
#include <QMatrix>

class CalibrationResult : public ConfigObject
{
    Q_OBJECT

    Q_PROPERTY(double matrix11 READ matrix11 WRITE setMatrix11 NOTIFY matrix11Changed)
    Q_PROPERTY(double matrix12 READ matrix12 WRITE setMatrix12 NOTIFY matrix12Changed)
    Q_PROPERTY(double matrix21 READ matrix21 WRITE setMatrix21 NOTIFY matrix21Changed)
    Q_PROPERTY(double matrix22 READ matrix22 WRITE setMatrix22 NOTIFY matrix22Changed)
    Q_PROPERTY(double dx READ dx WRITE setDx NOTIFY dxChanged)
    Q_PROPERTY(double dy READ dy WRITE setDy NOTIFY dyChanged)
    Q_PROPERTY(bool isValid READ isValid WRITE setIsValid NOTIFY isValidChanged)

public:
    CalibrationResult(QObject *parent = nullptr) : ConfigObject(parent)
    {
        init();
    }

    void fromQMatrix(const QMatrix &qMat)
    {
        setMatrix11(qMat.m11());
        setMatrix12(qMat.m12());
        setMatrix21(qMat.m21());
        setMatrix22(qMat.m22());
        setDx(qMat.dx());
        setDy(qMat.dy());
    }

    QPointF pixelToMech(double x, double y) const
    {
        return QPointF(matrix11() * x + matrix21() * y + dx(), matrix22() * y + matrix12() * x + dy());
    }

    QPointF pixelToMech(const QPointF &point) const
    {
        return pixelToMech(point.x(), point.y());
    }

    QPointF getDistanceFromImageCenter(double x, double y, CameraConfig *cameraConfig) const
    {
        return pixelToMech(x, y) - pixelToMech(cameraConfig->imageWidth() / 2, cameraConfig->imageHeight() / 2);
    }

    QPointF getDistanceFromImageCenter(const QPointF &point, CameraConfig *cameraConfig) const
    {
        return pixelToMech(point) - pixelToMech(cameraConfig->imageWidth() / 2, cameraConfig->imageHeight() / 2);
    }

    double matrix11() const
    {
        return m_matrix11;
    }

    double matrix12() const
    {
        return m_matrix12;
    }

    double matrix21() const
    {
        return m_matrix21;
    }

    double matrix22() const
    {
        return m_matrix22;
    }

    double dx() const
    {
        return m_dx;
    }

    double dy() const
    {
        return m_dy;
    }

    bool isValid() const
    {
        return m_isValid;
    }

public slots:
    void setMatrix11(double matrix11)
    {
        if (qFuzzyCompare(m_matrix11, matrix11))
            return;

        m_matrix11 = matrix11;
        emit matrix11Changed(m_matrix11);
    }

    void setMatrix12(double matrix12)
    {
        if (qFuzzyCompare(m_matrix12, matrix12))
            return;

        m_matrix12 = matrix12;
        emit matrix12Changed(m_matrix12);
    }

    void setMatrix21(double matrix21)
    {
        if (qFuzzyCompare(m_matrix21, matrix21))
            return;

        m_matrix21 = matrix21;
        emit matrix21Changed(m_matrix21);
    }

    void setMatrix22(double matrix22)
    {
        if (qFuzzyCompare(m_matrix22, matrix22))
            return;

        m_matrix22 = matrix22;
        emit matrix22Changed(m_matrix22);
    }

    void setDx(double dx)
    {
        if (qFuzzyCompare(m_dx, dx))
            return;

        m_dx = dx;
        emit dxChanged(m_dx);
    }

    void setDy(double dy)
    {
        if (qFuzzyCompare(m_dy, dy))
            return;

        m_dy = dy;
        emit dyChanged(m_dy);
    }

    void setIsValid(bool isValid)
    {
        if (m_isValid == isValid)
            return;

        m_isValid = isValid;
        emit isValidChanged(m_isValid);
    }

signals:
    void matrix11Changed(double matrix11);

    void matrix12Changed(double matrix12);

    void matrix21Changed(double matrix21);

    void matrix22Changed(double matrix22);

    void dxChanged(double dx);

    void dyChanged(double dy);

    void isValidChanged(bool isValid);

private:
    double m_matrix11 = 0;
    double m_matrix12 = 0;
    double m_matrix21 = 0;
    double m_matrix22 = 0;
    double m_dx = 0;
    double m_dy = 0;
    bool m_isValid = false;
};

class SCVISIONSHARED_EXPORT CalibrationConfig : public ConfigObject
{
    Q_OBJECT

public:
    enum CalibrationFunction
    {
        ThreePoints,
        FourPoints,
        NinePoints,
        Custom
    };
    Q_ENUM(CalibrationFunction)

    Q_PROPERTY(QString calibrationName READ calibrationName WRITE setCalibrationName NOTIFY calibrationNameChanged)
    Q_PROPERTY(QString locationName READ locationName WRITE setLocationName NOTIFY locationNameChanged)
    Q_PROPERTY(QString motorXName READ motorXName WRITE setMotorXName NOTIFY motorXNameChanged)
    Q_PROPERTY(QString motorYName READ motorYName WRITE setMotorYName NOTIFY motorYNameChanged)
    Q_PROPERTY(double calibrationXStep READ calibrationXStep WRITE setCalibrationXStep NOTIFY calibrationXStepChanged)
    Q_PROPERTY(double calibrationYStep READ calibrationYStep WRITE setCalibrationYStep NOTIFY calibrationYStepChanged)
    Q_PROPERTY(bool moveXFirst READ moveXFirst WRITE setMoveXFirst NOTIFY moveXFirstChanged)
    Q_PROPERTY(CalibrationFunction calibrationFunction READ calibrationFunction WRITE setCalibrationFunction NOTIFY calibrationFunctionChanged)
    Q_PROPERTY(int prCountForEachPoint READ prCountForEachPoint WRITE setPrCountForEachPoint NOTIFY prCountForEachPointChanged)
    Q_PROPERTY(double prErrorLimit READ prErrorLimit WRITE setPrErrorLimit NOTIFY prErrorLimitChanged)
    Q_PROPERTY(double calibrationVel READ calibrationVel WRITE setCalibrationVel NOTIFY calibrationVelChanged)
    Q_PROPERTY(double calibrationAcc READ calibrationAcc WRITE setCalibrationAcc NOTIFY calibrationAccChanged)
    Q_PROPERTY(int calibrationDelay READ calibrationDelay WRITE setCalibrationDelay NOTIFY calibrationDelayChanged)
    Q_PROPERTY(CalibrationResult *calibrationResult READ calibrationResult)

public:
    Q_INVOKABLE CalibrationConfig(QObject *parent = nullptr) : ConfigObject(parent)
    {
        m_calibrationResult = new CalibrationResult(this);
        setReadOnlyProperty("calibrationName");
        setReadOnlyProperty("locationName");
        setIdentityProperty("calibrationName");
        QStringList args;
        args << "motorXName"
             << "motorYName"
             << "calibrationResult";
        setEngineerAuthorities(args);
        init();
        RenameManager::getIns().subscribeAxisNameChanged(this, "motorXName");
        RenameManager::getIns().subscribeAxisNameChanged(this, "motorYName");
        RenameManager::getIns().subscribePrNameChanged(this, "locationName");
    }

    static EnumHelper<CalibrationFunction> &calibrationFunctionEnumInfo()
    {
        static EnumHelper<CalibrationFunction> instance(staticMetaObject, "CalibrationFunction");
        return instance;
    }

    void setOptionalMotorName(QVariantList motorNames)
    {
        setOptionalProperty("motorXName", motorNames);
        setOptionalProperty("motorYName", motorNames);
    }

    QString calibrationName() const
    {
        return m_calibrationName;
    }

    QString locationName() const
    {
        return m_locationName;
    }

    QString motorXName() const
    {
        return m_motorXName;
    }

    QString motorYName() const
    {
        return m_motorYName;
    }

    double calibrationVel() const
    {
        return m_calibrationVel;
    }

    double calibrationAcc() const
    {
        return m_calibrationAcc;
    }

    int calibrationDelay() const
    {
        return m_calibrationDelay;
    }

    double calibrationXStep() const
    {
        return m_calibrationXStep;
    }

    double calibrationYStep() const
    {
        return m_calibrationYStep;
    }

    int prCountForEachPoint() const
    {
        return m_prCountForEachPoint;
    }

    double prErrorLimit() const
    {
        return m_prErrorLimit;
    }

    CalibrationFunction calibrationFunction() const
    {
        return m_calibrationFunction;
    }

    CalibrationResult *calibrationResult() const
    {
        return m_calibrationResult;
    }

    bool moveXFirst() const
    {
        return m_moveXFirst;
    }

public slots:
    void setCalibrationName(QString calibrationName)
    {
        if (m_calibrationName == calibrationName)
            return;

        m_calibrationName = calibrationName;
        emit calibrationNameChanged(m_calibrationName);
    }

    void setLocationName(QString locationName)
    {
        if (m_locationName == locationName)
            return;

        m_locationName = locationName;
        emit locationNameChanged(m_locationName);
    }

    void setMotorXName(QString motorXName)
    {
        if (m_motorXName == motorXName)
            return;

        m_motorXName = motorXName;
        emit motorXNameChanged(m_motorXName);
    }

    void setMotorYName(QString motorYName)
    {
        if (m_motorYName == motorYName)
            return;

        m_motorYName = motorYName;
        emit motorYNameChanged(m_motorYName);
    }

    void setCalibrationVel(double calibrationVel)
    {
        if (qFuzzyCompare(m_calibrationVel, calibrationVel))
            return;

        m_calibrationVel = calibrationVel;
        emit calibrationVelChanged(m_calibrationVel);
    }

    void setCalibrationAcc(double calibrationAcc)
    {
        if (qFuzzyCompare(m_calibrationAcc, calibrationAcc))
            return;

        m_calibrationAcc = calibrationAcc;
        emit calibrationAccChanged(m_calibrationAcc);
    }

    void setCalibrationDelay(int calibrationDelay)
    {
        if (m_calibrationDelay == calibrationDelay)
            return;

        m_calibrationDelay = calibrationDelay;
        emit calibrationDelayChanged(m_calibrationDelay);
    }

    void setCalibrationXStep(double calibrationXStep)
    {
        if (qFuzzyCompare(m_calibrationXStep, calibrationXStep))
            return;

        m_calibrationXStep = calibrationXStep;
        emit calibrationXStepChanged(m_calibrationXStep);
    }

    void setCalibrationYStep(double calibrationYStep)
    {
        if (qFuzzyCompare(m_calibrationYStep, calibrationYStep))
            return;

        m_calibrationYStep = calibrationYStep;
        emit calibrationYStepChanged(m_calibrationYStep);
    }

    void setPrCountForEachPoint(int prCountForEachPoint)
    {
        if (m_prCountForEachPoint == prCountForEachPoint)
            return;

        m_prCountForEachPoint = prCountForEachPoint;
        emit prCountForEachPointChanged(m_prCountForEachPoint);
    }

    void setPrErrorLimit(double prErrorLimit)
    {
        if (qFuzzyCompare(m_prErrorLimit, prErrorLimit))
            return;

        m_prErrorLimit = prErrorLimit;
        emit prErrorLimitChanged(m_prErrorLimit);
    }

    void setCalibrationFunction(CalibrationFunction calibrationFunction)
    {
        if (m_calibrationFunction == calibrationFunction)
            return;

        m_calibrationFunction = calibrationFunction;
        emit calibrationFunctionChanged(m_calibrationFunction);
    }

    void setMoveXFirst(bool moveXFirst)
    {
        if (m_moveXFirst == moveXFirst)
            return;

        m_moveXFirst = moveXFirst;
        emit moveXFirstChanged(m_moveXFirst);
    }

signals:
    void calibrationNameChanged(QString calibrationName);

    void locationNameChanged(QString locationName);

    void motorXNameChanged(QString motorXName);

    void motorYNameChanged(QString motorYName);

    void calibrationVelChanged(double calibrationVel);

    void calibrationAccChanged(double calibrationAcc);

    void calibrationDelayChanged(int calibrationDelay);

    void calibrationXStepChanged(double calibrationXStep);

    void calibrationYStepChanged(double calibrationYStep);

    void prCountForEachPointChanged(int prCountForEachPoint);

    void prErrorLimitChanged(double prErrorLimit);

    void calibrationFunctionChanged(CalibrationFunction calibrationFunction);

    void moveXFirstChanged(bool moveXFirst);

private:
    QString m_calibrationName = "";
    QString m_locationName = "";
    QString m_motorXName = "";
    QString m_motorYName = "";
    double m_calibrationVel = 10;
    double m_calibrationAcc = 100;
    int m_calibrationDelay = 500;
    double m_calibrationXStep = 1;
    double m_calibrationYStep = 1;
    int m_prCountForEachPoint = 3;
    double m_prErrorLimit = 1;
    CalibrationFunction m_calibrationFunction{ ThreePoints };
    CalibrationResult *m_calibrationResult;
    bool m_moveXFirst = true;
};

#endif    // CALIBRATIONCONFIG_H
