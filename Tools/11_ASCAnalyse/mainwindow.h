#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStringList>
#include <QStandardItemModel>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void setupPlot(int lineNum, QVector<double> times, QVector<double> values);

public slots:
    void pushButton_UpdateFile_OnCliecked();
    void pushButton_UpdateSignal_OnCliecked();
    void pushButton_AddSignal_OnCliecked();
    void pushButton_DelSignal_OnCliecked();
    void wheelEvent(QWheelEvent *event);

private:
    Ui::MainWindow *ui;
    QStringList datalist;
    QStandardItemModel* model;
};

#endif // MAINWINDOW_H
