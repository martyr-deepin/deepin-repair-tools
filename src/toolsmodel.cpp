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

    return 0;
}

QVariant ToolsModel::data(const QModelIndex &index, int role) const
{
    switch (role)
    {
    case ToolsName:     return QString::number(index.row());
    default:;
    }

    return QVariant();
}
