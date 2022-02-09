#ifndef IMAGEITEM_H
#define IMAGEITEM_H

#include <QWidget>

namespace Ui {
class ImageItem;
}

class ImageItem : public QWidget
{
    Q_OBJECT

public:
    explicit ImageItem(QWidget *parent = 0);
    ~ImageItem();

private:
    Ui::ImageItem *ui;
};

#endif // IMAGEITEM_H
