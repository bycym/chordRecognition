#-------------------------------------------------
#
# Project created by QtCreator 2014-10-18T12:20:23
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = chordRecognition
TEMPLATE = app

INCLUDEPATH += ../lib-src/libsndfile
#LIBS = -L ../lib-src/libsndfile -lsndfile-1

SOURCES += main.cpp\
        mainwindow.cpp \
    fileoperator.cpp \
    sounddata.cpp

HEADERS  += mainwindow.h \
    fileoperator.h \
    sounddata.h

FORMS    += mainwindow.ui

unix: CONFIG += link_pkgconfig
unix: PKGCONFIG += sndfile
