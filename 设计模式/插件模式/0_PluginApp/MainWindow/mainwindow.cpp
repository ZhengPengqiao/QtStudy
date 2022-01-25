#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QtGui>
#include <QMessageBox>
#include <QLineEdit>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Echo Plugin Example");
    pluginInterface = nullptr;

    if (!loadPlugin())
    {
        QMessageBox::information(this, "Error", "Could not load the plugin");
    }
}

MainWindow::~MainWindow()
{
    delete ui;
    if (!unloadPlugin())
    {
        QMessageBox::information(this, "Error", "Could not unload the plugin");
    }
}


void MainWindow::pushButton_getInfo_onClick()
{
    ui->lineEdit_info->setText(pluginInterface->getPluginInterFaceInfo());
}

bool MainWindow::loadPlugin()
{
    bool ret = true;
    //获取当前应用程序所在路径
    QDir pluginsDir(qApp->applicationDirPath());
#if defined(Q_OS_WIN)
    if (pluginsDir.dirName().toLower() == "debug" || pluginsDir.dirName().toLower() == "release")
        pluginsDir.cdUp();
#elif defined(Q_OS_MAC)
    if (pluginsDir.dirName() == "MacOS")
    {
        pluginsDir.cdUp();
        pluginsDir.cdUp();
        pluginsDir.cdUp();
    }
#elif defined(Q_OS_LINUX)
    pluginsDir.cdUp();
#endif

    //切换到插件目录
    pluginsDir.cd("plugins");

    //遍历plugins目录下所有文件
    foreach (QString fileName, pluginsDir.entryList(QDir::Files))
    {
        qDebug() << "path:" << pluginsDir.absoluteFilePath(fileName);
        // QPluginLoader类提供了一个函数loader来加载插件的实例，如果动态链接库不是一个插件，或者编译的Qt的版本库不对，返回的QObject对象的指针为空。
        pluginLoader.setFileName(pluginsDir.absoluteFilePath(fileName));

        QObject *plugin = pluginLoader.instance();
        if (plugin)
        {
            QString pluginName = plugin->metaObject()->className();
            qDebug() << "load plugin Info:" << pluginLoader.metaData();

            //对插件初始化
            pluginInterface = qobject_cast<PluginInterface *>(plugin);
            if (pluginInterface)
            {
                ret =  true;
            }
            else
            {
                ret = false;
                qDebug() << "plugin:" << pluginLoader.metaData() << " can not pluginInterface";
            }
        }
        else
        {
            ret = false;
            qDebug() << "plugin:" <<  pluginLoader.metaData() << " Not Plugin";
        }
    }
    return ret;
}



bool MainWindow::unloadPlugin()
{
    bool ret = true;
    ret = pluginLoader.unload();
    if( ret )
    {
        qDebug() << "fileName:" << pluginLoader.metaData() << " unload success";
    }
    else
    {
        qDebug() << "fileName:" << pluginLoader.metaData() << " unload error";
    }
    return ret;
}
