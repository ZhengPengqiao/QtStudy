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

MainWindow::MainWindow(QFrame *parent) :
    QFrame(parent)
{
    key_count = 0;

    QRect rect = QApplication::desktop()->screenGeometry();

    qDebug() << "x:" << rect.x() << " y:" << rect.y()
             << " w:"<< rect.width() << " h:" << rect.height();

    testLabel = new QLabel(this);
    testLabel->setText("位置信息");
    testLabel->setGeometry(600, 250, 200, 100);

    keyLabel = new QLabel(this);
    keyLabel->setText("按键信息");
    keyLabel->setGeometry(400, 350, 400, 100);



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
    if( event->key() == Qt::Key_Return )
    {
        keyLabel->setText("Key_Return Press: " + QString::number(key_count++));
    }
}



