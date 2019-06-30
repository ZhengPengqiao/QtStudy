#include "tabframe.h"
#include "ui_tabframe.h"

TabFrame::TabFrame(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TabFrame)
{
    ui->setupUi(this);
}

TabFrame::~TabFrame()
{
    delete ui;
}


void TabFrame::button_exit_onClick()
{
    emit button_clicked(BUTTON_EXIT);
}

void TabFrame::button_test1_onClick()
{
    emit button_clicked(BUTTON_TEST1);
}

void TabFrame::button_test2_onClick()
{
    emit button_clicked(BUTTON_TEST2);
}

void TabFrame::button_test3_onClick()
{
    emit button_clicked(BUTTON_TEST3);
}
