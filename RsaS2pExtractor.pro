#-------------------------------------------------
# 
# Project  : RsaDeembed
# App name : RSA Deembed
# 
# 
# RsaToolbox
# Template : Rsa Vna Macro
#            Version 2.0.0
# 
# (C) Rohde & Schwarz America
# 
#-------------------------------------------------


QT       += core gui widgets
TARGET   =  RsaDeembed
TEMPLATE =  app

include(RsaToolbox/rsatoolbox.pri)
SOURCES    +=  main.cpp \
               mainwindow.cpp \
               getCalibrationDialog.cpp \
               getPortsDialog.cpp \
               getFilenamesDialog.cpp \
               Calibration.cpp \
               Data.cpp \
               CalibrationWidgetMap.cpp
HEADERS     += Settings.h \
               mainwindow.h \
               getCalibrationDialog.h \
               getPortsDialog.h \
               getFilenamesDialog.h \
               Calibration.h \
               Data.h \
               CalibrationWidgetMap.h
FORMS       += mainwindow.ui \
               getCalibrationDialog.ui \
               getPortsDialog.ui \
               getFilenamesDialog.ui
RESOURCES   += Resources.qrc
OTHER_FILES += VS2010Resources.rc \
               DEBUG_LOGFILE.txt

DEFINES += SOURCE_DIR=\\\"$$PWD\\\"
CONFIG(debug, debug|release):DEFINES += DEBUG_MODE

win32: RC_FILE = VS2010Resources.rc
win32: LIBS += -L$$PWD/
