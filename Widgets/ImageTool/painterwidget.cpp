#include "painterwidget.h"
#include <QPixmap>
#include <QPainter>
#include <QString>

PainterWidget::PainterWidget(QLabel *parent) : QLabel(parent)
{
}



void PainterWidget::paintEvent(QPaintEvent *event)
{
    (void)event;
    QPainter painter(this);
    ViewItem *item;

    painter.drawPixmap(0, 0, *this->pixmap());
    qDebug() << " length:  "<<  itemList.length();
    for(int i = 0; i < itemList.length(); i++)
    {
        item = itemList.at(i);
        if( item->isEnable )
        {
            painter.setBrush(QBrush(item->itemColor));
            painter.drawRect(item->itemRect);
        }
    }

    if( tempItem.isEnable )
    {
        painter.setBrush(QBrush(tempItem.itemColor));
        painter.drawRect(tempItem.itemRect);
    }

}


void PainterWidget::mousePressEvent(QMouseEvent *event)
{
    if(event->button()== Qt::LeftButton)
    {
        pointLT.setX(event->x());
        pointLT.setY(event->y());
        tempItem.itemRect.setTopLeft(pointLT);

        qDebug()<< "Press X:" << pointLT.x() << "  " << "Y:" << pointLT.y();
    }
}


void PainterWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button()== Qt::LeftButton)
    {
        pointRB.setX(event->x());
        pointRB.setY(event->y());
        qDebug()<< "Release  X:" << pointRB.x() << "  " << "Y:" << pointRB.y();
        ViewItem *item = new ViewItem();
        item->itemRect.setTopLeft(pointLT);
        item->itemRect.setBottomRight(pointRB);
        item->itemColor = Qt::red;
        item->isEnable = true;
        itemList.push_back(item);
        update();
    }

}

void PainterWidget::mouseMoveEvent(QMouseEvent *event)
{

    pointRB.setX(event->x());
    pointRB.setY(event->y());

    qDebug()<< "Move  X:" << pointRB.x() << "  " << "Y:" << pointRB.y();
    tempItem.itemColor = Qt::red;
    tempItem.isEnable = true;
    tempItem.itemRect.setBottomRight(pointRB);
    update();
}
