#ifndef PLAYSOUND_H
#define PLAYSOUND_H

#include <iostream>
#include <QWidget>
#include "sounddata.h"
#include <vector>
#include <QVector>
#include <QBuffer>
#include <QAudioDeviceInfo>
#include <QAudioFormat>
#include <QAudioOutput>
#include <QDebug>
#include <QMediaPlayer>

class PlaySound : public QWidget
{
    Q_OBJECT
public:
    explicit PlaySound(QWidget *parent = 0);
    ~PlaySound();
    bool Play(SoundData*& sndData);

signals:

public slots:
};

#endif // PLAYSOUND_H
