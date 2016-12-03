

QT        += core gui widgets testlib
TEMPLATE   = app
TARGET     = CoreDialogUiTest


include($$PWD/../../Core/core.pri)
# HEADERS     +=
SOURCES     += main.cpp

DEFINES   += SOURCE_DIR=\\\"$$PWD/\\\"
