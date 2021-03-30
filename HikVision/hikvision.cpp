#include "hikvision.h"

SILICOOL_CREATE_LOGGING_CATEGORY(hikCate, "HikVision")

HikVision::HikVision(QObject *parent)
{
    hikVisionConfig = new HikVisionConfig(this);
    hikVisionConfigFile = new ConfigFile("HikVisionConfig", hikVisionConfig, "./config/hikVisionConfig.json", false);
    hikVisionConfigFile->populate(true);
}

HikVision::~HikVision() {}

HikVisionConfig *HikVision::config() const
{
    return hikVisionConfig;
}

void HikVision::initImpl()
{
    if (m_handle != nullptr)
    {
        IMVS_PF_DestroyHandle(m_handle);
        m_handle = IMVS_NULL;
    }

    CHECK_MVS_RES(IMVS_PF_CreateHandle(&m_handle, hikVisionConfig->visionServerPath().toUtf8()));
    CHECK_MVS_RES(IMVS_PF_RegisterResultCallBack_V30(m_handle, callBackModuRes, this));
    loadSolution();
}

void HikVision::disposeImpl()
{
    if (m_handle != nullptr)
    {
        IMVS_PF_RegisterResultCallBack_V30(m_handle, nullptr, this);
        IMVS_PF_CloseSolution(m_handle);
        IMVS_PF_CloseVisionMaster(m_handle);
        IMVS_PF_DestroyHandle(m_handle);
    }
}

bool HikVision::performPr(QImage &image, VisionLocationConfig *prConfig, PRResultImageInfo **resultImageInfo, PRResultStruct &prResult)
{
    SCTimer sct(QString("performPr: %1").arg(prConfig->locationName()), hikCate());

    HikVisionLocationConfig *hikPrConfig = qobject_cast<HikVisionLocationConfig *>(prConfig);
    SC_ASSERT(hikPrConfig != nullptr);
    SC_ASSERT(image.format() == QImage::Format_Indexed8);

    IMVS_PF_INPUT_IMAGE_INFO stImageData;
    stImageData.nDataType = 0;
    stImageData.nModuleID = hikPrConfig->imageSourceModuleId();
    stImageData.stImageDataInfo.pImgData = (char *)image.bits();
    stImageData.stImageDataInfo.iImgDataLen = (int)image.sizeInBytes();
    stImageData.stImageDataInfo.iWidth = image.width();
    stImageData.stImageDataInfo.iHeight = image.height();
    stImageData.stImageDataInfo.iImgFormat = 1;

    {
        SCTimer sct("IMVS_PF_SetImageData", hikCate());

        int nRet = IMVS_PF_SetImageData(m_handle, &stImageData);
        if (IMVS_EC_OK != nRet)
        {
            qCCritical(hikCate()) << prConfig->locationName() << "IMVS_PF_SetImageData Error, error code:" << QString::number(nRet, 16);
            return false;
        }
    }

    int nRet = IMVS_PF_ExecuteOnce_V30(m_handle, hikPrConfig->processId(), nullptr);
    if (IMVS_EC_OK != nRet)
    {
        qCCritical(hikCate()) << prConfig->locationName() << "IMVS_PF_ExecuteOnce_V30 Error, error code:" << QString::number(nRet, 16);
        return false;
    }

    auto hikResult = new HikVisionResult();
    hikResult->resultModuleId = hikPrConfig->resultModuleId();
    auto hikResultImageInfo = new HikVisionResultImageInfo();
    (*resultImageInfo) = hikResultImageInfo;
    hikResult->resultImageInfo = hikResultImageInfo;
    AutoDeleter<HikVisionResult> t(hikResult);
    {
        QMutexLocker l(&hikResultsLocker);
        hikResults[hikPrConfig->processId()] = hikResult;
    }

    bool waitResult;
    {
        QMutex resultLocker;
        QMutexLocker l(&resultLocker);
        waitResult = hikResult->waiter.wait(&resultLocker, 3000);
    }
    {
        QMutexLocker l(&hikResultsLocker);
        hikResults.remove(hikPrConfig->processId());
    }

    if (!waitResult)
    {
        qCCritical(hikCate()) << prConfig->locationName() << "wait pr result timeout!";
        return false;
    }
    if (!hikResult->errString.isEmpty())
    {
        qCCritical(hikCate()) << prConfig->locationName() << hikResult->errString;
        return false;
    }

    prResult.x = hikResult->x;
    prResult.y = hikResult->y;
    prResult.theta = hikResult->theta;
    prResult.width = hikResult->width;
    prResult.height = hikResult->height;
    return true;
}

