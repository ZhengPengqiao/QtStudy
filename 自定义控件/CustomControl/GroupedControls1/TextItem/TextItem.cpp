#include "TextItem.h"
#include "ui_TextItem.h"

TextItem::TextItem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TextItem)
{
    ui->setupUi(this);
}

TextItem::~TextItem()
{
    delete ui;
}
