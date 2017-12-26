#ifndef DPKG_REPAIR_PLUGIN_H
#define DPKG_REPAIR_PLUGIN_H

#include "../../src/repairtoolsinterface.h"
#include "../../src/repairtoolsproxy.h"
#include "permissionsrepairwidget.h"

#include <QPointer>

class PermissionsRepair : public QObject, public RepairToolsInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID ModuleInterface_iid FILE "permissions-repair.json")
    Q_INTERFACES(RepairToolsInterface)

public:
    explicit PermissionsRepair();

    void init(RepairToolsProxy *toolsProxy);
    QWidget *centralWidget() { return m_centralWidget.data(); }
    const QString name() const { return tr("Permissions Repair"); }

private:
    QPointer<PermissionsRepairWidget> m_centralWidget;
};

#endif // DPKG_REPAIR_PLUGIN_H
