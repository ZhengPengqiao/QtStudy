#ifndef CONTECTFORM_H
#define CONTECTFORM_H

#include <QWidget>

namespace Ui {
class ContectFrame;
}

class ContectFrame : public QWidget
{
    Q_OBJECT

public:
    explicit ContectFrame(QWidget *parent = 0);
    ~ContectFrame();

    void setCurrentIndex(int index);

private:
    Ui::ContectFrame *ui;
};

#endif // CONTECTFORM_H
