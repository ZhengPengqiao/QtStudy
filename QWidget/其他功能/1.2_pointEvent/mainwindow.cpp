#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMouseEvent>
#include <QApplication>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->lineEdit_x->setText("10");
    ui->lineEdit_y->setText("10");

    fileDialog = new QFileDialog(this);
    connect(fileDialog,SIGNAL(accepted()),this,SLOT(showFileName()) );

}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::sendClick()
{
    int x = ui->lineEdit_x->text().toInt();
    int y = ui->lineEdit_y->text().toInt();
    QMessageBox::about(this,"sendClickPos","POS:"+ QString::number(x)+" "+QString::number(y));

    QWidget* w = QApplication::widgetAt(x, y);
    if (w)
    {
        QPoint p = w->mapFromGlobal(QPoint(x,y));

        QMessageBox::about(this,"QETWidget","title:" + w->objectName());

        QMouseEvent* press=new QMouseEvent(QEvent::MouseButtonPress, p, Qt::LeftButton,Qt::LeftButton,Qt::NoModifier);

        QApplication::postEvent(w,press);

        QMouseEvent* release=new QMouseEvent(QEvent::MouseButtonRelease, p, Qt::LeftButton,Qt::LeftButton,Qt::NoModifier);

        QApplication::postEvent(w,release);

    }
    else
    {
        QMessageBox::about(this,"QETWidget", "=NULL");
    }
}


void MainWindow::button1_on()
{
    QMessageBox::about(this,"button1_on","button1_on");
}

void MainWindow::button2_on()
{
    QMessageBox::about(this,"button2_on","button2_on");
}


void MainWindow::button3_on()
{
    QMessageBox::about(this,"button3_on","button3_on");
}


void MainWindow::button4_on()
{
    QMessageBox::about(this,"button4_on","button4_on");
}


void MainWindow::mousePressEvent(QMouseEvent *e)
{
    QString strX = QString::number(e->globalX());
    QString strY = QString::number(e->globalY());
    QMessageBox::about(this,"pressEvent",strX + "  " + strY);
}

void MainWindow::addFileNameInit()
{
    ui->fileList->addItem("aaaaaaaa");
    ui->fileList->addItem("bbbbbbbb");
    ui->fileList->addItem("cccccccc");
    ui->fileList->addItem("dddddddd");
    ui->fileList->addItem("eeeeeeee");
    ui->fileList->addItem("ffffffff");
    ui->fileList->addItem("gggggggg");
    ui->fileList->addItem("hhhhhhhh");
    ui->fileList->addItem("iiiiiiii");
    ui->fileList->addItem("jjjjjjjj");
}

void MainWindow::itemChicked(QListWidgetItem *item)
{
    QMessageBox::about(this,"",item->text());
}


void MainWindow::fileDialog_on()
{
    fileDialog->setWindowTitle(tr("Select File"));
    fileDialog->setDirectory(".");
    fileDialog->setModal(false);
    fileDialog->show();
}


void MainWindow::showFileName()
{
    QString path = fileDialog->selectedFiles()[0];
    QMessageBox::information(NULL, tr("Path"), tr("FileName:") + path);

}
