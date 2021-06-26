QT       += core gui charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets


CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Citac.cpp \
    Cvor.cpp \
    Okvir.cpp \
    Procesiranje.cpp \
    ProzorAnaliza.cpp \
    ProzorGlavni.cpp \
    WiFiSucelja.cpp \
    main.cpp

HEADERS += \
    Citac.h \
    Cvor.h \
    Okvir.h \
    Paketi.h \
    Procesiranje.h \
    ProzorAnaliza.h \
    ProzorGlavni.h \
    WiFiSucelja.h

FORMS += \
    ProzorAnaliza.ui \
    ProzorGlavni.ui

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
