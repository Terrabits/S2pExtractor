QT += core gui widgets

TARGET = PortsWidgetTest
CONFIG -= app_bundle

TEMPLATE = app

include(../../Core/core.pri)
SOURCES += main.cpp \
    setportcount.cpp

FORMS += \
    setportcount.ui

HEADERS += \
    setportcount.h
