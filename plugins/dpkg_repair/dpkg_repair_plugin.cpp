#include "dpkg_repair_plugin.h"

DPKGRepair::DPKGRepair()
{

}

void DPKGRepair::init(RepairToolsProxy *toolsProxy)
{
    Q_ASSERT(m_centralWidget.isNull());
    m_toolsProxy = toolsProxy;
    m_centralWidget = new DPKGRepairWidget;
    m_centralWidget->setToolsProxy(m_toolsProxy);
}
