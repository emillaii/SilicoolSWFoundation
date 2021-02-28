【zmq介绍】
zmq是一个多对多的消息队列，一个节点可以被多个节点订阅，一个节点也可以订阅多个节点
本模块使用zmq传输Log消息

【功能】
LogViewer
每一个Category的Log消息，拥有单独的Log展示页面
LogViewer会Sub所有配置好的地址中publish的log消息
所有的Log（包含defaultCategory的Log）会显示到default页面
所有的Warn及以上的Log会显示到Warn页面
每一个Category的Log会显示到对应的页面
可以设置每一个Category的LogLevel
小于LogLevel的Log，不会被Format及Publish

LogServer
LogServer会Sub所有配置好的地址中publish的log消息
所有的Log（包含defaultCategory的Log）会保存到SilicoolLog.log
所有的Warn及以上的Log会保存到Warn.log
每一个Category的Log会保存到对应文件
可以配置是否输出Log到控制台

Logger
可以配置是否输出Log到控制台
本地调试时，可以配置Logger输出到控制台

【使用】
每一个需要Publish log消息的应用程序，需要在main函数开始处添加以下代码：
Logger::getIns()->init();

及配置好./config/logConfig/loggerConfig.json中以下两个字段：
"logPublishAddr"， 此地址用于zmq将Log发布到网络中, 默认值为"tcp://127.0.0.1:60010"
"msgSubscribeAddr"， 此地址用于接收LogView设置Log等级的消息，默认值为"tcp://127.0.0.1:60001"

每一个需要publish log消息的应用程序，可以拥有多个category，也建议每一个模块使用不同的category，方便查看Log
----定义及使用category---
#include "Logger.h"
在.h文件的全局区域声明category
SILICOOL_DECLARE_LOGGING_CATEGORY（logCate1)
在.cpp文件的全局区域定义category
SILICOOL_CREATE_LOGGING_CATEGORY(logCate1, "logCate1")
接下来就可以在函数中使用了
qCDebug(logCate1()) << "msg";    
qCInfo(logCate1()) << "msg";   
qCWarning(logCate1()) << "msg";   
qCCritical(logCate1()) << "msg";   
若要在其他文件（不包含声明该category的头文件）中使用此category，需在该文件的全局区域声明：
#include "Logger.h"
SILICOOL_DECLARE_LOGGING_CATEGORY（logCate1,)


LogViewer及LogServer均需配置好./config/logConfig/addressToBeListened.json中，要订阅的Log消息的地址

LogViewer需配置好./config/logConfig/logToViewerConfig.json中，以下字段：
"msgPublishAddr", 用于发布设置Log等级的消息，默认值为"tcp://127.0.0.1:60001"

LogServer需配置好./config/logConfig/logToFileConfig.json中，以下字段：
"flushImmediately"，写文件时仅写入流，或者等待写入磁盘，前者速度快，后者保证已写入文件，默认值为true,
"logDir"，Log保存目录，默认值为 "C:\\SilicoolLog",
"maxFileCount"，每个类别的Log，每日最大的Log文件保存数量，超出数量时删除最早的Log文件，默认值为100
"maxFileSize"，单个Log文件的最大大小，默认为3M


【Log规范】
辅助调试使用qcDebug
运行日志使用qcInfo
报警及错误必须使用qcWarning或qcCritical
若有严重错误导致程序无法执行，可以使用qFatal，qFatal将打印一条消息后调用terminal中止程序


