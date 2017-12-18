#include "pluginsloader.h"

#include <QDebug>

PluginsLoader::PluginsLoader(QObject *parent)
    : QObject(parent)
{

}

void PluginsLoader::load()
{
    qDebug() << Q_FUNC_INFO;
}
