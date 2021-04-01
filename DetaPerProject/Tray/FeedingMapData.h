#ifndef FeedingMapData_H
#define FeedingMapData_H

#include "enumhelper.h"
#include "trayMap/materialdata.h"

class FeedingMapData : public MaterialData
{
    Q_OBJECT

    Q_PROPERTY(int materialState READ materialState WRITE setMaterialState NOTIFY materialStateChanged)

public:
    enum MaterialState
    {
        Init,
        Ok,
        Ng,
        PickFailed,
        InUse,
        Lost
    };
    Q_ENUM(MaterialState)

    static EnumHelper<MaterialState> &MaterialStateEnumInfo()
    {
        static EnumHelper<MaterialState> instance(staticMetaObject, "MaterialState");
        return instance;
    }

    Q_INVOKABLE FeedingMapData(QObject *parent = nullptr) : MaterialData(parent)
    {
        init();
    }

    int materialState() const
    {
        return m_materialState;
    }

    // MaterialData interface
public:
    virtual void reset(int materialState) override
    {
        setMaterialState(materialState);
    }
    virtual QString getMaterialStateStr() override
    {
        return MaterialStateEnumInfo().enumToName(static_cast<MaterialState>(m_materialState));
    }
    virtual void setMaterialStateStr(QString targetState) override
    {
        setMaterialState(MaterialStateEnumInfo().nameToEnum(targetState));
    }

public slots:
    void setMaterialState(int materialState)
    {
        if (m_materialState == materialState)
            return;

        m_materialState = materialState;
        emit materialStateChanged(m_materialState);
        emit stateChanged(rowIndex(), columnIndex());
    }

signals:
    void materialStateChanged(int materialState);

private:
    int m_materialState = 0;
};

#endif // FeedingMapData_H
