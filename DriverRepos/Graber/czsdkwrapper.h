#ifndef SDKWRAPPER_H
#define SDKWRAPPER_H
#include "CmtiSdk.h"
#include <string>
#include <map>
#include <vector>

class DeviceClient;
class DeviceController
{
public:
    static DeviceController *GetInstance();
    static void FreeInstance();
public:
    DeviceClient *GetDeviceClient(const std::string & devName);
    void ToggleDiscovery();
    int EnumerateDevice(std::vector<std::string> &deviceNameList);
    int GetDevicePropList(const std::string &devName, std::map<std::string, std::string> &propMap);

private:
    static DeviceController *m_instance;
    DeviceController();
    ~DeviceController();
    DeviceController(const DeviceController & /*another*/) {}
    DeviceController & operator =(const DeviceController &) { return *this; }

private:
    CmtiSdk::HDeviceController m_deviceController;
    std::map<CmtiSdk::HDeviceClient, DeviceClient *> m_mapHandle2DeviceClient;
};

class DeviceClient
{
public:
    DeviceClient(CmtiSdk::HDeviceClient hDevClient);
    ~DeviceClient();

public:
    /**
     * @brief 获取设备名
     *
     * @param devName 设备名，调用者负责内存管理
     * @return int 错误码
     */
    //int GetDeviceProperty(const std::string& propName, std::string& propVal) const;

    /**
     * @brief 获取采集帧率FPS（设备端事件上报）
     *
     * @param devCli 设备
     * @param fps 采集帧率FPS
     * @return int 错误码
     */
    int GetCaptureFps(float &fps);

    /**
     * @brief 获取传输帧率FPS（设备端事件上报）
     *
     * @param devCli 设备
     * @param fps 传输帧率FPS
     * @return int 错误码
     */
    int GetTransmitFps(float &fps);

    /**
    * @brief 获取接收帧率FPS（接收端SDK统计）
    *
    * @param hDevCli 设备
    * @param fps 传输帧率FPS
    * @return int 错误码
    */
    int GetReceiveFps(float &fps);

    /**
    * @brief 获取传输误帧率FER（接收端SDK统计）
    *
    * @param hDevCli 设备
    * @param fer 返回传输误帧率FER
    */
    int GetTransmitFer(float &fer);

    /**
    * @brief 搜索I2C地址
    * @param u8I2cAddr 输出参数，搜索到的I2C地址，由调用者分配空间
    * @param nCount 输入输出参数，输入表示分配的内存空间个数；输出表示实际返回的I2c地址个数。
    * @return int 错误码
    */
    //int SearchI2cAddress(uint8_t u8I2cAddr[], int& nCount);

    /**
    * @brief 写单个I2C寄存器
    *
    * @param i2cAddr I2C从地址
    * @param speedkHz I2C速率
    * @param mode I2C模式
    * @param regAddr 寄存器地址
    * @param regData 寄存器值
    * @return int 错误码
    */
    //int WriteSingleI2c(uint32_t i2cAddr, uint32_t speedkHz, uint32_t mode, uint32_t regAddr, uint32_t regData);

    /**
    * @brief 读单个I2C寄存器
    *
    * @param i2cAddr I2C从地址
    * @param speedkHz I2C速率
    * @param mode I2C模式
    * @param regAddr 寄存器地址
    * @param regData 输出参数，寄存器值
    * @return int 错误码
    */
    //int ReadSingleI2c(uint32_t i2cAddr, uint32_t speedkHz, uint32_t mode, uint32_t regAddr, uint32_t& regData);

    /**
     * @brief 批量写I2C寄存器参数（支持转义符处理）
     *
     * @param i2cAddr I2C从地址
     * @param speedkHz I2C速率
     * @param mode I2C模式
     * @param regAddr[] 寄存器地址数组
     * @param regData[] 寄存器值数组
     * @param delay[] 寄存器延时数组，以ms为单位
     * @param regNum 寄存器个数，即前三个数组的各自元素个数
     * @return int 错误码
     */
    int WriteDiscreteI2c(uint i2cAddr, uint speedkHz, uint mode, const std::vector<T_RegConf> &regList);

