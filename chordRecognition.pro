#-------------------------------------------------
#
# Project created by QtCreator 2015-01-01T22:13:34
#
#-------------------------------------------------

QT       += core gui network xml multimedia


#INCLUDEPATH += /usr/include/QtMultimediaKit
#INCLUDEPATH += /usr/include/QtMobility
#INCLUDEPATH += $$PWD/MLPACK/dependencies/armadillo/include

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = chordRecognition
TEMPLATE = app

QMAKE_CXXFLAGS += -std=c++0x

CONFIG += mobility
MOBILITY += multimedia


LIBS+= -lfftw3 \
       -larmadillo \



SOURCES += src/main.cpp\
        src/mainwindow.cpp \
    genchords/src/BeatDetection.cpp \
    genchords/src/Chordsequence.cpp \
    genchords/src/Chordtype.cpp \
    genchords/src/FFT.cpp \
    genchords/src/Key.cpp \
    genchords/src/Labelfile.cpp \
    genchords/src/PCP.cpp \
    genchords/src/PitchName.cpp \
    genchords/src/utilities.cpp \
    src/Utils/neuralutilities.cpp \
    src/IOMusicHandler/fileoperator.cpp \
    src/IOMusicHandler/sounddata.cpp \
    src/MI/getfeatures.cpp \
    src/IOMusicHandler/playsound.cpp \
    src/MI/neuralnetworks.cpp \
    src/Utils/matrixhelpers.cpp \
    src/MI/neuronlayer.cpp \
    src/neuralnetworkform.cpp


HEADERS  += src/mainwindow.h \
    genchords/src/BeatDetection.h \
    genchords/src/Chordsequence.h \
    genchords/src/Chordtype.h \
    genchords/src/FFT.h \
    genchords/src/genchords.h \
    genchords/src/Key.h \
    genchords/src/Labelfile.h \
    genchords/src/PCP.h \
    genchords/src/PitchName.h \
    genchords/src/utilities.h \
    genchords/src/learnchords.h \
    src/Utils/neuralutilities.h \
    src/IOMusicHandler/fileoperator.h \
    src/IOMusicHandler/sounddata.h \
    src/MI/getfeatures.h \
    src/IOMusicHandler/playsound.h \
    src/MI/neuralnetworks.h \
    src/Utils/matrixhelpers.h \
    src/MI/neuronlayer.h \
    src/neuralnetworkform.h

FORMS    += src/mainwindow.ui \
    src/neuralnetworkform.ui

