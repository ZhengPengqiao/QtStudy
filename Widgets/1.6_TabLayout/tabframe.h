#ifndef TABFRAME_H
#define TABFRAME_H

#include <QWidget>

namespace Ui {
class TabFrame;
}

class TabFrame : public QWidget
{
    Q_OBJECT

public:
    explicit TabFrame(QWidget *parent = 0);
    ~TabFrame();

    enum BUTTONID {
        BUTTON_EXIT=0,
        BUTTON_TEST1=1,
        BUTTON_TEST2=2,
        BUTTON_TEST3=3,
    };


signals:
    void button_clicked(int buttonID);

public slots:
    void button_exit_onClick();
    void button_test1_onClick();
    void button_test2_onClick();
    void button_test3_onClick();

private:
    Ui::TabFrame *ui;
};

#endif // TABFRAME_H
