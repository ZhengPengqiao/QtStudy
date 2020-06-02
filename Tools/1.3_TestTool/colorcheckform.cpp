#include "colorcheckform.h"
#include "ui_colorcheckform.h"
#include "ui_mainwindow.h"
#include <QTimer>
#include <QFileDialog>
#include <QDebug>
#include <QPainter>
#include <QPen>
#include <QDateTime>
#include "tthbpform.h"
#include "logform.h"

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
    delayCameraPowerDuration = ui->lineEdit_delayCameraPowerDuration->text().toInt();
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
    checkOperatCtrl(ui->comboBox_operat->currentText());
    checkPowerLevelCtrl(ui->comboBox_power_level->currentText());
    capture_frameh = 0;
    capture_framew = 0;
    capture_fps = 0;
    recd_file_time = ui->lineEdit_recd_file_time->text().toInt();
    osdTime = !ui->pushButton_OSDTime->isChecked();
    old_writeTime = 0;
    ts = 0;
    te = 0;
    frameCount = 0;
    fps = 0.0;

    needSaveErrImage = 0;
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
        if( checkOperat == CHECKOPERAT_HIG )
        {
            if( meanList[i] >= blackhold )
            {
                qlist_ret.append(true);
            }
            else
            {
                qlist_ret.append(false);
            }
        }
        else
        {
            if( meanList[i] <= blackhold )
            {
                qlist_ret.append(true);
            }
            else
            {
                qlist_ret.append(false);
            }
        }
    }
    return qlist_ret;
}

void ColorCheckForm::dealCtrl()
{
    unsigned char pinStatus = 0;
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
        else if( timeunit < (powerOffDuration+delayCameraPowerDuration) )
        {
            dealStatus = DEALSTATUS_DELAY_CAMERAPOWER;
            timeCount = timeunit - powerOffDuration;
        }
        else if( timeunit == (powerOffDuration+delayCameraPowerDuration) )
        {
            dealStatus = DEALSTATUS_CAMERAPOWER;
            timeCount = timeunit - powerOffDuration;
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
            switch(powerCtrl)
            {
                case POWERCTRL_DA_H_CAM_H: pinStatus=0x00; break;
                case POWERCTRL_DA_H_CAM_L: pinStatus=0x0F; break;
                case POWERCTRL_DA_L_CAM_H: pinStatus=0xF0; break;
                default: pinStatus=0xFF; break;
            }
            ui->statusBar->setText(QString("请求主机/摄像头断电 gpio2_0~gpio2_7"));

            QByteArray byteData;
            byteData.append(0x03);
            byteData.append(0x02);
            byteData.append(0x02);
            byteData.append(0x28);
            byteData.append(pinStatus);  // gpio2_0~gpio2_7 高电平
            byteData.append(0x03^0x02^0x02^0x28^pinStatus);
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
            switch(powerCtrl)
            {
                case POWERCTRL_DA_H_CAM_H: pinStatus=0xF0; break;
                case POWERCTRL_DA_H_CAM_L: pinStatus=0xFF; break;
                case POWERCTRL_DA_L_CAM_H: pinStatus=0x00; break;
                default: pinStatus=0x0F; break;
            }
            ui->statusBar->setText(QString("请求主机上电 gpio2_4~gpio2_7"));

            QByteArray byteData;
            byteData.append(0x03);
            byteData.append(0x02);
            byteData.append(0x02);
            byteData.append(0x28);
            byteData.append((char)0x0F);    // gpio2_4~gpio2_7 导通
            byteData.append(0x03^0x02^0x02^0x28^0x0F);
            byteData.append(0x3a);
            byteData.append(0x3c);
            emit colorCheck_sendData(byteData);
        }
        else if( dealStatus == DEALSTATUS_DELAY_CAMERAPOWER )
        {
            ui->statusBar->setText(QString("延时摄像头上电:%1").arg(timeCount));
        }
        else if( dealStatus == DEALSTATUS_CAMERAPOWER )
        {
            switch(powerCtrl)
            {
                case POWERCTRL_DA_H_CAM_H: pinStatus=0xFF; break;
                case POWERCTRL_DA_H_CAM_L: pinStatus=0xF0; break;
                case POWERCTRL_DA_L_CAM_H: pinStatus=0x0F; break;
                default: pinStatus=0x00; break;
            }
            ui->statusBar->setText(QString("请求摄像头上电 gpio2_0~gpio2_3"));

            QByteArray byteData;
            byteData.append(0x03);
            byteData.append(0x02);
            byteData.append(0x02);
            byteData.append(0x28);
            byteData.append((char)pinStatus);     // gpio2_0~gpio2_7 导通
            byteData.append(0x03^0x02^0x02^0x28^pinStatus);
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
                ui->listWidget_ErrTimes->
                        addItem(new QListWidgetItem(QString("黑屏 %1:").arg(checkBlankCount)
                            + QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss.zzz")));
                needSaveErrImage = 1;
                ui->statusBar->setText("等待保存错误图片");
                qDebug() << "等待保存错误图片" ;
            }
            else
            {
                ui->statusBar->setText("本次上电, 未检测到黑屏");
            }

        }

        ui->label_checkCount->setText(QString("测试次数:%1 黑屏次数:%2").arg(checkCount).arg(checkBlankCount));
    }

    LogForm::logI("ColorCheckForm", ui->statusBar->text());
}

