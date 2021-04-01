QT += quick     \
      quickcontrols2        \
      remoteobjects     \
      sql       \
      concurrent        \
      serialport    \
      charts    \
      widgets

CONFIG += c++11

QMAKE_CXXFLAGS += /utf-8
VERSION = 1.0.0.0
RC_ICONS = ./icons/Dtp.ico

QT       += network
# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Refer to the documentation for the
# deprecated API to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += QT_MESSAGELOGCONTEXT
DEFINES += QT_FORCE_ASSERTS
DEFINES += WIN32_LEAN_AND_MEAN

QMAKE_CXXFLAGS += /MP

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

TRANSLATIONS = languagePackage/DummyProject_chinese.ts languagePackage/DummyProject_english.ts

SOURCES += \
        ../DriverRepos/Camera/baslercamera.cpp \
        ../DriverRepos/GTMotion/gtaxis.cpp \
        ../DriverRepos/GTMotion/gtcontrolcard.cpp \
        ../DriverRepos/GTMotion/gtdi.cpp \
        ../DriverRepos/GTMotion/gtdo.cpp \
        ../DriverRepos/LightSourceController/wordoplightsourcecontroller.cpp \
        ../WorkerRepos/PressureMonitor/pressuremonitor.cpp \
        Vision/detapervisionfactory.cpp \
        Workers/Discharge/discharge.cpp \
        Workers/Feeding/feeding.cpp \
        Workers/PickArm/pickarm.cpp \
        Workers/ServerAndClient/tcpclient.cpp \
        Workers/ServerAndClient/tcpserver.cpp \
        detaperdispatcher.cpp \
        detapermotionelementfactory.cpp \
        main.cpp

RESOURCES += qml.qrc \
    ../CommonIcons/pics.qrc \
    ../QmlComponents/qmlcomponents.qrc \
    icons.qrc \
    languagepackage.qrc


INCLUDEPATH +=  ../ThirdPartyLib/zmq    \
                ../ThirdPartyLib/opencv/include    \
                ../libs/GtMotion_X64    \
                ../libs/basler/include  \
                ../Libs/AdaptiveVision/include

INCLUDEPATH +=   ../Utilities     \
                 ../SCMotion      \
                 ../SCLibs     \
                 ../SCVision    \
                 ../AdaptVision    \
                 ../DriverRepos

LIBS += ../libs/basler/lib/x64/PylonBase_v5_2.lib    \
        ../libs/GtMotion_X64/gts.lib    \
        ../libs/basler/lib/x64/GCBase_MD_VC141_v3_1_Basler_pylon.lib    \
        ../libs/basler/lib/x64/GenApi_MD_VC141_v3_1_Basler_pylon.lib    \
        ../libs/basler/lib/x64/PylonUtility_v5_2.lib    \
        ../Libs/AdaptiveVision/lib/x64/AVL.lib


DESTDIR = ./Release
OBJECTS_DIR = ./Release/Objs
MOC_DIR = ./Release/Mocs

LIBS += ../SCLibs/SCMotion.lib
LIBS += ../SCLibs/utilities.lib
LIBS += ../SCLibs/SCVision.lib
LIBS+= ../SCLibs/AdaptVision.lib

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    ../DriverRepos/Camera/baslercamera.h \
    ../DriverRepos/GTMotion/gtaxis.h \
    ../DriverRepos/GTMotion/gtaxisconfig.h \
    ../DriverRepos/GTMotion/gtcontrolcard.h \
    ../DriverRepos/GTMotion/gtcontrolcardconfig.h \
    ../DriverRepos/GTMotion/gtdi.h \
    ../DriverRepos/GTMotion/gtdo.h \
    ../DriverRepos/GTMotion/gtioconfig.h \
    ../DriverRepos/GTMotion/gtresulthandler.h \
    ../DriverRepos/LightSourceController/wordoplightsourcecontroller.h \
    ../WorkerRepos/PressureMonitor/pressuremonitor.h \
    Tray/DischargeMap.h \
    Tray/DischargeMapData.h \
    Tray/FeedingMap.h \
    Tray/FeedingMapData.h \
    Tray/PickArmMap.h \
    Tray/PickArmMapData.h \
    Vision/detapervisionfactory.h \
    Workers/Discharge/discharge.h \
    Workers/Discharge/dischargeconfig.h \
    Workers/Feeding/feeding.h \
    Workers/Feeding/feedingconfig.h \
    Workers/PickArm/pickarm.h \
    Workers/PickArm/pickarmconfig.h \
    Workers/ServerAndClient/tcpclient.h \
    Workers/ServerAndClient/tcpserver.h \
    detaperdispatcher.h \
    detapermotionelementfactory.h \
    namedefinition.h

DISTFILES +=
