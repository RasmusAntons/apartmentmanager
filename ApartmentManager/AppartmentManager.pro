#-------------------------------------------------
#
# Project created by QtCreator 2016-03-19T17:02:12
#
#-------------------------------------------------

QT       += core gui
QT       += webkit
QT       += network
QT       += xml
QT       += core gui webkitwidgets #Web Kit not default
QT       += sql
QT       += core

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = AppartmentManager
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    timethread.cpp

HEADERS  += mainwindow.h \
    timethread.h

FORMS    += mainwindow.ui
