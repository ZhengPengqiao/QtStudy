#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QImageReader>
#include <QPixmap>
#include <QBrush>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->scrollArea->setBackgroundRole(QPalette::Dark);
    ui->scrollArea->setVisible(true);

    ui->imageLabel->setPixmap(QPixmap(":/image/background.png"));
    ui->imageLabel->adjustSize();
}


MainWindow::~MainWindow()
{
    delete ui;
}
