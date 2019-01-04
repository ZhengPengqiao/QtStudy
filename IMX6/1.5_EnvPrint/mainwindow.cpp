#include "mainwindow.h"
#include <QDesktopWidget>
#include <QApplication>
#include <QPushButton>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QMouseEvent>
#include <QColor>
#include <QFrame>
#include <QDebug>
#include <QLabel>
#include <QProcess>

MainWindow::MainWindow(QFrame *parent) :
    QFrame(parent)
{
    key_count = 0;

    QRect rect = QApplication::desktop()->screenGeometry();

    qDebug() << "x:" << rect.x() << " y:" << rect.y()
             << " w:"<< rect.width() << " h:" << rect.height();

    testLabel = new QLabel(this);
    testLabel->setText("位置信息");
    testLabel->setGeometry(300, 50, 200, 100);

    keyLabel = new QLabel(this);
    keyLabel->setText("按键信息");
    keyLabel->setGeometry( 300, 100, 400, 100);


    envEdit = new QTextEdit(this);
    envEdit->setGeometry(150, 200, 600, 300);
    envEdit->setStyleSheet("background-color: rgb(255, 255, 255, 40);");
    envEdit->setReadOnly(true);

    QStringList environmentList = QProcess::systemEnvironment();
    foreach (QString environment, environmentList )
    {
        envEdit->append(environment);
    }


    QPushButton *ltButton = new QPushButton(this);
    ltButton->setGeometry(30,30, 30, 30);
    ltButton->setObjectName("ltButton");
    ltButton->setStyleSheet(QStringLiteral("#ltButton{border-image:url(:/assert/dot.png);}"));

    QPushButton *lbButton = new QPushButton(this);
    lbButton->setGeometry(30, rect.height()-60, 30, 30);
    lbButton->setObjectName("lbButton");
    lbButton->setStyleSheet(QStringLiteral("#lbButton{border-image:url(:/assert/dot.png);}"));

    QPushButton *rtButton = new QPushButton(this);
    rtButton->setGeometry(rect.width()-60,30, 30, 30);
    rtButton->setObjectName("rtButton");
    rtButton->setStyleSheet(QStringLiteral("#rtButton{border-image:url(:/assert/dot.png);}"));

    QPushButton *rbButton = new QPushButton(this);
    rbButton->setGeometry(rect.width()-60, rect.height()-60, 30, 30);
    rbButton->setObjectName("rbButton");
    rbButton->setStyleSheet(QStringLiteral("#rbButton{border-image:url(:/assert/dot.png);}"));

    QPushButton *ctButton = new QPushButton(this);
    ctButton->setGeometry((rect.width()-60)/2, (rect.height()-60)/2, 30, 30);
    ctButton->setObjectName("ctButton");
    ctButton->setStyleSheet(QStringLiteral("#ctButton{border-image:url(:/assert/dot.png);}"));
}

MainWindow::~MainWindow()
{

}


void MainWindow::mousePressEvent(QMouseEvent *event)
{
    testLabel->setText("POS"+QString::number(event->x()) +","+ QString::number(event->y()));
}


void MainWindow::keyPressEvent(QKeyEvent *event)
{
    keyLabel->setText("Key_Return: " + QString::number(key_count++) + " " + QString::number(event->key()));
}



