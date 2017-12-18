#ifndef TOOLSMODEL_H
#define TOOLSMODEL_H

#include "pluginsloader.h"

#include <QAbstractListModel>
#include <QPointer>

class ToolsModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit ToolsModel(QObject *parent = nullptr);

    enum ToolsRole
    {
        ToolsName = Qt::DisplayRole,
    };

    int rowCount(const QModelIndex &index) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

private:
    QPointer<PluginsLoader> m_pluginLoader;
};

#endif // TOOLSMODEL_H
