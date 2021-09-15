
#ifndef ECHOPLUGIN_H
#define ECHOPLUGIN_H

#include <QObject>
#include <QtPlugin>
#include <QPluginLoader>
#include "plugininterface.h"

#define EchoInterface_IID "Examples.Plugin.PluginInterface"    // 定义接口的IID

class PluginDemo : public QObject, public PluginInterface
{
    Q_OBJECT
    Q_INTERFACES(PluginInterface)

    // 注册插件接口: 第一个参数为插件的名称，第二个参数为插件类
    #if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
        Q_PLUGIN_METADATA(IID PluginInterface_IID)
    #else
        Q_EXPORT_PLUGIN2(PluginDemo, PluginDemo);
    #endif

public:
    explicit PluginDemo(QObject *parent = 0);
    QString getPluginInterFaceInfo();

};

#endif // ECHOPLUGIN_H
