#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidget>
#include <QMessageBox>
#include <QFileDialog>

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
     void mousePressEvent(QMouseEvent *e);

private: 
    Ui::MainWindow *ui;
    QFileDialog *fileDialog;

public slots:
    void sendClick();
    void button1_on();
    void button2_on();
    void button3_on();
    void button4_on();
    void addFileNameInit();
    void itemChicked(QListWidgetItem *item);
    void fileDialog_on();
    void showFileName();
};

#endif // MAINWINDOW_H
