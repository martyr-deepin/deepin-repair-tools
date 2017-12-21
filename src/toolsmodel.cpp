#include "toolsmodel.h"

#include <QTimer>
#include <QDebug>

ToolsModel::ToolsModel(QObject *parent)
    : QAbstractListModel(parent)
    , m_pluginLoader(new PluginsLoader(this))
{
    connect(m_pluginLoader, &PluginsLoader::pluginLoaded, this, &ToolsModel::onPluginLoaded);

    QTimer::singleShot(1, m_pluginLoader, &PluginsLoader::load);
}

ToolsModel::~ToolsModel()
{
    qDeleteAll(m_plugins);
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
    case ToolsName:         return plugin->name();
    case ToolsWidget:       return QVariant::fromValue(plugin->centralWidget());
    default:;
    }

    return QVariant();
}

void ToolsModel::onPluginLoaded(RepairToolsInterface *plugin)
{
    qDebug() << "plugin got:" << plugin->name();

    plugin->init();

    const int pluginCount = m_plugins.size();
    beginInsertRows(QModelIndex(), pluginCount, pluginCount);
    m_plugins << plugin;
    endInsertRows();
}
