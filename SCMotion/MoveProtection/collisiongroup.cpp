#include "collisiongroup.h"

bool CollisionConditionsConfig::add(int index, QString collisionConditionType, QString cmpOperationType)
{
    if (index < 0 || index > this->count())
    {
        qCritical() << QString("Index %1 out of range [0, %2]").arg(index).arg(this->count());
        return false;
    }
    if (collisionConditionType.isEmpty())
    {
        qCritical() << "collisionConditionType is empty!";
        return false;
    }
    if (cmpOperationType.isEmpty())
    {
        qCritical() << "cmpOperationType is empty!";
        return false;
    }

    CollisionCondition::Type collisionConditionTypeEnum = CollisionCondition::TypeEnumInfo().nameToEnum(collisionConditionType);
    ComparisonOperation::Type cmpOperationTypeEnum = ComparisonOperation::TypeEnumInfo().nameToEnum(cmpOperationType);

    if ((collisionConditionTypeEnum == CollisionCondition::SingleAxisCollisionCondition
         || collisionConditionTypeEnum == CollisionCondition::TwoAxisCollisionCondition)
        && cmpOperationTypeEnum == ComparisonOperation::NotInState)
    {
        qCritical() << QString("collisionConditionType(%1) and cmpOperationType(%2) did not match!").arg(collisionConditionType).arg(cmpOperationType);
        return false;
    }
    if ((collisionConditionTypeEnum == CollisionCondition::CylinderCollisionCondition
         || collisionConditionTypeEnum == CollisionCondition::DICollisionCondition)
        && cmpOperationTypeEnum != ComparisonOperation::NotInState)
    {
        qCritical() << QString("collisionConditionType(%1) and cmpOperationType(%2) did not match!").arg(collisionConditionType).arg(cmpOperationType);
        return false;
    }

    auto collisionCondition = CollisionCondition::createInstance(collisionConditionTypeEnum, this);
    auto cmpOperation = ComparisonOperation::createInstance(cmpOperationTypeEnum, collisionCondition);
    collisionCondition->setcollisionConditionType(collisionConditionType);
    cmpOperation->setCmpOperationType(cmpOperationType);

    collisionCondition->setCmpOperation(cmpOperation);
    collisionCondition->init();

    return ConfigManager::getIns()->executeCommand(new AddConfigObjectComand(this, index, collisionCondition));
}

QStringList CollisionConditionsConfig::getAffectedMotionElementNames()
{
    QStringList nameList;
    for (int i = 0; i < count(); i++)
    {
        auto names = getConfig<CollisionCondition>(i)->getAffectedMotionElementNames();
        for (int j = 0; j < names.count(); j++)
        {
            if (!nameList.contains(names[j]))
            {
                nameList.append(names[j]);
            }
        }
    }
    return nameList;
}

bool CollisionConditionsConfig::read(const QJsonValue &jsonValue)
{
    QJsonArray jsonArr = jsonValue.toArray();
    bool result = true;

    while (count() > 0)
    {
        auto configObj = at(count() - 1);
        executeRemoveConfigObject(count() - 1);
        configObj->setParent(nullptr);
        configObj->deleteLater();
    }

    for (int i = 0; i < jsonArr.count(); i++)
    {
        QJsonObject jsonObj = jsonArr[i].toObject();
        if (jsonObj.contains("collisionConditionType") && jsonObj.contains("cmpOperation")
            && jsonObj["cmpOperation"].toObject().contains("cmpOperationType"))
        {
            CollisionCondition::Type collisionConditionType
                = CollisionCondition::TypeEnumInfo().nameToEnum(jsonObj["collisionConditionType"].toString());
            ComparisonOperation::Type cmpOperationType
                = ComparisonOperation::TypeEnumInfo().nameToEnum(jsonObj["cmpOperation"].toObject()["cmpOperationType"].toString());

            auto collisionCondition = CollisionCondition::createInstance(collisionConditionType, this);
            auto cmpOperation = ComparisonOperation::createInstance(cmpOperationType, collisionCondition);
            collisionCondition->setCmpOperation(cmpOperation);
            collisionCondition->init();
            if (!collisionCondition->read(jsonObj))
            {
                result = false;
            }
            executeAddConfigObject(i, collisionCondition);
        }
        else
        {
            qCritical() << "Key not found: collisionConditionType or cmpOperation.cmpOperationType, Json object index:" << i;
            result = false;
        }
    }
    return result;
}

