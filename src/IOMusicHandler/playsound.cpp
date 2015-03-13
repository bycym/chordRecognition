#include "playsound.h"

PlaySound::PlaySound(QWidget *parent) : QWidget(parent)
{

}

PlaySound::~PlaySound()
{

}

bool PlaySound::Play(SoundData * & sndData)
{
    /*
    QByteArray byteArray;

    //generate ten random float value
    QVector<float> v;
    for (int i =0; i < 10;++i)
    {
        v << 100. * qrand()/RAND_MAX;
    }
    //ouput vector
    qDebug() << v;

    //write float value to array
    {
        QDataStream out (&byteArray,QIODevice::WriteOnly);
        foreach(float f,v)
        {
            out << f;
        }
    }

    //read all float from array
    QVector<float> v2;
    {
        QDataStream in(byteArray);
        while (!in.atEnd())
        {
            float f;
            in >> f;
            v2 << f;
        }
    }
    //ouput vector
    qDebug() << v2;

*/

    /*
    QByteArray array = query->value(0).toByteArray();
    QBuffer mediaStream(&array);
    QMediaPlayer mediaPlayer();
    //QMediaPlayer::setMedia(const QMediaContent & media, QIODevice * stream = 0);
    mediaPlayer.setMedia(QMediaContent(), &buffer);
    mediaPlayer.play();
*/
    float * yourData = sndData->audio_data_f_.data();
    QBuffer buffer;
    float tmp = 0;

    for(int i=0; i < sndData->audio_data_f_.size(); i++)
    {
        /*
        char a[sizeof(float)];
        memcpy(a, &f, sizeof(float));*/w
        buffer.write((char*) &sndData->audio_data_f_.at(i), sizeof(float));
    }
    buffer.seek(0);
    QAudioOutput * audio;
    QAudioFormat format;

    // Set up the forat
    format.setSampleRate(sndData->samplerate());
    format.setChannelCount(sndData->nChannels());
    //format.setCodec(sndData->ckID());
    format.setCodec("audio/pcm");
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