    /**
     * @brief 批量读I2C寄存器参数（支持转义符处理）
     *
     * @param i2cAddr I2C从地址
     * @param speedkHz I2C速率
     * @param mode I2C模式
     * @param regAddr[] 寄存器地址数组
     * @param regData[] 寄存器值数组
     * @param regNum 寄存器个数，即前两个数组的各自元素个数
     * @return int 错误码
     */
    int ReadDiscreteI2c(uint i2cAddr, uint speedkHz, uint mode, std::vector<T_RegConf> &regList);

    /**
     * @brief 写连续的I2C参数
     *
     * @param i2cAddr I2C从地址
     * @param speedkHz I2C速率
     * @param regAddr 寄存器地址
     * @param regAddrSize 寄存器地址长度
     * @param data 数据
     * @param dataSize 数据长度，按字节计算的长度
     * @return int 错误码
     */
    int WriteContinuousI2c(uint i2cAddr, uint speedkHz, uint regAddr, uint regAddrSize, const uchar *data, uint dataSize);

    /**
     * @brief 读连续的I2C参数
     *
     * @param i2cAddr I2C从地址
     * @param speedkHz I2C速率
     * @param regAddr 寄存器地址
     * @param regAddrSize 寄存器地址长度
     * @param data 数据，调用者分配内存
     * @param dataSize 数据长度
     * @return int 错误码
     */
    int ReadContinuousI2c(uint i2cAddr, uint speedkHz, uint regAddr, uint regAddrSize, uchar *data, uint dataSize);

    /**
     * @brief 写Sensor时钟
     *
     * @param clk100kHz 时钟（100kHz为单位）
     * @return int 错误码
     */
    int SetSensorClock(uint clk100kHz);

    /**
     * @brief 设置Sensor接口
     *
     * @param intf Sensor接口，参考E_InterfaceType定义
     * @return int 错误码
     */
    int SetSensorInterface(uint intf);

    /**
     * @brief 写Mipi参数
     *
     * @param laneNum Mipi Lanes
     * @param freqMHz Mipi时钟，默认800MHz
     * @param virtualChannel Mipi虚拟通道，默认0
     * @return int 错误码
     */
    int SetMipiParam(uint laneNum, uint freqMHz, uint virtualChannel);

    /**
     * @brief 写Sensor GPIO值
     *
     * @param pin 要操作的Pin引脚
     * @param level Pin引脚电平Mask
     * @return int 错误码
     */
    int SetSensorGpioPinLevel(ushort pin, ushort level);

    /**
     * @brief 写Sensor GPIO方向
     *
     * @param pin 要操作的Pin引脚
     * @param dir Pin引脚方向Mask
     * @return int 错误码
     */
    int SetSensorGpioPinDir(ushort pin, ushort dir);

    /**
     * @brief 写Sensor电压
     *
     * @param powerId[] 电源ID，参考E_PowerId定义
     * @param voltage[] 电压值，单位为mV
     * @param delay[] 延时，单位为ms
     * @param count 电压个数
     * @return int 错误码
     */
    int SetSensorPower(const std::vector<T_Power> &powerList);

    /**
     * @brief 读Sensor电压
     *
     * @param powerId[] 电源ID，参考E_PowerId定义
     * @param voltage[] 读到的电压值，单位为mV
     * @param count 电压个数
     * @return int 错误码
     */
    int GetSensorPower(std::vector<T_Power> &powerList);

    /**
     * @brief 读Senso反馈电压
     *
     * @param powerId[] 电源ID，参考E_PowerId定义
     * @param voltage[] 读到的电压值，单位为mV
     * @param count 电压个数
     * @return int 错误码
     */
    int GetSensorFeedBackVoltage(std::vector<T_Power> &powerList);

    /**
     * @brief 设备图像帧参数，在打开视频前需要设置图像帧参数
     *
     * @param imgFmt 图像格式，参考E_ImageFormat定义
     * @param imgMode 图像模式，参考E_ImageMode定义
     * @param width 宽度
     * @param height 高度
     * @param outImgFmt 输出图像格式，参考E_ImageFormat定义（可定制采集的输出格式，如ImgFmt_RAW8, ImgFmt_PackedRaw10...）
     * @return int 错误码
     */
    int SetFrameParam(uint imgFmt, uint imgMode, uint width, uint height, uint outImgFmt);

