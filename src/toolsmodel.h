#ifndef TOOLSMODEL_H
#define TOOLSMODEL_H

#include "pluginsloader.h"
#include "repairtoolsinterface.h"

#include <QAbstractListModel>
#include <QPointer>

class ToolsPage;
class ToolsModel : public QAbstractListModel
{
    Q_OBJECT

    friend class ToolsPage;

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

    void onPluginLoaded(RepairToolsInterface *plugin, const QJsonObject &metaData);

signals:
    void pluginsLoadFinished() const;

private slots:
    void initPlugins(RepairToolsProxy *toolsProxy);

private:
    RepairToolsProxy *m_pluginsProxyInter;
    QList<RepairToolsInterface *> m_plugins;
};

#endif // TOOLSMODEL_H
