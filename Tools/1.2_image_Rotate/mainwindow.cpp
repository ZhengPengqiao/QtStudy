#include "mainwindow.h"

MainWindow::MainWindow() :
    x(0),
    speed(0.5),
    pixIndex(1)
{
    QPushButton *selectButton = new QPushButton(this);
    selectButton->setText("下一个");

    connect(selectButton, &QPushButton::clicked, this, &MainWindow::SelectImagea);

    pix = new QPixmap(":/test4.png");
    setGeometry(100, 100, pix->width(), pix->height());
    showpix = *pix;
    m_nTimerId = startTimer(16);
}


MainWindow::~MainWindow()
{
    delete pix;
    if ( m_nTimerId != 0 )
    {
        killTimer(m_nTimerId);
    }
}

void MainWindow::SelectImagea()
{
    qDebug() << "pixIndex:" << pixIndex;

    delete pix;
    pix = new QPixmap( QString(":/test%1.png").arg(pixIndex) );

    pixIndex++;
    if( pixIndex > 5 )
    {
        pixIndex = 1;
    }

    showpix = *pix;
}


void MainWindow::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    painter.translate(  showpix.width()/2,  showpix.height()/2);
    painter.rotate(x);
    painter.translate( -showpix.width()/2, -showpix.height()/2);
    painter.drawPixmap(0,0, showpix);
}


void MainWindow::timerEvent(QTimerEvent *event)
{
    x += speed;
    update(0, 0, this->width(), this->height());
}



void MainWindow::resizeEvent(QResizeEvent *ev)
{
    qDebug() << ev->size();
    showpix = pix->scaled(ev->size().width(), ev->size().width());
}
