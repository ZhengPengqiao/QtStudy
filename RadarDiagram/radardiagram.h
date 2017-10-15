#ifndef RADARDIAGRAM_H
#define RADARDIAGRAM_H

#include <QWidget>
#include <QtMath>

#define PIUNIT ((float)3.141/180)


class RadarDiagrame : public QWidget
{
    Q_OBJECT
public:
    RadarDiagrame(QWidget *parent = 0);
    ~RadarDiagrame();
    QPoint center;
    QList<QColor> colorList;
    QList<QPoint> pointList;

    void addArc(int centerX, int centerY, int degreeS, int degreeL, int diameter);
    void addArcAreaPoint(int centerX, int centerY, int degreeS, int degreeL, int diameterS, int diameterL);
    void addArcAreaColor(QColor colorStart, QColor colorEnd, int colorGrade );
    void paintArcArea();
    void addArcArea(int centerX, int centerY, int degreeS, int degreeL, int diameterS,
                    int diameterL,QColor colorStart, QColor colorEnd, int colorGrade);
    int setShowStatus(int x, int y, int status);
protected:
    void mousePressEvent(QMouseEvent *);
    void paintEvent(QPaintEvent *);

private:
    QColor color;
    int status[3][7];
};

#endif // RADARDIAGRAM_H
