#include "GroupedControls.h"
#include "GroupedControlsplugin.h"

#include <QtPlugin>

GroupedControlsPlugin::GroupedControlsPlugin(QObject *parent)
    : QObject(parent)
{
    m_initialized = false;
}

void GroupedControlsPlugin::initialize(QDesignerFormEditorInterface * /* core */)
{
    if (m_initialized)
        return;

    // Add extension registrations, etc. here

    m_initialized = true;
}

bool GroupedControlsPlugin::isInitialized() const
{
    return m_initialized;
}

QWidget *GroupedControlsPlugin::createWidget(QWidget *parent)
{
    return new GroupedControls(parent);
}

QString GroupedControlsPlugin::name() const
{
    return QLatin1String("GroupedControls");
}

QString GroupedControlsPlugin::group() const
{
    return QLatin1String("Grouped Controls");
}

QIcon GroupedControlsPlugin::icon() const
{
    return QIcon(QLatin1String(":/GroupedControls.png"));
}

QString GroupedControlsPlugin::toolTip() const
{
    return QLatin1String("");
}

QString GroupedControlsPlugin::whatsThis() const
{
    return QLatin1String("");
}

bool GroupedControlsPlugin::isContainer() const
{
    return false;
}

QString GroupedControlsPlugin::domXml() const
{
    return QLatin1String("<widget class=\"GroupedControls\" name=\"groupedControls\">\n</widget>\n");
}

QString GroupedControlsPlugin::includeFile() const
{
    return QLatin1String("GroupedControls.h");
}
#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(groupedcontrolsplugin, GroupedControlsPlugin)
#endif // QT_VERSION < 0x050000
