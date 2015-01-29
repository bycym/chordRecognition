#-------------------------------------------------
#
# Project created by QtCreator 2015-01-01T22:13:34
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = chordRecognition
TEMPLATE = app

QMAKE_CXXFLAGS += -std=c++0x


LIBS+= -lsndfile


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
    genchords/src/Sounddata.cpp \
    genchords/src/utilities.cpp \
    genchords/lib-src/libsndfile/src/G72x/g721.c \
    genchords/lib-src/libsndfile/src/G72x/g723_16.c \
    genchords/lib-src/libsndfile/src/G72x/g723_24.c \
    genchords/lib-src/libsndfile/src/G72x/g723_40.c \
    genchords/lib-src/libsndfile/src/G72x/g72x.c \
    genchords/lib-src/libsndfile/src/GSM610/add.c \
    genchords/lib-src/libsndfile/src/GSM610/code.c \
    genchords/lib-src/libsndfile/src/GSM610/decode.c \
    genchords/lib-src/libsndfile/src/GSM610/gsm_create.c \
    genchords/lib-src/libsndfile/src/GSM610/gsm_decode.c \
    genchords/lib-src/libsndfile/src/GSM610/gsm_destroy.c \
    genchords/lib-src/libsndfile/src/GSM610/gsm_encode.c \
    genchords/lib-src/libsndfile/src/GSM610/gsm_option.c \
    genchords/lib-src/libsndfile/src/GSM610/long_term.c \
    genchords/lib-src/libsndfile/src/GSM610/lpc.c \
    genchords/lib-src/libsndfile/src/GSM610/preprocess.c \
    genchords/lib-src/libsndfile/src/GSM610/rpe.c \
    genchords/lib-src/libsndfile/src/GSM610/short_term.c \
    genchords/lib-src/libsndfile/src/GSM610/table.c \
    genchords/lib-src/libsndfile/src/aiff.c \
    genchords/lib-src/libsndfile/src/alaw.c \
    genchords/lib-src/libsndfile/src/au.c \
    genchords/lib-src/libsndfile/src/au_g72x.c \
    genchords/lib-src/libsndfile/src/avr.c \
    genchords/lib-src/libsndfile/src/command.c \
    genchords/lib-src/libsndfile/src/common.c \
    genchords/lib-src/libsndfile/src/dither.c \
    genchords/lib-src/libsndfile/src/double64.c \
    genchords/lib-src/libsndfile/src/dwd.c \
    genchords/lib-src/libsndfile/src/dwvw.c \
    genchords/lib-src/libsndfile/src/file_io.c \
    genchords/lib-src/libsndfile/src/float32.c \
    genchords/lib-src/libsndfile/src/gsm610.c \
    genchords/lib-src/libsndfile/src/htk.c \
    genchords/lib-src/libsndfile/src/ima_adpcm.c \
    genchords/lib-src/libsndfile/src/interleave.c \
    genchords/lib-src/libsndfile/src/ircam.c \
    genchords/lib-src/libsndfile/src/macbinary3.c \
    genchords/lib-src/libsndfile/src/macos.c \
    genchords/lib-src/libsndfile/src/mat4.c \
    genchords/lib-src/libsndfile/src/mat5.c \
    genchords/lib-src/libsndfile/src/ms_adpcm.c \
    genchords/lib-src/libsndfile/src/nist.c \
    genchords/lib-src/libsndfile/src/ogg.c \
    genchords/lib-src/libsndfile/src/paf.c \
    genchords/lib-src/libsndfile/src/pcm.c \
    genchords/lib-src/libsndfile/src/pvf.c \
    genchords/lib-src/libsndfile/src/raw.c \
    genchords/lib-src/libsndfile/src/rx2.c \
    genchords/lib-src/libsndfile/src/sd2.c \
    genchords/lib-src/libsndfile/src/sds.c \
    genchords/lib-src/libsndfile/src/sfendian.c \
    genchords/lib-src/libsndfile/src/sndfile.c \
    genchords/lib-src/libsndfile/src/strings.c \
    genchords/lib-src/libsndfile/src/svx.c \
    genchords/lib-src/libsndfile/src/txw.c \
    genchords/lib-src/libsndfile/src/ulaw.c \
    genchords/lib-src/libsndfile/src/voc.c \
    genchords/lib-src/libsndfile/src/vox_adpcm.c \
    genchords/lib-src/libsndfile/src/w64.c \
    genchords/lib-src/libsndfile/src/wav.c \
    genchords/lib-src/libsndfile/src/wav_w64.c \
    genchords/lib-src/libsndfile/src/wve.c \
    genchords/lib-src/libsndfile/src/xi.c \
    src/Utils/neuralutilities.cpp \
    src/IOMusicHandler/fileoperator.cpp


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
    genchords/src/Sounddata.h \
    genchords/src/utilities.h \
    genchords/lib-src/libsndfile/src/G72x/g72x.h \
    genchords/lib-src/libsndfile/src/G72x/g72x_priv.h \
    genchords/lib-src/libsndfile/src/GSM610/config.h \
    genchords/lib-src/libsndfile/src/GSM610/gsm.h \
    genchords/lib-src/libsndfile/src/GSM610/gsm610_priv.h \
    genchords/lib-src/libsndfile/src/au.h \
    genchords/lib-src/libsndfile/src/common.h \
    genchords/lib-src/libsndfile/src/config.h \
    genchords/lib-src/libsndfile/src/float_cast.h \
    genchords/lib-src/libsndfile/src/sf_unistd.h \
    genchords/lib-src/libsndfile/src/sfendian.h \
    genchords/lib-src/libsndfile/src/sndfile.h \
    genchords/lib-src/libsndfile/src/wav_w64.h \
    genchords/src/learnchords.h \
    src/Utils/neuralutilities.h \
    src/IOMusicHandler/fileoperator.h

FORMS    += src/mainwindow.ui
