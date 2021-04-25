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

SWFoundationDir = ../
LibsDir = ../libs/

RC_ICONS = M.ico

HEADERS += \
    $${SWFoundationDir}DriverRepos/GTMotion/gtaxis.h \
    $${SWFoundationDir}DriverRepos/GTMotion/gtaxisconfig.h \
    $${SWFoundationDir}DriverRepos/GTMotion/gtcontrolcard.h \
    $${SWFoundationDir}DriverRepos/GTMotion/gtcontrolcardconfig.h \
    $${SWFoundationDir}DriverRepos/GTMotion/gtdi.h \
    $${SWFoundationDir}DriverRepos/GTMotion/gtdispenser.h \
    $${SWFoundationDir}DriverRepos/GTMotion/gtdo.h \
    $${SWFoundationDir}DriverRepos/GTMotion/gtioconfig.h \
    $${SWFoundationDir}DriverRepos/GTMotion/gtresulthandler.h \
    dummydispatcher.h \
    dummymotionelementfactory.h

SOURCES += \
        $${SWFoundationDir}DriverRepos/GTMotion/gtaxis.cpp \
        $${SWFoundationDir}DriverRepos/GTMotion/gtcontrolcard.cpp \
        $${SWFoundationDir}DriverRepos/GTMotion/gtdi.cpp \
        $${SWFoundationDir}DriverRepos/GTMotion/gtdispenser.cpp \
        $${SWFoundationDir}DriverRepos/GTMotion/gtdo.cpp \
        dummydispatcher.cpp \
        dummymotionelementfactory.cpp \
        main.cpp

RESOURCES += qml.qrc \
    $${SWFoundationDir}CommonIcons/pics.qrc \
    $${SWFoundationDir}QmlComponents/qmlcomponents.qrc \
    icons.qrc \
    languagepackage.qrc


INCLUDEPATH +=  $${SWFoundationDir}ThirdPartyLib/zmq    \
                $${SWFoundationDir}Utilities     \
                $${SWFoundationDir}SCMotion      \
                $${SWFoundationDir}SCLibs

INCLUDEPATH += $${LibsDir}GtMotion_X64
LIBS += $${LibsDir}GtMotion_X64/gts.lib

DESTDIR = ./Release
OBJECTS_DIR = ./Release/Objs
MOC_DIR = ./Release/Mocs
LIBS += $${SWFoundationDir}SCLibs/SCMotion.lib
LIBS += $${SWFoundationDir}SCLibs/utilities.lib


# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES +=
