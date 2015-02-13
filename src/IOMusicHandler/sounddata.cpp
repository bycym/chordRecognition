#include "sounddata.h"

SoundData::SoundData()
{

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
            cout << "\nckDataID: " << ckDataID
                    << "\nckDataSize: " << ckDataSize << endl;

    return result;
}
