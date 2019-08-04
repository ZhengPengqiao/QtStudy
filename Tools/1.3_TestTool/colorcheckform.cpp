#include "colorcheckform.h"
#include "ui_colorcheckform.h"
#include "ui_mainwindow.h"
#include <QTimer>
#include <QFileDialog>
#include <QDebug>
#include <QPainter>
#include <QPen>
#include "tthbpform.h"

ColorCheckForm::ColorCheckForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ColorCheckForm),
    timeunit(-1),
    checkBlankCount(0),
    checkCount(0),
    tmpBlankCount(0)
{
    ui->setupUi(this);


    ui->scrollArea->setWidgetResizable(false);
    ui->scrollArea->setBackgroundRole(QPalette::Dark);
    ui->scrollArea->setAlignment(Qt::AlignCenter);
    ui->label->installEventFilter(this);

    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(ReadFrame()));


    dealtimer = new QTimer(this);
    connect(dealtimer,SIGNAL(timeout()),this,SLOT(dealCtrl()));



    getVideoMode(ui->combo_Video->currentText());
    index = 0;

    blackhold = ui->lineEdit_blackhold->text().toInt();
    checkDuration = ui->lineEdit_checkDuration->text().toInt();
    delayCheckDuration = ui->lineEdit_delayCheckDuration->text().toInt();
    powerOffDuration = ui->lineEdit_powerOffDuration->text().toInt();

    dealStatus = DEALSTATUS_NONE;

    if( ui->pushButton_blankCtrl->isChecked() )
    {
        ui->pushButton_blankCtrl->setText("遇到黑屏停止");
        blankStopCtrl = true;
    }
    else
    {
        ui->pushButton_blankCtrl->setText("遇到黑屏不停止");
        blankStopCtrl = false;
    }

    ui->label_checkCount->setText(QString("测试次数:%1 黑屏次数:%2").arg(checkCount).arg(checkBlankCount));
    checkColorCtrl(ui->comboBox_CheckColor->currentText());
    checkShowCtrl(ui->comboBox_showCtrl->currentText());
}


ColorCheckForm::~ColorCheckForm()
{
    delete ui;
}


QList<bool> ColorCheckForm::checkBlackDeal()
{
    int i = 0;
    QList<bool> qlist_ret;
    for(i = 0; i < labelImageList.count(); i++)
    {
        if( meanList[i] < blackhold )
        {
            qlist_ret.append(true);
        }
        else
        {
            qlist_ret.append(false);
        }
    }
    return qlist_ret;
}