void ColorCheckForm::ReadFrame()
{
    cv::Mat g_GaryFrame;
    cv::Mat g_GrayDetectedEdges;
    cv::Mat g_cannyDetectedEdges;
    vector<cv::Vec2f> lines;

    if(capture.isOpened())
    {
        frameCount++;

        capture >> frame;
        if(!frame.empty())
        {
            cv::Mat meanImage, mat_mean, mat_stddev;
            //  OpenCV中Mat读入的图像是BGR格式，要转换为RGB格式
            cv::cvtColor(frame, dst_frame, CV_BGR2RGB);
            rectangle(dst_frame, cv::Rect(rect.x(),rect.y(),rect.width(),rect.height()),
                      cv::Scalar(255, 0, 0), 1, cv::LINE_8, 0);


            for( int i = 0; i < labelImageList.count(); i++ )
            {
                cv::Rect rect(rectList.at(i).x(),
                          rectList.at(i).y(),
                          rectList.at(i).width(),
                          rectList.at(i).height());


                cv::rectangle(dst_frame, rect, cv::Scalar(colorList.at(i)->red(), colorList.at(i)->green(), colorList.at(i)->blue()), 1, cv::LINE_8, 0);

                dst_frame(rect).copyTo(roiMat);
                if( checkColor == CHECKCOLOR_RED )
                {
                    vector<cv::Mat> channels;
                    split(roiMat, channels);//将原图的red颜色通道分离
                    meanImage = channels.at(0);
                }
                else if( checkColor == CHECKCOLOR_GREEN )
                {
                    vector<cv::Mat> channels;
                    split(roiMat, channels);//将原图的green颜色通道分离
                    meanImage = channels.at(1);
                }
                else if( checkColor == CHECKCOLOR_BLUE )
                {
                    vector<cv::Mat> channels;
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


                cv::putText(dst_frame, "Val:"+QString::number(meanList.at(i), 'f', 1).toStdString(), cv::Point(rect.x, rect.y), cv::FONT_HERSHEY_SIMPLEX,
                        0.5, cv::Scalar(colorList.at(i)->red(), colorList.at(i)->green(), colorList.at(i)->blue()));
            }

            QDateTime time = QDateTime::currentDateTime();

            if( osdTime )
            {
                QString current_date = time.toString("yyyy.MM.dd hh:mm:ss.zzz")+
                        QString("  checkCount:%1 checkBlankCount:%2").arg(checkCount).arg(checkBlankCount);
                cv::putText(dst_frame, current_date.toStdString().c_str(), cv::Point(0, 20),
                        cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(255,0,0,1));


                if( frameCount%5 == 0 )
                {
                    te = (double)cv::getTickCount()/cv::getTickFrequency();
                    fps = 5.0 / (te - ts);
                    ts = (double)cv::getTickCount()/cv::getTickFrequency();
                }
                QString fpsString = QString("FPS: %1").arg(fps);
                // 将帧率信息写在输出帧上
                cv::putText(dst_frame, fpsString.toStdString().c_str(), cv::Point(5, 40), cv::FONT_HERSHEY_SIMPLEX,
                        0.5, cv::Scalar(0, 0, 0));
            }

            // 将抓取到的帧，转换为QImage格式。QImage::Format_RGB888不同的摄像头用不同的格式。
            QImage image((const uchar*)dst_frame.data, dst_frame.cols, dst_frame.rows, QImage::Format_RGB888);
            QPixmap frame_image = QPixmap::fromImage(image);
            ui->label->setPixmap(frame_image);    //  将图片显示到label上
            ui->label->resize( ui->label->pixmap()->size());    //  将label控件resize到fame的尺寸
            ui->label->setGeometry(0,0,ui->label->sizeHint().width(),ui->label->sizeHint().height());

            ui->scrollAreaWidgetContents->resize(ui->label->sizeHint());

            if( ui->pushButton_recd->isChecked() )
            {
                now_time = time.toTime_t();
                if( (now_time - start_time) >= recd_file_time )
                {
                    reOpenRecd();
                }
                recdVideo.WriteVideoData(frame_image, -1);
            }


            // 保存错误图片
            if( needSaveErrImage == 1 )
            {
                qDebug() << "saving err image:" << QString("黑屏 %1.jpg").arg(checkBlankCount);
                image.save(QString("黑屏 %1.jpg").arg(checkBlankCount), "JPG", 100);
                needSaveErrImage = 0;
                qDebug() << "save err image ok:" << QString("黑屏 %1.jpg").arg(checkBlankCount);
            }

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

    if( capture.isOpened() )
    {

        capture_framew = capture.get(CV_CAP_PROP_FRAME_WIDTH);
        capture_frameh = capture.get(CV_CAP_PROP_FRAME_HEIGHT);
        capture_fps = capture.get(CV_CAP_PROP_FPS);
        ui->statusBar->setText(QString("Capture W=%1 H=%2 Fps=%3").arg(capture_framew).arg(capture_frameh).arg(capture_fps));

    }
    else
    {
        if( video_mode == 1 )
        {
            ui->statusBar->setText(file_name + ": 视频文件打开失败");
            qDebug() << "open file_name:" << file_name << "Err";
        }
        else
        {
            ui->statusBar->setText(QString("video") + video_number + QString(": 视频文件打开失败") );
            qDebug() << "open VideoNum:" << video_number << "Err";
        }
    }

    LogForm::logI("ColorCheckForm", ui->statusBar->text());
}


void ColorCheckForm::on_button_CloseVideo_clicked()
{
    //  释放内存；
    capture.release();
    frame.release();

    ui->statusBar->setText("CloseVideo");
    qDebug() << "on_button_CloseVideo_clicked";
    LogForm::logI("ColorCheckForm", ui->statusBar->text());
}

void ColorCheckForm::on_button_StartVideo_clicked()
{
    if( !capture.isOpened() )
    {
        ui->statusBar->setText(ui->combo_Video->currentText() + ": Please Open Video First");
        return;
    }

    timer->start(30);

    ui->statusBar->setText("StartVideo");
    qDebug() << "on_button_StartVideo_clicked";
    LogForm::logI("ColorCheckForm", ui->statusBar->text());
}

void ColorCheckForm::on_button_StopVideo_clicked()
{
    timer->stop();      //  停止读取数据。

    ui->statusBar->setText("StopVideo");
    qDebug() << "on_button_StopVideo_clicked";
    LogForm::logI("ColorCheckForm", ui->statusBar->text());
}

void ColorCheckForm::on_combo_Video_Change(QString str)
{
    ui->statusBar->setText(str);
    getVideoMode(str);
    qDebug() << "on_combo_Video_Change" << str;
    LogForm::logI("ColorCheckForm", ui->statusBar->text());
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
    LogForm::logI("ColorCheckForm", ui->statusBar->text());
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
    LogForm::logI("ColorCheckForm", ui->statusBar->text());
}


void ColorCheckForm::on_button_Setting_clicked(bool val)
{
    if( val )
    {
        ui->pushButton_Setting->setText("修改参数");

        ui->lineEdit_blackhold->setDisabled(true);
        ui->lineEdit_checkDuration->setDisabled(true);
        ui->lineEdit_delayCheckDuration->setDisabled(true);
        ui->lineEdit_delayCameraPowerDuration->setDisabled(true);
        ui->lineEdit_powerOffDuration->setDisabled(true);


        blackhold = ui->lineEdit_blackhold->text().toInt();
        checkDuration = ui->lineEdit_checkDuration->text().toInt();
        delayCheckDuration = ui->lineEdit_delayCheckDuration->text().toInt();
        delayCameraPowerDuration = ui->lineEdit_delayCameraPowerDuration->text().toInt();
        powerOffDuration = ui->lineEdit_powerOffDuration->text().toInt();

        timeunit = -1;
        checkCount = 0;
        checkBlankCount = 0;

        ui->label_checkCount->setText(QString("测试次数:%1 黑屏次数:%2").arg(checkCount).arg(checkBlankCount));
        ui->listWidget_ErrTimes->clear();
        qDebug() << blackhold << checkDuration << delayCheckDuration << delayCameraPowerDuration << powerOffDuration;

        dealtimer->start(1000);
    }
    else
    {
        ui->pushButton_Setting->setText("确定配置");

        ui->lineEdit_blackhold->setDisabled(false);
        ui->lineEdit_checkDuration->setDisabled(false);
        ui->lineEdit_delayCheckDuration->setDisabled(false);
        ui->lineEdit_delayCameraPowerDuration->setDisabled(false);
        ui->lineEdit_powerOffDuration->setDisabled(false);

        dealtimer->stop();
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
    LogForm::logI("ColorCheckForm", ui->statusBar->text());
}

void ColorCheckForm::checkOperatCtrl(QString oper)
{
    if( oper == ">=" )
    {
        checkOperat = CHECKOPERAT_HIG;
        ui->statusBar->setText("Check >=");
    }
    else
    {
        checkOperat = CHECKOPERAT_LOW;
        ui->statusBar->setText("Check <=");
    }
    LogForm::logI("ColorCheckForm", ui->statusBar->text());
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
    LogForm::logI("ColorCheckForm", ui->statusBar->text());
}


void ColorCheckForm::checkPowerLevelCtrl(QString str)
{
    if( str == "主机:高有效 摄像头:高有效" )
    {
        powerCtrl = POWERCTRL_DA_H_CAM_H;
        ui->statusBar->setText("Check Power Level--> 主机:高有效 摄像头:高有效");
    }
    else if( str == "主机:高有效 摄像头:低有效" )
    {
        powerCtrl = POWERCTRL_DA_H_CAM_L;
        ui->statusBar->setText("Check Power Level--> 主机:高有效 摄像头:低有效");
    }
    else if( str == "主机:低有效 摄像头:高有效" )
    {
        powerCtrl = POWERCTRL_DA_L_CAM_H;
        ui->statusBar->setText("Check Power Level--> 主机:低有效 摄像头:高有效");
    }
    else
    {
        powerCtrl = POWERCTRL_DA_L_CAM_L;
        ui->statusBar->setText("Check Power Level--> 主机:低有效 摄像头:低有效");
    }
    LogForm::logI("ColorCheckForm", ui->statusBar->text());
}


void ColorCheckForm::on_combo_CheckColor_Change(QString color)
{
    checkColorCtrl(color);
}


void ColorCheckForm::on_combo_ShowCtrl_Change(QString str)
{
    checkShowCtrl(str);
}



void ColorCheckForm::on_combo_CheckOperat_Change(QString operat)
{
    checkOperatCtrl(operat);
}


void ColorCheckForm::on_combo_PowerLevel_Change(QString str)
{
    checkPowerLevelCtrl(str);
}

void ColorCheckForm::on_button_recd_clicked(bool val)
{
    int ret = 0;
    recd_file_time = ui->lineEdit_recd_file_time->text().toInt();
    qDebug() << "ColorCheckForm::on_button_recd_clicked:" << val;
    if( val )
    {
        QDateTime time = QDateTime::currentDateTime();
        start_time = time.toTime_t();
        QString current_date = time.toString("yyyyMMdd_hhmmss_zzz");

        // 点击了开始按钮,  将按钮上的文字显示关闭录像
        qDebug() << "正在录像文件: " << current_date+".avi";
        ret = recdVideo.open((current_date+".avi").toStdString().c_str(),
                        capture_framew, capture_frameh, capture_fps, (char*)"mjpg");
        if( ret != RecdVideo::RECDVIDEO_RETURN_SUCCESS )
        {
            ui->statusBar->setText("录像文件打开失败");
        }
        ui->pushButton_recd->setText("正在录像文件(点击关闭):"+current_date);
    }
    else
    {
        QDateTime time = QDateTime::currentDateTime();
        now_time = time.toTime_t();
        recdVideo.setVideo(capture_framew, capture_frameh,
                           recdVideo.videoFrames()/(now_time-start_time), (char*)"mjpg");
        // 点击了开始按钮,  将按钮上的文字显示关闭录像
        ui->pushButton_recd->setText("打开录像");
        ret = recdVideo.close();
        if( ret != RecdVideo::RECDVIDEO_RETURN_SUCCESS )
        {
            ui->statusBar->setText("录像文件关闭失败");
        }
    }
    LogForm::logI("ColorCheckForm", ui->statusBar->text());
}

void ColorCheckForm::reOpenRecd()
{
    if( recdVideo.isRecding() )
    {
        ui->statusBar->setText(QString("本次录制文件帧数fps:%1").arg(recdVideo.videoFrames()/recd_file_time));
        qDebug() << "frams" << recdVideo.videoFrames() << "fps:" << recdVideo.videoFrames()/recd_file_time;
        recdVideo.setVideo(capture_framew, capture_frameh,
                           recdVideo.videoFrames()/recd_file_time, (char*)"mjpg");
        recdVideo.close();
    }

    QDateTime time = QDateTime::currentDateTime();
    start_time = time.toTime_t();
    QString current_date = time.toString("yyyyMMdd_hhmmss_zzz");
    // 点击了开始按钮,  将按钮上的文字显示关闭录像
    recdVideo.open((current_date+".avi").toStdString().c_str(),
                    capture_framew, capture_frameh, capture_fps, (char*)"mjpg");
    ui->pushButton_recd->setText("正在录像文件(点击关闭):"+current_date);
    LogForm::logI("ColorCheckForm", ui->statusBar->text());
}


void ColorCheckForm::on_button_osdTime_clicked(bool val)
{
    osdTime = !val;
    if( val )
    {
        ui->pushButton_OSDTime->setText("图像上叠加时间");
        ui->statusBar->setText("图像上将 不会 叠加时间");
    }
    else
    {

        ui->pushButton_OSDTime->setText("图像上不叠加时间");
        ui->statusBar->setText("图像上将 会 叠加时间");
    }
    LogForm::logI("ColorCheckForm", ui->statusBar->text());
}


int ColorCheckForm::cameraDevices(vector<string>& list)
{
    (void)list;
    return 1;
}


/*************************************************************************
视频质量诊断----雪花噪声检测
一、雪花噪声即椒盐噪声，以前黑白电视常见的噪声现象。
二、原理
准备0°，45°，90°，135°4个方向的卷积模板。
用图像先和四个模板做卷积，用四个卷积绝对值最小值Min来检测噪声点。
求灰度图gray与其中值滤波图median。
判断噪声点：fabs(median-gray)>10 && min>0.1。
噪声点占整幅图像的比较即为雪花噪声率。
************************************************************************/
