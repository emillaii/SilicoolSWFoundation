#include "hikvision.h"
#include "QThread"
HikVision::HikVision(QObject *parent)
{
    Init();
}

HikVision::~HikVision()
{
    // unload vision master
}

int HikVision::Init()
{
    qDebug() << "Start server...";

    if(m_handle!=nullptr)
    {
        qDebug()<<"m_handle!=nullptr";
        IMVS_PF_DestroyHandle(m_handle);
        m_handle = IMVS_NULL;
    }
    qDebug()<<"m_handle==nullptr";

    int nRet = IMVS_PF_CreateHandle(&m_handle, mHikVisionMasterServerPath.toLatin1().data());
     qDebug()<<"IMVS_PF_CreateHandle" << nRet;
//    if(IMVS_EC_OK != nRet)
//    {
//        nRet = IMVS_PF_CreateHandle(&m_handle);
//         qDebug()<<"IMVS_PF_CreateHandle2" << nRet;
//    }

    unsigned int nLoadprogress = 0;

//    nRet = IMVS_PF_StartVisionMaster(m_handle, mHikVisionMasterAppPath.toUtf8(),50000);
//    qDebug()<<"IMVS_PF_StartVisionMaster" << nRet;

//    nRet = IMVS_PF_ShowVisionMaster(m_handle, 1);
//    qDebug()<<"IMVS_PF_ShowVisionMaster" << nRet;

    nRet = IMVS_PF_RegisterResultCallBack_V30(m_handle, CallBackModuRes, this);
    if (IMVS_EC_OK != nRet)
    {
        qDebug()<<"IMVS_PF_RegisterResultCallBack_V30 Error" << nRet;
    }
    else
    {
        qDebug()<<"IMVS_PF_RegisterResultCallBack_V30 OK" << nRet;
    }
    nRet = IMVS_PF_LoadSolution(m_handle, mSolutionPath.toUtf8(), "");
    qDebug()<<"IMVS_PF_LoadSolution" << nRet;
    QElapsedTimer qTimer;
    qTimer.start();
    bool IsTimeOut = false;
    while (nRet == IMVS_EC_OK)
    {
        nRet = IMVS_PF_GetLoadProgress(m_handle, &nLoadprogress);

        if(nLoadprogress == 100)
        {
            qDebug()<<"SolutionHaveBeenLoaded OK" << nRet;
            break;
        }
        if(qTimer.elapsed()>5000)
        {
            qDebug()<<"SolutionHaveBeenLoaded TimeOut" << nRet;
            break;
        }
    }
    return 0;
}

void HikVision::Close()
{
    IMVS_PF_CloseSolution(m_handle); //
    IMVS_PF_DestroyHandle(m_handle); //
}
bool HikVision::performPr(QImage &image, VisionLocationConfig *prConfig, PRResultImageInfo **resultImageInfo, PRResultStruct &prResult)
{
    //load vision master

    IMVS_PF_INPUT_IMAGE_INFO stImageData;

    //2.picture
    stImageData.nDataType = 0;
    stImageData.nModuleID = 0;
    stImageData.stImageDataInfo.pImgData = (char *)image.bits();//pixArray.data();//
    stImageData.stImageDataInfo.iImgDataLen = (int)image.sizeInBytes();
    stImageData.stImageDataInfo.iWidth = image.width();
    stImageData.stImageDataInfo.iHeight = image.height();
    stImageData.stImageDataInfo.iImgFormat = 1;
    int  nRet = IMVS_PF_SetImageData(m_handle, &stImageData); //

    //1.path
//    stImageData.nDataType = 1;
//    stImageData.nModuleID = 0;
    //std::string strImgPath = "C:\\Users\\Aini\\Desktop\\VisionMasterLearning\\BaslerPicture.png";//
    //strcpy_s(stInputImageInfo.strImagePath, IMVS_PF_MAX_PATH_LENGTH, "E:\\test.jpg");
    //_snprintf_s(stImageData.strImagePath, IMVS_PF_MAX_PATH_LENGTH, IMVS_PF_MAX_PATH_LENGTH - 1, "%s", strImgPath.c_str());
    //int nRet = IMVS_PF_SetImageData(m_handle, &stImageData); //
    if (IMVS_EC_OK != nRet)
    {
        qDebug()<<"IMVS_PF_SetImageData Error" << nRet;
    }
    else
    {
        qDebug()<<"IMVS_PF_SetImageData OK" << nRet;
    }

    nRet = IMVS_PF_ExecuteOnce(m_handle, NULL);
    if (IMVS_EC_OK != nRet)
    {
        qDebug()<<"IMVS_PF_ExecuteOnce Error" << nRet;
    }
    else
    {
        qDebug()<<"IMVS_PF_ExecuteOnce OK" << nRet;
    }

    //    IMVS_PF_CloseSolution(m_handle); //
    //    IMVS_PF_DestroyHandle(m_handle); //
    //QString strImgPath1 = QObject::tr("E:\\VisionMaster\\VisionMaster3.4.0\\Applications\\VisionMaster.exe");
    //QThread::msleep(10000);

    //HikVisionLocationConfig *hikVLCfg = qobject_cast<HikVisionLocationConfig *>(prConfig);
    //SC_ASSERT(hikVLCfg != nullptr);

    //HikVisionResultImageInfo *hikResultImageInfo = new HikVisionResultImageInfo();
    //(*resultImageInfo) = hikResultImageInfo;

    // TBD
    return true;

}

