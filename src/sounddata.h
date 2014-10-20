#ifndef SOUNDDATA_H
#define SOUNDDATA_H

#include <QString>
#include <iostream>
#include "sndfile.h"
#include <QDebug>

using namespace std;

class SoundData
{
private:
    float * srcBuffer_;
    long frames_;
    int sampleRate_;

    void importMP3(const QString fn);
    void import(const QString fn);

    // make copy constructor unusable by making it private
    SoundData(const SoundData &) { std::cerr << "Sounddata Copy Constructor should not be used";}
    void operator= (const SoundData &) {
        std::cerr << "Sounddata Assignment operator should not be used";
    }


public:
    SoundData();
    ~SoundData();


    float getsample(long nr) const;

    int samplerate() const { return sampleRate_;}
    long frames() const { return frames_; }
    const float * srcbuffer() const { return srcBuffer_; }
    bool initialized() const {return (frames_ > 0);}
};

#endif // SOUNDDATA_H
