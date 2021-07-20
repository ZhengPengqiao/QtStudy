#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include "cupdater.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void button_checkpack_onClick();
    void button_expack_onClick();
    void button_selectfile_onClick();
    void accept();
    void reject();
    void OnProgress(int);

private:
    Ui::MainWindow *ui;

    CUpdater upd;
    QString file_name;
};

#endif // MAINWINDOW_H
