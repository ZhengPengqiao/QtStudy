#include "widgetdemo.h"
#include "ui_widgetdemo.h"

WidgetDemo::WidgetDemo(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WidgetDemo),
    progress1(0),
    progress2(0)
{
    ui->setupUi(this);

    timer.start(33, this);
}



void WidgetDemo::timerEvent(QTimerEvent *event)
{
    progress1 += 1;
    if( progress1 >= 100 )
    {
        progress1 = 0;
    }

    progress2 += 2;
    if( progress2 >= 100 )
    {
        progress2 = 0;
    }

    ui->progress1->setValue(progress1);
    ui->progress2->setValue(progress2);
}


WidgetDemo::~WidgetDemo()
{
    delete ui;
}
