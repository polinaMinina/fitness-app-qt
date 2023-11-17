QT       += core gui sql widgets
QT += charts
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    calendar.cpp \
    chart.cpp \
    clickablecross.cpp \
    darkpalette.cpp \
    dumbellframe.cpp \
    main.cpp \
    mainwindow.cpp \
    profiler.cpp \
    sqlite.cpp \
    statistics.cpp

HEADERS += \
    calendar.h \
    chart.h \
    clickablecross.h \
    darkpalette.h \
    dumbellframe.h \
    mainwindow.h \
    profiler.h \
    sqlite.h \
    statistics.h

FORMS += \
    mainwindow.ui

TRANSLATIONS += \
    FitnessJournal_ru_RU.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
