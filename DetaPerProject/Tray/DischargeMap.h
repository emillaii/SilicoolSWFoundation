#ifndef DischargeMap_H
#define DischargeMap_H

#include "DischargeMapData.h"
#include "trayMap/traymap.h"

class DischargeMap : public TrayMap
{
    Q_OBJECT
public:
    DischargeMap(TrayConfig *trayConfig, QString materialDataStoreFileName)
        : TrayMap(trayConfig, "DischargeMap", materialDataStoreFileName, &DischargeMapData::staticMetaObject)
    {

    }
};
#endif // DischargeMap_H
