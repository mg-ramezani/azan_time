TEMPLATE = lib
CONFIG += console static c++17
CONFIG -= app_bundle
CONFIG -= qt

linux: DESTDIR = $$PWD/../build/ui

win32{
    DESTDIR = $$PWD/../build_windows/ui
    DEFINES += _USE_MATH_DEFINES
    DEFINES += NOMINMAX
}

QMAKE_POST_LINK += $$QMAKE_COPY $$quote($$PWD/calcpraye.h) $$quote($$PWD/../ui) $$escape_expand(\\n\\t)
QMAKE_POST_LINK += $$QMAKE_COPY $$quote($$PWD/types.h) $$quote($$PWD/../ui) $$escape_expand(\\n\\t)
QMAKE_POST_LINK += $$QMAKE_COPY $$quote($$PWD/prayes_calculator.h) $$quote($$PWD/../ui) $$escape_expand(\\n\\t)
QMAKE_POST_LINK += $$QMAKE_COPY $$quote($$PWD/noncopyable.hpp) $$quote($$PWD/../ui) $$escape_expand(\\n\\t)
QMAKE_POST_LINK += $$QMAKE_COPY $$quote($$PWD/cordinates.h) $$quote($$PWD/../ui) $$escape_expand(\\n\\t)

SOURCES += \
        calcpraye.cpp \
        prayes_calculator.cpp \
#        test.cpp

HEADERS += \
        calcpraye.h \
        cordinates.h \
        noncopyable.hpp \
        prayes_calculator.h \
        types.h
