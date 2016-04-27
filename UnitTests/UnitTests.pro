

QT        += core gui widgets testlib
TEMPLATE   = app
TARGET     = UnitTests
CONFIG    += testcase

include(../Core/core.pri)
HEADERS     += CalculateTest.h \
               CorrectionsTest.h \
    PortLoopTest.h \
    CalibrationSourceTest.h
SOURCES     += main.cpp \
               CalculateTest.cpp \
               CorrectionsTest.cpp \
    PortLoopTest.cpp \
    CalibrationSourceTest.cpp
#OTHER_FILES += LogName.txt

DEFINES   += SOURCE_DIR=\\\"$$PWD/\\\"

