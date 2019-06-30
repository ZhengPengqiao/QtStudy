#include "test2page.h"
#include "ui_test2page.h"

Test2Page::Test2Page(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Test2Page)
{
    ui->setupUi(this);
}

Test2Page::~Test2Page()
{
    delete ui;
}
