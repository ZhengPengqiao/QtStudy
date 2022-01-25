#include "test1page.h"
#include "ui_test1page.h"

Test1Page::Test1Page(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Test1Page)
{
    ui->setupUi(this);
}

Test1Page::~Test1Page()
{
    delete ui;
}
