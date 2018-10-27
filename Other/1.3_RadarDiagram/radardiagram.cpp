#include "radardiagram.h"
#include <QPainter>
#include <QEvent>
#include <QTimer>
#include <QDebug>
#include <stdio.h>
#include <QMessageBox>
#include <QString>
#include <QtMath>
#include <QList>

RadarDiagrame::RadarDiagrame(QWidget *parent): QWidget(parent)
{
    color.setRgb(0,0,0);
    center.setX(250);
    center.setY(250);
    memset(status,0, sizeof(status));
}

RadarDiagrame::~RadarDiagrame()
{

}

int RadarDiagrame::setShowStatus(int x, int y, int sta)
{
    status[x][y] = sta;
    update(rect());
    return 0;
}


void RadarDiagrame::mousePressEvent(QMouseEvent *)
{
    int red = color.red()+5;
    if( red >= 255 )
    {
        red = 255;
    }
    color.setRed(red);
    update(rect());
}


void RadarDiagrame::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.save();

    for(int i = 0; i < 7; i++)
    {
        for(int j = 0; j < 3; j++)
        {
            if(status[j][i] == 0)
            {
                addArcArea(50,100, 450+j*300, 200, 100+i*15,10,QColor(60,30*i,0), QColor(30,30*(i+1),0), 60);
            }
        }
    }

    painter.restore();
}


void RadarDiagrame::addArcArea(int centerX, int centerY, int degreeS, int degreeL,
                               int diameterS, int diameterL,QColor colorStart,
                               QColor colorEnd, int colorGrade)
{
    pointList.clear();
    colorList.clear();
    addArcAreaPoint(centerX,centerY, degreeS, degreeL, diameterS,diameterL);
    addArcAreaColor(colorStart, colorEnd, colorGrade);
    paintArcArea();
}


void RadarDiagrame::paintArcArea()
{
    QPainter painter(this);
    int index = 0, indexd = 0;
    for( int i = 0; i < pointList.length(); i++ )
    {
        index = i*colorList.length()/(pointList.length());
        if( index != indexd )
        {
            indexd = index;
            painter.setPen(colorList.at(index));
        }
        painter.drawPoint(pointList.at(i));
    }
}

void RadarDiagrame::addArcAreaPoint(int centerX, int centerY, int degreeS, int degreeL, int diameterS, int diameterL)
{
    for ( int i = diameterS; i < (diameterS+diameterL); i++ )
    {
        addArc(centerX,centerY,degreeS,degreeL,i);
    }
}


void RadarDiagrame::addArcAreaColor(QColor colorStart, QColor colorEnd, int colorGrade )
{
    int rStep = (colorEnd.red()-colorStart.red()) / colorGrade;
    int gStep = (colorEnd.green()-colorStart.green()) / colorGrade;
    int bStep = (colorEnd.blue()-colorStart.blue()) / colorGrade;
    for( int i = 0; i < colorGrade; i++ )
    {
        colorList.append(QColor(colorStart.red()+rStep*i,colorStart.green()+gStep*i,colorStart.blue()+bStep*i));
    }
}


void RadarDiagrame::addArc(int centerX, int centerY, int degreeS, int degreeL, int diameter)
{
    float x = 0;
    float y = 0;
    for( int i = degreeS; i < (degreeS+degreeL); i++ )
    {
        x = centerX+(int)diameter*sin(i*PIUNIT/10);
        y = centerY+(int)diameter*cos(i*PIUNIT/10);
        pointList.append(QPoint(x,y));
    }
}
