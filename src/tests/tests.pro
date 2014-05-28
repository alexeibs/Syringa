TEMPLATE = app
TARGET = tests
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += console
CONFIG += c++11

INCLUDEPATH += ../
LIBS += -lsyringa

build_pass:CONFIG(debug, debug|release): {
    LIBS += -L../../build/debug
    DESTDIR = ../../build/debug
}

build_pass:CONFIG(release, debug|release): {
    LIBS += -L../../build/release
    DESTDIR = ../../build/release
}

SOURCES += main.cpp \
    simplereport.cpp

HEADERS += \
    simplereport.h
