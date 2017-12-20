#ifndef PLUGINSLOADER_H
#define PLUGINSLOADER_H

#include <QObject>

class PluginsLoader : public QObject
{
    Q_OBJECT

public:
    explicit PluginsLoader(QObject *parent = nullptr);

public slots:
    void load();

signals:
    void pluginFounded(const QString &path) const;
    void finished() const;
};

#endif // PLUGINSLOADER_H
