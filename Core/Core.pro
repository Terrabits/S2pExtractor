

QT += core gui widgets
TEMPLATE = lib
CONFIG += staticlib
DESTDIR = $$PWD
TARGET = Core
CONFIG(debug, debug|release): TARGET = $$join(TARGET,,,d)

include($$PWD/../RsaToolbox/rsatoolbox.pri)
HEADERS     += Settings.h \
               CalculateThread.h \
               Calculate.h \
               Channel.h \
               Corrections.h \
    CalibrationSource.h \
    PortLoop.h \
    Error.h \
    CalDialog.h \
    CalWidget.h
               ...
INCLUDEPATH += $$PWD
SOURCES     += CalculateThread.cpp \
               Calculate.cpp \
               Channel.cpp \
               Corrections.cpp \
    CalibrationSource.cpp \
    PortLoop.cpp \
    Error.cpp \
    CalDialog.cpp \
    CalWidget.cpp
FORMS       += \
    CalDialog.ui \
    CalWidget.ui
OTHER_FILES += core.pri

DEFINES     += SOURCE_DIR=\\\"$$PWD/\\\"
CONFIG(debug, debug|release):DEFINES += DEBUG_MODE
CONFIG(release, debug|release):DEFINES += QT_NO_DEBUG_OUTPUT