void HikVision::loadSolution()
{
    try
    {
        QString solutionPath = QDir(VisionConfigDir::getIns().dutRelatedConfigDir()).absoluteFilePath("HikSolution.sol");
        qDebug() << "Load solution..." << solutionPath;
        CHECK_MVS_RES(IMVS_PF_SyncLoadSolution(m_handle, solutionPath.toUtf8(), "", false));
    }
    catch (SilicoolException &se)
    {
        qCritical() << se.what();
    }
}

void HikVision::startVisionMaster(bool start)
{
    if (start)
    {
        CHECK_MVS_RES(IMVS_PF_StartVisionMaster(m_handle, hikVisionConfig->visionMasterPath().toUtf8(), 10000));
    }
    else
    {
        CHECK_MVS_RES(IMVS_PF_CloseVisionMaster(m_handle));
    }
}

void HikVision::showVisionMaster(bool show)
{
    CHECK_MVS_RES(IMVS_PF_ShowVisionMaster(m_handle, show ? 1 : 0));
}

void HikVision::printProcessModuleIds()
{
    IMVS_PF_PROCESS_INFO_LIST processInfos;
    CHECK_MVS_RES(IMVS_PF_GetAllProcessList(m_handle, &processInfos));
    qDebug() << "Process count:" << processInfos.nNum;
    for (int i = 0; i < processInfos.nNum; i++)
    {
        IMVS_PF_PROCESS_INFO *processInfo = &processInfos.astProcessInfo[i];
        qDebug() << "Process" << processInfo->nProcessID << "name" << processInfo->strProcessName;

        IMVS_PF_MODULE_INFO_LIST moduleInfos;
        CHECK_MVS_RES(IMVS_PF_GetModulesByProcessId(m_handle, processInfo->nProcessID, &moduleInfos));
        qDebug() << "Module count:" << moduleInfos.nNum;

        for (int j = 0; j < moduleInfos.nNum; j++)
        {
            IMVS_PF_MODULE_INFO *moduleInfo = &moduleInfos.astModuleInfo[j];
            qDebug() << "process" << moduleInfo->nProcessID << "module id" << moduleInfo->nModuleID << "moduleName" << moduleInfo->strModuleName;
        }
    }
}

int HikVision::callBackModuRes(IN IMVS_PF_OUTPUT_PLATFORM_INFO *const pstInputPlatformInfo, IN void *const pUser)
{
    HikVision *pCtrlDemoThis = (HikVision *)pUser;
    if (pCtrlDemoThis == IMVS_NULL)
    {
        qCritical() << "pCtrlDemoThis is null!";
        return IMVS_EC_NULL_PTR;
    }
    if (pstInputPlatformInfo == IMVS_NULL)
    {
        qCritical() << "pstInputPlatformInfo is null!";
        return IMVS_EC_NULL_PTR;
    }
    if (IMVS_NULL == (pstInputPlatformInfo->pData))
    {
        qCritical() << "pstInputPlatformInfo->pData is null!";
        return IMVS_EC_NULL_PTR;
    }
    return pCtrlDemoThis->callBackModuResFunc(pstInputPlatformInfo);
}