void ColorCheckForm::dealCtrl()
{
    if( ui->pushButton_Setting->isChecked() )
    {
        // 判断遇到黑屏是否停止检测
        if( blankStopCtrl && checkBlankCount )
        {
            ui->statusBar->setText(QString("遇到黑屏, 停止检测"));
            return;
        }

        timeunit++;
        if( timeunit == 0 )
        {
            dealStatus = DEALSTATUS_POWEROFF;
        }
        else if( timeunit < powerOffDuration )
        {
            dealStatus = DEALSTATUS_POWEROFF_DELAY;
            timeCount = timeunit;
        }
        else if( timeunit == powerOffDuration )
        {
            dealStatus = DEALSTATUS_POWERON;
        }
        else if( timeunit <= (powerOffDuration+delayCheckDuration) )
        {
            dealStatus = DEALSTATUS_DELAY_CHECK;
            timeCount = timeunit - powerOffDuration;
        }
        else if( timeunit <= (powerOffDuration+delayCheckDuration+checkDuration) )
        {
            dealStatus = DEALSTATUS_CHECKING;
            timeCount = timeunit - powerOffDuration-delayCheckDuration;
        }
        else
        {
            dealStatus = DEALSTATUS_CHECK_OK;
            timeunit = -1;
        }


        if( (dealStatus == DEALSTATUS_NONE || dealStatus == DEALSTATUS_POWEROFF) )
        {
            ui->statusBar->setText(QString("请求断电"));
            QByteArray byteData;
            byteData.append(0x03);
            byteData.append(0x02);
            byteData.append(0x02);
            byteData.append(0x28);
            byteData.append(0xff);
            byteData.append(0x03^0x02^0x02^0x28^0xff);
            byteData.append(0x3a);
            byteData.append(0x3c);
            emit colorCheck_sendData(byteData);
            tmpBlankTimes = 0;
        }
        else if( dealStatus == DEALSTATUS_POWEROFF_DELAY )
        {
            ui->statusBar->setText(QString("等待掉电完成:%1").arg(timeCount));
        }
        else if( dealStatus == DEALSTATUS_POWERON )
        {
            ui->statusBar->setText(QString("请求上电"));

            QByteArray byteData;
            byteData.append(0x03);
            byteData.append(0x02);
            byteData.append(0x02);
            byteData.append(0x28);
            byteData.append((char)0x00);
            byteData.append(0x03^0x02^0x02^0x28^0x00);
            byteData.append(0x3a);
            byteData.append(0x3c);
            emit colorCheck_sendData(byteData);
        }
        else if( dealStatus == DEALSTATUS_DELAY_CHECK )
        {
            ui->statusBar->setText(QString("等待程序启动完成:%1").arg(timeCount));
        }
        else if( dealStatus == DEALSTATUS_CHECKING )
        {
            tmpBlankCount = 0;
            QList<bool> qlist_ret = checkBlackDeal();
            QString str("检测黑屏:");
            str += QString("%1 --> ").arg(timeCount);
            // 检查本次测试是不是有黑屏
            for( int i = 0; i < qlist_ret.count(); i++ )
            {
                str += QString(" %1").arg(qlist_ret[i]);
                if( qlist_ret[i] )
                {
                    tmpBlankCount++;    // 上电后多次检测, 本次检测的黑块数
                }
            }

            if( tmpBlankCount )
            {
                tmpBlankTimes++;  // 上电后多次检测, 检测到黑屏的个数
            }

            str += QString("     :tmpBlankCount=%1 tmpBlankTimes=%2").arg(tmpBlankCount).arg(tmpBlankTimes);
            ui->statusBar->setText(str);
        }
        else if( dealStatus == DEALSTATUS_CHECK_OK )
        {
            checkCount++;   // 记录上电检测次数

            if( tmpBlankTimes == checkDuration )
            {
                checkBlankCount++;  // 记录上电黑屏的次数
                ui->statusBar->setText("本次上电, 检测到黑屏");
            }
            else
            {
                ui->statusBar->setText("本次上电, 未检测到黑屏");
            }

        }

        ui->label_checkCount->setText(QString("测试次数:%1 黑屏次数:%2").arg(checkCount).arg(checkBlankCount));
    }
}

