#include "lcdpage.h"
#include "ui_lcdpage.h"

LcdPage::LcdPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LcdPage),
    lcdNum(0)
{
    ui->setupUi(this);
    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(timerUpDate()));
    timer->start(1000);
}

LcdPage::~LcdPage()
{
    delete ui;
}


void LcdPage::timerUpDate()
{
    ui->lcdNumber->display(lcdNum++);
}
