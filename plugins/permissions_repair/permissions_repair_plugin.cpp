#include "permissions_repair_plugin.h"

PermissionsRepair::PermissionsRepair()
{

}

void PermissionsRepair::init(RepairToolsProxy *toolsProxy)
{
    Q_ASSERT(m_centralWidget.isNull());
    m_toolsProxy = toolsProxy;
    m_centralWidget = new PermissionsRepairWidget;
    m_centralWidget->setToolsRroxy(toolsProxy);
}
