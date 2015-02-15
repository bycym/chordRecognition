#include "sounddata.h"

SoundData::SoundData()
{
    pcm = 0;
    ckID[5] = 0;
    waveID[5] = 0;

    // format:
    ckFormatID[5] = 0;
    ckFormatSize = 0;
        wFormatTag = 0;
        nChannels = 0;
        nSamplesPerSec = 0;
        nAvgBytesPerSec = 0;
        nBlockAlign = 0;
        wBitsPerSample = 0;

        // NONPCM
        cbSize = 0;

        // EXTENSIBLE format
        wValidBitsPerSample = 0;
        dwChannelMask = 0;
        subFormat[5] = 0;

    // NONPCM or EXTENSIBLE format
    // fact:
    ckFactID[5] = 0;
    ckFactSize = 0;
        dwSampleLength = 0;

    // data:
    ckDataID[5] = 0;
    ckDataSize = 0;
}

SoundData::~SoundData()
{

}

string SoundData::info()
{
    string result;
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

    cout << "ckID: " << ckID
                << "\nckSize: " << ckSize
                << "\nwaveID: " << waveID

                // format:
                << "\nckFormatID: " << ckFormatID
                << "\nckFormatSize: " << ckFormatSize
                    << "\nwFormatTag: " << wFormatTag
                    << "\nnChannels: " << nChannels
                    << "\nnSamplesPerSec: " << nSamplesPerSec
                    << "\nnAvgBytesPerSe: " << nAvgBytesPerSec
                    << "\nnBlockAlign: " << nBlockAlign
                    << "\nwBitsPerSample: " << wBitsPerSample;
        if(pcm != 0) // if its not PCM
        {
            if(pcm == 1) // if its NONPCM format
            {
                cout << "\ncbSize: " << cbSize;
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
            cout << "\nckDataID: " << ckDataID
                    << "\nckDataSize: " << ckDataSize << endl;

    return result;
}
