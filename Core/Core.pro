

QT += core gui widgets
TEMPLATE = lib
CONFIG += staticlib
DESTDIR = $$PWD
TARGET = Core
CONFIG(debug, debug|release): TARGET = $$join(TARGET,,,d)

include($$PWD/../RsaToolbox/_install/rsatoolbox.pri)
HEADERS     += Settings.h \
               CalculateThread.h \
               Calibration.h \
               CalibrationWidgetMap.h \
               Data.h \
               getCalibrationDialog.h \
               getFilenamesDialog.h \
               getPortsDialog.h \
               PortsWidgetMap.h \
               Calculate.h \
               Channel.h \
               Corrections.h \
    CalibrationSource.h \
    PortLoop.h
               ...
INCLUDEPATH += $$PWD
SOURCES     += CalculateThread.cpp \
               Calibration.cpp \
               CalibrationWidgetMap.cpp \
               Data.cpp \
               getCalibrationDialog.cpp \
               getFilenamesDialog.cpp \
               getPortsDialog.cpp \
               PortsWidgetMap.cpp \
               Calculate.cpp \
               Channel.cpp \
               Corrections.cpp \
    CalibrationSource.cpp \
    PortLoop.cpp
FORMS       += getCalibrationDialog.ui \
               getFilenamesDialog.ui \
               getPortsDialog.ui
OTHER_FILES += core.pri

DEFINES     += SOURCE_DIR=\\\"$$PWD/\\\"
CONFIG(debug, debug|release):DEFINES += DEBUG_MODE
CONFIG(release, debug|release):DEFINES += QT_NO_DEBUG_OUTPUT

