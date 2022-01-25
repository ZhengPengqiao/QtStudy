#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QImage>
#include <QScrollBar>
#include <QLabel>
#include <QScrollArea>
#include <QtWidgets>
class MainWindow : public QMainWindow
{
public:
    MainWindow();
    ~MainWindow();
    void paintEvent(QPaintEvent *event);
    void timerEvent(QTimerEvent *event);

private:
    QLabel *imageLabel;
    int x;
    int m_nTimerId;
    int rectSize;
    int speed;
};

#endif