int __stdcall HikVision::CallBackModuRes(IN IMVS_PF_OUTPUT_PLATFORM_INFO * const pstInputPlatformInfo, IN void * const pUser)
{
    HikVision * pCtrlDemoThis = (HikVision *)pUser;
    int nRet = IMVS_EC_UNKNOWN;

    if (pCtrlDemoThis)
    {
        nRet = pCtrlDemoThis->CallBackModuResFunc(pstInputPlatformInfo);
        if (IMVS_EC_OK != nRet)
        {
            return nRet;
        }
    }

    return IMVS_EC_OK;
}

int HikVision::CallBackModuResFunc(IN IMVS_PF_OUTPUT_PLATFORM_INFO * const pstInputPlatformInfo)
{

    //	if (CDefine::IMVS_PF_STATUS_STOP_CALLBACK == m_nCallBackStatus)
    //	{
    //		return IMVS_EC_NOT_SUPPORT;
    //	}
    if (IMVS_NULL == pstInputPlatformInfo)
    {
        return IMVS_EC_INVALID_HANDLE;
    }
    if (IMVS_NULL == (pstInputPlatformInfo->pData))
    {
        return IMVS_EC_INVALID_HANDLE;
    }
    qDebug()<<"HikVision::CallBackModuResFunc"<<pstInputPlatformInfo->nInfoType;
    if (IMVS_ENUM_CTRLC_OUTPUT_PLATFORM_INFO_MODULE_RESULT == pstInputPlatformInfo->nInfoType)
    {
        // Parsing module callback results after V3.0
        IMVS_PF_MODU_RES_INFO * pstPFModuResInfoList = (IMVS_PF_MODU_RES_INFO *)pstInputPlatformInfo->pData;

        // Determine if the process ID matches
        int m_nProgressFlag = 0;
        qDebug()<<"CopyModuResultByModu start"<<pstPFModuResInfoList->nProcessID;
        if ((10000 == pstPFModuResInfoList->nProcessID)  && (0 == m_nProgressFlag))
        {
            CopyModuResultByModu(pstPFModuResInfoList);
            qDebug()<<"CopyModuResultByModu end";
        }
    }
}

