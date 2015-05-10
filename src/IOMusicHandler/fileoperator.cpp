#include "fileoperator.h"

FileOperator::FileOperator(QWidget *parent) :
    QWidget(parent)
{
    fileName_ = "NO.DATA";
    //sndData_ = nullptr;
    movie_ = new QMovie("loading.gif");
    processLabel_ = new QLabel(this);
    processLabel_->setMovie(movie_);
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

    if(fn.isEmpty())
        return false;

    FILE * fp = fopen(fn.toUtf8().constData(), "rb");

    if( fp == NULL )
        return false;

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
/*
    cout << endl << "-----------------------" << endl;
    cout << "fn: " << fn.toUtf8().constData() <<  endl;
    cout << "wFormatTag: " << wFormatTag << endl;
    cout << "-----------------------" << endl;
*/
    if(wFormatTag == WAVE_FORMAT_PCM){
        //cout << "WAVE_FORMAT_PCM" << endl;
        pcm = 0;
    } else {
        fread(&cbSize, sizeof(short), 1, fp);

        if(wFormatTag == WAVE_FORMAT_EXTENSIBLE){
            //cout << "WAVE_FORMAT_EXTENSIBLE" << endl;
            // EXTENSION format
            pcm = 2;
            fread(&wValidBitsPerSample, sizeof(short), 1, fp);
            fread(&dwChannelMask, sizeof(int), 1, fp);
            // fread(&subFormat, sizeof(char), 16, fp);
            fread(subFormat, sizeof(char), 4, fp);
            subFormat[4] = '\0';
        } else {
            // NONPCM
            //cout << "non pcm" << endl;
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

    //cout << "ckDataID: " << ckDataID << endl;
    if(strncmp(ckDataID, "data", 4) == 0)
    {
        success = true;
        //cout << "create object" << endl;
        //sndData_ = new SoundData();
        // buffer init
        //bufferCounter_ = 0;
        //buffer_ = new QByteArray[bufferLength_];
        // if(ckDataSize < 100*1024 byte)
        // buffer = new QByteArray[100*1024];

        ////// sndData init START //////
        sndData->waveFileName(fn.toStdString());
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
        sndData->nBlockAlign(nBlockAlign);
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
        sndData->init(ckDataSize);


        unsigned int num_samp = nSamplesPerSec; // Sampling Rate / Sample Rate

        if(false) //if(wBitsPerSample == 16)
        {
            num_samp = ckDataSize/(nChannels * (wBitsPerSample/8));
            //std::cout << "(16bits) Number of Samples: " << num_samp << endl;




            int counter = 0;
            short int readBit0;
            short int readBit1;
            double conv0;
            double conv1;

            while(counter <= num_samp)
            {
                char dataWrite[20];

                if(nChannels== 2)
                {
                    fread(&readBit0, 1, 2, fp);
                    fread(&readBit1, 1, 2, fp);
                    conv0 = readBit0/32768.0;
                    conv1 = readBit1/32768.0;
                    sndData->audio_data_f_.push_back(readBit0);
                    sndData->audio_data_f_.push_back(readBit1);
                    //cout << readBit0<<" : "<<conv0 << endl;
                    //cout << readBit1<<" : "<< conv1 << endl;
                }

                if(nChannels == 1)
                {
                    fread(&readBit1, 1, 2, fp);
                    conv1 = readBit1/32768.0;
                    sndData->audio_data_f_.push_back(readBit1);
                    //cout << readBit1<<" : "<< conv1 << endl;

                }

                counter++;


            }






        }
        else
        {


            ////// read data from file START //////

            const unsigned int num_ch = nChannels;
            const unsigned int bits_per_samp = wBitsPerSample;
            unsigned char buffer;

            for (unsigned int i = 0; i != num_samp; ++i)
            {
                for (unsigned int j = 0; j != num_ch; ++j)
                {
                    unsigned int tmp = 0;
                    for (unsigned int k = 0; k != bits_per_samp; k+=8)
                    {
                        fread(&buffer, sizeof(char), 1, fp);
                        tmp += buffer << k;
                    }
                    double push_me = conv_bit_size(tmp, bits_per_samp);
                    //std::cout << "push me " << push_me << endl;
                    sndData->audio_data_f_.push_back(push_me);
                }
            }

            ////// read data from file END //////
            fileName_ = fn;
        }
    }

/*
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


        /// convert qbytearray to float for FFTW
        //bool ok;
        //sndData->audio_data_ = QByteArray::number(sndData->audio_data_->toFloat(&ok));
    }*/

    //cout << "pcm: " << pcm << endl;
    if(success && pcm != -1){
        fileName_ = fn;

    }
    else{
        QMessageBox mb;
        mb.setIcon(QMessageBox::Critical);
        mb.setText("Nem sikerült a megnyitás.");
        mb.setInformativeText("Hibás wFormatTag!");
        mb.exec();
    }
    fclose(fp);
    return success;
}


bool FileOperator::openDir(QVector<SoundData*> &dir)
{
    bool success = false;

    dir.clear();
    QString path = QDir::homePath();
    if(fileName_ != ""){
        QFileInfo info(fileName_);
        path = info.absolutePath();
    }

    QString dirname = QFileDialog::getExistingDirectory(this,
                        "Adatbázis mappa megnyitása",
                        path,
                        QFileDialog::ShowDirsOnly
                        | QFileDialog::DontResolveSymlinks);



    QStringList nameFilter("*.wav");

    QStringList all_dirs;
    all_dirs << dirname;
    QDirIterator directories(dirname, QDir::Dirs | QDir::NoSymLinks | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);

    // if just one root dir
    if(!(directories.hasNext()))
    {
        //qDebug() << dirname << endl;
        QDir openDir(dirname);
        QStringList files = openDir.entryList(nameFilter);


        // all files in that directory
        for(auto st : files)
        {
            SoundData * sd = new SoundData();

            QString tmp = dirname + "/" + st;
            if(performLoadOperation(tmp, sd))
            {
                std::string dbTag = dbTagCreate(st, ".");
                qDebug() << "OK!" << st << ", dbTag: " << QString::fromStdString(dbTag) <<endl;
                sd->dbTag(dbTag);
                dir.push_back(sd);
            }
            else
            {
                qDebug() << "PROBLEM!" << tmp << endl;
            }

        }
    }
    else
    {

        // open sub directories
        while(directories.hasNext())
        {
            directories.next();
            all_dirs << directories.filePath();
            //qDebug() << directories.filePath() << endl;
            QDir openDir(directories.filePath());
            QStringList files = openDir.entryList(nameFilter);


            // all files in that directory
            for(auto st : files)
            {
                SoundData * sd = new SoundData();

                QString tmp = directories.filePath() + "/" + st;
                if(performLoadOperation(tmp, sd))
                {
                    std::string dbTag = dbTagCreate(directories.filePath(), "/");
                    qDebug() << "OK!" << st << ", dbTag: " << QString::fromStdString(dbTag) <<endl;
                    sd->dbTag(dbTag);
                    dir.push_back(sd);
                }
                else
                {
                    qDebug() << "PROBLEM!" << tmp << endl;
                }
            }
        }
    }
    cout << endl;
    qDebug() << dir.size() << " piece of wav" << endl;
    if(dir.size() > 0)
    {
        success = true;
    }

    return success;
}

std::string FileOperator::dbTagCreate(QString dirPath, QString split)
{
    //qDebug() << dirPath << endl;
    std::string result;
    QStringList fileParts = dirPath.split(split);
    //if(fileParts.last() == "a")
    if("/" == split)
    {
        result = fileParts.last().toStdString();
    }
    else if("." == split)
    {
        result = fileParts.first().toStdString();
        result = result.at(0);
    }
    return result;
}
