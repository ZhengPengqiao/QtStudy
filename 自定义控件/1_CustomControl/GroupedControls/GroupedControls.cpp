#include "GroupedControls.h"
#include "ui_GroupedControls.h"

GroupedControls::GroupedControls(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GroupedControls)
{
    ui->setupUi(this);
}

GroupedControls::~GroupedControls()
{
    delete ui;
}
