TEMPLATE = lib
CONFIG += console static c++17
CONFIG -= app_bundle
CONFIG -= qt

DESTDIR = $$PWD/../build/ui

SOURCES += \
        calcpraye.cpp \
        prayes_calculator.cpp \
#        test.c pp

HEADERS += \
        calcpraye.h \
        cordinates.h \
        noncopyable.hpp \
        prayes_calculator.h \
        types.h
