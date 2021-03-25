#-------------------------------------------------
#
# Project created by QtCreator 2021-03-03T16:42:30
#
#-------------------------------------------------

QT += remoteobjects
QT += quick
CONFIG += console

TARGET = HikVision
TEMPLATE = lib

DEFINES += HIKVISION_LIBRARY

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += QT_MESSAGELOGCONTEXT
DEFINES += WIN32_LEAN_AND_MEAN


INCLUDEPATH +=  ../UtilitiesLib

INCLUDEPATH +=   ../Utilities     \
                 ../SCVision \
                 ../SCMotion \
                 ../ThirdPartyLib/zmq \
                 ../Libs/HikLibs/Includes \
                 ../Libs/HikLibs/Libraries/Common/MvRender/include

#QMAKE_LFLAGS += /MANIFESTUAC:"level='requireAdministrator' uiAccess='false'"

DESTDIR = ./Release
OBJECTS_DIR = ./Release/Objs
MOC_DIR = ./Release/Mocs
LIBS += ../../SCLibs/utilities.lib
LIBS += ../../SCLibs/SCVision.lib
LIBS += ../../Libs/HikLibs/Libraries/win64/C/iMVS-6000PlatformSDK.lib
LIBS += ../../Libs/HikLibs/Libraries/Common/MvRender/lib/win64/MvRender.lib

SOURCES += \
        hikvision.cpp

HEADERS += \
        hikvision.h \
        hikvision_global.h  \
        hikvisionconfig.h \
        hikvisionlocationconfig.h \
        hikvisionresult.h \
        hikvisionresultimageinfo.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}


