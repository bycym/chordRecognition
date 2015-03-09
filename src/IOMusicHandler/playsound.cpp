#include "playsound.h"

PlaySound::PlaySound(QWidget *parent) : QWidget(parent)
{

}

PlaySound::~PlaySound()
{

}

bool PlaySound::Play(SoundData * & sndData)
{
    float * yourData = sndData->audio_data_f_.data();
    QBuffer buffer;
    float tmp = 0;

    for(int i=0; i < sizeof(yourData); i++)
    {
        tmp = yourData[i];
        buffer.write((char*) &tmp, 4);
    }
    buffer.seek(0);
    QAudioOutput * audio;
    QAudioFormat format;

    // Set up the forat
    format.setSampleRate(sndData->nAvgBytesPerSec());
    format.setChannelCount(sndData->nChannels());
    format.setCodec(sndData->ckID());
    /// TODO:
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setSampleType(QAudioFormat::Float);

    QAudioDeviceInfo info(QAudioDeviceInfo::defaultOutputDevice());
    if(!info.isFormatSupported(format))
    {
        qWarning() << "Raw audio format not supported by backend, cannot play audio.";
        return false;
    }

    audio = new QAudioOutput(format); // don't forget to delete when not needed anymore
    audio->start(&buffer);

    return true;
}
