#-------------------------------------------------
#
# Project created by QtCreator 2020-01-09T20:29:25
#
#-------------------------------------------------

QT       += quick sql
QT +=   remoteobjects
QT += concurrent
QT += serialport

TARGET = utilities
TEMPLATE = lib

DEFINES += UTILITIES_LIBRARY

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

RC_FILE += utilities_resource.rc
TRANSLATIONS = languagePackage/utilities_chinese.ts languagePackage/utilities_english.ts

LIBS += ../ThirdPartyLib/zmq/libzmq-v141-mt-4_3_2.lib
LIBS += -lVersion
INCLUDEPATH += ../ThirdPartyLib/zmq

REPC_SOURCE += configManager/configShare.rep    \
               TaskEngine/instructionExecutor.rep
REPC_REPLICA += configManager/configShare.rep   \
               TaskEngine/instructionExecutor.rep

SOURCES += \
        DBHelper/dbhelper.cpp \
        TaskEngine/instruction.cpp \
        TaskEngine/instructionexecutionclient.cpp \
        TaskEngine/instructionexecutionserver.cpp \
        TaskEngine/objectlivedthreadinstructionexecutor.cpp \
        TaskEngine/taskengineemanager.cpp \
        commonmethod.cpp \
        configManager/configarray.cpp \
        configManager/configbase.cpp \
        configManager/configfile.cpp \
        configManager/configmanager.cpp \
        configManager/configobject.cpp \
        configManager/configobjectarray.cpp \
        configManager/configobjectlanguagemanager.cpp \
        configManager/configshareclient.cpp \
        configManager/configshareserver.cpp \
        csvHelper/csvhelper.cpp \
        duttypemanager.cpp \
        errorHandling/actionerror.cpp \
        errorHandling/kicktrayerror.cpp \
        errorHandling/scrunnable.cpp \
        errorHandling/silicolerror.cpp \
        errorHandling/silicolerrorhandler.cpp \
        extendedqtimer.cpp \
        externexelaucher.cpp \
        fileContent/filecontent.cpp \
        fileversionhelper.cpp \
        heightMap/heightmap.cpp \
        highpricisiontimer.cpp \
        imageprovider.cpp \
        indexvalidator.cpp \
        jsonHelper/jsonhelper.cpp \
        loghelper.cpp \
        loging/LogCollector.cpp \
        loging/LogToFileHandler.cpp \
        loging/Logger.cpp \
        loging/consoleoutputer.cpp \
        loging/logmodel.cpp \
        loging/logtoviewerhandler.cpp \
        loging/rollbackfile.cpp \
        magazineMap/magazinemap.cpp \
        mashineStateIndicator/mashinestateindicator.cpp \
        errorHandling/errorreporter.cpp \
        mymath.cpp \
        processkiller.cpp \
        serialPortWrapper/scserialport.cpp \
        singletonthread.cpp \
        systemCore/alarmhelper.cpp \
        systemCore/dispatcher.cpp \
        systemCore/statemachine.cpp \
        systemCore/worker.cpp \
        systemCore/workerevent.cpp \
        trayMap/trayconfig.cpp \
        trayMap/traymap.cpp \
        uiHelper/msgboxmodel.cpp \
        uiHelper/uioperation.cpp \
        uiHelper/uioperationimpl.cpp \
        uphhelper.cpp \
        DBHelper/mysqltablemodel.cpp \
        userManagement/usermanagement.cpp \
        zmqWrapper/publisher.cpp \
        zmqWrapper/subscriber.cpp \
        pathCanvasEditor/pathsettingtable.cpp \
        pathCanvasEditor/pointstablemodel.cpp

HEADERS += \
        DBHelper/dbhelper.h \
        TaskEngine/commandextractor.h \
        TaskEngine/instruction.h \
        TaskEngine/instructionexecutionclient.h \
        TaskEngine/instructionexecutionresult.h \
        TaskEngine/instructionexecutionserver.h \
        TaskEngine/objectlivedthreadinstructionexecutor.h \
        TaskEngine/taskenginee.h \
        TaskEngine/taskengineemanager.h \
        basicconfig.h \
        commonmethod.h \
        configManager/configarray.h \
        configManager/configbase.h \
        configManager/configfile.h \
        configManager/configmanager.h \
        configManager/configobject.h \
        configManager/configobjectarray.h \
        configManager/configobjectlanguagemanager.h \
        configManager/configshareclient.h \
        configManager/configshareserver.h \
        csvHelper/csvhelper.h \
        deleter.h \
        duttypemanager.h \
        enumhelper.h \
        errorHandling/actionerror.h \
        errorHandling/errorhandlingoption.h \
        errorHandling/kicktrayerror.h \
        errorHandling/scassert.h \
        errorHandling/scrunnable.h \
        errorHandling/silicolerror.h \
        errorHandling/silicolerrorhandler.h \
        errorHandling/stoppederror.h \
        extendedqtimer.h \
        externexelaucher.h \
        fileContent/filecontent.h \
        fileversionhelper.h \
        heightMap/heightmap.h \
        highpricisiontimer.h \
        imageprovider.h \
        indexvalidator.h \
        jsonHelper/jsonhelper.h \
        languageManager/languageconfig.h \
        languageManager/languagemanager.h \
        loghelper.h \
        loging/LogCollector.h \
        loging/LogToFileHandler.h \
        loging/Logger.h \
        loging/LoggerConfig.h \
        loging/consoleoutputer.h \
        loging/loglevel.h \
        loging/logmodel.h \
        loging/logtoviewerhandler.h \
        loging/rollbackfile.h \
        magazineMap/magazineconfig.h \
        magazineMap/magazinemap.h \
        magazineMap/traydata.h \
        mashineStateIndicator/buzzer.h \
        mashineStateIndicator/mashinestateindicator.h \
        mashineStateIndicator/towerlight.h \
        errorHandling/errorreporter.h \
        mymath.h \
        pathCanvasEditor/patheditorconfig.h \
        processkiller.h \
        serialPortWrapper/scserialport.h \
        singletonthread.h \
        systemCore/alarmhelper.h \
        systemCore/dispatcher.h \
        systemCore/priorityqueue.h \
        systemCore/statemachine.h \
        systemCore/statemashinedefinition.h \
        systemCore/worker.h \
        systemCore/workerevent.h \
        trayMap/materialdata.h \
        trayMap/trayconfig.h \
        trayMap/traymap.h \
        uiHelper/btnnamedefinition.h \
        uiHelper/msgboxmodel.h \
        uiHelper/uioperation.h \
        uiHelper/uioperationimpl.h \
        uphhelper.h \
        DBHelper/mysqltablemodel.h \
        userManagement/usermanagement.h \
        utilities_global.h  \
        zmqWrapper/publisher.h \
        zmqWrapper/subscriber.h \
        pathCanvasEditor/pathsettingtable.h \
        pathCanvasEditor/pointstablemodel.h



CONFIG(debug, debug|release){
    DESTDIR = ./Debug
    OBJECTS_DIR = ./Debug/Objs
    MOC_DIR = ./Debug/Mocs
}else{
    DESTDIR = ./Release
    OBJECTS_DIR = ./Release/Objs
    MOC_DIR = ./Release/Mocs
}

unix {
    target.path = /usr/lib
    INSTALLS += target
}

win32-msvc*:QMAKE_CXXFLAGS += /wd"4819"
