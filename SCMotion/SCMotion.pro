#-------------------------------------------------
#
# Project created by QtCreator 2020-02-26T09:32:24
#
#-------------------------------------------------

QT       += quick
QT += remoteobjects
QT += concurrent

TARGET = SCMotion
TEMPLATE = lib

DEFINES += SCMOTION_LIBRARY

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += QT_MESSAGELOGCONTEXT
DEFINES += WIN32_LEAN_AND_MEAN

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

RC_FILE += SCMotion_resource.rc
TRANSLATIONS = languagePackage/SCMotion_chinese.ts languagePackage/SCMotion_english.ts

SOURCES += \
    AxisModule/automove.cpp \
    AxisModule/axismodule.cpp \
    AxisModule/singleaxis.cpp \
    AxisModule/xymodule.cpp \
    AxisModule/xyzmodule.cpp \
    BasicElement/dummydi.cpp \
    BasicElement/dummydo.cpp \
    BasicElement/globalstate.cpp \
    BasicElement/scaxis.cpp \
    BasicElement/scdi.cpp \
    BasicElement/scdo.cpp \
    CompositeElement/sccylinder.cpp \
    CompositeElement/scdispenser.cpp \
    CompositeElement/scvacuum.cpp \
    CompositeElement/transfer.cpp \
    CompositeElement/twoaxisplanner.cpp \
    MotionManager/distatemonitor.cpp \
    MotionManager/motionconfigmanager.cpp \
    MotionManager/motionmanager.cpp \
    MotionManager/motionstatepublisher.cpp \
    MotionManager/motionstatereporter.cpp \
    MoveProtection/collisioncondition.cpp \
    MoveProtection/collisiongroup.cpp \
    MoveProtection/comparisonoperation.cpp \
    MoveProtection/ignorecondition.cpp \
    MoveProtection/moveprotection.cpp \
    TowerLightBuzzer/scbuzzer.cpp \
    TowerLightBuzzer/sctowerlight.cpp \
    TowerLightBuzzer/singletowerlight.cpp \
    XYZRDebugger/xyzrdebugger.cpp \
    XYZRDebugger/xyzrdebuggermanager.cpp \
    mastermotionmanager.cpp \
    motionutility.cpp \
    scmotion.cpp

HEADERS += \
    AxisModule/automove.h \
    AxisModule/axismodule.h \
    AxisModule/singleaxis.h \
    AxisModule/xymodule.h \
    AxisModule/xyzmodule.h \
    BasicElement/axisconfig.h \
    BasicElement/basicelementfactory.h \
    BasicElement/controlcard.h \
    BasicElement/dummyaxis.h \
    BasicElement/dummycontrolcard.h \
    BasicElement/dummydi.h \
    BasicElement/dummydo.h \
    BasicElement/globalstate.h \
    BasicElement/ioconfig.h \
    BasicElement/scaxis.h \
    BasicElement/scdi.h \
    BasicElement/scdo.h \
    BasicElement/softlandingpos.h \
    CompositeElement/cylinderconfig.h \
    CompositeElement/dispensepath.h \
    CompositeElement/dummydispenser.h \
    CompositeElement/sccylinder.h \
    CompositeElement/scdispenser.h \
    CompositeElement/scvacuum.h \
    CompositeElement/transfer.h \
    CompositeElement/transferconfig.h \
    CompositeElement/twoaxisplanner.h \
    CompositeElement/vacuumconfig.h \
    MotionManager/distatemonitor.h \
    MotionManager/globalmotionelements.h \
    MotionManager/motionconfigmanager.h \
    MotionManager/motionelementcontainer.h \
    MotionManager/motionelementuilayout.h \
    MotionManager/motionmanager.h \
    MotionManager/motionmanagerconfig.h \
    MotionManager/motionstate.h \
    MotionManager/motionstategetter.h \
    MotionManager/motionstatepublisher.h \
    MotionManager/motionstatereporter.h \
    MoveProtection/collisioncondition.h \
    MoveProtection/collisiongroup.h \
    MoveProtection/comparisonoperation.h \
    MoveProtection/ignorecondition.h \
    MoveProtection/moveprotection.h \
    TowerLightBuzzer/buzzerconfig.h \
    TowerLightBuzzer/scbuzzer.h \
    TowerLightBuzzer/sctowerlight.h \
    TowerLightBuzzer/singletowerlight.h \
    XYZRDebugger/xyzrdebugger.h \
    XYZRDebugger/xyzrdebuggerconfig.h \
    XYZRDebugger/xyzrdebuggermanager.h \
    mastermotionmanager.h \
    motionelementdefinition.h \
    motionutility.h \
    scmotion.h \
    scmotion_global.h


REPC_SOURCE += MotionManager/motionManager.rep
REPC_REPLICA += MotionManager/motionManager.rep

INCLUDEPATH += ../Utilities \
                ../UtilitiesLib \
                ../ThirdPartyLib/zmq

CONFIG(debug, debug|release){
    DESTDIR = ./Debug
    OBJECTS_DIR = ./Debug/Objs
    MOC_DIR = ./Debug/Mocs
    LIBS += ../UtilitiesLib/Debug/utilities.lib
    DEPENDPATH += ../UtilitiesLib/Debug
}else{
    DESTDIR = ./Release
    OBJECTS_DIR = ./Release/Objs
    MOC_DIR = ./Release/Mocs
    LIBS += ../UtilitiesLib/Release/utilities.lib
    DEPENDPATH += ../UtilitiesLib/Release
}

unix {
    target.path = /usr/lib
    INSTALLS += target
}

win32-msvc*:QMAKE_CXXFLAGS += /wd"4819"
