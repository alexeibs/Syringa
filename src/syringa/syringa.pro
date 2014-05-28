TEMPLATE = lib
TARGET = syringa
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += c++11

DEFINES += SYRINGA_EXPORTS

build_pass:CONFIG(debug, debug|release): {
    DESTDIR = ../../build/debug
}

build_pass:CONFIG(release, debug|release): {
    DESTDIR = ../../build/release
}

SOURCES += syringa.cpp

HEADERS += \
    syringa.h \
    export.h
