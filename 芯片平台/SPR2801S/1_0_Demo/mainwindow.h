#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "spr2801s.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    // 点击帮助按钮
    void pushButton_help_OnClick();
    // 识别指定的图片
    void pushButton_IdentifyPicture_OnClick();
    // 识别文件中的图片
    void pushButton_IdentifyFolderPicture_OnClick();
    // 识别视频文件
    void pushButton_IdentifyFileVideo_OnClick();
    // 识别摄像头视频
    void pushButton_IdentifyCameraVideo_OnClick();
    // 选择模型
    void pushButton_SelectModel_OnClick();
    // 选择识别图片
    void pushButton_SelectPicture_OnClick();
    // 选择识别文件夹
    void pushButton_SelectFolder_OnClick();
    // 选择识别视频
    void pushButton_SelectVideo_OnClick();
    // 选择识别摄像头
    void pushButton_SelectCamera_OnClick();
private:
    SPR2801S spr2801s;

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
