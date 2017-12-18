#ifndef PLUGINSLOADER_H
#define PLUGINSLOADER_H

#include <QObject>

class PluginsLoader : public QObject
{
    Q_OBJECT

public:
    explicit PluginsLoader(QObject *parent = nullptr);
};

#endif // PLUGINSLOADER_H
