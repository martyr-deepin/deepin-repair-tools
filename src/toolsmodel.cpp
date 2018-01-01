#include "toolsmodel.h"

#include <QTimer>
#include <QDebug>
#include <QJsonObject>

ToolsModel::ToolsModel(QObject *parent)
    : QAbstractListModel(parent)
{
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

void ToolsModel::onPluginLoaded(RepairToolsInterface *plugin, const QJsonObject &metaData)
{
    const QString &uuid = metaData.value("MetaData").toObject().value("uuid").toString();
//    qDebug() << "plugin got:" << plugin->name() << uuid;

    // preset order list
    static const QStringList pluginsOrderList = {
        "1c13875d-2b33-43ee-9629-e7945c3c729f",     // disk_clean
        "2934612b-31ee-40a3-a1ab-6ec70adae478",     // dpkg_repair
        "037a8825-fab1-4dd0-9076-1c8268123af4",     // grub_repair
        "f5cc897b-dffd-44d5-9a4b-88c045d0bf15",     // permissions_repair
        "3a84c482-343b-4b7f-bbbc-41fc9b15234f",     // password_reset
    };
    static QSet<QString> loadedPlugins;

    Q_ASSERT(!loadedPlugins.contains(uuid));

    // find insert position
    int insert_index = 0;
    for (const auto &id : pluginsOrderList)
    {
        if (uuid == id)
            break;
        if (loadedPlugins.contains(id))
            ++insert_index;
    }

    // append to loaded list
    loadedPlugins << uuid;

    // init plugin before insert
    plugin->init(m_pluginsProxyInter);
    beginInsertRows(QModelIndex(), insert_index, insert_index);
    m_plugins.insert(insert_index, plugin);
    endInsertRows();
}

void ToolsModel::initPlugins(RepairToolsProxy *toolsProxy)
{
    m_pluginsProxyInter = toolsProxy;

    PluginsLoader *ploader = new PluginsLoader;

    connect(ploader, &PluginsLoader::pluginLoaded, this, &ToolsModel::onPluginLoaded);
    connect(ploader, &PluginsLoader::finished, this, &ToolsModel::pluginsLoadFinished);
    connect(ploader, &PluginsLoader::finished, ploader, &PluginsLoader::deleteLater);

    QTimer::singleShot(1, ploader, &PluginsLoader::load);
}
