#ifndef TOOLSMODEL_H
#define TOOLSMODEL_H

#include "pluginsloader.h"
#include "repairtoolsinterface.h"

#include <QAbstractListModel>
#include <QPointer>

class ToolsModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit ToolsModel(QObject *parent = nullptr);
    ~ToolsModel();

    enum ToolsRole
    {
        ToolsName = Qt::DisplayRole,

        ToolsWidget = Qt::UserRole,
    };

    int rowCount(const QModelIndex &index) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

private slots:
    void onPluginLoaded(RepairToolsInterface *plugin, const QJsonObject &metaData);

private:
    QPointer<PluginsLoader> m_pluginLoader;
    QList<RepairToolsInterface *> m_plugins;
};

#endif // TOOLSMODEL_H
