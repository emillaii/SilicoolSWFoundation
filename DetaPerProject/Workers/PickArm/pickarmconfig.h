#ifndef PICKARMCONFIG_H
#define PICKARMCONFIG_H

#include "configManager/configobject.h"
#include "configManager/configobjectarray.h"

class PickArmConfig : public ConfigObject
{
    Q_OBJECT
    Q_PROPERTY(double PA_L_Scale READ PA_L_Scale WRITE setPA_L_Scale NOTIFY PA_L_ScaleChanged)

    Q_PROPERTY(double PA_L_Offset_X READ PA_L_Offset_X WRITE setPA_L_Offset_X NOTIFY PA_L_Offset_XChanged)
    Q_PROPERTY(double PA_L_Offset_Y READ PA_L_Offset_Y WRITE setPA_L_Offset_Y NOTIFY PA_L_Offset_YChanged)
    Q_PROPERTY(double PA_R_Offset_X READ PA_R_Offset_X WRITE setPA_R_Offset_X NOTIFY PA_R_Offset_XChanged)
    Q_PROPERTY(double PA_R_Offset_Y READ PA_R_Offset_Y WRITE setPA_R_Offset_Y NOTIFY PA_R_Offset_YChanged)
    Q_PROPERTY(int PACalibStepCount READ PACalibStepCount WRITE setPACalibStepCount NOTIFY PACalibStepCountChanged)
    Q_PROPERTY(int CalibrateStep READ CalibrateStep WRITE setCalibrateStep NOTIFY CalibrateStepChanged)
    Q_PROPERTY(int VacummDelay READ VacummDelay WRITE setVacummDelay NOTIFY VacummDelayChanged)

    Q_PROPERTY(double GetMaterialFixOffset_X READ GetMaterialFixOffset_X WRITE setGetMaterialFixOffset_X NOTIFY GetMaterialFixOffset_XChanged)
    Q_PROPERTY(double GetMaterialFixOffset_Y READ GetMaterialFixOffset_Y WRITE setGetMaterialFixOffset_Y NOTIFY GetMaterialFixOffset_YChanged)
    Q_PROPERTY(double GetMaterialFixOffset_Theta READ GetMaterialFixOffset_Theta WRITE setGetMaterialFixOffset_Theta NOTIFY GetMaterialFixOffset_ThetaChanged)
    Q_PROPERTY(double PlaceMaterialFixOffset_X READ PlaceMaterialFixOffset_X WRITE setPlaceMaterialFixOffset_X NOTIFY PlaceMaterialFixOffset_XChanged)
    Q_PROPERTY(double PlaceMaterialFixOffset_Y READ PlaceMaterialFixOffset_Y WRITE setPlaceMaterialFixOffset_Y NOTIFY PlaceMaterialFixOffset_YChanged)
    Q_PROPERTY(double PlaceMaterialFixOffset_Theta READ PlaceMaterialFixOffset_Theta WRITE setPlaceMaterialFixOffset_Theta NOTIFY PlaceMaterialFixOffset_ThetaChanged)

public:
    PickArmConfig(QObject *parent = nullptr) : ConfigObject (parent)
    {
        init();
    }

    double PA_L_Scale() const
    {
        return m_PA_L_Scale;
    }

    double PA_L_Offset_X() const
    {
        return m_PA_L_Offset_X;
    }

    double PA_L_Offset_Y() const
    {
        return m_PA_L_Offset_Y;
    }

    double PA_R_Offset_X() const
    {
        return m_PA_R_Offset_X;
    }

    double PA_R_Offset_Y() const
    {
        return m_PA_R_Offset_Y;
    }

    int PACalibStepCount() const
    {
        return m_PACalibStepCount;
    }

    int CalibrateStep() const
    {
        return m_CalibrateStep;
    }

    int VacummDelay() const
    {
        return m_VacummDelay;
    }



    double GetMaterialFixOffset_X() const
    {
        return m_GetMaterialFixOffset_X;
    }

    double GetMaterialFixOffset_Y() const
    {
        return m_GetMaterialFixOffset_Y;
    }

    double PlaceMaterialFixOffset_X() const
    {
        return m_PlaceMaterialFixOffset_X;
    }

    double PlaceMaterialFixOffset_Y() const
    {
        return m_PlaceMaterialFixOffset_Y;
    }

    double PlaceMaterialFixOffset_Theta() const
    {
        return m_PlaceMaterialFixOffset_Theta;
    }

    double GetMaterialFixOffset_Theta() const
    {
        return m_GetMaterialFixOffset_Theta;
    }

public slots:
    void setPA_L_Scale(double PA_L_Scale)
    {
        if (qFuzzyCompare(m_PA_L_Scale, PA_L_Scale))
            return;

        m_PA_L_Scale = PA_L_Scale;
        emit PA_L_ScaleChanged(m_PA_L_Scale);
    }

    void setPA_L_Offset_X(double PA_L_Offset_X)
    {
        if (qFuzzyCompare(m_PA_L_Offset_X, PA_L_Offset_X))
            return;

        m_PA_L_Offset_X = PA_L_Offset_X;
        emit PA_L_Offset_XChanged(m_PA_L_Offset_X);
    }

    void setPA_L_Offset_Y(double PA_L_Offset_Y)
    {
        if (qFuzzyCompare(m_PA_L_Offset_Y, PA_L_Offset_Y))
            return;

        m_PA_L_Offset_Y = PA_L_Offset_Y;
        emit PA_L_Offset_YChanged(m_PA_L_Offset_Y);
    }

