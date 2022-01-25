#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QVBoxLayout>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    openGLWidget = new OpenGLWidget();
    ui->openGlLayout->addWidget(openGLWidget);
}


MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::setXRotation(int val)
{
    openGLWidget->setXRotation(val);
}

void MainWindow::setYRotation(int val)
{
    openGLWidget->setYRotation(val);
}

void MainWindow::setZRotation(int val)
{
    openGLWidget->setZRotation(val);
}
