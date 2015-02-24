 #include "fileoperator.h"

FileOperator::FileOperator(QWidget *parent) :
    QWidget(parent)
{
    fileName_ = "NO.DATA";
    //sndData_ = nullptr;
}

FileOperator::~FileOperator()
{
    //delete sndData_;
    //delete buffer_;
}

void FileOperator::init()
{

}

bool FileOperator::open(SoundData*& sndData)
{
    QString path = QDir::homePath();
    if(fileName_ != ""){
        QFileInfo info(fileName_);
        path = info.absolutePath();
    }
    QString fn = QFileDialog::getOpenFileName(
                this,
                "Fájl megnyitása...",
                path,
                "Hangfájl (*.wav);;Mindenfájl (*.*)"
                );
    bool success;
    if(!fn.isEmpty())
        success = performLoadOperation(fn, sndData);
    return success;
}

bool FileOperator::performLoadOperation(QString fn, SoundData*& sndData)
{
    bool success = false;

    FILE * fp = fopen(fn.toUtf8().constData(), "rb");

    if( fp == NULL )
    {
        return false;
    }

    // Header Init:
    short pcm = -1;
    char ckID[4];                   // "riff"
    int ckSize;                     // 4 + (PCM_OR_NONPCM?(24):(26 + 12)) + (8 + M * Nc * Ns + (0 or 1))
    char waveID[4];                 // "WAVE"

    // format:
    char ckFormatID[4];             // "fmt"
    int ckFormatSize;               // PCM_OR_NONPCM?(16):(18)
        short wFormatTag;           // PCM_OR_NONPCM?(WAVE_FORMAT_PCMformatcode
        short nChannels;            // Nc
        int nSamplesPerSec;         // F
        int nAvgBytesPerSec;        // F * M * Nc
        short nBlockAlign;          // M * Nc
        short wBitsPerSample;       // rounds up to 8 * M
        // if its NONPCM
        short cbSize;               // Size of the extension:0

        // if its EXTENSIBLE format
        short wValidBitsPerSample;
        int dwChannelMask;
        /*** TODO */
        char subFormat[5];
        // end of EXTENSIBLE format

    // if its NONPCM or EXTENSIBLE format
    // fact:
    char ckFactID[5];               // "fact
    int ckFactSize;                 // chunk size: 4
        int dwSampleLength;         // Nc * Ns
    //endif



    // data:
    char ckDataID[4];               // "data"
    int ckDataSize;                     // M * Nc * Ns
    // data                         // Nc * Ns channel-interleaved M-byte samples
    // pad, 0 or 1                  // Padding byte if M * Nc * Ns is odd


    // Header Read:


    fread(ckID, sizeof(char), 4, fp);
    ckID[4] = '\0';
    fread(&ckSize, sizeof(int), 1, fp);

    fread(waveID, sizeof(char), 4, fp);
    waveID[4] = '\0';
    // format:
    fread(ckFormatID, sizeof(char), 4, fp);
    ckFormatID[4] = '\0';
    fread(&ckFormatSize, sizeof(int), 1, fp);

        fread(&wFormatTag, sizeof(short), 1, fp);
        fread(&nChannels, sizeof(short), 1, fp);
        fread(&nSamplesPerSec, sizeof(int), 1, fp);
        fread(&nAvgBytesPerSec, sizeof(int), 1, fp);
        fread(&nBlockAlign, sizeof(short), 1, fp);
        fread(&wBitsPerSample, sizeof(short), 1, fp);

    cout << endl << "-----------------------" << endl;
    cout << "fn: " << fn.toUtf8().constData() <<  endl;
    cout << "wFormatTag: " << wFormatTag << endl;
    cout << "-----------------------" << endl;

    if(wFormatTag == WAVE_FORMAT_PCM){
        cout << "WAVE_FORMAT_PCM" << endl;
        pcm = 0;
    } else {
        fread(&cbSize, sizeof(short), 1, fp);

        if(wFormatTag == WAVE_FORMAT_EXTENSIBLE){
            cout << "WAVE_FORMAT_EXTENSIBLE" << endl;
            // EXTENSION format
            pcm = 2;
            fread(&wValidBitsPerSample, sizeof(short), 1, fp);
            fread(&dwChannelMask, sizeof(int), 1, fp);
            // fread(&subFormat, sizeof(char), 16, fp);
            fread(subFormat, sizeof(char), 4, fp);
            subFormat[4] = '\0';
        } else {
            // NONPCM
            cout << "non pcm" << endl;
            pcm = 1;
        }
        fread(ckFactID, sizeof(char), 4, fp);
        ckFactID[4] = '\0';
        fread(&ckFactSize, sizeof(int), 1, fp);
            fread(&dwSampleLength, sizeof(int), 1, fp);
    }

    // data:
    fread(ckDataID, sizeof(char), 4, fp);
    ckDataID[4] = '\0';
    fread(&ckDataSize, sizeof(int), 1, fp);

    cout << "ckDataID: " << ckDataID << endl;
    if(strncmp(ckDataID, "data", 4) == 0)
    {
        success = true;
        cout << "create object" << endl;
        //sndData_ = new SoundData();
        // buffer init
        //bufferCounter_ = 0;
        //buffer_ = new QByteArray[bufferLength_];
        // if(ckDataSize < 100*1024 byte)
        // buffer = new QByteArray[100*1024];

        ////// sndData init START //////
        sndData->pcm(pcm);
        sndData->ckID(ckID);
        sndData->ckSize(ckSize);
        sndData->waveID(waveID);

        sndData->ckFormatID(ckFormatID);
        sndData->ckFormatSize(ckFormatSize);
        sndData->wFormatTag(wFormatTag);
        sndData->nChannels(nChannels);
        sndData->nSamplesPerSec(nSamplesPerSec);
        sndData->nAvgBytesPerSec(nAvgBytesPerSec);
        sndData->wBitsPerSample(wBitsPerSample);


        if(wFormatTag != WAVE_FORMAT_PCM){
            sndData->cbSize(cbSize);

            if(wFormatTag == WAVE_FORMAT_EXTENSIBLE){
                // EXTENSION format
                sndData->wValidBitsPerSample(wValidBitsPerSample);
                sndData->dwChannelMask(dwChannelMask);
                sndData->subFormat(subFormat);
            }
            sndData->ckFactID(ckFactID);
            sndData->ckFactSize(ckFactSize);
            sndData->dwSampleLength(dwSampleLength);
        }
        sndData->ckDataID(ckDataID);
        sndData->ckDataSize(ckDataSize);
        ////// sndData init END //////

        sndData->info();

    }


    // Qt audio buffering
    QFile audio_file(fn);

    QAudioFormat format;

    // header init from ansi c fread
    //format.setFrequency(1000);
    format.setSampleSize(sndData->ckFormatSize());
    format.setSampleRate(sndData->nSamplesPerSec());
    format.setChannelCount(sndData->nChannels());
    format.setCodec("audio/pcm");
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setSampleType(QAudioFormat::SignedInt);

    /*
    QAudioDeviceInfo info(QAudioDeviceInfo::defaultOutputDevice());
    if (!info.isFormatSupported(format)) {
        qWarning()<<"raw audio format not supported by backend, cannot play audio.";
        return;
    }*/

    QEventLoop loop;

    //Qt data buffering
    if(audio_file.open(QIODevice::ReadOnly))
    {
        audio_file.seek(44); // skip wav header
        // buffer size is the file data size
        while((sndData->audio_data_ = audio_file.read(sndData->ckDataSize()))>0)
        {
        // audio_file.close();

        sndData->audio_buffer_ = new QBuffer(&sndData->audio_data_);

        qDebug() << sndData->audio_buffer_->size();

        sndData->audio_buffer_->open(QIODevice::ReadOnly);
        }

    }

    cout << "pcm: " << pcm << endl;
    if(success && pcm != -1)
        fileName_ = fn;
    else{
        QMessageBox mb;
        mb.setIcon(QMessageBox::Critical);
        mb.setText("Nem sikerült a megnyitás.");
        mb.setInformativeText("Hibás wFormatTag!");
        mb.exec();
    }

    return success;
}


bool FileOperator::loadBuffer()
{
    /*
    QIODevice device()
    QDataStream input()
    QByteArray buffer;
    int length = ...;

    char temp* = 0;
    input.readBytes (temp, length);
    buffer.append (temp, length);
    delete [] temp;

    if(sndData_->ckDataSize() <= bufferCounter_){
        bufferCounter_ += bufferLength_;
        // do some stuff
    }*/
    return false;
}
