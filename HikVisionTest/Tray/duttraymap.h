#ifndef DUTTRAYMAP_H
#define DUTTRAYMAP_H

#include "dutstate.h"
#include "trayMap/traymap.h"

class DutTrayMap : public TrayMap
{
    Q_OBJECT
public:
    DutTrayMap(TrayConfig *trayConfig, QString materialDataStoreFileName)
        : TrayMap(trayConfig, "DutTrayMap", materialDataStoreFileName, &DutData::staticMetaObject)
    {
    }
};

#endif    // DUTTRAYMAP_H
