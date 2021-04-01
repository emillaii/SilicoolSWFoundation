#ifndef FeedingMap_H
#define FeedingMap_H

#include "FeedingMapData.h"
#include "trayMap/traymap.h"

class FeedingMap : public TrayMap
{
    Q_OBJECT
public:
    FeedingMap(TrayConfig *trayConfig, QString materialDataStoreFileName)
        : TrayMap(trayConfig, "PickArmMap", materialDataStoreFileName, &FeedingMapData::staticMetaObject)
    {

    }
};
#endif // FeedingMap_H
