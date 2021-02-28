#ifndef MOVEPROTECTION_H
#define MOVEPROTECTION_H

#include "collisiongroup.h"
#include "scmotion_global.h"
#include <QObject>
#include "loghelper.h"

class SCMOTIONSHARED_EXPORT MoveProtection : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool enable READ enable WRITE setEnable NOTIFY enableChanged)

public:
    MoveProtection(ConfigObjectArray *collisionGroups) : collisionGroups(collisionGroups) {}

    void checkAxisIsAbleToMove(QString axisName, double axisTargetPos)
    {
        checkCollision(axisName, axisTargetPos);
    }

    void checkCylIsAbleToMove(QString cylName, int cylTargetState)
    {
        checkCollision(cylName, cylTargetState);
    }

    bool axisIsAbleToMove(QString axisName, double targetPos);

    void update();

    bool enable() const
    {
        return m_enable;
    }

public slots:
    void setEnable(bool enable)
    {
        if (m_enable == enable)
            return;

        m_enable = enable;
        emit enableChanged(m_enable);
    }

signals:
    void enableChanged(bool enable);

private:
    void checkCollision(QString name, double targetState);

private:
    ConfigObjectArray *collisionGroups;
    bool m_enable = true;
    QMap<QString, QList<CollisionGroup *>> relativeCollisionGroups;
};

#endif    // MOVEPROTECTION_H
