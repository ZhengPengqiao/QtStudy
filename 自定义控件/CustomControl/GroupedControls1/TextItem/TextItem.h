#ifndef TEXTITEM_H
#define TEXTITEM_H

#include <QWidget>

namespace Ui {
class TextItem;
}

class TextItem : public QWidget
{
    Q_OBJECT

public:
    explicit TextItem(QWidget *parent = 0);
    ~TextItem();

private:
    Ui::TextItem *ui;
};

#endif // TEXTITEM_H
