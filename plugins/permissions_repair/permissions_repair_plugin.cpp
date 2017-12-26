#include "permissions_repair_plugin.h"

PermissionsRepair::PermissionsRepair()
{

}

void PermissionsRepair::init()
{
    Q_ASSERT(m_centralWidget.isNull());
    m_centralWidget = new PermissionsRepairWidget;
}
