#ifndef DISPENSEPATH_H
#define DISPENSEPATH_H

#include "configManager/configobject.h"
#include "configManager/configobjectarray.h"

enum PathType
{
    LinePath = 0,
    ArcPath
};

enum ArcDir
{
    CW = 0,
    CCW
};

struct PathEndPoint
{
public:
    PathEndPoint(double x = 0, double y = 0, double z = 0, PathType pathType = LinePath) : x(x), y(y), z(z), pathType(pathType)
    {
        maxVel = 0;
        endVel = 0;
        arcDir = CW;
        isDisabled = false;
    }

    double *dataPtr()
    {
        return reinterpret_cast<double *>(this);
    }

    QString toString()
    {
        return QString("x: %1, y: %2, z: %3, maxVel: %4, endVel: %5").arg(x).arg(y).arg(z).arg(maxVel).arg(endVel);
    }

public:
    double x;
    double y;
    double z;
    PathType pathType;
    double maxVel;
    double endVel;
    bool isDisabled;
    QPointF arcCenter;
    ArcDir arcDir;
};

class PathEndPointCfg : public ConfigObject
{
    Q_OBJECT

    Q_PROPERTY(double x READ x WRITE setX NOTIFY xChanged)
    Q_PROPERTY(double y READ y WRITE setY NOTIFY yChanged)
    Q_PROPERTY(double z READ z WRITE setZ NOTIFY zChanged)
    Q_PROPERTY(double vel READ vel WRITE setVel NOTIFY velChanged)
    Q_PROPERTY(double disable READ disable WRITE setDisable NOTIFY disableChanged)
    Q_PROPERTY(int pathType READ pathType WRITE setPathType NOTIFY pathTypeChanged)
    Q_PROPERTY(double arcCenterX READ arcCenterX WRITE setArcCenterX NOTIFY arcCenterXChanged)
    Q_PROPERTY(double arcCenterY READ arcCenterY WRITE setArcCenterY NOTIFY arcCenterYChanged)
    Q_PROPERTY(int arcDir READ arcDir WRITE setArcDir NOTIFY arcDirChanged)

public:
    Q_INVOKABLE PathEndPointCfg(QObject *parent = nullptr) : ConfigObject(parent)
    {
        init();
    }

    double x() const
    {
        return m_x;
    }

    double y() const
    {
        return m_y;
    }

    double z() const
    {
        return m_z;
    }

    double vel() const
    {
        return m_vel;
    }

    double disable() const
    {
        return m_disable;
    }

    int pathType() const
    {
        return m_pathType;
    }

    double arcCenterX() const
    {
        return m_arcCenterX;
    }

    double arcCenterY() const
    {
        return m_arcCenterY;
    }

    int arcDir() const
    {
        return m_arcDir;
    }

public slots:
    void setX(double x)
    {
        if (qFuzzyCompare(m_x, x))
            return;

        m_x = x;
        emit xChanged(m_x);
    }

    void setY(double y)
    {
        if (qFuzzyCompare(m_y, y))
            return;

        m_y = y;
        emit yChanged(m_y);
    }

    void setZ(double z)
    {
        if (qFuzzyCompare(m_z, z))
            return;

        m_z = z;
        emit zChanged(m_z);
    }

    void setVel(double vel)
    {
        if (qFuzzyCompare(m_vel, vel))
            return;

        m_vel = vel;
        emit velChanged(m_vel);
    }

    void setDisable(double disable)
    {
        if (qFuzzyCompare(m_disable, disable))
            return;

        m_disable = disable;
        emit disableChanged(m_disable);
    }

    void setPathType(int pathType)
    {
        if (m_pathType == pathType)
            return;

        m_pathType = pathType;
        emit pathTypeChanged(m_pathType);
    }

    void setArcCenterX(double arcCenterX)
    {
        if (qFuzzyCompare(m_arcCenterX, arcCenterX))
            return;

        m_arcCenterX = arcCenterX;
        emit arcCenterXChanged(m_arcCenterX);
    }

    void setArcCenterY(double arcCenterY)
    {
        if (qFuzzyCompare(m_arcCenterY, arcCenterY))
            return;

        m_arcCenterY = arcCenterY;
        emit arcCenterYChanged(m_arcCenterY);
    }

    void setArcDir(int arcDir)
    {
        if (m_arcDir == arcDir)
            return;

        m_arcDir = arcDir;
        emit arcDirChanged(m_arcDir);
    }

signals:
    void xChanged(double x);

    void yChanged(double y);

    void zChanged(double z);

    void velChanged(double vel);

    void disableChanged(double disable);

    void pathTypeChanged(int pathType);

    void arcCenterXChanged(double arcCenterX);

    void arcCenterYChanged(double arcCenterY);

    void arcDirChanged(int arcDir);

private:
    double m_x = 0;
    double m_y = 0;
    double m_z = 0;
    double m_vel = 10;
    double m_disable = false;
    int m_pathType{ LinePath };
    double m_arcCenterX = 0;
    double m_arcCenterY = 0;
    int m_arcDir{ CW };
};

#endif    // DISPENSEPATH_H
