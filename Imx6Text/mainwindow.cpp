#include "mainwindow.h"
#include <QDesktopWidget>
#include <QApplication>
#include <QPushButton>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QMouseEvent>
#include <QColor>
#include <switchbutton.h>

MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent)
{
    this->setGeometry(QApplication::desktop()->screenGeometry());

    QPushButton *q = new QPushButton(this);
    q->setStyleSheet("background-color:rgb(255,255,0);");
    q->setGeometry(100,100,100,100);
    QPushButton *q1 = new QPushButton(this);
    q1->setGeometry(200,200,200,200);
    QPushButton *q2 = new QPushButton(this);
    q2->setGeometry(400,400,300,300);

    SwitchButton *sb = new SwitchButton(this);
    sb->setGeometry(400,100,100,20);




    QHBoxLayout *hbox = new QHBoxLayout();
    QPushButton *pButton1 = new QPushButton("One");
    QPushButton *pButton2 = new QPushButton("Two");
    QPushButton *pButton3 = new QPushButton("Three");
    QPushButton *pButton4 = new QPushButton("Four");
    QPushButton *pButton5 = new QPushButton("Five");

    hbox->addWidget(pButton1);
    hbox->addWidget(pButton2);
    hbox->addWidget(pButton3);
    hbox->addWidget(pButton4);
    hbox->addWidget(pButton5);

    this->setLayout(hbox);
}

MainWindow::~MainWindow()
{

}


void MainWindow::mousePressEvent(QMouseEvent *event)
{
    QMessageBox::about(this,"POS","POS"+QString::number(event->x()) +","+ QString::number(event->y()));
}
