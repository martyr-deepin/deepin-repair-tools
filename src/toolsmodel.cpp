#include "toolsmodel.h"

#include <QTimer>

ToolsModel::ToolsModel(QObject *parent)
    : QAbstractListModel(parent)
    , m_pluginLoader(new PluginsLoader(this))
{
    QTimer::singleShot(1, m_pluginLoader, &PluginsLoader::load);
}

int ToolsModel::rowCount(const QModelIndex &index) const
{
    Q_UNUSED(index);

    return m_plugins.size();
}

QVariant ToolsModel::data(const QModelIndex &index, int role) const
{
    Q_ASSERT(index.isValid());

    RepairToolsInterface *plugin = m_plugins[index.row()];

    switch (role)
    {
    case ToolsName:     return plugin->name();
    default:;
    }

    return QVariant();
}