int HikVision::CopyModuResultByModu(IN IMVS_PF_MODU_RES_INFO * const pstPFModuResInfoList)
{
    // HPFeatureMatch module
    if (0 == strcmp(MODU_NAME_HPFEATUREMATCHMODU, (pstPFModuResInfoList->strModuleName)))
    {
        IMVS_PF_HPFEATUREMATCH_MODU_INFO * pstHPFeatureMatchModuRes = (IMVS_PF_HPFEATUREMATCH_MODU_INFO *)pstPFModuResInfoList->pData;
        m_nMatchPtNum = pstHPFeatureMatchModuRes->iMatchNum;
        for (int i = 0; i < (int)m_nMatchPtNum; i++)
        {
            if (i < 64)    //This example shows 64 matches
            {
                nCDfine.m_stMatchFeatureRect.nNum = i + 1;
                nCDfine.m_stMatchFeatureRect.stMatchRectInfo[i].fCenterX = pstHPFeatureMatchModuRes->pstMatchBaseInfo[i].stMatchBox.stCenterPt.fPtX;
                nCDfine.m_stMatchFeatureRect.stMatchRectInfo[i].fCenterY = pstHPFeatureMatchModuRes->pstMatchBaseInfo[i].stMatchBox.stCenterPt.fPtY;
                nCDfine.m_stMatchFeatureRect.stMatchRectInfo[i].fHeight  = pstHPFeatureMatchModuRes->pstMatchBaseInfo[i].stMatchBox.fHeight;
                nCDfine.m_stMatchFeatureRect.stMatchRectInfo[i].fWidth   = pstHPFeatureMatchModuRes->pstMatchBaseInfo[i].stMatchBox.fWidth;
                nCDfine.m_stMatchFeatureRect.stMatchRectInfo[i].fAngle   = pstHPFeatureMatchModuRes->pstMatchBaseInfo[i].stMatchBox.fAngle;
                qDebug()<<"Match return"<<"fCenterX:"<<nCDfine.m_stMatchFeatureRect.stMatchRectInfo[i].fCenterX;
                qDebug()<<"Match return"<<"fCenterY:"<<nCDfine.m_stMatchFeatureRect.stMatchRectInfo[i].fCenterY;
                qDebug()<<"Match return"<<"fHeight:"<<nCDfine.m_stMatchFeatureRect.stMatchRectInfo[i].fHeight;
                qDebug()<<"Match return"<<"fWidth:"<<nCDfine.m_stMatchFeatureRect.stMatchRectInfo[i].fWidth;
                qDebug()<<"Match return"<<"fAngle:"<<nCDfine.m_stMatchFeatureRect.stMatchRectInfo[i].fAngle;
            }
        }

//        if (pstHPFeatureMatchModuRes->stMatchConInfo.iPtNum > 0)
//        {
//            if (NULL == nCDfine.m_stOutLineInfo.pstMatchPtInfo)
//            {
//                nCDfine.m_stOutLineInfo.pstMatchPtInfo = new(std::nothrow) IMVS_PATMATCH_POINT_INFO[pstHPFeatureMatchModuRes->stMatchConInfo.iPtNum];
//                if (NULL == nCDfine.m_stOutLineInfo.pstMatchPtInfo)
//                {
//                    return IMVS_EC_RESOURCE_CREATE;
//                }
//                nCDfine.m_stOutLineInfo.nLen = pstHPFeatureMatchModuRes->stMatchConInfo.iPtNum;
//            }
//            else
//            {
//                if (pstHPFeatureMatchModuRes->stMatchConInfo.iPtNum > int(nCDfine.m_stOutLineInfo.nLen))
//                {
//                    delete[] nCDfine.m_stOutLineInfo.pstMatchPtInfo;
//                    nCDfine.m_stOutLineInfo.pstMatchPtInfo = NULL;
//                    nCDfine.m_stOutLineInfo.pstMatchPtInfo = new(std::nothrow) IMVS_PATMATCH_POINT_INFO[pstHPFeatureMatchModuRes->stMatchConInfo.iPtNum];
//                    if (NULL == nCDfine.m_stOutLineInfo.pstMatchPtInfo)
//                    {
//                        return IMVS_EC_RESOURCE_CREATE;
//                    }
//                    nCDfine.m_stOutLineInfo.nLen = pstHPFeatureMatchModuRes->stMatchConInfo.iPtNum;
//                }
//                else
//                {
//                    memset(nCDfine.m_stOutLineInfo.pstMatchPtInfo, 0, (nCDfine.m_stOutLineInfo.nLen * sizeof(IMVS_PATMATCH_POINT_INFO)));
//                }
//            }

//            if (NULL != pstHPFeatureMatchModuRes->stMatchConInfo.pstPatMatchPt)
//            {
//                memcpy_s(nCDfine.m_stOutLineInfo.pstMatchPtInfo, ((pstHPFeatureMatchModuRes->stMatchConInfo.iPtNum) * sizeof(IMVS_PATMATCH_POINT_INFO)),
//                         pstHPFeatureMatchModuRes->stMatchConInfo.pstPatMatchPt,
//                         ((pstHPFeatureMatchModuRes->stMatchConInfo.iPtNum) * sizeof(IMVS_PATMATCH_POINT_INFO)));
//                nCDfine.m_stOutLineInfo.nLen = pstHPFeatureMatchModuRes->stMatchConInfo.iPtNum;
//            }
//        }
    }
    else
    {
        return IMVS_EC_NOT_SUPPORT;
    }
    return IMVS_EC_OK;
}

