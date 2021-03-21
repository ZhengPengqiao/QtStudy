#include "videocheckform.h"
#include "ui_videocheckform.h"

VideoCheckForm::VideoCheckForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::VideoCheckForm)
{
    ui->setupUi(this);
}

VideoCheckForm::~VideoCheckForm()
{
    delete ui;
}
