#ifndef AUDIOBUFFER_H
#define AUDIOBUFFER_H

#include <QAudioFormat>
#include <QIODevice>
#include <QMutex>
#include <QDebug>

class AudioBuffer : public QIODevice
{
Q_OBJECT

public:
//----------------------------------------------------------------------------------------------------------------------------------
AudioBuffer(/const QAudioFormat &format,/ QObject parent = NULL, int bufSize = 1400) : QIODevice(parent)
{
m_bufSize = bufSize4;

    m_buffer1 = new const char[m_bufSize];
    m_buffer2 = new const char[m_bufSize];

    m_rightOffset = 0;
    m_leftOffset  = 0;
    m_cursor      = 0;

    qMemSet((void *)m_buffer1,0,m_bufSize);

}

//----------------------------------------------------------------------------------------------------------------------------------
~AudioBuffer()
{
delete m_buffer1;
delete m_buffer2;
}
//----------------------------------------------------------------------------------------------------------------------------------
void start()
{
if(!isOpen())
open(QIODevice::ReadWrite);
}
//----------------------------------------------------------------------------------------------------------------------------------
void stop()
{
close();
}
//----------------------------------------------------------------------------------------------------------------------------------
qint64 readData(char * data, qint64 len)
{
int free2end = m_bufSize-m_cursor;

    if(len <= free2end){
        qMemCopy(data,m_buffer1+m_cursor,len);

        m_rightOffset += len;
        m_cursor += len;

        return len;
    }
    else
    {
        qMemCopy(data,m_buffer1+m_cursor,free2end);

        m_cursor = 0;

        qMemCopy(data+free2end,m_buffer1,len-free2end);

        m_rightOffset = len-free2end;
        m_leftOffset = 0;
        m_cursor += m_rightOffset;

        return len;
    }

    return 0;
}

//----------------------------------------------------------------------------------------------------------------------------------
qint64 writeData(const char *data, qint64 len)
{
int free2end = m_bufSize-m_rightOffset;

    if(len <= free2end) // if we don't reach end of buffer
    {
        qMemCopy((void*)(m_buffer1+m_rightOffset),data,len);
        m_rightOffset += len;

        qDebug() << m_buffer1;

        return len;
    }
    else if(len <= (m_leftOffset+free2end))
    {
        qMemCopy((void*)(m_buffer1+m_rightOffset),data,free2end); //write to end
        m_leftOffset = len-free2end; // need to write more from source and set new left offset
        qMemCopy((void*)m_buffer1,data+free2end,m_leftOffset); //write lefted part 
        m_rightOffset = m_leftOffset; // set new right offset value
        m_leftOffset  = 0; // set new left offset value

        qDebug() << m_buffer1;

        return len;
    }
    else
    {
        qMemCopy((void*)(m_buffer1+m_rightOffset),data,free2end); //write to end
        m_rightOffset += len-free2end; // need to write more from source

        len -= free2end;

        qMemCopy((void*)m_buffer1,data+free2end,m_leftOffset); //write lefted part 
        m_rightOffset = 0; // set new right offset value

        len -= m_leftOffset;

        m_leftOffset = 0;

        qDebug() << m_buffer1;

        return len;
    }

    return 0;
}

//----------------------------------------------------------------------------------------------------------------------------------
private:
const char * m_buffer1;
const char * m_buffer2;
int m_rightOffset;
int m_leftOffset;
int m_cursor;
int m_bufSize;
};

#endif//AUDIOBUFFER_H