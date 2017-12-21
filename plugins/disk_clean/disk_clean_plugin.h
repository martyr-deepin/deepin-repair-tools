#ifndef DISK_CLEAN_PLUGIN_H
#define DISK_CLEAN_PLUGIN_H

#include "../../src/repairtoolsinterface.h"

class DiskClean : public QObject, public RepairToolsInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID ModuleInterface_iid FILE "disk-clean.json")
    Q_INTERFACES(RepairToolsInterface)

public:
    QWidget *centralWidget();
    QString name();
};

#endif // DISK_CLEAN_PLUGIN_H
