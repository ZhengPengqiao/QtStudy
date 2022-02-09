#include "TextItemPlugin.h"
#include "TextItem.h"

#include <QtPlugin>

TextItemPlugin::TextItemPlugin(QObject* parent) : QObject(parent)
{
    m_initialized = false;
}

void TextItemPlugin::initialize(QDesignerFormEditorInterface* /* core */)
{
    if (m_initialized)
        return;

    // Add extension registrations, etc. here

    m_initialized = true;
}

bool TextItemPlugin::isInitialized() const
{
    return m_initialized;
}

QWidget* TextItemPlugin::createWidget(QWidget* parent)
{
    return new TextItem(parent);
}

QString TextItemPlugin::name() const
{
    return QLatin1String("TextItem");
}

QString TextItemPlugin::group() const
{
    return QLatin1String("GroupedControls");
}

QIcon TextItemPlugin::icon() const
{
    return QIcon(QLatin1String(":/icon/TextItem.png"));
}

QString TextItemPlugin::toolTip() const
{
    return QLatin1String("");
}

QString TextItemPlugin::whatsThis() const
{
    return QLatin1String("");
}

bool TextItemPlugin::isContainer() const
{
    return false;
}

QString TextItemPlugin::domXml() const
{
    return QLatin1String("<widget class=\"TextItem\" name=\"textItem\">\n</widget>\n");
}

QString TextItemPlugin::includeFile() const
{
    return QLatin1String("TextItem.h");
}
