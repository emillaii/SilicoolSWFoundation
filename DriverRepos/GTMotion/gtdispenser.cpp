#include "gtdispenser.h"

int GTDispenser::GtCore1DispenserCount = 0;
int GTDispenser::GtCore2DispenserCount = 0;

GTDispenser::GTDispenser(const QLoggingCategory &logCate, DispenserConfig *dispenserConfig, QObject *parent)
    : SCDispenser(logCate, dispenserConfig, parent)
{
}

void GTDispenser::init(SCAxis *xAxis, SCAxis *yAxis, SCAxis *zAxis, SCDO *shotGlueOut)
{
#define CHECK_GT_PRT(ptr)                                                                                                                            \
    if (ptr == nullptr)                                                                                                                              \
    {                                                                                                                                                \
        throw SilicolAbort(QString("GtDispenser init failed! %1 is null!").arg(#ptr));                                                               \
    }

    Q_UNUSED(zAxis);
    this->m_xAxis = qobject_cast<GTAxis *>(xAxis);
    this->m_yAxis = qobject_cast<GTAxis *>(yAxis);
    this->m_shotGlueOut = qobject_cast<GTDO *>(shotGlueOut);

    CHECK_GT_PRT(m_xAxis);
    CHECK_GT_PRT(m_yAxis);
    CHECK_GT_PRT(m_shotGlueOut);

    if (qAbs(m_xAxis->getGtAxisConfig()->index() - m_yAxis->getGtAxisConfig()->index()) > 7)
    {
        throw SilicolAbort(tr("The range of x y axis index must less than 8!"), EX_LOCATION);
    }
    SC_ASSERT(m_shotGlueOut->getGtioConfig()->index() <= 16)
    int xAxisCoreNo = m_xAxis->getGtAxisConfig()->coreNo();
    int yAxisCoreNo = m_yAxis->getGtAxisConfig()->coreNo();
    int shotGlueOutCoreNo = m_shotGlueOut->getGtioConfig()->coreNo();
    if (xAxisCoreNo != yAxisCoreNo || shotGlueOutCoreNo != xAxisCoreNo)
    {
        throw SilicolAbort(tr("Interpolation related axes and DO must in the same GT core!"), EX_LOCATION);
    }
    if (!qFuzzyCompare(m_xAxis->getGtAxisConfig()->scale(), m_yAxis->getGtAxisConfig()->scale()))
    {
        throw SilicolAbort(tr("Interpolation related xy axes must have the same scale!"), EX_LOCATION);
    }

    SC_ASSERT(xAxisCoreNo == 1 || xAxisCoreNo == 2)
    m_core = xAxisCoreNo;
    if (!giveCrdIndex)
    {
        if (m_core == 1)
        {
            GtCore1DispenserCount++;
            m_crd = GtCore1DispenserCount;
            m_psoIndex = m_crd;
            if (GtCore1DispenserCount > 2)
            {
                qWarning() << "More than 2 GTDispenser was created in Core1...";
            }
        }
        else
        {
            GtCore2DispenserCount++;
            m_crd = GtCore2DispenserCount;
            m_psoIndex = m_crd;
            if (GtCore2DispenserCount > 2)
            {
                qWarning() << "More than 2 GTDispenser was created in Core2...";
            }
        }
        giveCrdIndex = true;
    }
    SCDispenser::init(xAxis, yAxis, zAxis, shotGlueOut);
}

void GTDispenser::enterInterpolationMode()
{
    short minIndex = qMin(m_xAxis->getGtAxisConfig()->index(), m_yAxis->getGtAxisConfig()->index());
    checkResult1(GTN_SetCrdMapBase(m_core, m_crd, minIndex));
    TCrdPrm crdprm;
    checkResult1(GTN_GetCrdPrm(m_core, m_crd, &crdprm));
    crdprm.dimension = 2;
    int xMaxVel = m_xAxis->getGtAxisConfig()->maxVel() * m_xAxis->getGtAxisConfig()->scale() / VelCoeff;
    int yMaxVel = m_yAxis->getGtAxisConfig()->maxVel() * m_yAxis->getGtAxisConfig()->scale() / VelCoeff;
    int xMaxAcc = m_xAxis->getGtAxisConfig()->maxAcc() * m_xAxis->getGtAxisConfig()->scale() / AccCoeff;
    int yMaxAcc = m_yAxis->getGtAxisConfig()->maxAcc() * m_yAxis->getGtAxisConfig()->scale() / AccCoeff;
    crdprm.synVelMax = qMin(xMaxVel, yMaxVel);
    crdprm.synAccMax = qMin(xMaxAcc, yMaxAcc);
    crdprm.evenTime = 0;
    crdprm.profile[m_xAxis->getGtAxisConfig()->index() - minIndex] = 1;
    crdprm.profile[m_yAxis->getGtAxisConfig()->index() - minIndex] = 2;
    crdprm.setOriginFlag = 1;
    checkResult1(GTN_SetCrdPrm(m_core, m_crd, &crdprm));
    if (dispenserConfig()->useFlyDispensing())
    {
        gpoStation = (m_shotGlueOut->getGtioConfig()->index() - 1) / 10 + 1;
        gpoIndex = (m_shotGlueOut->getGtioConfig()->index() - 1) % 10 + 1;
        short permit = 0x2;
        checkResult1(GTN_SetTerminalPermitEx(m_core, gpoStation, MC_GPO, &permit, gpoIndex, 1));
    }
}

void GTDispenser::exitInterpolationMode()
{
    GTN_PosCompareStop(m_core, m_psoIndex);
    m_xAxis->setMoveMode(GTAxis::Pos, false);
    m_yAxis->setMoveMode(GTAxis::Pos, false);
    if (dispenserConfig()->useFlyDispensing())
    {
        short permit = 0x1;
        checkResult1(GTN_SetTerminalPermitEx(m_core, gpoStation, MC_GPO, &permit, gpoIndex, 1));
    }
}

bool GTDispenser::isFinished()
{
    short sRtn = GTN_CrdStatus(m_core, m_crd, &m_runStatus, &m_segment, 0);
    if (sRtn != CMD_SUCCESS)
    {
        qCritical() << "GTN_CrdStatus failed! Error code:" << sRtn;
        return false;
    }
    return m_runStatus == 0;
}

void GTDispenser::executePath(QVector<PathEndPoint> &path)
{
    checkResult1(GTN_CrdClear(m_core, m_crd, 0));

    if (dispenserConfig()->useFlyDispensing())
    {
        flyDispenseImpl(path);
    }
    else
    {
        generalDispenseImpl(path);
    }
}

void GTDispenser::generalDispenseImpl(QVector<PathEndPoint> &dispensePath)
{
    auto generalDispConfig = dispenserConfig()->generalDispenserConfig();
    if (dispensePath.length() == 2)
    {
        double lineLen = getLineLen(dispensePath[0], dispensePath[1]);
        double openOffset = generalDispConfig->openOffset();
        if (openOffset < 0)
        {
            openOffset += lineLen;
        }
        double closeOffset = generalDispConfig->closeOffset();
        if (closeOffset < 0)
        {
            closeOffset += lineLen;
        }
        if (closeOffset - openOffset < MinLineLen)
        {
            throw SilicolAbort("Error open close offset!", EX_LOCATION);
        }
        splitPath(dispensePath, 0, openOffset);
        splitPath(dispensePath, 1, closeOffset - openOffset);
    }
    else
    {
        splitPath(dispensePath, 0, generalDispConfig->openOffset());
        splitPath(dispensePath, dispensePath.length() - 2, generalDispConfig->closeOffset());
    }

    short doMask = 1 << (m_shotGlueOut->getGtioConfig()->index() - 1);
    bool reversal = m_shotGlueOut->getGtioConfig()->reversal();
    short trueValue = reversal ? 0 : 0xFFFF;
    short falsevalue = reversal ? 0xFFFF : 0;
    for (int i = 0; i < dispensePath.count(); i++)
    {
        if (dispensePath[i].pathType == ArcPath)
        {
            throw SilicolAbort("Unimplemented path type: ArcPath");
        }

        if (dispenserConfig()->enableDispensing())
        {
            if (i > 2 && i < dispensePath.length() - 1)
            {
                short value = dispensePath[i].isDisabled ? falsevalue : trueValue;
                checkResult1(GTN_BufIO(m_core, m_crd, MC_GPO, doMask, value));
            }
        }

        appendLinePoint(dispensePath[i]);

        if (dispenserConfig()->enableDispensing())
        {
            if (i == 1)
            {
                checkResult1(GTN_BufIO(m_core, m_crd, MC_GPO, doMask, trueValue));
            }
            if (i == dispensePath.length() - 2)
            {
                checkResult1(GTN_BufIO(m_core, m_crd, MC_GPO, doMask, falsevalue));
            }
        }
    }
    checkResult1(GTN_SetCrdJerk(m_core, m_crd, generalDispConfig->maxJerk() * m_xAxis->getGtAxisConfig()->scale() / JerkCoeff));
    checkResult1(GTN_CrdStart(m_core, 1 << (m_crd - 1), 0));
}

void GTDispenser::flyDispenseImpl(QVector<PathEndPoint> &dispensePath)
{
    double allLineLen = 0;
    for (int i = 0; i < dispensePath.count(); i++)
    {
        if (dispensePath[i].pathType == ArcPath)
        {
            throw SilicolAbort("Unimplemented path type: ArcPath");
        }

        appendLinePoint(dispensePath[i]);
        if (i > 0)
        {
            allLineLen += getLineLen(dispensePath[i - 1], dispensePath[i]);
        }
    }

    auto flyDispensingConfig = dispenserConfig()->flyDispenserConfig();

    if (dispenserConfig()->enableDispensing())
    {
        TPosCompareMode mode;
        checkResult1(GTN_GetPosCompareMode(m_core, m_psoIndex, &mode));
        mode.mode = 2;
        mode.dimension = 2;
        mode.sourceMode = 1;
        mode.sourceX = m_xAxis->getGtAxisConfig()->index();
        mode.sourceY = m_yAxis->getGtAxisConfig()->index();
        mode.outputMode = 0;
        mode.outputCounter = 1;
        mode.errorBand = 0;
        mode.outputPulseWidth = flyDispensingConfig->shotGlueDotTime() * 1000;
        checkResult1(GTN_SetPosCompareMode(m_core, m_psoIndex, &mode));

        TPosComparePsoPrm comparePsoPrm;
        checkResult1(GTN_PosCompareClear(m_core, m_psoIndex));
        checkResult1(GTN_GetPosComparePsoPrm(m_core, m_psoIndex, &comparePsoPrm));
        comparePsoPrm.count = 1;
        comparePsoPrm.startPosX = dispensePath[0].x * m_xAxis->getGtAxisConfig()->scale();
        comparePsoPrm.startPosY = dispensePath[0].y * m_yAxis->getGtAxisConfig()->scale();
        comparePsoPrm.gpo = m_shotGlueOut->getGtioConfig()->index();
        comparePsoPrm.syncPos = flyDispensingConfig->shotGlueDotStep() * m_xAxis->getGtAxisConfig()->scale();
        checkResult1(GTN_SetPosComparePsoPrm(m_core, m_psoIndex, &comparePsoPrm));
        checkResult1(GTN_PosCompareStart(m_core, m_psoIndex));
    }

    checkResult1(GTN_SetCrdJerk(m_core, m_crd, flyDispensingConfig->maxJerk() * m_xAxis->getGtAxisConfig()->scale() / JerkCoeff));
    checkResult1(GTN_CrdStart(m_core, 1 << (m_crd - 1), 0));
}

void GTDispenser::appendLinePoint(const PathEndPoint &point)
{
    double xAxisScale = m_xAxis->getGtAxisConfig()->scale();
    double yAxisScale = m_yAxis->getGtAxisConfig()->scale();

    double acc = dispenserConfig()->useFlyDispensing() ? dispenserConfig()->flyDispenserConfig()->maxAcc()
                                                       : dispenserConfig()->generalDispenserConfig()->maxAcc();
    acc = acc * xAxisScale / AccCoeff;
    checkResult1(GTN_LnXY(m_core, m_crd, point.x * xAxisScale, point.y * yAxisScale, point.maxVel * xAxisScale / VelCoeff, acc,
                          point.endVel * xAxisScale / VelCoeff, 0));
}

double GTDispenser::getLineLen(const PathEndPoint &point1, const PathEndPoint &point2)
{
    double deltaX = point1.x - point2.x;
    double deltaY = point1.y - point2.y;
    //    double deltaZ = point1.z - point2.z;
    //    return sqrt(deltaX * deltaX + deltaY * deltaY + deltaZ * deltaZ);
    return sqrt(deltaX * deltaX + deltaY * deltaY);
}

void GTDispenser::splitPath(QVector<PathEndPoint> &path, int index, double distanceForSplit)
{
    if (index + 1 >= path.length())
    {
        throw SilicolAbort("Error index!", EX_LOCATION);
    }
    PathEndPoint point1 = path[index];
    PathEndPoint point2 = path[index + 1];
    PathEndPoint newPoint;
    if (qFuzzyCompare(distanceForSplit, 0))
    {
        newPoint = point1;
    }
    else
    {
        double lineLen = getLineLen(point1, point2);
        if (distanceForSplit > lineLen)
        {
            distanceForSplit = lineLen - MinLineLen;
        }
        if (distanceForSplit < -lineLen)
        {
            distanceForSplit = -lineLen + MinLineLen;
        }
        double ratio = distanceForSplit / lineLen;
        if (ratio < 0)
        {
            ratio += 1;
        }
        newPoint = point2;
        newPoint.x = point1.x + (point2.x - point1.x) * ratio;
        newPoint.y = point1.y + (point2.y - point1.y) * ratio;
        //        newPoint.z = point1.z + (point2.z - point1.z) * ratio;
    }
    newPoint.endVel = point2.maxVel;
    path.insert(index + 1, newPoint);
}
