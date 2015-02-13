 #include "fileoperator.h"

FileOperator::FileOperator(QWidget *parent) :
    QWidget(parent)
{
    fileName = "NO.DATA";
    //sndData = nullptr;
}

FileOperator::~FileOperator()
{
    //delete sndData;
}

void FileOperator::init()
{

}

bool FileOperator::open()
{
    QString path = QDir::homePath();
    if(fileName != ""){
        QFileInfo info(fileName);
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
        success = performLoadOperation(fn);
    return success;
}

bool FileOperator::performLoadOperation(QString fn)
{
    bool success = false;

    FILE * fp = fopen(fn.toUtf8().constData(), "rb");

    // Header Init:

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
        short subFormat[8];
        // end of EXTENSIBLE format

    // if its NONPCM or EXTENSIBLE format
    // fact:
    char ckFactID[4];               // "fact
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
    fread(&ckSize, sizeof(int), 1, fp);

    fread(waveID, sizeof(char), 4, fp);

    // format:
    fread(ckFormatID, sizeof(char), 4, fp);
    fread(&ckFormatSize, sizeof(int), 1, fp);

        fread(&wFormatTag, sizeof(short), 1, fp);
        fread(&nChannels, sizeof(short), 1, fp);
        fread(&nSamplesPerSec, sizeof(int), 1, fp);
        fread(&nAvgBytesPerSec, sizeof(int), 1, fp);
        fread(&nBlockAlign, sizeof(short), 1, fp);
        fread(&wBitsPerSample, sizeof(short), 1, fp);

    //if(wFormatTag != WAVE_FORMAT_PCM)
    //{
        // NONPCM
        fread(&cbSize, sizeof(short), 1, fp);

        // EXTENSION format
        fread(&wValidBitsPerSample, sizeof(short), 1, fp);
        fread(&dwChannelMask, sizeof(int), 1, fp);
        fread(&subFormat, sizeof(short), 8, fp);



        fread(ckFactID, sizeof(char), 4, fp);
        fread(&ckFactSize, sizeof(int), 1, fp);
            fread(&dwSampleLength, sizeof(int), 1, fp);
    //}

    // data:
    fread(ckDataID, sizeof(char), 4, fp);
    fread(&ckDataSize, sizeof(int), 1, fp);


    if(strcmp(ckDataID, "data"))
    {
        cout << "create object" << endl;

    }


    if(success)
        fileName = fn;
    else{
        QMessageBox mb;
        mb.setIcon(QMessageBox::Critical);
        mb.setText("Nem sikerült a megnyitás.");
        mb.setInformativeText("Próbálj valami okosat tenni.");
        mb.exec();
    }

    return success;
}
