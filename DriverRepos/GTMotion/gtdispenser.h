#ifndef GTDISPENSER_H
#define GTDISPENSER_H

#include "CompositeElement/scdispenser.h"
#include "gtaxis.h"
#include "gtdo.h"
#include "ringnet.h"

class GTDispenser : public SCDispenser
{
    Q_OBJECT

public:
    GTDispenser(const QLoggingCategory &logCate, DispenserConfig *dispenserConfig, QObject *parent = nullptr);

    // SCDispenser interface
public:
    virtual void init(SCAxis *xAxis, SCAxis *yAxis, SCAxis *zAxis, SCDO *shotGlueOut) override;

protected:
    virtual void enterInterpolationMode() override;
    virtual void exitInterpolationMode() override;
    virtual bool isFinished() override;
    virtual void executePath(QVector<PathEndPoint> &path) override;

private:
    void generalDispenseImpl(QVector<PathEndPoint> &dispensePath);
    void flyDispenseImpl(QVector<PathEndPoint> &dispensePath);
    void appendLinePoint(const PathEndPoint &point);
    double getLineLen(const PathEndPoint &point1, const PathEndPoint &point2);
    void splitPath(QVector<PathEndPoint> &path, int index, double distanceForSplit);

private:
    const double MinLineLen = 0.001;

    static int GtCore1DispenserCount;
    static int GtCore2DispenserCount;
    bool giveCrdIndex = false;

    short m_crd = 1;
    int m_psoIndex = 1;

    short m_core = 1;

    short m_runStatus;
    long m_segment;
    double maxAcc = 0;

    short gpoStation;
    short gpoIndex;

    GTAxis *m_xAxis;
    GTAxis *m_yAxis;
    GTAxis *m_zAxis;
    GTDO *m_shotGlueOut;
};

#endif    // GTDISPENSER_H
