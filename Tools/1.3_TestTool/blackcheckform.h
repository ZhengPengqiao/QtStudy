#ifndef BLACKCHECKFORM_H
#define BLACKCHECKFORM_H

#include <QWidget>


#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"


using namespace cv;
using namespace std;

namespace Ui {
class BlackCheckForm;
}

class BlackCheckForm : public QWidget
{
    Q_OBJECT

public:
    explicit BlackCheckForm(QWidget *parent = 0);
    ~BlackCheckForm();

    void getVideoMode(QString str);

public slots:
    void on_button_CloseVideo_clicked();
    void on_button_StopVideo_clicked();
    void on_button_StartVideo_clicked();
    void on_button_OpenVideo_clicked();
    void on_combo_Video_Change(QString str);
    void ReadFrame();

private:
    Ui::BlackCheckForm *ui;
    QTimer *timer;
    VideoCapture capture;
    Mat frame;
    Mat dst_frame;
    QString file_name;
    int video_mode;
    int video_number;

    Rect rect_roi;


};



#endif // BLACKCHECKFORM_H
