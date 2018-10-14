#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "painterwidget.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    PainterWidget *painterwidget;
private:
    Ui::MainWindow *ui;
public slots:
    void addItem();
    void delItem();
    void openFile();
};

#endif // MAINWINDOW_H
