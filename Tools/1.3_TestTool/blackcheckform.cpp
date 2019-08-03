#include "blackcheckform.h"
#include "ui_blackcheckform.h"
#include "ui_mainwindow.h"
#include <QTimer>
#include <QFileDialog>
#include <QDebug>
#include <QPainter>
#include <QPen>

BlackCheckForm::BlackCheckForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BlackCheckForm)
{
    ui->setupUi(this);


    ui->scrollArea->setWidgetResizable(false);
    ui->scrollArea->setBackgroundRole(QPalette::Dark);
    ui->scrollArea->setAlignment(Qt::AlignCenter);
    ui->label->installEventFilter(this);

    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(ReadFrame()));

    getVideoMode(ui->combo_Video->currentText());
    index = 0;

}


BlackCheckForm::~BlackCheckForm()
{
    delete ui;
}

void BlackCheckForm::ReadFrame()
{
    Mat g_GaryFrame;
    Mat g_GrayDetectedEdges;
    Mat g_cannyDetectedEdges;
    vector<Vec2f> lines;

    if(capture.isOpened())
    {
        capture >> frame;
        if(!frame.empty())
        {
            cvtColor(frame, dst_frame, CV_BGR2RGB);     //  OpenCV中Mat读入的图像是BGR格式，要转换为RGB格式

            // 将抓取到的帧，转换为QImage格式。QImage::Format_RGB888不同的摄像头用不同的格式。
            QImage image((const uchar*)dst_frame.data, dst_frame.cols, dst_frame.rows, QImage::Format_RGB888);
            ui->label->setPixmap(QPixmap::fromImage(image));    //  将图片显示到label上
            ui->label->resize( ui->label->pixmap()->size());    //  将label控件resize到fame的尺寸
            ui->label->setGeometry(0,0,ui->label->sizeHint().width(),ui->label->sizeHint().height());

            for( int i = 0; i < ui->formLayout->rowCount(); i++ )
            {
                Rect rect(rectList.at(i).x(),
                          rectList.at(i).y(),
                          rectList.at(i).width(),
                          rectList.at(i).height());

                dst_frame(rect).copyTo(roiMat);
                roiimage = QImage((const uchar*)roiMat.data, roiMat.cols,
                                  roiMat.rows, roiMat.cols*3, QImage::Format_RGB888);
                labelList.at(i)->setPixmap(QPixmap::fromImage(roiimage));    //  将图片显示到label上
                labelList.at(i)->resize(200,100);    //  将label控件resize到fame的尺寸
            }

            ui->scrollAreaWidgetContents->resize(ui->label->sizeHint());
        }
    }
}

void BlackCheckForm::on_button_OpenVideo_clicked()
{
    if( video_mode == 1 )
    {
        if( file_name.isEmpty() )
        {
            file_name = QFileDialog::getOpenFileName(this, tr("Open Video"), ".", tr("Video File(*.avi *.mp4 *.h264)"));
            ui->statusBar->setText("file_name :" + file_name);
        }
        capture.open(file_name.toStdString());
        qDebug() << "on_button_OpenVideo_clicked FileName:" << file_name;
    }
    else
    {
        capture.open(video_number);
        qDebug() << "on_button_OpenVideo_clicked VideoNum:" << video_number;
    }

}


void BlackCheckForm::on_button_CloseVideo_clicked()
{
    //  释放内存；
    capture.release();
    frame.release();

    ui->statusBar->setText("CloseVideo");
    qDebug() << "on_button_CloseVideo_clicked";
}

void BlackCheckForm::on_button_StartVideo_clicked()
{
    timer->start(25);   //  开始计时，每隔25毫秒更新一次，超时则发出timeout()信号

    ui->statusBar->setText("StartVideo");
    qDebug() << "on_button_StartVideo_clicked";
}

void BlackCheckForm::on_button_StopVideo_clicked()
{
    timer->stop();      //  停止读取数据。

    ui->statusBar->setText("StopVideo");
    qDebug() << "on_button_StopVideo_clicked";
}

void BlackCheckForm::on_combo_Video_Change(QString str)
{
    ui->statusBar->setText(str);
    getVideoMode(str);
    qDebug() << "on_combo_Video_Change" << str;
}

void BlackCheckForm::getVideoMode(QString str)
{
    if( str == "Camera Video0" )
    {
        video_mode = 0;
        video_number = 0;
        ui->statusBar->setText("Camera Video0");
    }
    else if( str == "Camera Video1" )
    {
        video_mode = 0;
        video_number = 1;
        ui->statusBar->setText("Camera Video1");
    }
    else if( str == "Camera Video2" )
    {
        video_mode = 0;
        video_number = 2;
        ui->statusBar->setText("Camera Video2");
    }
    else if( str == "File Video" )
    {
        video_mode = 1;
        file_name = QFileDialog::getOpenFileName(this, tr("Open Video"), ".", tr("Video File(*.avi *.mp4 *.h264)"));
        ui->statusBar->setText("File Video file_name :" + file_name);
    }
}


void BlackCheckForm::on_button_AddRect_clicked()
{
    QLabel *label = new QLabel;
    ui->formLayout->addRow(new QLabel(QString("(%1,%2)(%3,%4)")
                                      .arg(rect.x()).arg(rect.y()).arg(rect.width()).arg(rect.height())), label);
    rectList.append(rect);
    labelList.append(label);
}


void BlackCheckForm::mouseReleaseEvent(QMouseEvent *ev)
{
    QPoint tp = ev->pos() - ui->scrollArea->pos() - ui->label->pos() - ui->scrollAreaWidgetContents->pos();
    rect.setBottomRight(tp);
    qDebug() << tp << "release" << rect;
}

void BlackCheckForm::mousePressEvent(QMouseEvent *ev)
{
    QPoint tp = ev->pos() - ui->scrollArea->pos() - ui->label->pos() - ui->scrollAreaWidgetContents->pos();
    rect.setTopLeft(tp);
    rect.setBottomRight(tp);
    qDebug() << tp << "press" << rect;
}


void BlackCheckForm::mouseMoveEvent(QMouseEvent *ev)
{
    QPoint tp = ev->pos() - ui->scrollArea->pos() - ui->label->pos() - ui->scrollAreaWidgetContents->pos();
    rect.setBottomRight(tp);
    qDebug() << tp << "move" << rect;
}

