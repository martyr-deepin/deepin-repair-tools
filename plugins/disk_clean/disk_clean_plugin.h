#ifndef DISK_CLEAN_PLUGIN_H
#define DISK_CLEAN_PLUGIN_H

#include "../../src/repairtoolsinterface.h"
#include "diskcleanwidget.h"

#include <QPointer>
#include <QDebug>

class RepairToolsProxy;
class DiskClean : public QObject, public RepairToolsInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID ModuleInterface_iid FILE "disk-clean.json")
    Q_INTERFACES(RepairToolsInterface)

public:
    explicit DiskClean();

    void init(RepairToolsProxy *toolsProxy);
    QWidget *centralWidget() { return m_centralWidget.data(); }
    const QString name() const { return tr("Disk Clean"); }

private:
    QPointer<DiskCleanWidget> m_centralWidget;
};

#endif // DISK_CLEAN_PLUGIN_H
