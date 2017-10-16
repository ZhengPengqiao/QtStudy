#ifndef CBACKGROUNDWND_H
#define CBACKGROUNDWND_H

#include <QWidget>

class QPaintEvent;
class QPushButton;
class QMouseEvent;

class CBackgroundWnd : public QWidget
{
    Q_OBJECT

public:
    CBackgroundWnd(QWidget *parent = 0);
    ~CBackgroundWnd();

protected:
    virtual void paintEvent(QPaintEvent * event);
    virtual void mousePressEvent(QMouseEvent * event);

public slots:
    void onBtn1Click();

private:
    QPushButton *m_pBtn1;
};

#endif // CBACKGROUNDWND_H
