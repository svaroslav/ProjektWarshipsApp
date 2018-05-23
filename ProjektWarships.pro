#-------------------------------------------------
#
# Project created by QtCreator 2017-02-01T10:58:07
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ProjektWarships
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    view.cpp \
    ship.cpp \
    fire.cpp

HEADERS  += mainwindow.h \
    view.h \
    ship.h \
    fire.h

FORMS    += mainwindow.ui
