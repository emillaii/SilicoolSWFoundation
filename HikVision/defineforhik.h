#ifndef DEFINEFORHIK_H
#define DEFINEFORHIK_H
#include "iMVS-6000PlatformSDKC.h"
#include "MvRender.h"
#include <vector>

 class CDefine
{
 public:
     const static unsigned int IMVS_PF_STATUS_START_CALLBACK   = 1;       // CH: 开始回调状态 | EN: Callback start status
     const static unsigned int IMVS_PF_STATUS_STOP_CALLBACK    = 0;       // CH: 停止回调状态 | EN: Callback stop status
     const static unsigned int IMVS_PF_STATUS_SHOW_CALLBACK    = 2;       // CH: 显示回调状态 | EN: Show callback status
     const static unsigned int IMVS_PF_CALLBACK_NORMAL         = 0;       // CH: 传统数据回调方式 | EN: Traditional data callbacks way
     const static unsigned int IMVS_PF_CALLBACK_V30            = 1;       // CH: V30版本数据回调方式 | EN: V30 version data callbacks way
     const static unsigned int IMVS_PF_CALLBACK_V32            = 2;       // CH: V32版本数据回调方式 | EN: V32 version data callbacks way
     const static unsigned int IMVS_PF_PROGRESS_SHOW           = 1;       // CH: 方案加载保存进度显示 | EN: Show the solution loading and saving progress
     const static unsigned int IMVS_PF_MAX_DRAWPOINTS_NUM      = 1024;    // CH: 特征匹配点最大绘制数量 | EN: The maximum number of feature matching points

    // CH: 其他变量 | EN: Other variables
        struct
        {
            unsigned char* pData;        // CH: 实际数据 | EN: Data pointer
            unsigned int   nSize;        // CH: 数据分配大小 | EN: Data size
            unsigned int   nLen;         // CH: 数据实际大小 | EN: Data len
            unsigned int   nReserved[4]; // CH: 保留字段 | EN: Reserved
        }m_stImgData;

        // CH: 特征匹配框信息结构体 | EN: Feature matching box information structure
        typedef struct IMVS_MATCH_RECT_INFO__
        {
            float   fCenterX;     // CH: 匹配框中心X | EN: Match box center X
            float   fCenterY;     // CH: 匹配框中心Y | EN: Match box center Y
            float   fWidth;       // CH: 匹配框宽度 | EN: Match box width
            float   fHeight;      // CH: 匹配框高度 | EN: Matching box height
            float   fAngle;       // CH: 角度 | EN: Angle
            float   fReserved[4]; // CH: 保留字段 | EN: Reserved
        }IMVS_MATCH_RECT_INFO;

        struct
        {
            // CH: 本示例最多展示64个匹配结果 | EN: This example shows up to 64 matches
            IMVS_MATCH_RECT_INFO stMatchRectInfo[64];
            unsigned int         nNum;    // CH: 匹配个数 | EN: Number of matches
        }m_stMatchFeatureRect;

        MVRRECTF m_stMatchRect[64];   // CH: 模板匹配框结构体 | EN: Template matching box structure

//        CDC*     m_pcPicDC;           // CH: 图像PictureControl的DC | EN: Picture Control DC
//        CDC      m_cSrcCacheDC;       // CH: 图像缓存DC | EN: Picture Cache DC
//        CRect    m_cRectSrcClient;    // CH: 图像客户区区域 | EN: Picture client
//        CStatic* m_pcSrcPic;          // CH: 图像Picture指针 | EN: Picture point
        void*    m_handle;            // CH: 操作句柄 | EN: Handle
        void*    m_pRenderHandle;     // CH: 渲染句柄 | EN: Render handle

        unsigned int m_nImgWidth;       // CH: 图像宽度 | EN: Image width
        unsigned int m_nImgHeight;      // CH: 图像高度 | EN: Image height
        unsigned int m_nImgFormat;      // CH: 图像像素格式 | EN: Image pixel format
        unsigned int m_nImgFmtTemp;     // CH: 图像像素格式临时值 | EN: Image pixel format temporary value
        unsigned int m_nCallBackStatus; // CH: 回调状态 | EN: Callback status
        unsigned int m_nCallBackShow;   // CH: 显示回调结果状态 | EN: Display callback result status

       // HWND         m_hwndDisplay;     // CH: 渲染窗口句柄 | EN: Render window handle
        MVRCOLORF    m_stColor;         // CH: 显示颜色 | EN: Color
        MVRPOINTF    m_stCirCenter;     // CH: 圆心 | EN: Center of circle
        float        m_fR1;             // CH: 半径1 | EN: Radius1
        float        m_fR2;             // CH: 半径2 | EN: Radius2

        // CH: 绘制圆形结构体 | EN: Drawing a circular structure
        struct
        {
            // CH: 最大支持64个 | EN: The maximum number of support is 64
            unsigned int nCirNum;            // CH: 绘制圆数目 | EN: Number of circles
            MVRPOINTF    stCirCenter[64];    // CH: 圆心 | EN: Center of circle
            float        fR1[64];            // CH: 半径1 | EN: Radius1
            float        fR2[64];            // CH: 半径2 | EN: Radius2
        }m_stDrawCirInfo;

        std::vector<std::string> m_vctStrParamVal; // CH: 模块参数值 | EN: Module parameter list

        // CH: 特征匹配点指针 | EN: Feature matching points' pointer
        struct
        {
            IMVS_PATMATCH_POINT_INFO* pstMatchPtInfo;    // CH: 匹配特征点指针 | EN: Feature matching points' pointer
            unsigned int nLen;                           // CH: 匹配个数 | EN: Number of matches
        }m_stOutLineInfo;

        unsigned int m_nMatchPtNum;    // CH: 匹配点数量 | EN: Number of matching points
       // CRect        m_cRectImg;       // CH: 图像适应Pic控件大小及位置 | EN: Image size and location in picture control
      //  CRect        m_cRectImgCopy;   // CH: 图像适应Pic控件大小及位置备份 | EN: Image size and location copy in picture control
        unsigned int m_nWorkStatus;    // CH: 流程工作状态 | EN: Process work status
        unsigned char m_chImgdata[10]; // CH: 图像对比数据 | EN: Image comparison data
        int          m_nShowProcessID; // CH: 显示用流程ID | EN: Process ID for showing
        unsigned int m_nProgressFlag;  // CH: 显示加载或保存进度以及选择下拉显示流程标志位 | EN: The flag of load and save progress
       // CRITICAL_SECTION m_ImgDataSection; // CH: 图像数据临界区 | EN: The critical section of image data
        unsigned int  m_nMainCallBackWay;  // CH: 注册回调类型 | EN: Register callback type
        unsigned int  m_nCallBackShowV32;  // CH: 显示V32回调配置结果 | EN: Show V32 callback configuration results

};

#endif // DEFINEFORHIK_H
