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
NeuralNetworks
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

    void init();

    /*!
     * \brief open the SoundData sound data what we like to recognize
     * \param sndData sound data, mainWindow create this
     * \return everything ok? rue -> yeah
     */
    bool open(SoundData*& sndData);

    /*!
     * \brief performLoadOperation read the file header and data struct
     * \param s string filename what the open function opened
     * \param sndData the SoundData object what represent the wave
     * \return everything ok? true -> yeah
     */
    bool performLoadOperation(QString s, SoundData*& sndData);
    QString fileName() { return fileName_; }


    inline int conv_bit_size(unsigned int in, int bps)
    {
        const unsigned int max = (1 << (bps-1)) - 1;
        return in > max ? in - (max<<1) : in;
    }

    //SoundData * sndData_;

signals:

public slots:
};

#endif // FILEOPERATOR_H
