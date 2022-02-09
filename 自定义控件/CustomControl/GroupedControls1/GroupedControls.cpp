#include "GroupedControls.h"
#include "ImageItem/ImageItemPlugin.h"
#include "TextItem/TextItemPlugin.h"

GroupedControls::GroupedControls(QObject* parent) : QObject(parent)
{
    m_widgets.append(new ImageItemPlugin(this));
    m_widgets.append(new TextItemPlugin(this));
}

QList<QDesignerCustomWidgetInterface*> GroupedControls::customWidgets() const
{
    return m_widgets;
}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(GroupedControls, GroupedControls)
#endif  // QT_VERSION < 0x050000
