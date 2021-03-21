#ifndef PLUGININTERFACE_H
#define PLUGININTERFACE_H

#include <QString>

//定义接口
class PluginInterface
{
public:
    virtual ~PluginInterface() {}
    virtual QString getPluginInterFaceInfo() = 0;
};


QT_BEGIN_NAMESPACE
#define PluginInterface_IID "Examples.Plugin.PluginInterface"    // 定义接口的IID
Q_DECLARE_INTERFACE(PluginInterface, PluginInterface_IID)
QT_END_NAMESPACE

#endif // PLUGININTERFACE_H
