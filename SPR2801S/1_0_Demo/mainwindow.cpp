#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "spr2801s.h"
#include "helpdialog.h"
#include <QDebug>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    spr2801s.initModel("../doc/Modules/gti_gnet3_fc20_2801.model");
}

MainWindow::~MainWindow()
{
    delete ui;
}


/**
 * @brief pushButton_IdentifyPicture_OnClick
 * 识别指定的图片
 */
void MainWindow::pushButton_IdentifyPicture_OnClick()
{
    spr2801s.main_showImage();
}

/**
 * @brief pushButton_IdentifyFolderPicture_OnClick
 * 识别文件中的图片
 */
void MainWindow::pushButton_IdentifyFolderPicture_OnClick()
{
    spr2801s.main_playVideo();
}

/**
 * @brief pushButton_IdentifyFileVideo_OnClick
 * 识别视频文件
 */
void MainWindow::pushButton_IdentifyFileVideo_OnClick()
{
    spr2801s.main_playVideo();
}

/**
 * @brief pushButton_IdentifyCameraVideo_OnClick
 * 识别摄像头视频
 */
void MainWindow::pushButton_IdentifyCameraVideo_OnClick()
{
    spr2801s.main_cameraShow();
}

/**
 * @brief pushButton_help_OnClick
 * 点击帮助按钮
 */
void MainWindow::pushButton_help_OnClick()
{
    HelpDialog *helpDialog=new HelpDialog(this);
    Qt::WindowFlags flags=helpDialog->windowFlags();
    helpDialog->setWindowFlags(flags | Qt::MSWindowsFixedSizeDialogHint);
    // 以模态方式显示对话框
    int ret=helpDialog->exec ();
    //OK按钮被按下，获取对话框上的输入，设置行数和列数
    if (ret==QDialog::Accepted)
    {

    }
    delete helpDialog;
}

/**
 * @brief pushButton_SelectModel_OnClick
 * 选择模型
 */
void MainWindow::pushButton_SelectModel_OnClick()
{
    QString fileName = QFileDialog::getOpenFileName(
        this, tr("open a file."), "./", tr("All files(*.*)"));
    ui->lineEdit_Model->setText(fileName);
}


/**
 * @brief pushButton_SelectPicture_OnClick
 * 选择识别图片
 */
void MainWindow::pushButton_SelectPicture_OnClick()
{
    QString fileName = QFileDialog::getOpenFileName(
        this, tr("open a file."), "./", tr("All files(*.*)"));
    ui->lineEdit_Picture->setText(fileName);
}

/**
 * @brief pushButton_SelectFolder_OnClick
 * 选择识别文件夹
 */
void MainWindow::pushButton_SelectFolder_OnClick()
{
    QString fileName = QFileDialog::getOpenFileName(
        this, tr("open a file."), "./", tr("All files(*.*)"));
    ui->lineEdit_Folder->setText(fileName);
}


/**
 * @brief pushButton_SelectVideo_OnClick
 * 选择识别视频
 */
void MainWindow::pushButton_SelectVideo_OnClick()
{
    QString fileName = QFileDialog::getOpenFileName(
        this, tr("open a file."), "./", tr("All files(*.*)"));
    ui->lineEdit_Video->setText(fileName);
}

/**
 * @brief pushButton_SelectCamera_OnClick
 * 选择识别摄像头
 */
void MainWindow::pushButton_SelectCamera_OnClick()
{
    QString fileName = QFileDialog::getOpenFileName(
        this, tr("open a file."), "./", tr("All files(*.*)"));
    ui->lineEdit_Camera->setText(fileName);
}
