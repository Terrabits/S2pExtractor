

QT += core gui widgets
TEMPLATE = lib
CONFIG += staticlib
DESTDIR = $$PWD
TARGET = Core
CONFIG(debug, debug|release): TARGET = $$join(TARGET,,,d)

include($$PWD/../RsaToolbox/rsatoolbox.pri)
HEADERS     += Settings.h \
               Calculate.h \
               Channel.h \
               Corrections.h \
    CalibrationSource.h \
    PortLoop.h \
    Error.h \
    CalDialog.h \
    CalWidget.h \
    labeledbutton.h \
    portswidget.h \
    filenamedelegate.h \
    tabthroughtable.h \
    commonports.h \
    fixfilenameextensions.h \
    saveresults.h
               ...
INCLUDEPATH += $$PWD
SOURCES     += \
               Calculate.cpp \
               Channel.cpp \
               Corrections.cpp \
    CalibrationSource.cpp \
    PortLoop.cpp \
    Error.cpp \
    CalDialog.cpp \
    CalWidget.cpp \
    labeledbutton.cpp \
    portswidget.cpp \
    filenamedelegate.cpp \
    tabthroughtable.cpp \
    commonports.cpp \
    fixfilenameextensions.cpp \
    saveresults.cpp
FORMS       += \
    CalDialog.ui \
    labeledbutton.ui \
    portswidget.ui
OTHER_FILES += core.pri

DEFINES     += SOURCE_DIR=\\\"$$PWD/\\\"
CONFIG(debug, debug|release):DEFINES += DEBUG_MODE
CONFIG(release, debug|release):DEFINES += QT_NO_DEBUG_OUTPUT

RESOURCES += \
    coreresources.qrc

