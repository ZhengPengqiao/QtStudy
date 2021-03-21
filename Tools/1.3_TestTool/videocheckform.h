#ifndef VIDEOCHECKFORM_H
#define VIDEOCHECKFORM_H

#include <QWidget>

namespace Ui {
class VideoCheckForm;
}

class VideoCheckForm : public QWidget
{
    Q_OBJECT

public:
    explicit VideoCheckForm(QWidget *parent = nullptr);
    ~VideoCheckForm();

private:
    Ui::VideoCheckForm *ui;
};

#endif // VIDEOCHECKFORM_H
