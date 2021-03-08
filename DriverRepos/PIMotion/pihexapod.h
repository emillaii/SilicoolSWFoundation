#ifndef PIHEXAPOD_H
#define PIHEXAPOD_H

#include "PI_GCS2_DLL.h"
#include "piaxisconfig.h"
#include "piaxis.h"

class PIHexapod: public QObject
{
    Q_OBJECT

public:
    PIHexapod();
    ~PIHexapod(){}
    void setRotationCenter(double *pos);
    void setConnectMsg(int id, bool connect);

private:
    bool isConnected = false;
    int controllerID;
    QString coordSystemName = "SCAOA_CoordSystem";
    QString axisNameList;
    PIAxis *axisX;
    PIAxis *axisY;
    PIAxis *axisZ;
    PIAxis *axisA;
    PIAxis *axisB;
    PIAxis *axisC;
};

#endif // PIHEXAPOD_H
