#include "painterwidget.h"
#include <QPixmap>
#include <QPainter>
#include <QString>

PainterWidget::PainterWidget(QLabel *parent) : QLabel(parent)
{
    isCanAddFlag = false;
}



void PainterWidget::paintEvent(QPaintEvent *event)
{
    (void)event;
    QPainter painter(this);
    QRect *rectItem;

    painter.drawPixmap(0, 0, *this->pixmap());
    for(int i = 0; i < rectList.length(); i++)
    {
        rectItem = rectList.at(i);
        painter.setBrush(QBrush(Qt::blue));
        painter.drawRect(*rectItem);
    }

    painter.setBrush(QBrush(Qt::blue));
    painter.drawRect(rectTmp);

}


void PainterWidget::mousePressEvent(QMouseEvent *event)
{
    if(event->button()== Qt::LeftButton)
    {
        rectTmp.setLeft(event->x());
        rectTmp.setTop(event->y());
        rectTmp.setRight(event->x());
        rectTmp.setBottom(event->y());
        emit onPointChange(rectTmp);
        update();
    }
}


void PainterWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button()== Qt::LeftButton)
    {
        rectTmp.setRight(event->x());
        rectTmp.setBottom(event->y());
        emit onPointChange(rectTmp);
        update();
        isCanAddFlag = true;
    }

}

void PainterWidget::mouseMoveEvent(QMouseEvent *event)
{
    rectTmp.setRight(event->x());
    rectTmp.setBottom(event->y());
    emit onPointChange(rectTmp);
    update();
}

void PainterWidget::addRectItem(QString itemName)
{
    QRectF * rectF = new QRectF();
    if( itemName.isEmpty() )
    {
        QMessageBox::warning(NULL, "warning", "当天不具备添加条件: 名称为空", QMessageBox::Yes, QMessageBox::Yes);
        return;
    }

    if( isCanAddFlag )
    {
        isCanAddFlag = false;
    }
    else
    {
        QMessageBox::warning(NULL, "warning", "当天不具备添加条件: 确认是个已经添加？", QMessageBox::Yes, QMessageBox::Yes);
        return;
    }

    QRect *item = new QRect(rectTmp);
    rectF->setX( (float)rectTmp.x() / pixmap()->width() * 2 - 1 );
    rectF->setY( 1 - (float)rectTmp.y() / pixmap()->height() * 2 );
    rectF->setWidth( (float)rectTmp.width() / pixmap()->width() * 2 );
    rectF->setHeight( (float)rectTmp.height() / pixmap()->height() * 2 );

    rectList.push_back(item);
    strList.push_back(itemName);
    rectGlList.push_back(rectF);

    emit onRectItemChange(rectList, strList, rectGlList);
}


void PainterWidget::delRectItem(int i)
{
    strList.removeAt(i);
    rectList.removeAt(i);
    rectGlList.removeAt(i);
}
