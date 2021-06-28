
CONFIG += c++11

TARGET = viewerConsole

TEMPLATE = app

SOURCES += main.cpp \
    viewerconsole.cpp \
    ../recorder.cpp \
    conscolor.cpp \
    datefilter.cpp

HEADERS += \
    viewerconsole.h \
    ../recorder.h \
    conscolor.h \
    datefilter.h