    /**
     * @brief 获取图像帧参数
     *
     * @param imgFmt 图像格式，参考E_ImageFormat定义
     * @param imgMode 图像模式，参考E_ImageMode定义
     * @param width 宽度
     * @param height 高度
     * @param size 输出图像大小
     * @return int 错误码
     */
    int GetFrameParam(uint &imgFmt, uint &imgMode, uint &width, uint &height, uint &size);

    /**
    * @brief 设置ROI参数，支持热切换
    *
    * @param roiRect ROI矩形框定义，参考T_Rect定义
    * @param roiCount ROI矩形框个数
    * @return int 错误码
    */
    int SetRoiParam(const T_Rect roiRect[], uint roiCount);

    /**
     * @brief 视频控制
     *
     * @param ctrl 控制命令，0为关闭，1为打开
     * @return int 错误码
     */
    int VideoControl(uint ctrl);

    /**
     * @brief 设置采集帧超时
     *
     * @param 超时时间，-1表示一直等待
     * @return int 错误码
     */
    int SetGrabTimeout(uint grabTimeout);

    /**
     * @brief GetSystemTimestamp 获取盒子当前系统时间
     *
     * @param timestamp 时间戳
     * @return int 错误码
     */
    int GetSystemTimestamp(uint64 & timestamp);

    /**
    * @brief 丢弃当前帧后的指定帧数。阻塞操作，函数在丢弃N帧后，有新帧到达时才返回。超时时间按设置的grabTimeout*N计算。
    *
    * @param hDevCli 设备对象
    * @param count 要丢弃的帧数
    * @return int 错误码
    */
    int SkipFrame(int count);
    /**
     * @brief 从缓冲池拷贝数据到应用层
     *
     * @param pbuffer 应用层缓冲区
     * @param bufferLen 应用层缓冲区长度
     * @param timestamp 时间戳
     * @return int 错误码
     */
    int GrabFrame(uchar *pbuffer, int bufferLen, uint64 & timestamp);
    int GrabLatestFrame(uchar *pbuffer, int bufferLen, uint64 & timestamp);

    /**
     * @brief 从队列中取出一个Buffer(与EnqueueFrameBuffer配对使用)，使用缓冲池中的缓冲区，无需要用户分配内存
     *
     * @param bufIdx 缓冲区索引，使用完毕后在EnqueueFrameBuffer中归还
     * @param pbuffer 引用类型，缓冲区
     * @param timestamp 时间戳
     * @return int 错误码
     */
    int DequeueFrameBuffer(int & bufIdx, uchar * & pbuffer, uint64 & timestamp);
    int DequeueFrameBuffer2(int & bufIdx, uchar * & pbuffer, uint64 &headTimestamp, uint64 &tailTimestamp, uint32 &frameSequence);
    int DequeueLatestFrameBuffer(int & bufIdx, uchar * & pbuffer, uint64 & timestamp);

    /**
     * @brief 入队一个Buffer(与DequeueFrameBuffer配对使用)
     *
     * @param bufIdx 缓冲区索引
     * @return int 错误码
     */
    int EnqueueFrameBuffer(int bufIdx);

    /**
     * @brief 设置开短路测试参数
     *
     * @param supplyVol 测试供电电压，单位uV，推荐1.4V
     * @param supplyCurrent 测试供电电流，单位uA，推荐500uA
     * @param pinId[] 待测试的Pin引脚定义，参考E_OSM_PIN_TYPE定义
     * @param openStdVol[] 开路标准，单位uV，可以独立设置，推荐1V
     * @param shortStdVol[] 短路标准，单位uV，可以独立设置，推荐200mV
     * @param count 引脚个数
     * @return int 错误码
     */
    int SetOsTestConfig(uint supplyVol, uint supplyCurrent, const uint pinId[],
                        const uint openStdVol[], const uint shortStdVol[], uint count);

    /**
     * @brief 设置开短路测试电流方向
     *
     * @param bool 电流方向
     * @return int 错误码
     */
    int SetOsTestCurrentDirection(bool positive);

