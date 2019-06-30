#ifndef TEST1PAGE_H
#define TEST1PAGE_H

#include <QWidget>

namespace Ui {
class Test1Page;
}

class Test1Page : public QWidget
{
    Q_OBJECT

public:
    explicit Test1Page(QWidget *parent = 0);
    ~Test1Page();

private:
    Ui::Test1Page *ui;
};

#endif // TEST1PAGE_H
