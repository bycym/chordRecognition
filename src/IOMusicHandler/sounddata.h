#ifndef SOUNDDATA_H
#define SOUNDDATA_H

#include <iostream>

using namespace std;

#define WAVE_FORMAT_PCM 1
#define WAVE_FORMAT_EXTENSIBLE -2
/*

PCM_OR_NONPCM true => PCM


//// PCM DATA
char ckID[4];                   // "riff"
int ckSize;                     // 4 + 24 + (8 + M * Nc * Ns + (0 or 1))
char waveID[4];                 // "WAVE"

// format:
char ckFormatID[4];             // "fmt"
int ckFormatSize;               // 16
    short wFormatTag;           // WAVE_FORMAT_PCM
    short nChannels;            // Nc
    int nSamplesPerSec;         // F
    int nAvgBytesPerSec;        // F * M * Nc
    short nBlockAlign;          // M * Nc
    short wBitsPerSample;       // rounds up to 8 * M

// data:
char ckDataID[4];               // "data"
int ckSize;                     // M * Nc * Ns
// data                         // Nc * Ns channel-interleaved M-byte samples
// pad, 0 or 1                  // Padding byte if M * Nc * Ns is odd


//// Non PCM DATA
char ckID[4];                   // "riff"
int ckSize;                     // 4 + 26 + 12 + (8 + M * Nc * Ns + (0 or 1))
char waveID[4];                 // "WAVE"

// format:
char ckFormatID[4];             // "fmt"
int ckFormatSize;               // 18
    short wFormatTag;           // WAVE_FORMAT_EXTENSIBLE
    short nChannels;            // Nc
    int nSamplesPerSec;         // F
    int nAvgBytesPerSec;        // F * M * Nc
    short nBlockAlign;          // M * Nc
    short wBitsPerSample;       // 8 * M (float data) or 16 (log-PCM data)
    short cbSize;               // Size of the extension: 0

// fact:
char ckFactID[4];               // "fact
int ckFactSize;                 // chunk size: 4
    int dwSampleLength;         // Nc * Ns


// data:
char ckDataID[4];               // "data"
int ckSize;                     // M * Nc * Ns
// data                         // Nc * Ns channel-interleaved M-byte samples
// pad, 0 or 1                  // Padding byte if M * Nc * Ns is odd


/// Extensible Format
char ckID[4];                   // "riff"
int ckSize;                     // 4 + 48 + 12 + (8 + M * Nc * Ns + (0 or 1))
char waveID[4];                 // "WAVE"

// format:
char ckFormatID[4];             // "fmt"
int ckFormatSize;               // 40
    short wFormatTag;           // WAVE_FORMAT_EXTENSIBLE
    short nChannels;            // Nc
    int nSamplesPerSec;         // F
    int nAvgBytesPerSec;        // F * M * Nc
    short nBlockAlign;          // M * Nc
    short wBitsPerSample;       // 8 * M
    short cbSize;               // Size of the extension: 22

    // more stuff:
    short wValidBitsPerSample   // at most 8 * M
    int dwChannelMask           // Speaker position mask: 0
    <type> subFormat               // GUID (first two bytes are the data format code)
    <type> char subFormat[17]

// fact:
char ckFactID[4];               // "fact"
int ckFactSize;                 // chunk size: 4
    int dwSampleLength;         // Nc * Ns


// data:
char ckDataID[4];               // "data"
int ckSize;                     // M * Nc * Ns
// data                         // Nc * Ns channel-interleaved M-byte samples
// pad, 0 or 1                  // Padding byte if M * Nc * Ns is odd


*/



class SoundData
{
private:
    short pcm;                       // 0 is PCM, 1 is NON-PCP, 2 extensible format

    char ckID[5];                   // "riff"
    int ckSize;                     // 4 + (PCM_OR_NONPCM?(24):(26 + 12):(48 + 12) + (8 + M * Nc * Ns + (0 or 1))
    char waveID[5];                 // "WAVE"

    // format:
    char ckFormatID[5];             // "fmt"
    int ckFormatSize;               // PCM_OR_NONPCM?(16):(18):(40)
        short wFormatTag;           // PCM_OR_NONPCM?(WAVE_FORMAT_PCM):(formatcode):(WAVE_FORMAT_EXTENSIBLE)
        short nChannels;            // Nc
        int nSamplesPerSec;         // F
        int nAvgBytesPerSec;        // F * M * Nc
        short nBlockAlign;          // M * Nc
        short wBitsPerSample;       // rounds up to 8 * M

        // if its NONPCM or EXTENSIBLE format
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
    char ckDataID[5];               // "data"
    int ckDataSize;                     // M * Nc * Ns
    // data                         // Nc * Ns channel-interleaved M-byte samples
    // pad, 0 or 1                  // Padding byte if M * Nc * Ns is odd



public:
    SoundData();
    ~SoundData();

    string info();
};

#endif // SOUNDDATA_H
