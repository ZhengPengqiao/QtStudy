#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QImageReader>
#include <QPixmap>
#include <QBrush>
#include <QStandardItem>
#include <QStandardItemModel>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->scrollArea->setVisible(true);

    ui->imageLabel->setPixmap(QPixmap(":/image/background2.png"));
    ui->imageLabel->adjustSize();

    QStandardItemModel *item_model = new QStandardItemModel();
    item_model->setHorizontalHeaderItem(0, new QStandardItem(QObject::tr("Name")));
    item_model->setHorizontalHeaderItem(1, new QStandardItem(QObject::tr("image_TL.")));
    item_model->setHorizontalHeaderItem(2, new QStandardItem(QObject::tr("image_RB")));
    item_model->setHorizontalHeaderItem(3, new QStandardItem(QObject::tr("image_W")));
    item_model->setHorizontalHeaderItem(4, new QStandardItem(QObject::tr("image_H")));
    item_model->setHorizontalHeaderItem(5, new QStandardItem(QObject::tr("OpenGL_x,y,w,h")));
    //利用setModel()方法将数据模型与QTableView绑定
    ui->tableView->setModel(item_model);
    ui->statusBar->showMessage("open : /image/background2.png");
}


MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::addItem()
{
    ui->statusBar->showMessage("添加一条项目");
}


void MainWindow::delItem()
{
    ui->statusBar->showMessage("删除一条项目");
}
