#ifndef REPAIRTOOLSINTERFACE_H
#define REPAIRTOOLSINTERFACE_H

#include <QWidget>

class RepairToolsInterface
{
public:
    virtual ~RepairToolsInterface() {}

    virtual QWidget *centralWidget() = 0;
    virtual QString name() = 0;
};

QT_BEGIN_NAMESPACE

#define ModuleInterface_iid "com.deepin.repairtools.RepairToolsInterface"

Q_DECLARE_INTERFACE(RepairToolsInterface, ModuleInterface_iid)
QT_END_NAMESPACE

#endif // REPAIRTOOLSINTERFACE_H
