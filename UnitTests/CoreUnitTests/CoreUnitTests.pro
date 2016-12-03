

QT        += core gui widgets testlib
TEMPLATE   = app
TARGET     = UnitTests
CONFIG    += testcase

include($$PWD/../../Core/core.pri)
HEADERS     += CalculateTest.h \
               CorrectionsTest.h \
               PortLoopTest.h \
               CalibrationSourceTest.h \
               ChannelTest.h
SOURCES     += main.cpp \
               CalculateTest.cpp \
               CorrectionsTest.cpp \
               PortLoopTest.cpp \
               CalibrationSourceTest.cpp \
               ChannelTest.cpp
#OTHER_FILES += LogName.txt

DEFINES   += SOURCE_DIR=\\\"$$PWD/\\\"
CONFIG(debug, debug|release):DEFINES += DEBUG_MODE
