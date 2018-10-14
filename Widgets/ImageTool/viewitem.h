#ifndef VIEWITEM_H
#define VIEWITEM_H

#include <QRect>
#include <iostream>

QT_BEGIN_NAMESPACE

class ViewItem
{
    enum Type {
        rect,
        fileRect,
        circle,
        fileCircle,
    };

public:
    ViewItem();

    bool isEnable;
    QRect itemRect;

    Type itemtype;
    Qt::GlobalColor itemColor;

private:


};
QT_END_NAMESPACE

#endif // VIEWITEM_H
