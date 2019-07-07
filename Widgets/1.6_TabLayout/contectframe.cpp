#include "contectframe.h"
#include "ui_contectframe.h"

ContectFrame::ContectFrame(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ContectFrame)
{
    ui->setupUi(this);
}

ContectFrame::~ContectFrame()
{
    delete ui;
}

void ContectFrame::setCurrentIndex(int index)
{
    ui->stackedWidget->setCurrentIndex(index);
}
