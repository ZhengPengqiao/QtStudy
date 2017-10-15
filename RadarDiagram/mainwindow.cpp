#include "mainwindow.h"
#include "radardiagram.h"
#include <QPushButton>
#include <QTextEdit>
#include <QHBoxLayout>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    rd = new RadarDiagrame(this);
    rd->setGeometry(rect());

    linex = new QTextEdit(this);
    linex->setGeometry(0,0,100,50);
    liney = new QTextEdit(this);
    liney->setGeometry(0,60,100,50);
    sendShow = new QPushButton("SendShow",this);
    sendShow->setGeometry(0,120,100,50);
    sendCancel = new QPushButton("sendCancel",this);
    sendCancel->setGeometry(0,180,100,50);
    connect(sendShow,SIGNAL(clicked()), this, SLOT(sendShow_chick()));
    connect(sendCancel,SIGNAL(clicked()), this, SLOT(sendCancel_chick()));
}


void MainWindow::sendCancel_chick()
{
    rd->setShowStatus(linex->toPlainText().toInt(),liney->toPlainText().toInt(),0);
}


void MainWindow::sendShow_chick()
{
    rd->setShowStatus(linex->toPlainText().toInt(),liney->toPlainText().toInt(),1);
}

MainWindow::~MainWindow()
{

}
