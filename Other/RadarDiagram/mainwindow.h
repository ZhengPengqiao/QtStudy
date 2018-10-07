#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QTextEdit>
#include "radardiagram.h"
class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QTextEdit *linex;
    QTextEdit *liney;
    QPushButton *sendShow;
    QPushButton *sendCancel;
    RadarDiagrame *rd;
signals:

public slots:
    void sendShow_chick();
    void sendCancel_chick();
};

#endif // MAINWINDOW_H
