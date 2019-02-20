#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QImage>
#include <QScrollBar>
#include <QLabel>
#include <QScrollArea>
#include <QtWidgets>
#include <QPixmap>

class MainWindow : public QMainWindow
{
public:
    MainWindow();
    ~MainWindow();
    void paintEvent(QPaintEvent *event);
    void timerEvent(QTimerEvent *event);

private:
    int x;
    int m_nTimerId;
    int speed;
    QPixmap *pix;
};

#endif
