#include "pluginsloader.h"

#include <QDebug>
#include <QDir>
#include <QLibrary>

PluginsLoader::PluginsLoader(QObject *parent)
    : QObject(parent)
{

}

void PluginsLoader::load()
{
#ifdef QT_DEBUG
    const QDir pluginsDir("plugins");
#else
    const QDir pluginsDir("../lib/deepin-repair-tools/plugins");
#endif
    const QStringList plugins = pluginsDir.entryList(QDir::Files);

    for (const QString &file : plugins)
    {
        if (!QLibrary::isLibrary(file))
            continue;

        qDebug() << "pluginFounded: " << file;

        emit pluginFounded(pluginsDir.absoluteFilePath(file));
    }

    emit finished();
}