    void setPA_R_Offset_X(double PA_R_Offset_X)
    {
        if (qFuzzyCompare(m_PA_R_Offset_X, PA_R_Offset_X))
            return;

        m_PA_R_Offset_X = PA_R_Offset_X;
        emit PA_R_Offset_XChanged(m_PA_R_Offset_X);
    }

    void setPA_R_Offset_Y(double PA_R_Offset_Y)
    {
        if (qFuzzyCompare(m_PA_R_Offset_Y, PA_R_Offset_Y))
            return;

        m_PA_R_Offset_Y = PA_R_Offset_Y;
        emit PA_R_Offset_YChanged(m_PA_R_Offset_Y);
    }

    void setPACalibStepCount(int PACalibStepCount)
    {
        if (m_PACalibStepCount == PACalibStepCount)
            return;

        m_PACalibStepCount = PACalibStepCount;
        emit PACalibStepCountChanged(m_PACalibStepCount);
    }

    void setCalibrateStep(int CalibrateStep)
    {
        if (m_CalibrateStep == CalibrateStep)
            return;

        m_CalibrateStep = CalibrateStep;
        emit CalibrateStepChanged(m_CalibrateStep);
    }

    void setVacummDelay(int VacummDelay)
    {
        if (m_VacummDelay == VacummDelay)
            return;

        m_VacummDelay = VacummDelay;
        emit VacummDelayChanged(m_VacummDelay);
    }

    void setGetMaterialFixOffset_X(double GetMaterialFixOffset_X)
    {
        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_GetMaterialFixOffset_X, GetMaterialFixOffset_X))
            return;

        m_GetMaterialFixOffset_X = GetMaterialFixOffset_X;
        emit GetMaterialFixOffset_XChanged(m_GetMaterialFixOffset_X);
    }

    void setGetMaterialFixOffset_Y(double GetMaterialFixOffset_Y)
    {
        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_GetMaterialFixOffset_Y, GetMaterialFixOffset_Y))
            return;

        m_GetMaterialFixOffset_Y = GetMaterialFixOffset_Y;
        emit GetMaterialFixOffset_YChanged(m_GetMaterialFixOffset_Y);
    }



    void setPlaceMaterialFixOffset_X(double PlaceMaterialFixOffset_X)
    {
        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_PlaceMaterialFixOffset_X, PlaceMaterialFixOffset_X))
            return;

        m_PlaceMaterialFixOffset_X = PlaceMaterialFixOffset_X;
        emit PlaceMaterialFixOffset_XChanged(m_PlaceMaterialFixOffset_X);
    }

    void setPlaceMaterialFixOffset_Y(double PlaceMaterialFixOffset_Y)
    {
        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_PlaceMaterialFixOffset_Y, PlaceMaterialFixOffset_Y))
            return;

        m_PlaceMaterialFixOffset_Y = PlaceMaterialFixOffset_Y;
        emit PlaceMaterialFixOffset_YChanged(m_PlaceMaterialFixOffset_Y);
    }

    void setPlaceMaterialFixOffset_Theta(double PlaceMaterialFixOffset_Theta)
    {
        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_PlaceMaterialFixOffset_Theta, PlaceMaterialFixOffset_Theta))
            return;

        m_PlaceMaterialFixOffset_Theta = PlaceMaterialFixOffset_Theta;
        emit PlaceMaterialFixOffset_ThetaChanged(m_PlaceMaterialFixOffset_Theta);
    }

    void setGetMaterialFixOffset_Theta(double GetMaterialFixOffset_Theta)
    {
        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_GetMaterialFixOffset_Theta, GetMaterialFixOffset_Theta))
            return;

        m_GetMaterialFixOffset_Theta = GetMaterialFixOffset_Theta;
        emit GetMaterialFixOffset_ThetaChanged(m_GetMaterialFixOffset_Theta);
    }

signals:
    void PA_L_ScaleChanged(double PA_L_Scale);

    void PA_L_Offset_XChanged(double PA_L_Offset_X);

    void PA_L_Offset_YChanged(double PA_L_Offset_Y);

    void PA_R_Offset_XChanged(double PA_R_Offset_X);

    void PA_R_Offset_YChanged(double PA_R_Offset_Y);

    void PACalibStepCountChanged(int PACalibStepCount);

    void CalibrateStepChanged(int CalibrateStep);

    void VacummDelayChanged(int VacummDelay);

    void GetMaterialFixOffset_XChanged(double GetMaterialFixOffset_X);

    void GetMaterialFixOffset_YChanged(double GetMaterialFixOffset_Y);

    void PlaceMaterialFixOffset_XChanged(double PlaceMaterialFixOffset_X);

    void PlaceMaterialFixOffset_YChanged(double PlaceMaterialFixOffset_Y);

    void PlaceMaterialFixOffset_ThetaChanged(double PlaceMaterialFixOffset_Theta);

    void GetMaterialFixOffset_ThetaChanged(double GetMaterialFixOffset_Theta);

private:
    double m_PA_L_Scale = 0;
    double m_PA_L_Offset_X = 0;
    double m_PA_L_Offset_Y = 0;
    double m_PA_R_Offset_X = 0;
    double m_PA_R_Offset_Y = 0;

    int m_PACalibStepCount = 3;
    int m_CalibrateStep = 0;
    int m_VacummDelay = 0;

    double m_GetMaterialFixOffset_X = 0;
    double m_GetMaterialFixOffset_Y = 0;
    double m_PlaceMaterialFixOffset_Y = 0;
    double m_PlaceMaterialFixOffset_X = 0;
    double m_PlaceMaterialFixOffset_Theta = 0;
    double m_GetMaterialFixOffset_Theta = 0;
};

#endif // PICKARMCONFIG_H
