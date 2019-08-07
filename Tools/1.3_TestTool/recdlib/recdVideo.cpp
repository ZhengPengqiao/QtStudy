#include "recdVideo.h"
#include <QApplication>
#include <QBuffer>
#include <QFile>
#include <QDebug>

RecdVideo::RecdVideo()
{
    recd_status = false;
}

RecdVideo::~RecdVideo()
{
    if (m_avi)
    {
        AVI_close(m_avi);
        recd_status = false;
    }
}

void RecdVideo::Start(QString path, int width, int height, int fps)
{
    m_avi = AVI_open_output_file(const_cast<char*>(path.toStdString().c_str()));

    //adjust the screenshot size.
    AVI_set_video(m_avi, width, height, fps, "mjpg");
    recd_status = true;
}

void RecdVideo::Stop()
{
    AVI_close(m_avi);
    recd_status = false;
}

void RecdVideo::WriteVideoData(const QPixmap &map, int quality)
{
    QByteArray ba;
    QBuffer    bf(&ba);
    if (!map.save(&bf, "jpg", quality))
    {
        exit(0);
    }

    if (m_avi)
    {
        //write in file.
        AVI_write_frame(m_avi, ba.data(), ba.size(), 1);
    }
    else
    {
        qDebug() << "m_avi = NULL : Err";
    }
}


bool RecdVideo::isRecding()
{
    return recd_status;
}
