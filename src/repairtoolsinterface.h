#ifndef REPAIRTOOLSINTERFACE_H
#define REPAIRTOOLSINTERFACE_H

#include <QWidget>

class RepairToolsProxy;
class RepairToolsInterface
{
public:
    virtual ~RepairToolsInterface() {}

    virtual void init(RepairToolsProxy *toolsProxy) = 0;
    virtual QWidget *centralWidget() = 0;
    virtual const QString name() const = 0;

protected:
    RepairToolsProxy *m_toolsProxy;
};

QT_BEGIN_NAMESPACE

#define ModuleInterface_iid "com.deepin.repairtools.RepairToolsInterface"
Q_DECLARE_INTERFACE(RepairToolsInterface, ModuleInterface_iid)

QT_END_NAMESPACE

#endif // REPAIRTOOLSINTERFACE_H
