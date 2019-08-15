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

int RecdVideo::open(QString path, int width, int height, int fps, char *compressor)
{
    m_avi = AVI_open_output_file(const_cast<char*>(path.toStdString().c_str()));

    if( m_avi == NULL )
    {
        return -RECDVIDEO_RETURN_OPENFAILD;
    }
    //adjust the screenshot size.
    AVI_set_video(m_avi, width, height, fps, compressor);
    recd_status = true;
}

int RecdVideo::close()
{
    if( m_avi == NULL )
    {
        return -RECDVIDEO_RETURN_AVINULL;
    }

    AVI_close(m_avi);
    recd_status = false;
}

int RecdVideo::WriteVideoData(const QPixmap &map, int quality)
{
    if( m_avi == NULL )
    {
        return -RECDVIDEO_RETURN_AVINULL;
    }

    QByteArray ba;
    QBuffer    bf(&ba);
    if (!map.save(&bf, "jpg", quality))
    {
        exit(0);
    }

    //write in file.
    AVI_write_frame(m_avi, ba.data(), ba.size(), 1);
}


bool RecdVideo::isRecding()
{
    return recd_status;
}


int RecdVideo::videoFrames()
{
    if( m_avi == NULL )
    {
        return -RECDVIDEO_RETURN_AVINULL;
    }

    return AVI_video_frames(m_avi);
}

int RecdVideo::setVideo(int width, int height, int fps, char *compressor)
{
    if( m_avi == NULL )
    {
        return -RECDVIDEO_RETURN_AVINULL;
    }

    AVI_set_video(m_avi, width, height, fps, compressor);
}