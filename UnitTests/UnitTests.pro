

QT        += core gui widgets testlib
TEMPLATE   = app
TARGET     = UnitTests
CONFIG    += testcase

include(../Core/core.pri)
HEADERS     += CalculateTest.h \
               CorrectionsTest.h
SOURCES     += main.cpp \
               CalculateTest.cpp \
               CorrectionsTest.cpp
#OTHER_FILES += LogName.txt

DEFINES   += SOURCE_DIR=\\\"$$PWD/\\\"

