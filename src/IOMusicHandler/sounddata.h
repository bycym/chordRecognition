#ifndef SOUNDDATA_H
#define SOUNDDATA_H

#include <iostream>
#include <string.h>
#include <QByteArray>
#include <QBuffer>
#include <vector>
#include <QVector>


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


    ///// <header> /////
    string waveFileName_;
    short pcm_;                       // 0 is PCM, 1 is NON-PCP, 2 extensible format

    char ckID_[5];                   // "riff"
    int ckSize_;                     // 4 + (PCM_OR_NONPCM?(24):(26 + 12):(48 + 12) + (8 + M * Nc * Ns + (0 or 1))
    char waveID_[5];                 // "WAVE"

    // format:
    char ckFormatID_[5];             // "fmt"
    int ckFormatSize_;               // PCM_OR_NONPCM?(16):(18):(40)
        short wFormatTag_;           // PCM_OR_NONPCM?(WAVE_FORMAT_PCM):(formatcode):(WAVE_FORMAT_EXTENSIBLE)
        short nChannels_;            // Nc

        /***
         * Sampling Rate / Sample Rate – The number of samples (or frames) that exist for each second of data.
         * This field is represented in Hz, or “per second.” For example, CD-quality audio has 44,100 samples
         * per second. A higher sampling rate means higher fidelity audio.
         * */
        int nSamplesPerSec_;         // F Sampling rate (blocks per second / frames)
        int nAvgBytesPerSec_;        // F * M * Nc ??frames
        short nBlockAlign_;          // M * Nc
        short wBitsPerSample_;       // rounds up to 8 * M Bits per sample

        // if its NONPCM or EXTENSIBLE format
        short cbSize_;               // Size of the extension:0

        // if its EXTENSIBLE format
        short wValidBitsPerSample_;
        int dwChannelMask_;
        /*** TODO */
        char subFormat_[5];
        // end of EXTENSIBLE format

    // if its NONPCM or EXTENSIBLE format
    // fact:
    char ckFactID_[5];               // "fact
    int ckFactSize_;                 // chunk size: 4
        int dwSampleLength_;         // Nc * Ns
    //endif


    // data:
    char ckDataID_[5];               // "data"
    int ckDataSize_;                     // M * Nc * Ns
    // data                         // Nc * Ns channel-interleaved M-byte samples
    // pad, 0 or 1                  // Padding byte if M * Nc * Ns is odd

    ///// </header> /////


    ///// <data> /////

    //QByteArray * audio_data;



    ///// </data> /////

    /// database tag
    std::string dbTag_;




public:
    SoundData();
    ~SoundData();

    /***
     * TODO: audio_data_ pointer?
     * get/set
     * */

    /*** TODO original audio_data_; */
    //QByteArray audio_data_;
    QByteArray audio_data_;
    QBuffer * audio_buffer_;
    //float * audio_data_f_;

    // vector -> data()
    //vector<float> * audio_data_f_;
    vector<float> audio_data_f_;
    //QVector<float> * audio_data_f_;


    void info();
    /// qt table view
    QVector<QString> infoQt();
    void init(int sizeOfData);


    /// for genchords
    // Sampling rate (blocks per second) = nSamplesPerSec
    int samplerate(){ return nSamplesPerSec_; }

    // Bits per sample = wBitsPerSample
    long frames(){ return audio_data_f_.size()-1; }
    //long frames(){ return wBitsPerSample_; }
    /// end


    // get:

    //float* audio_data_f(){ return audio_data_f_->data(); }

    std::string dbTag(){ return dbTag_; }
    void dbTag(std::string s){ dbTag_ = s; }

    string waveFileName(){ return waveFileName_; }
    short pcm() { return pcm_;}
    char* ckID() { return ckID_; }
    int ckSize() { return ckSize_; }
    char* waveID() { return waveID_; }

    char* ckFormatID(){ return ckFormatID_; }
    int ckFormatSize(){ return ckFormatSize_; }
    short wFormatTag(){ return wFormatTag_; }
    short nChannels(){ return nChannels_; }
    int nSamplesPerSec(){ return nSamplesPerSec_; }
    int nAvgBytesPerSec(){ return nAvgBytesPerSec_; }
    short nBlockAlign(){ return nBlockAlign_; }
    short wBitsPerSample(){ return wBitsPerSample_; }


    short cbSize(){ return cbSize_; }
    short wValidBitsPerSample(){ return wValidBitsPerSample_; }
    int dwChannelMask(){ return dwChannelMask_; }
    /*** TODO */
    char* subFormat(){ return subFormat_; }

    char* ckFactID(){ return ckFactID_; }
    int ckFactSize(){ return ckFactSize_; }
    int dwSampleLength(){ return dwSampleLength_; }

    char* ckDataID(){ return ckDataID_; }
    int ckDataSize(){ return ckDataSize_; }

    // set:
    void waveFileName(string s){ waveFileName_ = s; }
    void pcm(short p){ pcm_ = p; }
    void ckID(char* c){ strncpy(ckID_, c, 5); }
    void ckSize(int c){ ckSize_ = c; }
    void waveID(char* w){ strncpy(waveID_, w, 5); }

    void ckFormatID(char* c){ strncpy(ckFormatID_, c, 5); }
    void ckFormatSize(int siz){ ckFormatSize_ = siz; }
    void wFormatTag(short w){ wFormatTag_ = w; }
    void nChannels(short n){ nChannels_ = n; }
    void nSamplesPerSec(int n){ nSamplesPerSec_ = n; }
    void nAvgBytesPerSec(int n){ nAvgBytesPerSec_ = n; }
    void nBlockAlign(short n){ nBlockAlign_ = n; }
    void wBitsPerSample(short w){ wBitsPerSample_ = w; }


    void cbSize(short c){ cbSize_ = c; }
    void wValidBitsPerSample(short w){ wValidBitsPerSample_ = w; }
    void dwChannelMask(int d){ dwChannelMask_ = d; }
    /*** TODO */
    void subFormat(char* c){ strncpy(subFormat_, c, 5); }

    void ckFactID(char* c){ strncpy(ckFactID_, c, 5); }
    void ckFactSize(int c){ ckFactSize_ = c; }
    void dwSampleLength(int d){ dwSampleLength_ = d; }

    void ckDataID(char* c){ strncpy(ckDataID_, c, 5); }
    void ckDataSize(int c){ ckDataSize_ = c; }


    float getSample(long nr) const {
        // audio_data_f_.size() = frames
        if( nr < audio_data_f_.size()) {
            return audio_data_f_[nr];
        }
        std::cout << "ERROR: getsample - nr out of range" << endl;
        std::cerr << "getsample - nr out of range";

        //throw "out of range";
        return -100;
    }

};

#endif // SOUNDDATA_H