int HikVision::callBackModuResFunc(IN IMVS_PF_OUTPUT_PLATFORM_INFO *const pstInputPlatformInfo)
{
    //    if(pstInputPlatformInfo->nInfoType != 7)
    //    {
    //        qDebug() << "CallBackModuResFunc" << pstInputPlatformInfo->nInfoType;
    //    }
    //    if(IMVS_ENUM_CTRLC_OUTPUT_PLATFORM_INFO_WORK_STATE == pstInputPlatformInfo->nInfoType)
    //    {
    //        IMVS_PF_MODULE_WORK_STAUS__ *info = (IMVS_PF_MODULE_WORK_STAUS__ *)pstInputPlatformInfo->pData;
    //        qDebug() << "nWorkStatus" << info->nWorkStatus <<"nProcessID" <<  info->nProcessID;
    //    }

    if (IMVS_ENUM_CTRLC_OUTPUT_PLATFORM_INFO_STOP == pstInputPlatformInfo->nInfoType)
    {
        IMVS_PF_STATUS_STOP_INFO__ *info = (IMVS_PF_STATUS_STOP_INFO__ *)pstInputPlatformInfo->pData;

        QMutexLocker l(&hikResultsLocker);
        if (hikResults.contains(info->nProcessID))
        {
            HikVisionResult *hikResult = hikResults[info->nProcessID];
            if (!hikResult->gotModuleResult)
            {
                hikResult->errString = "Process stopped!";
                hikResult->waiter.wakeAll();
            }
        }
        return IMVS_EC_OK;
    }

    if (IMVS_ENUM_CTRLC_OUTPUT_PLATFORM_INFO_MODULE_RESULT == pstInputPlatformInfo->nInfoType)
    {
        IMVS_PF_MODU_RES_INFO *info = (IMVS_PF_MODU_RES_INFO *)pstInputPlatformInfo->pData;

        QMutexLocker l(&hikResultsLocker);
        if (!hikResults.contains(info->nProcessID))
        {
            return IMVS_EC_OK;
        }
        auto hikResult = hikResults[info->nProcessID];
        if (info->nStatus != 1)
        {
            hikResult->errString = QString("%1 failed! Error code: %2").arg(info->strModuleName).arg(QString::number(info->nErrorCode, 16));
            hikResult->waiter.wakeAll();
            return IMVS_EC_OK;
        }
        if (info->nModuleID != hikResult->resultModuleId)
        {
            return IMVS_EC_OK;
        }

        hikResult->gotModuleResult = true;

        if (strcmp(info->strModuleName, MODU_NAME_HPFEATUREMATCHMODU) == 0)
        {
            IMVS_PF_HPFEATUREMATCH_MODU_INFO__ *hpMathInfo = (IMVS_PF_HPFEATUREMATCH_MODU_INFO__ *)info->pData;

            if (hpMathInfo->iMatchNum < 1 || hpMathInfo->iModuStatu != 1)
            {
                hikResult->errString = "feature match failed!";
                hikResult->waiter.wakeAll();
                return IMVS_EC_OK;
            }
            auto matchBaseInfo = hpMathInfo->pstMatchBaseInfo[0];
            hikResult->theta = matchBaseInfo.stMatchBox.fAngle;
            hikResult->x = matchBaseInfo.stMatchPt.stMatchPt.fPtX;
            hikResult->y = matchBaseInfo.stMatchPt.stMatchPt.fPtY;
            hikResult->resultImageInfo->m_point = QPointF(hikResult->x, hikResult->y);
            for (int i = 0; i < hpMathInfo->stMatchConInfo.iPtNum; i++)
            {
                QPointF p1
                    = QPointF(hpMathInfo->stMatchConInfo.pstPatMatchPt[i].fMatchOutlineX, hpMathInfo->stMatchConInfo.pstPatMatchPt[i].fMatchOutlineY);
                hikResult->resultImageInfo->m_points.append(p1);
            }
            hikResult->resultImageInfo->m_text = "Match Point:(" + QString::number(hikResult->x, '.', 3) + "," + QString::number(hikResult->y, '.', 3)
                                                 + ")" + ", " + "Angle:" + QString::number(hikResult->theta, '.', 6);

            hikResult->waiter.wakeAll();
            return IMVS_EC_OK;
        }

        if (strcmp(info->strModuleName, MODU_NAME_FASTFEATUREMATCHMODU) == 0)
        {
            IMVS_PF_FASTFEATUREMATCH_MODU_INFO__ *fMathInfo = (IMVS_PF_FASTFEATUREMATCH_MODU_INFO__ *)info->pData;

            if (fMathInfo->iMatchNum < 1 || fMathInfo->iModuStatu != 1)
            {
                hikResult->errString = "feature match failed!";
                hikResult->waiter.wakeAll();
                return IMVS_EC_OK;
            }
            auto matchBaseInfo = fMathInfo->pstMatchBaseInfo[0];
            hikResult->theta = matchBaseInfo.stMatchBox.fAngle;
            hikResult->x = matchBaseInfo.stMatchPt.stMatchPt.fPtX;
            hikResult->y = matchBaseInfo.stMatchPt.stMatchPt.fPtY;

            hikResult->resultImageInfo->m_point = QPointF(hikResult->x, hikResult->y);
            for (int i = 0; i < fMathInfo->stMatchConInfo.iPtNum; i++)
            {
                QPointF p1
                    = QPointF(fMathInfo->stMatchConInfo.pstPatMatchPt[i].fMatchOutlineX, fMathInfo->stMatchConInfo.pstPatMatchPt[i].fMatchOutlineY);
                hikResult->resultImageInfo->m_points.append(p1);
            }
            hikResult->resultImageInfo->m_text = "Match Point:(" + QString::number(hikResult->x, '.', 3) + "," + QString::number(hikResult->y, '.', 3)
                                                 + ")" + ", " + "Angle:" + QString::number(hikResult->theta, '.', 6);

            hikResult->waiter.wakeAll();
            return IMVS_EC_OK;
        }

        if (strcmp(info->strModuleName, MODU_NAME_QUADRANGEFINDMODULE) == 0)
        {
            IMVS_PF_QUADRANGEFIND_MODU_INFO *rectInfo = (IMVS_PF_QUADRANGEFIND_MODU_INFO *)info->pData;

            if (rectInfo->iModuStatu != 1)
            {
                hikResult->errString = "match rectangle failed!";
                hikResult->waiter.wakeAll();
                return IMVS_EC_OK;
            }
            hikResult->x = rectInfo->stCentralPoint.fPtX;
            hikResult->y = rectInfo->stCentralPoint.fPtY;
            hikResult->theta = calcAngle(rectInfo->stEdgeLine0);
            hikResult->resultImageInfo->m_point = QPointF(hikResult->x, hikResult->y);
            hikResult->resultImageInfo->m_lines.append(QLineF(rectInfo->stEdgeLine0.stStartPt.fPtX, rectInfo->stEdgeLine0.stStartPt.fPtY,
                                                              rectInfo->stEdgeLine0.stEndPt.fPtX, rectInfo->stEdgeLine0.stEndPt.fPtY));
            hikResult->resultImageInfo->m_lines.append(QLineF(rectInfo->stEdgeLine1.stStartPt.fPtX, rectInfo->stEdgeLine1.stStartPt.fPtY,
                                                              rectInfo->stEdgeLine1.stEndPt.fPtX, rectInfo->stEdgeLine1.stEndPt.fPtY));
            hikResult->resultImageInfo->m_lines.append(QLineF(rectInfo->stEdgeLine2.stStartPt.fPtX, rectInfo->stEdgeLine2.stStartPt.fPtY,
                                                              rectInfo->stEdgeLine2.stEndPt.fPtX, rectInfo->stEdgeLine2.stEndPt.fPtY));
            hikResult->resultImageInfo->m_lines.append(QLineF(rectInfo->stEdgeLine3.stStartPt.fPtX, rectInfo->stEdgeLine3.stStartPt.fPtY,
                                                              rectInfo->stEdgeLine3.stEndPt.fPtX, rectInfo->stEdgeLine3.stEndPt.fPtY));

            hikResult->resultImageInfo->m_text = "Center Point:(" + QString::number(hikResult->x, '.', 3) + ","
                                                 + QString::number(hikResult->y, '.', 3) + ")" + ", "
                                                 + "Angle:" + QString::number(hikResult->theta, '.', 6);

            hikResult->waiter.wakeAll();
            return IMVS_EC_OK;
        }

        if (strcmp(info->strModuleName, MODU_NAME_CIRCLEFINDMODU) == 0)
        {
            IMVS_PF_CIRCLEFIND_MODU_INFO *circleInfo = (IMVS_PF_CIRCLEFIND_MODU_INFO *)info->pData;
            if (circleInfo->iModuStatu != 1)
            {
                hikResult->errString = "match circle failed!";
                hikResult->waiter.wakeAll();
                return IMVS_EC_OK;
            }
            hikResult->x = circleInfo->stCirPt.fPtX;
            hikResult->y = circleInfo->stCirPt.fPtY;
            hikResult->radius = circleInfo->fRadius;
            hikResult->resultImageInfo->m_center = QPointF(hikResult->x, hikResult->y);
            hikResult->resultImageInfo->m_radius = hikResult->radius;
            hikResult->resultImageInfo->m_text
                = "Center Point:(" + QString::number(hikResult->x, '.', 3) + "Radius:" + QString::number(hikResult->radius, '.', 3);

            hikResult->waiter.wakeAll();
            return IMVS_EC_OK;
        }

        if (strcmp(info->strModuleName, MODU_NAME_IMAGESHARPNESSMODU) == 0)
        {
            IMVS_PF_IMAGESHARPNESS_MODU_INFO *imagesharpness = (IMVS_PF_IMAGESHARPNESS_MODU_INFO *)info->pData;
            if (imagesharpness->iModuStatu != 1)
            {
                hikResult->errString = "imagesharpness failed!";
                hikResult->waiter.wakeAll();
                return IMVS_EC_OK;
            }
            hikResult->fsharpness = imagesharpness->fSharpness;
            hikResult->resultImageInfo->m_text = "image sharpness:(" + QString::number(hikResult->fsharpness, '.', 3);

            hikResult->waiter.wakeAll();
            return IMVS_EC_OK;
        }
    }
    return IMVS_EC_OK;
}

