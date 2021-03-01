QT += testlib
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

SOURCES +=  tst_csvhelpertest.cpp

LIBS += ../../UtilitiesLib/Release/utilities.lib

INCLUDEPATH += ../../Utilities

HEADERS += \
    testclass.h
