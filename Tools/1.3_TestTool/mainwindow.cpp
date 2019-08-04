#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPushButton>
#include <QDebug>
#include <QProxyStyle>
#include "customtabstyle.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tabWidget->tabBar()->setStyle(new CustomTabStyle);

    connect(ui->TTHBPPage, &TTHBPForm::tthbp_receiveData,
            ui->colorCheckPage, &ColorCheckForm::colorCheck_receiveData);

    connect(ui->colorCheckPage, &ColorCheckForm::colorCheck_sendData,
            ui->TTHBPPage, &TTHBPForm::tthbp_sendData);

    connect(ui->colorCheckPage, &ColorCheckForm::colorCheck_openTTHBP,
            ui->TTHBPPage, &TTHBPForm::button_OpenSerial_onClick);

    connect(ui->colorCheckPage, &ColorCheckForm::colorCheck_closeTTHBP,
            ui->TTHBPPage, &TTHBPForm::button_CloseSerial_onClick);

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::resizeEvent(QResizeEvent *event)
{
    qDebug() << event;
}


