#include "cbackgroundwnd.h"

#include <QDesktopWidget>
#include <QApplication>
#include <QPalette>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QPainter>
#include <QBrush>
#include <QPushButton>
#include <QDebug>

#include "clayersetup.h"

CBackgroundWnd::CBackgroundWnd(QWidget *parent)
    : QWidget(parent)
{
    this->setGeometry(0,0,
        QApplication::desktop()->width(),
        QApplication::desktop()->height());

    QPalette bg = this->palette();
    bg.setBrush(this->backgroundRole(),QBrush(Qt::black));
    this->setPalette(bg);
    this->setAutoFillBackground(true);

    m_pBtn1 = new QPushButton("Button", this);
    m_pBtn1->setGeometry(1050, 50, 150, 60);
    m_pBtn1->hide();

    connect(m_pBtn1, SIGNAL(clicked(bool)), this, SLOT(onBtn1Click()));
}

CBackgroundWnd::~CBackgroundWnd()
{

}

void CBackgroundWnd::mousePressEvent(QMouseEvent * event)
{
    QWidget::mousePressEvent(event);
#if 1
    QImage *image = CLayerSetup::getInstance()->getNextAlphaImage();
    QPainter apainter(image);

    QBrush absh(Qt::SolidPattern);
    absh.setColor(QColor(Qt::white));
    apainter.fillRect(image->rect(), absh);

    static bool bxxx = true;
    if(bxxx) {
        apainter.fillRect(600, 350, 600, 350, Qt::gray);
    } else {
        apainter.fillRect(100, 350, 500, 350, Qt::gray);
    }

    bxxx = !bxxx;
    CLayerSetup::getInstance()->switchNextAlphaBuffer();
#else
    onBtn1Click();
#endif
}

void CBackgroundWnd::paintEvent(QPaintEvent * )
{
    CLayerSetup *layer = CLayerSetup::getInstance();

    QPainter painter(this);
    QBrush bsh(Qt::SolidPattern);

    bsh.setColor(QColor(Qt::blue));
    painter.fillRect(100, 50, 400, 200, bsh);

    bsh.setColor(layer->getAlphaColor());
    painter.fillRect(600, 50, 400, 200, bsh);
}

void CBackgroundWnd::onBtn1Click()
{
    static bool bxxx = false;
    CLayerSetup::getInstance()->enableLocalAlpha(bxxx);
    bxxx = !bxxx;
}
