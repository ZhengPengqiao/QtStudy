#ifndef IMAGECTRL_H
#define IMAGECTRL_H

#include <QWidget>
#include <QLabel>
#include <QMessageBox>
#include <QMouseEvent>
#include <QList>
#include <QPoint>
#include <QDebug>
#include <QList>

class PainterWidget : public QLabel
{
    Q_OBJECT
public:
    explicit PainterWidget(QLabel *parent = 0);
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void addRectItem(QString itemName);
    void delRectItem(int i);
    int saveSubImage();

    QList<QRect*> rectList;
    QList<QRectF*> rectGlList;
    QList<QString> strList;

    QRect rectTmp;

private:
    bool isCanAddFlag;

signals:
    void onPointChange(QRect &rectTmp);
    void onRectItemChange(QList<QRect*> rectList, QList<QString> strList, QList<QRectF*> rectGlList);

public slots:
};

#endif // IMAGECTRL_H