bool IgnoreCollisionConditionsConfig::add(int index, QString ignoreCollisionConditionType)
{
    if (index < 0 || index > this->count())
    {
        qCritical() << QString("Index %1 out of range [0, %2]").arg(index).arg(this->count());
        return false;
    }
    if (ignoreCollisionConditionType.isEmpty())
    {
        qCritical() << "ignoreCollisionConditionType is empty!";
        return false;
    }

    IgnoreCollisionCondition::Type ignoreCollisionConditionTypeEnum
        = IgnoreCollisionCondition::TypeEnumInfo().nameToEnum(ignoreCollisionConditionType);

    auto ignoreCollisionCondition = IgnoreCollisionCondition::createInstance(ignoreCollisionConditionTypeEnum, this);
    ignoreCollisionCondition->setIgnoreCollisionConditionType(ignoreCollisionConditionType);

    return ConfigManager::getIns()->executeCommand(new AddConfigObjectComand(this, index, ignoreCollisionCondition));
}

bool IgnoreCollisionConditionsConfig::read(const QJsonValue &jsonValue)
{
    QJsonArray jsonArr = jsonValue.toArray();
    bool result = true;

    while (count() > 0)
    {
        auto configObj = at(count() - 1);
        executeRemoveConfigObject(count() - 1);
        configObj->setParent(nullptr);
        configObj->deleteLater();
    }

    for (int i = 0; i < jsonArr.count(); i++)
    {
        QJsonObject jsonObj = jsonArr[i].toObject();
        if (jsonObj.contains("ignoreCollisionConditionType"))
        {
            IgnoreCollisionCondition::Type ignoreCollisionConditionTypeEnum
                = IgnoreCollisionCondition::TypeEnumInfo().nameToEnum(jsonObj["ignoreCollisionConditionType"].toString());
            auto ignoreCollisionCondition = IgnoreCollisionCondition::createInstance(ignoreCollisionConditionTypeEnum, this);

            if (!ignoreCollisionCondition->read(jsonObj))
            {
                result = false;
            }
            executeAddConfigObject(i, ignoreCollisionCondition);
        }
        else
        {
            qCritical() << "Key not found: ignoreCollisionConditionType, Json object index:" << i;
            result = false;
        }
    }
    return result;
}

CollisionGroup::CollisionGroup(QObject *parent) : ConfigObject(parent)
{
    m_collisionConditions = new CollisionConditionsConfig();
    m_ignoreCollisionConditions = new IgnoreCollisionConditionsConfig();
    init();
}

bool CollisionGroup::willCollide(QString name, double targetState)
{
    if (!m_enable)
    {
        return false;
    }
    for (int i = 0; i < ignoreCollisionConditions()->count(); i++)
    {
        auto ignoreCollisionCondition = ignoreCollisionConditions()->getConfig<IgnoreCollisionCondition>(i);
        if (ignoreCollisionCondition->meetCondition(name, targetState))
        {
            return false;
        }
    }
    for (int i = 0; i < collisionConditions()->count(); i++)
    {
        auto collisionCondition = collisionConditions()->getConfig<CollisionCondition>(i);
        if (!collisionCondition->meetCondition(name, targetState))
        {
            return false;
        }
    }
    return true;
}

QString CollisionGroup::getCollisionDescription()
{
    QString description = groupDescription();

    for (int i = 0; i < collisionConditions()->count(); i++)
    {
        auto collisionCondition = collisionConditions()->getConfig<CollisionCondition>(i);
        description += "\r\n";
        description += collisionCondition->getCollisionDescription();
    }
    return description;
}
