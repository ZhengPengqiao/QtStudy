#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
        this->windowPos = this->pos();
        this->mousePos = event->globalPos();
        this->dPos = mousePos - windowPos;
}
void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
        this->move(event->globalPos() - this->dPos);
}
