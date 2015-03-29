#include "playsound.h"

PlaySound::PlaySound(QWidget *parent) : QWidget(parent)
{

}

PlaySound::~PlaySound()
{

}

bool PlaySound::Play(SoundData * & sndData)
{
    QVector<double> yourData;// = QVector<float>::fromStdVector(sndData->audio_data_f_);
    for(int i = 0; i < 100; i++)
        yourData.push_back((double)1000);

    QBuffer buffer;
    float tmp = 0;
    for(int i=0; i < yourData.count(); i++)
    {
        tmp = yourData[i];
        buffer.write((char*) &tmp, 4);
    }
    buffer.seek(0);

    QAudioOutput* audio;
    QAudioFormat format;
    // Set up the format, eg.
    format.setSampleRate(8000); // adapt this to your needs
    format.setChannelCount(1); // adapt this to your needs
    format.setSampleSize(32); // float32
    format.setCodec("audio/pcm");
    format.setByteOrder(QAudioFormat::LittleEndian); // adapt this to your needs
    format.setSampleType(QAudioFormat::Float);

    QAudioDeviceInfo info(QAudioDeviceInfo::defaultOutputDevice());
    if (!info.isFormatSupported(format)) {
        qWarning() << "Raw audio format not supported by backend, cannot play audio.";
        return false;
    }

    audio = new QAudioOutput(format); // don't forget to delete when not needed anymore
    audio->start(&buffer);

    return true;
}