void ColorCheckForm::ReadFrame()
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
            Mat meanImage, mat_mean, mat_stddev;
            //  OpenCV中Mat读入的图像是BGR格式，要转换为RGB格式
            cvtColor(frame, dst_frame, CV_BGR2RGB);

            rectangle(dst_frame, Rect(rect.x(),rect.y(),rect.width(),rect.height()),
                      Scalar(255, 0, 0), 1, LINE_8, 0);


            for( int i = 0; i < labelImageList.count(); i++ )
            {
                Rect rect(rectList.at(i).x(),
                          rectList.at(i).y(),
                          rectList.at(i).width(),
                          rectList.at(i).height());


                rectangle(dst_frame, rect, Scalar(colorList.at(i)->red(), colorList.at(i)->green(), colorList.at(i)->blue()), 1, LINE_8, 0);

                dst_frame(rect).copyTo(roiMat);
                if( checkColor == CHECKCOLOR_RED )
                {
                    vector<Mat> channels;
                    split(roiMat, channels);//将原图的red颜色通道分离
                    meanImage = channels.at(0);
                }
                else if( checkColor == CHECKCOLOR_GREEN )
                {
                    vector<Mat> channels;
                    split(roiMat, channels);//将原图的green颜色通道分离
                    meanImage = channels.at(1);
                }
                else if( checkColor == CHECKCOLOR_BLUE )
                {
                    vector<Mat> channels;
                    split(roiMat, channels);//将原图的blue颜色通道分离
                    meanImage = channels.at(2);
                }
                else //CHECKCOLOR_GRAY
                {
                    cvtColor(roiMat, meanImage, CV_RGB2GRAY); // 转换为灰度图
                }
                meanStdDev(meanImage, mat_mean, mat_stddev);

                if( showCtrl )
                {
                    // 注意这里的第四个参数, 指定每行的长度, 不然图像会倾斜
                    roiimage = QImage((const uchar*)meanImage.data, meanImage.cols,
                                  meanImage.rows, meanImage.cols, QImage::Format_Grayscale8);

                }
                else
                {
                    // 注意这里的第四个参数, 指定每行的长度, 不然图像会倾斜
                    roiimage = QImage((const uchar*)roiMat.data, roiMat.cols,
                                      roiMat.rows, roiMat.cols*3, QImage::Format_RGB888);
                }

                labelImageList.at(i)->setPixmap(QPixmap::fromImage(roiimage));    //  将图片显示到label上


                meanList[i] = mat_mean.at<double>(0,0);
                QString label_name = QString("(%1,%2)(%3,%4)=%5")
                        .arg(rect.x).arg(rect.y)
                        .arg(rect.width).arg(rect.height)
                        .arg(QString::number(meanList.at(i), 'f', 1));

                labelNameList.at(i)->setText(label_name);
            }

            // 将抓取到的帧，转换为QImage格式。QImage::Format_RGB888不同的摄像头用不同的格式。
            QImage image((const uchar*)dst_frame.data, dst_frame.cols, dst_frame.rows, QImage::Format_RGB888);
            ui->label->setPixmap(QPixmap::fromImage(image));    //  将图片显示到label上
            ui->label->resize( ui->label->pixmap()->size());    //  将label控件resize到fame的尺寸
            ui->label->setGeometry(0,0,ui->label->sizeHint().width(),ui->label->sizeHint().height());

            ui->scrollAreaWidgetContents->resize(ui->label->sizeHint());
        }
    }
}

void ColorCheckForm::on_button_OpenVideo_clicked()
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


void ColorCheckForm::on_button_CloseVideo_clicked()
{
    //  释放内存；
    capture.release();
    frame.release();

    ui->statusBar->setText("CloseVideo");
    qDebug() << "on_button_CloseVideo_clicked";
}

void ColorCheckForm::on_button_StartVideo_clicked()
{
    timer->start(25);   //  开始计时，每隔25毫秒更新一次，超时则发出timeout()信号

    ui->statusBar->setText("StartVideo");
    qDebug() << "on_button_StartVideo_clicked";

    dealtimer->start(1000);
}

void ColorCheckForm::on_button_StopVideo_clicked()
{
    timer->stop();      //  停止读取数据。
    dealtimer->stop();

    ui->statusBar->setText("StopVideo");
    qDebug() << "on_button_StopVideo_clicked";
}

void ColorCheckForm::on_combo_Video_Change(QString str)
{
    ui->statusBar->setText(str);
    getVideoMode(str);
    qDebug() << "on_combo_Video_Change" << str;
}

void ColorCheckForm::getVideoMode(QString str)
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


void ColorCheckForm::on_button_AddRect_clicked()
{
    QLabel *label_name = new QLabel(QString("(%1,%2)(%3,%4)")
                                    .arg(rect.x()).arg(rect.y())
                                    .arg(rect.width()).arg(rect.height()));
    QLabel *label_image = new QLabel();
    QColor *color = new QColor(qrand()%255,qrand()%255,qrand()%255);
    ui->formLayout->addRow(label_name, label_image);
    rectList.append(rect);
    labelNameList.append(label_name);
    labelImageList.append(label_image);
    colorList.append(color);
    meanList.append(0.0f);


}


void ColorCheckForm::mouseReleaseEvent(QMouseEvent *ev)
{
    QPoint tp = ev->pos() - ui->scrollArea->pos() - ui->label->pos() - ui->scrollAreaWidgetContents->pos();
    rect.setBottomRight(tp);
    qDebug() << tp << "release" << rect;
}

void ColorCheckForm::mousePressEvent(QMouseEvent *ev)
{
    QPoint tp = ev->pos() - ui->scrollArea->pos() - ui->label->pos() - ui->scrollAreaWidgetContents->pos();
    rect.setTopLeft(tp);
    rect.setBottomRight(tp);
    qDebug() << tp << "press" << rect;
}