int HikVision::copyModuResultByModu(IN IMVS_PF_MODU_RES_INFO *const pstPFModuResInfoList)
{
    if (0 == strcmp(MODU_NAME_SAVEIMAGE, (pstPFModuResInfoList->strModuleName)))
    {
        if (0 == pstPFModuResInfoList->nModuleID)
        {

            IMVS_PF_SAVEIMAGE_MODU_INFO__ *pOutPutImageModuRes = (IMVS_PF_SAVEIMAGE_MODU_INFO__ *)pstPFModuResInfoList->pData;

            int callBackImgFormate = pOutPutImageModuRes->stOutImgInfo.iImgFormat;
            auto qImgFormate = (callBackImgFormate == IMVS_PF_IMG_FORMAT_MONO8) ? QImage::Format_Indexed8 : QImage::Format_RGB888;
            QImage image(pOutPutImageModuRes->stOutImgInfo.iWidth, pOutPutImageModuRes->stOutImgInfo.iHeight, qImgFormate);

            qDebug() << "hik image info" << pOutPutImageModuRes->stOutImgInfo.iWidth << pOutPutImageModuRes->stOutImgInfo.iHeight
                     << pOutPutImageModuRes->stOutImgInfo.iImgDataLen;

            qDebug() << "image info before load" << image.width() << image.height() << image.format() << image.byteCount() << image.bytesPerLine();

            if (image.bytesPerLine() == image.width() * 3)
            {
                qDebug() << "copy 1";
                memcpy(image.bits(), pOutPutImageModuRes->stOutImgInfo.pImgData, pOutPutImageModuRes->stOutImgInfo.iImgDataLen);
            }
            else
            {
                qDebug() << "copy 2";
                int bytesPerLine = image.width() * 3;
                for (int i = 0; i < image.height(); i++)
                {
                    memcpy(image.scanLine(i), pOutPutImageModuRes->stOutImgInfo.pImgData + i * bytesPerLine, bytesPerLine);
                }
            }
        }
    }
    else if (0 == strcmp(MODU_NAME_HPFEATUREMATCHMODU, (pstPFModuResInfoList->strModuleName)))
    {
        if (2 == pstPFModuResInfoList->nModuleID)
        {
            qDebug() << "get PR result Process1" << QDateTime::currentMSecsSinceEpoch();

            IMVS_PF_HPFEATUREMATCH_MODU_INFO *pstHPFeatureMatchModuRes = (IMVS_PF_HPFEATUREMATCH_MODU_INFO *)pstPFModuResInfoList->pData;
            m_nMatchPtNum = pstHPFeatureMatchModuRes->iMatchNum;
            IMVS_PF_2DPOINT_F point = pstHPFeatureMatchModuRes->pstMatchBaseInfo->stMatchPt.stMatchPt;
            QPointF pp = QPointF(point.fPtX, point.fPtY);

            for (int i = 0; i < (int)m_nMatchPtNum; i++)
            {
            }
        }
    }

    else if (0 == MODU_NAME_CIRCLEFINDMODU, (pstPFModuResInfoList->strModuleName))
    {
        if (2 == pstPFModuResInfoList->nModuleID)
        {
            IMVS_PF_CIRCLEFIT_MODU_INFO *pstCircleFindModuRes = (IMVS_PF_CIRCLEFIT_MODU_INFO *)pstPFModuResInfoList->pData;
            double radius = pstCircleFindModuRes->fRadius;
            IMVS_PF_2DPOINT_F center = pstCircleFindModuRes->stCentPt;
        }
    }
    else if (0 == MODU_NAME_IMAGESHARPNESSMODU, (pstPFModuResInfoList->strModuleName))
    {
        if (2 == pstPFModuResInfoList->nModuleID)
        {
            IMVS_PF_IMAGESHARPNESS_MODU_INFO *pstimagesharpnessModuRes = (IMVS_PF_IMAGESHARPNESS_MODU_INFO *)pstPFModuResInfoList->pData;
            double imagesharpness = pstimagesharpnessModuRes->fSharpness;
            QString text = QString::number(imagesharpness, '.', 4);
        }
    }

    return 0;
}

