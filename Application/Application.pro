#-------------------------------------------------
# 
# Project  : R&S S2P Extractor
# App name : S2P Extractor
# 
# 
# RsaToolbox
# Template : Rsa Vna Macro
#            Version 3.?
# 
# (C) Rohde & Schwarz America
# 
#-------------------------------------------------


QT      += core gui widgets #printsupport

TARGET = S2PExtractor
TEMPLATE = app

include(../Core/core.pri)
HEADERS     += mainwindow.h
INCLUDEPATH += $$PWD
SOURCES     += main.cpp \
               mainwindow.cpp
FORMS       += mainwindow.ui
RESOURCES   += Resources.qrc
OTHER_FILES += VS2010Resources.rc \
               DEBUG_LOGFILE.txt
win32:      RC_FILE = VS2010Resources.rc

DEFINES += SOURCE_DIR=\\\"$$PWD\\\"
CONFIG(debug, debug|release):DEFINES += DEBUG_MODE
