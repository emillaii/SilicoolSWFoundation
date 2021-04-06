#ifndef TRAYLOADER_H
#define TRAYLOADER_H

#include "AxisModule/xymodule.h"
#include "CompositeElement/sccylinder.h"
#include "Tray/duttraymap.h"
#include "VisionLocation/visionlocation.h"
#include "VisionManager/visionmanager.h"
#include "namedefinition.h"
#include "systemCore/worker.h"
#include "trayMap/trayconfig.h"
#include "trayloaderconfig.h"
#include <QObject>

class TrayLoader : public Worker
{
    Q_OBJECT
public:
    explicit TrayLoader(TrayConfig *trayConfig, TrayMap *trayMap, TrayLoaderConfig *trayLoaderConfig, QObject *parent = nullptr);

public:
    void resetGotDut();
    const PrOffset &lastDutPrOffset();
    QPoint lastDutIndex() const;
    bool gotDut() const;

    // 自动运行逻辑
public slots:
    void getDut();
    void changeTray();

    // 手动调试操作
public slots:
    void moveToPrPos(int unitRowIndex, int unitColumnIndex, int rowIndex, int columnIndex);
    void prTest();

    // Worker interface
protected:
    virtual void initEventHandle() override;
    virtual void startEventHandle(const WorkerEvent &event) override;
    virtual void resetEventHandle() override;
    virtual void homeWorker(bool moveToReadyPosAfterHome) override;

private:
    void moveToPrPosImpl(int rowIndex, int columnIndex);
    void moveToPrPosImpl(int unitRowIndex, int unitColumnIndex, int rowIndex, int columnIndex);
    void verifyTrayMap();

private:
    XYModule *tlXy;
    SCCylinder *tlTrayClamp;
    SCDI *tlTraySensor;
    SCDO *tlDemoDO;
    VisionLocation *dutDownlookLocation;

    TrayConfig *trayConfig;
    TrayMap *trayMap;
    TrayLoaderConfig *trayLoaderConfig;

    bool m_gotDut = false;
    PrOffset m_lastDutOffset;
    QPoint m_lastDutIndex;
    int m_nContinuallyPrFailed = 0;
};

#endif    // TRAYLOADER_H
