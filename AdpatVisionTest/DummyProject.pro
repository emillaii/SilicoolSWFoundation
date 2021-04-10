QT += quick     \
      quickcontrols2        \
      remoteobjects     \
      sql       \
      concurrent        \
      serialport    \
      charts

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Refer to the documentation for the
# deprecated API to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += QT_MESSAGELOGCONTEXT
DEFINES += WIN32_LEAN_AND_MEAN

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

TRANSLATIONS = languagePackage/DummyProject_chinese.ts languagePackage/DummyProject_english.ts

SWFoundationDir = ../
LibsDir = ../libs/

HEADERS += \
    $${SWFoundationDir}DriverRepos/LightSourceController/wordoplightsourcecontroller.h \
    $${SWFoundationDir}WorkerRepos/PressureMonitor/pressuremonitor.h \
    $${SWFoundationDir}DriverRepos/Camera/baslercamera.h \
    $${SWFoundationDir}DriverRepos/Camera/dvp_camera.h \
    $${SWFoundationDir}DriverRepos/Camera/hikcamera.h \
    Tray/dutstate.h \
    Tray/duttraymap.h \
    Vision/dummyvisionfactory.h \
    Workers/PickArm/pickarm.h \
    Workers/TrayLoader/trayloader.h \
    Workers/TrayLoader/trayloaderconfig.h \
    dummydispatcher.h \
    dummymotionelementfactory.h \
    namedefinition.h

SOURCES += \
        $${SWFoundationDir}DriverRepos/LightSourceController/wordoplightsourcecontroller.cpp \
        $${SWFoundationDir}WorkerRepos/PressureMonitor/pressuremonitor.cpp \
        $${SWFoundationDir}DriverRepos/Camera/baslercamera.cpp \
        $${SWFoundationDir}DriverRepos/Camera/dvp_camera.cpp \
        $${SWFoundationDir}DriverRepos/Camera/hikcamera.cpp \
        Vision/dummyvisionfactory.cpp \
        Workers/PickArm/pickarm.cpp \
        Workers/TrayLoader/trayloader.cpp \
        dummydispatcher.cpp \
        dummymotionelementfactory.cpp \
        main.cpp

RESOURCES += qml.qrc \
    $${SWFoundationDir}CommonIcons/pics.qrc \
    $${SWFoundationDir}QmlComponents/qmlcomponents.qrc \
    icons.qrc \
    languagepackage.qrc


INCLUDEPATH +=  $${SWFoundationDir}ThirdPartyLib/zmq    \
                $${SWFoundationDir}ThirdPartyLib/opencv/include     \
                $${SWFoundationDir}Utilities     \
                $${SWFoundationDir}SCMotion      \
                $${SWFoundationDir}SCVision    \
                $${SWFoundationDir}SCLibs     \
                $${SWFoundationDir}AdaptVision     \
                $${SWFoundationDir}HikVision     \
                $${SWFoundationDir}DriverRepos

INCLUDEPATH += $${LibsDir}/AdaptiveVision/include
INCLUDEPATH += $${LibsDir}/HikLibs/Includes
LIBS += $${LibsDir}/AdaptiveVision/lib/x64/AVL.lib

INCLUDEPATH += $${LibsDir}basler/include    \
               $${LibsDir}dvp/include    \
               $${LibsDir}hik/Includes

LIBS += $${LibsDir}dvp/lib/x64/DVPCamera64.lib    \
        $${LibsDir}hik/lib/win64/MvCameraControl.lib


LIBS += $${LibsDir}basler/lib/x64/PylonBase_v5_2.lib    \
        $${LibsDir}basler/lib/x64/GCBase_MD_VC141_v3_1_Basler_pylon.lib    \
        $${LibsDir}basler/lib/x64/GenApi_MD_VC141_v3_1_Basler_pylon.lib    \
        $${LibsDir}basler/lib/x64/PylonUtility_v5_2.lib

DESTDIR = ./Release
OBJECTS_DIR = ./Release/Objs
MOC_DIR = ./Release/Mocs

LIBS += $${SWFoundationDir}SCLibs/SCMotion.lib
LIBS += $${SWFoundationDir}SCLibs/utilities.lib
LIBS += $${SWFoundationDir}SCLibs/SCVision.lib
LIBS += $${SWFoundationDir}SCLibs/AdaptVision.lib
LIBS += $${SWFoundationDir}SCLibs/HikVision.lib


# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES +=
