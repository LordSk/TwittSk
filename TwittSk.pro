#-------------------------------------------------
#
# Project created by QtCreator 2015-02-24T08:52:25
#
#-------------------------------------------------

QT       += core gui network webkitwidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TwittSk
TEMPLATE = app


SOURCES += main.cpp\
	mainwindow.cpp \
    netrequestfactory.cpp

HEADERS  += mainwindow.h \
    netrequestfactory.h

FORMS    += mainwindow.ui

CONFIG += c++11
