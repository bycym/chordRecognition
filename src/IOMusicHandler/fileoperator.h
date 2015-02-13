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
//#include <QAudioInput>



//#include "../../genchords/src/Sounddata.h"

using namespace std;

class FileOperator : public QWidget
{
    Q_OBJECT

private:
    //Sounddata * sndData;
    QByteArray *buffer;
    //QAudioInput *audioInput;

    QString fileName;



public:
    explicit FileOperator(QWidget *parent = 0);
    //void save();
    //void saveAs();

    ~FileOperator();

    void init();
    bool open();
    bool performLoadOperation(QString);

signals:

public slots:
};

#endif // FILEOPERATOR_H
