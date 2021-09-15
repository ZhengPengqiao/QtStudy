#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPluginLoader>
#include <QString>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QGridLayout>
#include "plugininterface.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void createGUI();
    //加载插件
    bool loadPlugin();
    bool unloadPlugin();

    QPluginLoader pluginLoader;
    PluginInterface *pluginInterface;
public slots:
    void pushButton_getInfo_onClick();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
