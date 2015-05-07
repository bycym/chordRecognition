#include "sounddata.h"

SoundData::SoundData()
{
    pcm_ = 0;
    ckID_[5] = 0;
    waveID_[5] = 0;

    // format:
    ckFormatID_[5] = 0;
    ckFormatSize_ = 0;
        wFormatTag_ = 0;
        nChannels_ = 0;
        nSamplesPerSec_ = 0;
        nAvgBytesPerSec_ = 0;
        nBlockAlign_ = 0;
        wBitsPerSample_ = 0;

        // NONPCM
        cbSize_ = 0;

        // EXTENSIBLE format
        wValidBitsPerSample_ = 0;
        dwChannelMask_ = 0;
        subFormat_[5] = 0;

    // NONPCM or EXTENSIBLE format
    // fact:
    ckFactID_[5] = 0;
    ckFactSize_ = 0;
        dwSampleLength_ = 0;

    // data:
    ckDataID_[5] = 0;
    ckDataSize_ = 0;
}

SoundData::~SoundData()
{

}

void SoundData::init(int sizeOfData)
{
    audio_data_f_.clear();
    //audio_data = new QByteArray();
    //QBuffer* audio_buffer = new QBuffer(&audio_data);

    //audio_data_f_ = new vector<float>(sizeOfData);
    //audio_data_f_ = new QVector<float>(sizeOfData);
}
QVector<QString> SoundData::infoQt()
{
    QVector<QString> info;
    info.push_back("fileName: " + QString::fromStdString(waveFileName_));
    info.push_back("ckID: " + QString::fromUtf8(ckID_,5));

    info.push_back("ckSize: " + QString::number(ckSize_));
    info.push_back("waveID: " + QString::fromUtf8(waveID_,5));

    // format:
    info.push_back("ckFormatID: " + QString::fromUtf8(ckFormatID_,5));
    info.push_back("ckFormatSize: " + QString::number(ckFormatSize_));
    info.push_back("wFormatTag: " + QString::number(wFormatTag_));
    info.push_back("nChannels: " + QString::number(nChannels_));
    info.push_back("nSamplesPerSec: " + QString::number(nSamplesPerSec_));
    info.push_back("nAvgBytesPerSe: " + QString::number(nAvgBytesPerSec_));
    info.push_back("nBlockAlign: " + QString::number(nBlockAlign_));
    info.push_back("wBitsPerSample: " + QString::number(wBitsPerSample_));
    if(pcm_ != 0) // if its not PCM
    {
        if(pcm_ == 1) // if its NONPCM format
        {
            info.push_back("cbSize: " + QString::number(cbSize_));
        }
        if(pcm_ == 2) // if its EXTENSIBLE format
        {
            info.push_back("wValidBitsPerSample: " + QString::number(wValidBitsPerSample_));
            info.push_back("dwChannelMask: " + QString::number(dwChannelMask_));
            info.push_back("subFormat: " + QString::fromUtf8(subFormat_,5));

        }
        // fact for NONPCM and EXTENSIBLE format too
        // fact:
        info.push_back("ckFactID: " + QString::fromUtf8(ckFactID_,5));
        info.push_back("ckFactSize: " + QString::number(ckFactSize_));
        info.push_back("dwSampleLength: " + QString::number(dwSampleLength_));
    }

    // data:
    info.push_back("ckDataID: " + QString::fromUtf8(ckDataID_,5));
    info.push_back("ckDataSize: " + QString::number(ckDataSize_));

    info.push_back("Samples: " + QString::number(audio_data_f_.size()));
    info.push_back("Frames: " + QString::number(frames()));
    info.push_back("Bit rate: " + QString::number(wBitsPerSample_));

    return info;
}

void SoundData::info()
{
    //string result;
    /*
    result = "ckID: " + ckID
            + "\nckSize: " + ckSize
            + "\nwaveID: " + waveID

            // format:
            + "\nckFormatID: " + ckFormatID
            + "\nckFormatSize: " + ckFormatSize
                + "\nwFormatTag: " + wFormatTag
                + "\nnChannels: " + nChannels
                + "\nnSamplesPerSec: " + nSamplesPerSec
                + "\nnAvgBytesPerSe: " + nAvgBytesPerSec
                + "\nnBlockAlign: " + nBlockAlign
                + "\nwBitsPerSample: " + wBitsPerSample;
    if(pcm != 0) // if its not PCM
    {
        if(pcm == 1) // if its NONPCM format
        {
            cout << "\ncbSize: " << cbSize
        }
        if(pcm == 2) // if its EXTENSIBLE format
        {
            cout << "\nwValidBitsPerSample: " << wValidBitsPerSample
                 << "\ndwChannelMask: " << dwChannelMask
                 << "\nsubFormat: " << subFormat;
        }
            // fact for NONPCM and EXTENSIBLE format too
            // fact:
            cout << "\nckFactID: " << ckFactID
                 << "\nckFactSize: " << ckFactSize
                 << "\ndwSampleLength: " << dwSampleLength;
        }

        // data:
        result += "\nckDataID: " + ckDataID
                + "\nckDataSize: " + ckDataSize;
    */
    //printf("%s", ckID_);
    cout << "filename: " << waveFileName_ << endl;
    cout << "ckID: " << ckID_
                << "\nckSize: " << ckSize_
                << "\nwaveID: " << waveID_

                // format:
                << "\nckFormatID: " << ckFormatID_
                << "\nckFormatSize: " << ckFormatSize_
                    << "\nwFormatTag: " << wFormatTag_
                    << "\nnChannels: " << nChannels_
                    << "\nnSamplesPerSec: " << nSamplesPerSec_
                    << "\nnAvgBytesPerSe: " << nAvgBytesPerSec_
                    << "\nnBlockAlign: " << nBlockAlign_
                    << "\nwBitsPerSample: " << wBitsPerSample_;
        if(pcm_ != 0) // if its not PCM
        {
            if(pcm_ == 1) // if its NONPCM format
            {
                cout << "\ncbSize: " << cbSize_;
            }
            if(pcm_ == 2) // if its EXTENSIBLE format
            {
                cout << "\nwValidBitsPerSample: " << wValidBitsPerSample_
                     << "\ndwChannelMask: " << dwChannelMask_
                     << "\nsubFormat: " << subFormat_;

            }
            // fact for NONPCM and EXTENSIBLE format too
            // fact:
                cout << "\nckFactID: " << ckFactID_
                        << "\nckFactSize: " << ckFactSize_
                        << "\ndwSampleLength: " << dwSampleLength_;
        }

            // data:
            cout << "\nckDataID: " << ckDataID_
                    << "\nckDataSize: " << ckDataSize_ << endl;

    //return result;
}


