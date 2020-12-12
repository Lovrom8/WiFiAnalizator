QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    GlavniProzor.cpp \
    WiFiAnaliza.cpp \
    WifiSucelja.cpp \
    main.cpp

HEADERS += \
    GlavniProzor.hpp \
    WiFiAnaliza.h \
    WifiSucelja.hpp \
    list.h \
    nl80211.h

FORMS += \
    GlavniProzor.ui \
    WiFiAnaliza.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../lib/x86_64-linux-gnu/release/ -lnl-3
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../lib/x86_64-linux-gnu/debug/ -lnl-3
else:unix: LIBS += -L$$PWD/../../../lib/x86_64-linux-gnu/ -lnl-3

INCLUDEPATH += $$PWD/../../../usr/include/libnl3
DEPENDPATH += $$PWD/../../../usr/include/libnl3


win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../lib/x86_64-linux-gnu/release/ -lnl-genl-3
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../lib/x86_64-linux-gnu/debug/ -lnl-genl-3
else:unix: LIBS += -L$$PWD/../../../lib/x86_64-linux-gnu/ -lnl-genl-3
