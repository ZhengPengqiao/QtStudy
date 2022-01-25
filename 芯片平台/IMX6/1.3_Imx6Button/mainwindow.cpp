#include "mainwindow.h"
#include <QDesktopWidget>
#include <QApplication>
#include <QPushButton>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QMouseEvent>
#include <QDebug>
#include <QWidget>


MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent)
{
//    this->setGeometry(QApplication::desktop()->screenGeometry());
    this->setFocusPolicy(Qt::StrongFocus);

    QPushButton *button = new QPushButton("One",this);
    button->setGeometry(100,100,100,100);
    QPushButton *button1 = new QPushButton("Two", this);
    button1->setGeometry(200,200,200,200);

}

MainWindow::~MainWindow()
{

}


void MainWindow::mousePressEvent(QMouseEvent *event)
{
    qDebug() <<"mousePressEvent: "+QString::number(event->x()) + "," + QString::number(event->y());

    //QMessageBox::about(this,"mousePressEvent","POS"+QString::number(event->x()) +","+ QString::number(event->y()));

}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    qDebug() <<"mouseReleaseEvent: "+QString::number(event->x()) + "," + QString::number(event->y());

    //QMessageBox::about(this,"mouseReleaseEvent","POS"+QString::number(event->x()) +","+ QString::number(event->y()));

}


void MainWindow::mouseDoubleClickEvent(QMouseEvent *event)
{
    qDebug() <<"mouseDoubleClickEvent: "+QString::number(event->x()) + "," + QString::number(event->y());
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    qDebug() << "Key" + QString::number(event->key());
    event->ignore();       /* ignore the event */
}
