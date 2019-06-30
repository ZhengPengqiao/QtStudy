#include "test3page.h"
#include "ui_test3page.h"

Test3Page::Test3Page(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Test3Page),
    lcdNum(0)
{
    ui->setupUi(this);
    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(timerUpDate()));
    timer->start(1000);
}

Test3Page::~Test3Page()
{
    delete ui;
}


void Test3Page::timerUpDate()
{
    ui->lcdNumber->display(lcdNum++);
}
