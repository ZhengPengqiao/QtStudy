#ifndef RECDVIDEO_H
#define RECDVIDEO_H

#include <QWidget>
#include "avilib.h"

class RecdVideo
{
public:
    explicit RecdVideo();
    ~RecdVideo();

    int open(QString path, int width, int height, int fps, char *compressor);
    int close();
    int WriteVideoData(const QPixmap &map, int quality);
    bool isRecding();
    int videoFrames();
    int setVideo(int width, int height, int fps, char *compressor);


    enum{
        RECDVIDEO_RETURN_SUCCESS,
        RECDVIDEO_RETURN_AVINULL,
        RECDVIDEO_RETURN_OPENFAILD,
    };

private:
    avi_t*       m_avi;         //screenshot class.
    bool recd_status;
};

#endif // RECDVIDEO_H
