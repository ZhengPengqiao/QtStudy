#include "ImageItemPlugin.h"
#include "ImageItem.h"

#include <QtPlugin>

ImageItemPlugin::ImageItemPlugin(QObject* parent) : QObject(parent)
{
    m_initialized = false;
}

void ImageItemPlugin::initialize(QDesignerFormEditorInterface* /* core */)
{
    if (m_initialized)
        return;

    // Add extension registrations, etc. here

    m_initialized = true;
}

bool ImageItemPlugin::isInitialized() const
{
    return m_initialized;
}

QWidget* ImageItemPlugin::createWidget(QWidget* parent)
{
    return new ImageItem(parent);
}

QString ImageItemPlugin::name() const
{
    return QLatin1String("ImageItem");
}

QString ImageItemPlugin::group() const
{
    return QLatin1String("GroupedControls");
}

QIcon ImageItemPlugin::icon() const
{
    return QIcon(QLatin1String(":/icon/ImageItem.png"));
}

QString ImageItemPlugin::toolTip() const
{
    return QLatin1String("");
}

QString ImageItemPlugin::whatsThis() const
{
    return QLatin1String("");
}

bool ImageItemPlugin::isContainer() const
{
    return false;
}

QString ImageItemPlugin::domXml() const
{
    return QLatin1String("<widget class=\"ImageItem\" name=\"imageItem\">\n</widget>\n");
}

QString ImageItemPlugin::includeFile() const
{
    return QLatin1String("ImageItem.h");
}
