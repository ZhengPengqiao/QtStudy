#ifndef FIGUREDFORM_H
#define FIGUREDFORM_H

#include <QWidget>
#include <QList>
#include <QLabel>
#include <QMouseEvent>
#include <QColor>

#include "recdlib/recdVideo.h"
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"


/* winnt.h头文件是window系统自带的，然后用到了ACCESS_MASK作为类型别名，而OpenCV里面也用到了ACCESS_MASK，
如果代码中使用了全局using namespace cv;的话，就会造成两个ACCESS_MASK冲突，傻傻分不清楚。
*/
using namespace std;

namespace Ui {
class FiguredForm;
}

class FiguredForm : public QWidget
{
    Q_OBJECT

public:
    explicit FiguredForm(QWidget *parent = 0);
    ~FiguredForm();

    void getVideoMode(QString str);
    void checkColorCtrl(QString color);
    void checkOperatCtrl(QString oper);
    void checkShowCtrl(QString color);
    void checkPowerLevelCtrl(QString str);
    void reOpenRecd();
    int getPiexSum(cv::Mat &image);
    int cameraDevices(vector<string>& list);

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
    void on_button_FiguredCtrl_clicked(bool val);
    void on_combo_CheckColor_Change(QString color);
    void on_combo_ShowCtrl_Change(QString str);
    void on_combo_CheckOperat_Change(QString operat);
    void on_combo_PowerLevel_Change(QString str);
    void on_button_osdTime_clicked(bool val);
    void on_button_updateRect_clicked();

signals:
    void colorCheck_openTTHBP();
    void colorCheck_closeTTHBP();
    void colorCheck_sendData(QByteArray buf);


private:
    Ui::FiguredForm *ui;
    QTimer *dealtimer;
    QTimer *timer;
    cv::VideoCapture capture;
    cv::Mat frame;
    cv::Mat dst_frame;
    QString file_name;
    int video_mode;
    int video_number;
    int index;

    QRect rect;
    QList<QRect> rectList;
    QList<cv::Mat*> matList;
    QList<bool> imageChangeList;
    QList<QLabel*> labelImageList;
    QList<QLabel*> labelNameList;
    QList<QColor*> colorList;
    QList<double> meanList;
    cv::Mat roiMat;
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
    int checkFiguredCount;  // 记录上电黑屏的次数
    int checkCount;         // 记录上电检测次数
    bool figuredStopCtrl; // 标记遇到黑屏时,是否停止断电
    bool osdTime;       // 记录是否叠加时间

    int tmpFiguredCount; // 上电后多次检测, 本次检测的黑块数
    int tmpFiguredTimes; // 上电后多次检测, 检测到黑屏的个数

    enum{
        CHECKCOLOR_RED,
        CHECKCOLOR_BLUE,
        CHECKCOLOR_GREEN,
        CHECKCOLOR_GRAY,
    }checkColor;

    enum{
        SHOWCTRL_RGB,
        SHOWCTRL_CHANNEL,
        SHOWCTRL_ABSDIFF,
    }showCtrl;

    enum{
        POWERCTRL_DA_H_CAM_H,
        POWERCTRL_DA_H_CAM_L,
        POWERCTRL_DA_L_CAM_H,
        POWERCTRL_DA_L_CAM_L,
    }powerCtrl;

    enum{
        CHECKOPERAT_LOW,
        CHECKOPERAT_HIG,
    }checkOperat;

    RecdVideo recdVideo;
    int capture_frameh;
    int capture_framew;

    unsigned int start_time;
    unsigned int now_time;
    unsigned int recd_file_time;
    qint64 old_writeTime;
    int figuredSum;
    int frameCount;
    double capture_fps;
    double ts = 0;
    double te = 0;
    int needSaveErrImage;
};


#endif // FIGUREDFORM_H
