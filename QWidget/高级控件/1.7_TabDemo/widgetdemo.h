#ifndef WIDGETDEMO_H
#define WIDGETDEMO_H

#include <QWidget>
#include <QBasicTimer>

namespace Ui {
class WidgetDemo;
}

class WidgetDemo : public QWidget
{
    Q_OBJECT

public:
    explicit WidgetDemo(QWidget *parent = 0);
    ~WidgetDemo();
    void timerEvent(QTimerEvent *e);

    QBasicTimer timer;

    int progress1;
    int progress2;

private:
    Ui::WidgetDemo *ui;
};

#endif // WIDGETDEMO_H
