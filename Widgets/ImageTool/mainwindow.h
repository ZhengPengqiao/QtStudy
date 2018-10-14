#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStandardItem>
#include <QStandardItemModel>

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
    QStandardItemModel *item_model;
private:
    Ui::MainWindow *ui;
public slots:
    void addItem();
    void delItem();
    void openFile();
    void onPointChange(QRect &rectTmp);

    void onSpinBoxXChange(int value);
    void onSpinBoxYChange(int value);
    void onSpinBoxHeightChange(int value);
    void onSpinBoxWidthChange(int value);
    void onRectItemChange(QList<QRect*> rectList, QList<QString> strList);
};

#endif // MAINWINDOW_H
