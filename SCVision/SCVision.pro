#-------------------------------------------------
#
# Project created by QtCreator 2020-04-13T20:31:23
#
#-------------------------------------------------

QT       += quick
QT += remoteobjects
QT += concurrent

TARGET = SCVision
TEMPLATE = lib

DEFINES += SCVISION_LIBRARY

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += QT_MESSAGELOGCONTEXT

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

RC_FILE += SCVision_resource.rc
TRANSLATIONS = languagePackage/SCVision_chinese.ts languagePackage/SCVision_english.ts

SOURCES += \
        Calibration/affinetransformcalc.cpp \
        Calibration/calibration.cpp \
        TwoDimCali/twodimensioncalibration.cpp \
        VisionLocation/proffset.cpp \
        VisionLocation/visionlocation.cpp \
        VisionManager/visionconfigmanager.cpp \
        VisionManager/visionmanager.cpp \
        imagesaver.cpp \
        sccamera.cpp \
        visionconfigdir.cpp

HEADERS += \
        Calibration/affinetransformcalc.h \
        Calibration/calibration.h \
        Calibration/calibrationconfig.h \
        TwoDimCali/twodimensioncalibration.h \
        VisionLocation/proffset.h \
        VisionLocation/visionlocation.h \
        VisionLocation/visionlocationconfig.h \
        VisionManager/moveandprconfig.h \
        VisionManager/visionconfigmanager.h \
        VisionManager/visionelementdefinition.h \
        VisionManager/visionmanager.h \
        dummycamera.h \
        imagesaver.h \
        lightsourcecontroller.h \
        sccamera.h \
        sccameraconfig.h \
        scvision.h \
        scvision_global.h \
        visionconfigdir.h \
        visionfactory.h

INCLUDEPATH += ../utilities     \
               ../SCMotion      \
               ../SCMotion/MotionManager        \
               ../UtilitiesLib      \
               ../SCMotionLib      \
               ../ThirdPartyLib/zmq \
               ../ThirdPartyLib/opencv/include

LIBS += ../ThirdPartyLib/opencv/lib/opencv_world450.lib

CONFIG(debug, debug|release){
    DESTDIR = ./Debug
    OBJECTS_DIR = ./Debug/Objs
    MOC_DIR = ./Debug/Mocs
    LIBS += ../SCMotionLib/Debug/SCMotion.lib
    LIBS += ../UtilitiesLib/Debug/utilities.lib
}else{
    DESTDIR = ./Release
    OBJECTS_DIR = ./Release/Objs
    MOC_DIR = ./Release/Mocs
    LIBS += ../SCMotionLib/Release/SCMotion.lib
    LIBS += ../UtilitiesLib/Release/utilities.lib
}

unix {
    target.path = /usr/lib
    INSTALLS += target
}

win32-msvc*:QMAKE_CXXFLAGS += /wd"4819"
