#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->label_filename->setText("please select file name");

    connect(&upd, SIGNAL(updateDone()), this, SLOT(accept()));
    connect(&upd, SIGNAL(updateFail()), this, SLOT(reject()));
    connect(&upd, SIGNAL(progress(int)), this, SLOT(OnProgress(int)));
}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::accept()
{
    qDebug() << "updateDone";
    ui->statusBar->showMessage("update Done");
}


void MainWindow::reject()
{
    qDebug() << "updateFail";
    ui->statusBar->showMessage("update Fail");
}


void MainWindow::OnProgress(int val)
{
    ui->progressBar->setValue(val);
}


void MainWindow::button_checkpack_onClick()
{
    qDebug() << "button_checkpack onClick";
    if (!upd.CheckPackage(file_name))
    {
        qDebug() << "CheckPackage Falied";
        ui->statusBar->showMessage("CheckPackage Falied");
        return;
    }
    else
    {
        qDebug() << "CheckPackage Success";
        ui->statusBar->showMessage("CheckPackage Success");
    }

    qDebug() << "Version:" << upd.GetVersion();
    ui->statusBar->showMessage("CheckPackage Success" + upd.GetVersion());
}



void MainWindow::button_expack_onClick()
{
    qDebug() << "button_expack onClick";
    ui->statusBar->showMessage("button_expack onClick");

    if (!upd.CheckPackage(file_name))
    {
        qDebug() << "CheckPackage Falied";
        ui->statusBar->showMessage("CheckPackage Falied");
        return;
    }
    else
    {
        qDebug() << "CheckPackage Success";
        ui->statusBar->showMessage("CheckPackage Success");
    }

    upd.run();
}

void MainWindow::button_selectfile_onClick()
{

    file_name = QFileDialog::getOpenFileName(this, tr("Update File"), ".", tr("Update File(*.bin)"));
    ui->label_filename->setText(file_name);
}
