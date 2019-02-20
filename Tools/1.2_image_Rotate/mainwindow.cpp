#include "mainwindow.h"

MainWindow::MainWindow()
    : x(0),
     speed(1)
{
    pix = new QPixmap("./test2.png");
    setGeometry(100, 100, pix->width(), pix->height());
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

    painter.begin(this);
    painter.translate(  pix->width()/2,  pix->height()/2);
    painter.rotate(x);
    painter.translate( -pix->width()/2, -pix->height()/2);
    painter.drawPixmap(0,0, *pix);
    painter.end();
}


void MainWindow::timerEvent(QTimerEvent *event)
{
    x += speed;
    update(0, 0, this->width(), this->height());
}
