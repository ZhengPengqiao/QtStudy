#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "openglwidget.h"
#include <QVBoxLayout>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    OpenGLWidget *openGLWidget = new OpenGLWidget();
    ui->openGlLayout->addWidget(openGLWidget);
}


MainWindow::~MainWindow()
{
    delete ui;
}
