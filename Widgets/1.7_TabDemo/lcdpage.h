#ifndef TEST3PAGE_H
#define TEST3PAGE_H

#include <QWidget>
#include <QTimer>

namespace Ui {
class LcdPage;
}

class LcdPage : public QWidget
{
    Q_OBJECT

public:
    explicit LcdPage(QWidget *parent = 0);
    ~LcdPage();

    QTimer *timer;
    int lcdNum;

public slots:
    void timerUpDate();

private:
    Ui::LcdPage *ui;
};

#endif // TEST3PAGE_H
