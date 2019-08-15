#ifndef COLORCHECKFORM_H
#define COLORCHECKFORM_H

#include <QWidget>
#include <QList>
#include <QLabel>
#include <QMouseEvent>
#include <QColor>

#include "recdlib/recdVideo.h"
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"


using namespace cv;
using namespace std;

namespace Ui {
class ColorCheckForm;
}

class ColorCheckForm : public QWidget
{
    Q_OBJECT

public:
    explicit ColorCheckForm(QWidget *parent = 0);
    ~ColorCheckForm();

    void getVideoMode(QString str);
    void checkColorCtrl(QString color);
    void checkShowCtrl(QString color);
    void reOpenRecd();

public slots:
    void on_button_CloseVideo_clicked();
    void on_button_StopVideo_clicked();
    void on_button_StartVideo_clicked();
    void on_button_OpenVideo_clicked();
    void on_button_Setting_clicked(bool val);
    void on_button_recd_clicked(bool val);
    void on_combo_Video_Change(QString str);
    void on_button_AddRect_clicked();
    void ReadFrame();
    void mouseReleaseEvent(QMouseEvent * ev);
    void mouseMoveEvent(QMouseEvent *ev);
    void mousePressEvent(QMouseEvent *ev);
    QList<bool> checkBlackDeal();
    void dealCtrl();
    void colorCheck_receiveData(QByteArray buf);
    void on_button_TTHBP_clicked(bool val);
    void on_button_BlankCtrl_clicked(bool val);
    void on_combo_CheckColor_Change(QString color);
    void on_combo_ShowCtrl_Change(QString str);
    void on_button_osdTime_clicked(bool val);

signals:
    void colorCheck_openTTHBP();
    void colorCheck_closeTTHBP();
    void colorCheck_sendData(QByteArray buf);


private:
    Ui::ColorCheckForm *ui;
    QTimer *dealtimer;
    QTimer *timer;
    VideoCapture capture;
    Mat frame;
    Mat dst_frame;
    QString file_name;
    int video_mode;
    int video_number;
    int index;

    QRect rect;
    QList<QRect> rectList;
    QList<QLabel*> labelImageList;
    QList<QLabel*> labelNameList;
    QList<QColor*> colorList;
    QList<double> meanList;
    Mat roiMat;
    QImage roiimage;


    int blackhold;
    int checkDuration;
    int delayCheckDuration;
    int delayCameraPowerDuration;
    int powerOffDuration;
    int timeunit;
    enum{
        DEALSTATUS_POWERON,
        DEALSTATUS_DELAY_CAMERAPOWER,
        DEALSTATUS_CAMERAPOWER,
        DEALSTATUS_DELAY_CHECK,
        DEALSTATUS_CHECKING,
        DEALSTATUS_CHECK_OK,
        DEALSTATUS_POWEROFF,
        DEALSTATUS_POWEROFF_DELAY,
        DEALSTATUS_NONE,
    }dealStatus;
    int timeCount;
    int checkBlankCount;  // 记录上电黑屏的次数
    int checkCount;         // 记录上电检测次数
    bool blankStopCtrl; // 标记遇到黑屏时,是否停止断电
    bool osdTime;       // 记录是否叠加时间

    int tmpBlankCount; // 上电后多次检测, 本次检测的黑块数
    int tmpBlankTimes; // 上电后多次检测, 检测到黑屏的个数

    enum{
        CHECKCOLOR_RED,
        CHECKCOLOR_BLUE,
        CHECKCOLOR_GREEN,
        CHECKCOLOR_GRAY,
    }checkColor;

    enum{
        SHOWCTRL_RGB,
        SHOWCTRL_CHANNEL,
    }showCtrl;

    RecdVideo recdVideo;
    int capture_frameh;
    int capture_framew;
    int capture_fps;

    unsigned int start_time;
    unsigned int now_time;
    int recd_file_time;
    qint64 old_writeTime;
};



#endif // COLORCHECKFORM_H
