#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    cmd = new QProcess(this);

    /* 将QProcess的输出打印到界面 */
    connect(cmd,&QProcess::readyReadStandardOutput,this,&MainWindow::on_readyReadStandardOutput);
    connect(cmd,&QProcess::readyReadStandardError,this,&MainWindow::on_readyReadStandardError);


    /* 将QProcess的 "启动/写入/结束" 打印到界面 */
    connect(cmd,&QProcess::started,this,&MainWindow::on_started);
    connect(cmd,&QProcess::bytesWritten,this,&MainWindow::on_byteWritten);
}

MainWindow::~MainWindow()
{
    delete ui;

    if (cmd->state() == QProcess::Running)
    {
        cmd->terminate();
        cmd->waitForFinished();
    }
}

void MainWindow::run_cmd()
{
    cmd->waitForStarted();
    cmd->write(ui->lineEdit->text().toLocal8Bit() + '\n');
    ui->textEdit->append(ui->lineEdit->text());
    ui->lineEdit->clear();
}

void MainWindow::start_shell()
{
    cmd->start("busybox ash");
}

void MainWindow::on_readyReadStandardOutput()
{
    QString outStr = QString::fromLocal8Bit(cmd->readAllStandardOutput());
    ui->textEdit->append(outStr);
}

void MainWindow::on_readyReadStandardError()
{
    QString outStr = QString::fromLocal8Bit(cmd->readAllStandardError());
    ui->textEdit->append(outStr);
}

void MainWindow::on_started()
{
    ui->statusBar->showMessage("解析器启动正常", 0);
}

void MainWindow::on_byteWritten()
{
    ui->statusBar->showMessage("指令正在解析:"+ui->lineEdit->text(), 0);
}
