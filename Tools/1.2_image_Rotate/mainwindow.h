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
    void resizeEvent(QResizeEvent *ev);
public slots:
    void SelectImagea();

private:
    double x;
    int m_nTimerId;
    double speed;
    QPixmap *pix;
    QPixmap showpix;
    int pixIndex;
};

#endif
