#include "mainwindow.h"

MainWindow::MainWindow()
   : imageLabel(new QLabel),
     x(0),
     rectSize(50),
     speed(16)
{

    imageLabel->setBackgroundRole(QPalette::Base);
    imageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    imageLabel->setScaledContents(true);

    resize(800, 480);
    m_nTimerId = startTimer(15);
}



MainWindow::~MainWindow()
{
    if ( m_nTimerId != 0 )
    {
        killTimer(m_nTimerId);
    }
}


void MainWindow::paintEvent(QPaintEvent *event)

{

    QPainter painter(this);

    // 画一个实心矩形
    QBrush bruch(Qt::black);     // 画刷
    painter.setBrush(bruch);            // 设置画刷
    painter.drawRect(QRect(x, this->height()/2-rectSize/2, rectSize, rectSize));

}


void MainWindow::timerEvent(QTimerEvent *event)
{
    if( x >= (this->width()-rectSize) )
    {
        speed = -16;
    }
    else if( x <= 0 )
    {
        speed = 16;
    }

    x += speed;

    update(x-rectSize, this->height()/2-rectSize/2, rectSize*3, rectSize*2);
}
