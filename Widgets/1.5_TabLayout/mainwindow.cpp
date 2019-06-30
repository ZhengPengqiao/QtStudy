#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPushButton>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);
    connect(ui->tabFrame, &TabFrame::button_clicked, this, &MainWindow::tabFrame_obclicked);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::resizeEvent(QResizeEvent *event)
{
    qDebug() << event;
}


//拖拽操作
void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        m_bDrag = true;
        //获得鼠标的初始位置
        mouseStartPoint = event->globalPos();
        //mouseStartPoint = event->pos();
        //获得窗口的初始位置
        windowTopLeftPoint = this->frameGeometry().topLeft();
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    if(m_bDrag)
    {
        //获得鼠标移动的距离
        QPoint distance = event->globalPos() - mouseStartPoint;
        //QPoint distance = event->pos() - mouseStartPoint;
        //改变窗口的位置
        this->move(windowTopLeftPoint + distance);
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        m_bDrag = false;
    }
}



void MainWindow::tabFrame_obclicked(int buttonID)
{
    switch( buttonID )
    {
        case TabFrame::BUTTON_EXIT:
            exit(0);
        break;
        case TabFrame::BUTTON_TEST1:
            ui->contextFrame->setCurrentIndex(0);
        break;
        case TabFrame::BUTTON_TEST2:
            ui->contextFrame->setCurrentIndex(1);
        break;
        case TabFrame::BUTTON_TEST3:
            ui->contextFrame->setCurrentIndex(2);
        break;
    }
}
