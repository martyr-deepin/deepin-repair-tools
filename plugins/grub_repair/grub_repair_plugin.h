#ifndef GRUB_REPAIR_PLUGIN_H
#define GRUB_REPAIR_PLUGIN_H

#include "../../src/repairtoolsinterface.h"
#include "../../src/repairtoolsproxy.h"
#include "grubrepairwidget.h"

#include <QPointer>

class GrubRepair : public QObject, public RepairToolsInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID ModuleInterface_iid FILE "grub-repair.json")
    Q_INTERFACES(RepairToolsInterface)

public:
    explicit GrubRepair();

    void init(RepairToolsProxy *toolsProxy);
    QWidget *centralWidget() { return m_centralWidget.data(); }
    const QString name() const { return tr("Boot Repair"); }

private:
    QPointer<GrubRepairWidget> m_centralWidget;
};

#endif // GRUB_REPAIR_PLUGIN_H
