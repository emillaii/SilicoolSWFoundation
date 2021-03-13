TEMPLATE = subdirs

SUBDIRS += \
    HikVision
SUBDIRS += \
    TestForHikVision
# 加上ordered配置就可以按顺序编译SUBDIRS
CONFIG += ordered

#QMAKE_LFLAGS += /MANIFESTAUAC:"level='requireAdministrator' uiAccess='false'"




