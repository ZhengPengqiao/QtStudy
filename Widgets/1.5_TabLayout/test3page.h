#ifndef TEST3PAGE_H
#define TEST3PAGE_H

#include <QWidget>
#include <QTimer>

namespace Ui {
class Test3Page;
}

class Test3Page : public QWidget
{
    Q_OBJECT

public:
    explicit Test3Page(QWidget *parent = 0);
    ~Test3Page();

    QTimer *timer;
    int lcdNum;

public slots:
    void timerUpDate();

private:
    Ui::Test3Page *ui;
};

#endif // TEST3PAGE_H