void ColorCheckForm::mouseMoveEvent(QMouseEvent *ev)
{
    QPoint tp = ev->pos() - ui->scrollArea->pos() - ui->label->pos() - ui->scrollAreaWidgetContents->pos();
    rect.setBottomRight(tp);
    qDebug() << tp << "move" << rect;
}


void ColorCheckForm::on_button_TTHBP_clicked(bool val)
{
    if( val )
    {
        emit colorCheck_openTTHBP();
        ui->pushButton_tthbp->setText("TTHBP 关闭");
        ui->statusBar->setText("TTHBP 打开");
    }
    else
    {

        emit colorCheck_closeTTHBP();
        ui->pushButton_tthbp->setText("TTHBP 打开");
        ui->statusBar->setText("TTHBP 关闭");
    }
}


void ColorCheckForm::on_button_Setting_clicked(bool val)
{
    if( val )
    {
        ui->pushButton_Setting->setText("修改参数");

        ui->lineEdit_blackhold->setDisabled(true);
        ui->lineEdit_checkDuration->setDisabled(true);
        ui->lineEdit_delayCheckDuration->setDisabled(true);
        ui->lineEdit_powerOffDuration->setDisabled(true);


        blackhold = ui->lineEdit_blackhold->text().toInt();
        checkDuration = ui->lineEdit_checkDuration->text().toInt();
        delayCheckDuration = ui->lineEdit_delayCheckDuration->text().toInt();
        powerOffDuration = ui->lineEdit_powerOffDuration->text().toInt();

        timeunit = -1;
        checkCount = 0;
        checkBlankCount = 0;

        ui->label_checkCount->setText(QString("测试次数:%1 黑屏次数:%2").arg(checkCount).arg(checkBlankCount));

        qDebug() << blackhold << checkDuration << delayCheckDuration << powerOffDuration;
    }
    else
    {
        ui->pushButton_Setting->setText("确定配置");

        ui->lineEdit_blackhold->setDisabled(false);
        ui->lineEdit_checkDuration->setDisabled(false);
        ui->lineEdit_delayCheckDuration->setDisabled(false);
        ui->lineEdit_powerOffDuration->setDisabled(false);
    }
}


void ColorCheckForm::colorCheck_receiveData(QByteArray buf)
{
    qDebug() << "blackCheck_receiveData:" << buf;
}

void ColorCheckForm::on_button_BlankCtrl_clicked(bool val)
{
    if( val )
    {
        ui->pushButton_blankCtrl->setText("遇到黑屏停止");
        blankStopCtrl = true;
    }
    else
    {
        ui->pushButton_blankCtrl->setText("遇到黑屏不停止");
        blankStopCtrl = false;
    }
}


void ColorCheckForm::checkColorCtrl(QString color)
{
    if( color == "Blue" )
    {
        checkColor = CHECKCOLOR_BLUE;
        ui->statusBar->setText("Check Color Blue");
    }
    else if( color == "Green" )
    {
        checkColor = CHECKCOLOR_GREEN;
        ui->statusBar->setText("Check Color Green");
    }
    else if( color == "Red" )
    {
        checkColor = CHECKCOLOR_RED;
        ui->statusBar->setText("Check Color Red");
    }
    else
    {
        checkColor = CHECKCOLOR_GRAY;
        ui->statusBar->setText("Check Color Gray");
    }
}


void ColorCheckForm::checkShowCtrl(QString color)
{
    if( color == "显示原图" )
    {
        showCtrl = SHOWCTRL_RGB;
        ui->statusBar->setText("Show Ctrl : 显示原图");
    }
    else if( color == "显示效果图" )
    {
        showCtrl = SHOWCTRL_CHANNEL;
        ui->statusBar->setText("Show Ctrl : 显示效果图");
    }
    else
    {
        showCtrl = SHOWCTRL_RGB;
        ui->statusBar->setText("Show Ctrl : 显示原图");
    }
}


void ColorCheckForm::on_combo_CheckColor_Change(QString color)
{
    checkColorCtrl(color);
}


void ColorCheckForm::on_combo_ShowCtrl_Change(QString str)
{
    checkShowCtrl(str);
}
