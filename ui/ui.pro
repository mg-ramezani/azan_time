QT     += core gui widgets multimedia
CONFIG += c++17

linux: LIBS += $$PWD/../build/ui/libprayes_time_calculator.a
win32: LIBS += $$PWD/../build_windows/ui/prayes_time_calculator.lib

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    Widget.cpp \
    widgets/slider_action.cpp

HEADERS += \
    Widget.h \
    calcpraye.h \
    types.h \
    prayes_calculator.h \
    noncopyable.hpp \
    widgets/slider_action.h


FORMS += \
    Widget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resource.qrc
