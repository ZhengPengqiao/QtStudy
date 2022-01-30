#ifndef GROUPEDCONTROLS_H
#define GROUPEDCONTROLS_H

#include <QWidget>

namespace Ui {
class GroupedControls;
}

class GroupedControls : public QWidget
{
    Q_OBJECT

public:
    explicit GroupedControls(QWidget *parent = nullptr);
    ~GroupedControls();

private:
    Ui::GroupedControls *ui;
};

#endif // GROUPEDCONTROLS_H
