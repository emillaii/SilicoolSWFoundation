#ifndef PICKARMMAP_H
#define PICKARMMAP_H

#include "PickArmMapData.h"
#include "trayMap/traymap.h"

class PickArmMap : public TrayMap
{
    Q_OBJECT
public:
    PickArmMap(TrayConfig *trayConfig, QString materialDataStoreFileName)
        : TrayMap(trayConfig, "PickArmMap", materialDataStoreFileName, &PickArmMapData::staticMetaObject)
    {

    }
};
#endif // PICKARMMAP_H