void HikVision::drawResultImage(QImage &image, PRResultImageInfo *resultImageInfo)
{
    SCTimer sct("drawResultImage", hikCate());

    HikVisionResultImageInfo *hikResultImageInfo = qobject_cast<HikVisionResultImageInfo *>(resultImageInfo);
    if (hikResultImageInfo != nullptr)
    {
        {
            SCTimer sct("image format convert", hikCate());
            image = image.convertToFormat(QImage::Format_RGB888);
        }

        QPainter painter(&image);
        // draw points
        if (!hikResultImageInfo->m_points.isEmpty())
        {
            painter.setPen(QPen(Qt::green, LineWidth));
            painter.drawPoints(hikResultImageInfo->m_points);
        }
        // draw line
        if (!hikResultImageInfo->m_line.isNull())
        {
            QLineF line = hikResultImageInfo->m_line;
            painter.setRenderHint(QPainter::Antialiasing, true);
            painter.setPen(QPen(Qt::green, LineWidth));
            painter.drawLine(line);
        }
        // draw lines
        if (!hikResultImageInfo->m_lines.isEmpty())
        {
            painter.setRenderHint(QPainter::Antialiasing, true);
            painter.setPen(QPen(Qt::blue, LineWidth));
            QLineF *lines = new QLineF[static_cast<unsigned long long>(hikResultImageInfo->m_lines.count())];
            for (int i = 0; i < hikResultImageInfo->m_lines.count(); i++)
            {
                lines[i] = hikResultImageInfo->m_lines.at(i);
            }
            painter.drawLines(lines, hikResultImageInfo->m_lines.count());
        }
        // draw rect
        if (!hikResultImageInfo->m_rect.isNull())
        {
            QRectF rect = hikResultImageInfo->m_rect;
            painter.setPen(QPen(QColor(0, 0, 255), LineWidth));
            // painter.setBrush(QColor(255, 160, 90));
            painter.drawRect(rect);
        }
        // draw circle
        if (hikResultImageInfo->m_radius > 0 && !hikResultImageInfo->m_center.isNull())
        {
            double radius = hikResultImageInfo->m_radius;
            QPointF center = hikResultImageInfo->m_center;
            painter.setPen(QPen(QColor(0, 0, 255), LineWidth));
            // painter.setBrush(QColor(255, 160, 90));
            painter.drawEllipse(center, radius, radius);
        }
        // draw ploygon
        if (!hikResultImageInfo->m_mpolygon.isEmpty())
        {
            QList<QPointF> polygon = hikResultImageInfo->m_mpolygon;
            painter.setPen(QPen(QColor(0, 0, 255), LineWidth));
            QPointF *Points = new QPointF[static_cast<unsigned long long>(polygon.count())];
            for (int i = 0; i < polygon.count(); i++)
            {
                Points[i] = polygon.at(i);
            }
            painter.drawPolygon(Points, polygon.count());
            delete[] Points;
        }
        // draw text
        if (!hikResultImageInfo->m_text.isNull())
        {
            QString text = hikResultImageInfo->m_text;
            painter.setPen(QPen(Qt::green, 1));
            painter.setBrush(Qt::NoBrush);
            QFont font;
            font.setFamily("宋体");
            font.setPixelSize(100);
            font.setBold(true);
            painter.setFont(font);
            int startPos = image.width() * 0.1;
            painter.drawText(startPos, startPos, text);
        }

        // draw point
        if (!hikResultImageInfo->m_point.isNull())
        {
            QPointF point = hikResultImageInfo->m_point;
            painter.setPen(QPen(Qt::red, 4));
            painter.drawLine(QPointF(point.x(), point.y() - LineLen), QPointF(point.x(), point.y() + LineLen));
            painter.drawLine(QPointF(point.x() - LineLen, point.y()), QPointF(point.x() + LineLen, point.y()));
        }
        // image.save("hikresult.png", "PNG", -1);
    }
    else
    {
        qCritical() << "no PR result";
    }
}
double HikVision::getObjectSharpness(QImage &image, VisionLocationConfig *prConfig, PRResultImageInfo **resultImageInfo)
{
    (*resultImageInfo) = new HikVisionResultImageInfo();
    cv::Mat imageSobel, imageLaplacian;

    cv::Mat srcGray(image.height(), image.width(), CV_8UC1, image.bits());
    double result = .0f;
    for (int i=srcGray.rows/4;i<3*srcGray.rows/4;i++)
    {
        uchar *data = srcGray.ptr<uchar>(i);
        for (int j=srcGray.cols/4;j<3*srcGray.cols/4;j++)
        {
            result += (data[j+2]-data[j])*(data[j+2]-data[j]);
        }
    }
    result = 4*result/srcGray.total();

    return result;
}
