#ifndef FILEOPERATOR_H
#define FILEOPERATOR_H

#include <QWidget>
#include <QFileDialog>
#include <QDir>
#include <QFileInfo>
#include <QFileDialog>
#include <QDebug>
#include <QTextStream>
#include <QMessageBox>
#include <iostream>
#include "sounddata.h"

#include <QDataStream>
#include <QByteArray>
#include <QFile>
#include <QAudioFormat>
#include <QEventLoop>
#include <QAudioDeviceInfo>
#include <QVector>
//#include <QAudioInput>



//#include "../../genchords/src/Sounddata.h"

using namespace std;

class FileOperator : public QWidget
{
    Q_OBJECT

private:

    //QByteArray * buffer_;
    //short bufferCounter_;
    //short bufferLength_;

    //QAudioInput *audioInput;

    QString fileName_;



public:
    explicit FileOperator(QWidget *parent = 0);
    //void save();
    //void saveAs();

    ~FileOperator();

    bool loadBuffer();
    void init();
    bool open(SoundData*& sndData);
    bool performLoadOperation(QString, SoundData*& sndData);

    QString fileName() { return fileName_; }
    //SoundData * sndData_;

signals:

public slots:
};

#endif // FILEOPERATOR_H
