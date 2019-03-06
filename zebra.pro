HEADERS += \
    zebra.h \
    win.h \
    alarm.h \
    apptmodel.h \
    inf.h \
    tabledelegate.h \
    messages.h \
    messmodel.h

SOURCES += \
    zebra.cpp \
    main.cpp \
    win.cpp \
    alarm.cpp \
    apptmodel.cpp \
    inf.cpp \
    tabledelegate.cpp \
    messages.cpp \
    messmodel.cpp

FORMS += \
    zebra.ui \
    alarm.ui \
    messages.ui

RESOURCES += \
    systray.qrc

QT += xml svg core gui network widgets multimedia xmlpatterns

RC_FILE = zebra.rc

TRANSLATIONS = zebra_ru_RU.ts

LIBS += -lUser32

CONFIG(release, debug|release):DEFINES += QT_NO_DEBUG_OUTPUT

