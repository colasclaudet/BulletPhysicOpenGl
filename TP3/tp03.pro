#-------------------------------------------------
#
# Project created by QtCreator 2019-01-12T12:27:11
#
#-------------------------------------------------

QT       += core gui

CONFIG += c++14

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = tp01
TEMPLATE = app


SOURCES += main.cpp\
        boxbullet.cpp \
        objectbullet.cpp \
        princ.cpp \
        glarea.cpp \
        spherebullet.cpp

HEADERS  += princ.h \
        boxbullet.h \
        glarea.h \
        objectbullet.h \
        spherebullet.h

FORMS    += princ.ui

RESOURCES += \
    tp01.qrc

unix: CONFIG += link_pkgconfig
unix: PKGCONFIG += bullet
