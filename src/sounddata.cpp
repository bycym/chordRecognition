#include "sounddata.h"

void SoundData::importMP3(const QString fn)
{

}

void SoundData::import(const QString fn)
{

    SF_INFO info;
    // the first param is: const char *path, then it's need a convert
    SNDFILE * sndfile = sf_open(fn.toUtf8().constData() ,SFM_READ, &info);
    // what the fuck sndfile drop?
    qDebug() << sndfile << endl;
    if(!sndfile){
        cerr << "import: soundfile error: " << sf_strerror(sndfile) << endl;
        throw "soundfileerror";
    }
    srcBuffer_ = new float[info.frames];
    frames_ = info.frames;
    sampleRate_ = info.samplerate;

    // if the number of channels is 1
    if(info.channels == 1) {
        long srclen = sf_readf_float(sndfile, srcBuffer_, info.channels*info.frames);
        if(srclen != info.frames){
            cerr << "File contains " << info.frames << " frames. But "
                 << srclen << " have been read." << endl;
            throw "soundfileerror";
        }
        cout << "Read successfully." << endl;
    }
    else {
        // if the number of channels is more
        float * interleaved = new float[info.frames*info.channels];
        /*
                The sf_readf_XXXX functions return the number of frames read.
                Unless the end of the file was reached during the read, the
                return value should equal the number of frames requested.
                Attempts to read beyond the end of the file will not result
                in an error but will cause the sf_readf_XXXX functions to
                return less than the number of frames requested or 0 if already at the end of the file.
        */
        long srclen = sf_readf_float(sndfile, interleaved, info.channels*info.frames);
        // if the number of channels
        if(srclen != info.frames){
            cerr << "File contains " << info.frames << " frames of each of " << info.channels
                 << "channels. But " << srclen << " have been read. " << endl;
            throw "soundfileerror";
        }
    }

}

SoundData::SoundData()
{
}


float SoundData::getsample(long nr) const
{

}














