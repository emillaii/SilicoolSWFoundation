QT += quick

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Refer to the documentation for the
# deprecated API to know how to port your code away from it.

DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += QT_MESSAGELOGCONTEXT
DEFINES += QT_FORCE_ASSERTS
DEFINES += WIN32_LEAN_AND_MEAN

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += main.cpp

HEADERS += \
    configmanagerunittest.h

RESOURCES += qml.qrc \
    ../../CommonIcons/pics.qrc \
    ../../QmlComponents/qmlcomponents.qrc

INCLUDEPATH +=   ../../Utilities

INCLUDEPATH +=   ../../ThirdPartyLib/zmq
LIBS += ../../ThirdPartyLib/zmq/libzmq-v141-mt-4_3_2.lib



    DESTDIR = ./Release
    OBJECTS_DIR = ./Release/Objs
    MOC_DIR = ./Release/Mocs
    LIBS += ../../UtilitiesLib/Release/utilities.lib



# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


