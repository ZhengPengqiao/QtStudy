#ifndef RECDVIDEO_H
#define RECDVIDEO_H

#include <QWidget>
#include "avilib.h"

class RecdVideo
{
public:
    explicit RecdVideo();
    ~RecdVideo();

    void Start(QString path, int width, int height, int fps);
    void Stop();
    void WriteVideoData(const QPixmap &map, int quality);
    bool isRecding();


private:
    avi_t*       m_avi;         //screenshot class.
    bool recd_status;
};

#endif // RECDVIDEO_H
