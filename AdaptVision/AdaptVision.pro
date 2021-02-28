#-------------------------------------------------
#
# Project created by QtCreator 2020-05-08T20:00:12
#
#-------------------------------------------------

QT += quick
QT += remoteobjects

TARGET = AdaptVision
TEMPLATE = lib

DEFINES += ADAPTVISION_LIBRARY

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0



INCLUDEPATH +=  ../UtilitiesLib

INCLUDEPATH +=   ../Utilities     \
                 ../SCVision \
                 ../SCMotion \
                 ../ThirdPartyLib/zmq

LIBS += -L$$PWD/../libs/AdaptiveVision/lib/x64/ -lAVL

INCLUDEPATH += $$PWD/../libs/AdaptiveVision/include
DEPENDPATH += $$PWD/../libs/AdaptiveVision/include

CONFIG(debug, debug|release){
    DESTDIR = ./Debug
    OBJECTS_DIR = ./Debug/Objs
    MOC_DIR = ./Debug/Mocs
    LIBS += ../UtilitiesLib/Debug/utilities.lib
    LIBS += ../SCVisionLib/Debug/SCVision.lib

}else{
    DESTDIR = ./Release
    OBJECTS_DIR = ./Release/Objs
    MOC_DIR = ./Release/Mocs
    LIBS += ../UtilitiesLib/Release/utilities.lib
    LIBS += ../SCVisionLib/Release/SCVision.lib
}

SOURCES += \
        adaptvision.cpp \
        circleedgefitting.cpp \
        edgefitting.cpp \
        genericncctemplatematching.cpp \
        glueinspection.cpp

HEADERS += \
        adaptvision.h \
        adaptvision_global.h  \
        adaptvisionlocationconfig.h \
        adaptvisionprresultimageinfo.h \
        circleedgefitting.h \
        commonutils.h \
        edgefitting.h \
        genericncctemplatematching.h \
        glueinspection.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
