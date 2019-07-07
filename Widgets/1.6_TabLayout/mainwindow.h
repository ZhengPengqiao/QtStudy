#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSplitter>
#include "contectframe.h"
#include <QMouseEvent>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();


protected:
    virtual void resizeEvent(QResizeEvent *event) override;
    //拖拽窗口
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

public slots:
    void tabFrame_obclicked(int buttonID);

private:
    bool        m_bDrag;
    QPoint      mouseStartPoint;
    QPoint      windowTopLeftPoint;

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
