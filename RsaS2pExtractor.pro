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


QT      += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = RsaDeembed
TEMPLATE = app

include(RsaToolbox/rsatoolbox.pri)

SOURCES +=  main.cpp \
            mainwindow.cpp \
    getCalibrationDialog.cpp \
    getPortsDialog.cpp \
    getFilenamesDialog.cpp
HEADERS  += Settings.h \
            mainwindow.h \
    getCalibrationDialog.h \
    getPortsDialog.h \
    getFilenamesDialog.h
FORMS    += mainwindow.ui \
    getCalibrationDialog.ui \
    getPortsDialog.ui \
    getFilenamesDialog.ui
RESOURCES += Resources.qrc
OTHER_FILES += VS2010Resources.rc \
               DEBUG_LOGFILE.txt

DEFINES += SOURCE_DIR=\\\"$$PWD\\\"
CONFIG(debug, debug|release):DEFINES += DEBUG_MODE

win32: RC_FILE = VS2010Resources.rc
win32: LIBS += -L$$PWD/
