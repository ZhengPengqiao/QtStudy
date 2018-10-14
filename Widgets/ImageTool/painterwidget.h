#ifndef IMAGECTRL_H
#define IMAGECTRL_H

#include <QWidget>
#include <QLabel>
#include <QMessageBox>
#include <QMouseEvent>
#include <QList>
#include "viewitem.h"
#include <QPoint>
#include <QDebug>

class PainterWidget : public QLabel
{
    Q_OBJECT
public:
    explicit PainterWidget(QLabel *parent = 0);
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

    QList<ViewItem*> itemList;
    QPoint pointLT;
    QPoint pointRB;
private:
    ViewItem tempItem;
signals:

public slots:
};

#endif // IMAGECTRL_H
