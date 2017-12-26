#ifndef DPKG_REPAIR_PLUGIN_H
#define DPKG_REPAIR_PLUGIN_H

#include "../../src/repairtoolsinterface.h"
#include "../../src/repairtoolsproxy.h"
#include "dpkgrepairwidget.h"

#include <QPointer>

class DPKGRepair : public QObject, public RepairToolsInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID ModuleInterface_iid FILE "dpkg-repair.json")
    Q_INTERFACES(RepairToolsInterface)

public:
    explicit DPKGRepair();

    void init(RepairToolsProxy *toolsProxy);
    QWidget *centralWidget() { return m_centralWidget.data(); }
    const QString name() const { return tr("DPKG Repair"); }

private:
    QPointer<DPKGRepairWidget> m_centralWidget;
};

#endif // DPKG_REPAIR_PLUGIN_H
