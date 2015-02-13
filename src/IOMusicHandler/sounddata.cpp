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
    if(pcm)
    {
        result += "\ncbSize: " + cbSize
        // fact:
                + "\nckFactID: " + ckFactID
                    + "\nckFactSize: " + ckFactSize
                    + "\ndwSampleLength: " + dwSampleLength;
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
        if(pcm)
        {
            cout << "\ncbSize: " << cbSize
            // fact:
                    << "\nckFactID: " << ckFactID
                        << "\nckFactSize: " << ckFactSize
                        << "\ndwSampleLength: " << dwSampleLength;
        }

            // data:
            cout << "\nckDataID: " << ckDataID
                    << "\nckDataSize: " << ckDataSize << endl;

    return result;
}
