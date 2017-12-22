#ifndef PLUGINSLOADER_H
#define PLUGINSLOADER_H

#include <QObject>

class RepairToolsInterface;
class PluginsLoader : public QObject
{
    Q_OBJECT

public:
    explicit PluginsLoader(QObject *parent = nullptr);

public slots:
    void load();

signals:
    void pluginLoaded(RepairToolsInterface *plugin, const QJsonObject &metaData) const;
    void finished() const;
};

#endif // PLUGINSLOADER_H
