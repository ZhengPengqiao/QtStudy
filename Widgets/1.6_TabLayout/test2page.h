#ifndef TEST2PAGE_H
#define TEST2PAGE_H

#include <QWidget>

namespace Ui {
class Test2Page;
}

class Test2Page : public QWidget
{
    Q_OBJECT

public:
    explicit Test2Page(QWidget *parent = 0);
    ~Test2Page();

private:
    Ui::Test2Page *ui;
};

#endif // TEST2PAGE_H
