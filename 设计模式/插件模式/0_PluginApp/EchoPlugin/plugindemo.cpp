#include "plugindemo.h"
#include <QString>
#include <QDebug>

PluginDemo::PluginDemo(QObject *parent) :
    QObject(parent)
{
}

QString PluginDemo::getPluginInterFaceInfo()
{
    return "Hello World";
}

