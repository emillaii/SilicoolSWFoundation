#include "moveprotection.h"

bool MoveProtection::axisIsAbleToMove(QString axisName, double targetPos)
{
    if (relativeCollisionGroups.contains(axisName))
    {
        foreach (auto collisionGroup, relativeCollisionGroups[axisName])
        {
            if (collisionGroup->willCollide(axisName, targetPos))
            {
                return false;
            }
        }
    }
    return true;
}

void MoveProtection::update()
{
    foreach (auto collisionGroupList, relativeCollisionGroups.values())
    {
        collisionGroupList.clear();
    }
    relativeCollisionGroups.clear();

    for (int i = 0; i < collisionGroups->count(); i++)
    {
        CollisionGroup *collisionGroup = collisionGroups->getConfig<CollisionGroup>(i);
        auto affectedMotionElements = collisionGroup->collisionConditions()->getAffectedMotionElementNames();
        foreach (auto name, affectedMotionElements)
        {
            if (!relativeCollisionGroups.contains(name))
            {
                relativeCollisionGroups[name] = QList<CollisionGroup *>();
            }
            if (!relativeCollisionGroups[name].contains(collisionGroup))
            {
                relativeCollisionGroups[name].append(collisionGroup);
            }
        }
    }
}

void MoveProtection::checkCollision(QString name, double targetState)
{
    if (!enable())
    {
        return;
    }
    if (!relativeCollisionGroups.contains(name))
    {
        return;
    }
    foreach (auto collisionGroup, relativeCollisionGroups[name])
    {
        if (collisionGroup->willCollide(name, targetState))
        {
            throw ActionError(name, collisionGroup->getCollisionDescription());
        }
    }
}