    /**
     * @brief 读开短路测试结果
     *
     * @param pinId[] 测试的Pin引脚定义，参考E_OSM_PIN_TYPE定义
     * @param openVol[] 开路电压，单位uV
     * @param shortVol[] 短路电压，单位uV
     * @param result[] 测试结果
     * @param count 引脚个数
     * @return int 错误码
     */
    int ReadOsTestResult(const uint pinId[], uint openVol[], uint shortVol[], uint result[], uint count);

    /**
     * @brief 获取电流
     *
     * @param powerId[] 电源ID，参考E_PowerId定义
     * @param currentRange[] 电流量程，参考E_CurrentRange定义
     * @param current[] 电流值，单位为nA
     * @param count 电源个数
     * @return int 错误码
     */
    int GetCurrent(const uint powerId[], const uint currentRange[], float current[], uint count);

    /**
     * @brief 获取电流
     *
     * @param powerId[] 电源ID，参考E_PowerId定义
     * @param upperLimit[] 电流限制范围，找寻此范围内的powerId
     * @param autoHighPrecision[] 各类型电流自动匹配高精度标志，0：不匹配，1：向下匹配高精度
     * @param current[] 电流值，单位为nA
     * @param count 电源个数
     * @return int 错误码
     */
    int GetCurrentV2(const int powerIds[], const int upperLimit[], const uint16 atuoHighPrecision[],
                     float current_nA[], int count);

    /**
    * @brief 设置过流参数
    *
    * @param powerId[] 电源ID，参考E_PowerId定义
    * @param currentThrd_mA[] 过流阈值，单位为mA。
    * @param debounceInterval_ms[] 防抖时间，单位为ms。
    * @param count 电源个数
    * @return int 错误码
    */
    int SetOvercurrentParam(const int powerId[], const int currentThrd_mA[], const int debounceInterval_ms[], int count);

    /**
    * @brief 获取过流参数
    *
    * @param powerId[] 电源ID，参考E_PowerId定义
    * @param currentThrd_mA[] 输出参数，过流阈值，单位为mA。
    * @param debounceInterval_ms[] 输出参数，防抖时间，单位为ms
    * @param count 电源个数
    * @return int 错误码
    */
    int GetOvercurrentParam(const int powerId[], int currentThrd_mA[], int debounceInterval_ms[], int count);

    /**
    * @brief 查询过流
    *
    * @param powerId[] 输出参数，返回过流的电源ID，参考E_PowerId定义，调用者分配内存。
    * @param count 输入输出参数，输入表示分配的PowerId数组空间个数；输出表示实际返回的PowerId个数。
    * @return int 错误码
    */
    int QueryOvercurrent(int powerId[], int &count);

    /**
     * @brief 打开蜂鸣器开
     *
     * @param delay 延时时间，单位ms
     * @return int 错误码
     */
    int SetBeepOn(uint delay);

    /**
     * @brief 使能/禁止图像处理插件
     *
     * @param ctrl 使能(1)/禁止(0)
     * @param pluginID 自定义的插件ID，参考《》文档中插件ID定义
     * @return int 错误码
     */
    int ImageProcessingControl(uint ctrl, uint pluginID);

    /**
     * @brief 设置插件参数
     *
     * @param pluginID 自定义的插件ID，参考《》文档中插件ID定义
     * @param param 序列化后的参数缓冲区
     * @param size 序列化后参数的长度，单位为字节
     * @return int 错误码
     */
    int SetImageProcessingParam(uint pluginID, void *param, int size);

    /**
     * @brief 发送升级文件
     */
    int SendUpgradeFile(const char *filePathName);

    int PowerPinOsTest(const int powerId[], const int supplyVoltage_mV[], const int upperLimitCurrent_nA[], const int autoHighPrecision[],
        int leakCurrent_nA[], int count);

    int SetI2cPullupResistor(int sclPullupResistor, int sdaPullupResistor);

    int GetI2cPullupResistor(int &sclPullupResistor, int &sdaPullupResistor);

    int SetMipiClockMode(bool discontinuous);

    int GetMipiClockMode(bool &discontinuous);

    int SetTestPattern(uint16_t mode, uint16_t fps);
    int GetTestPattern(uint16_t &mode, uint16_t &fps);

    int SetTransmitDelay(uint delay);
    int GetTransmitDelay(uint &delay);



private:
    CmtiSdk::HDeviceClient m_hDevClient;
};

#endif // SDKWRAPPER_H
