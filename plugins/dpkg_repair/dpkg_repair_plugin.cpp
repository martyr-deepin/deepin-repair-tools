#include "dpkg_repair_plugin.h"

DPKGRepair::DPKGRepair()
{

}

void DPKGRepair::init()
{
    Q_ASSERT(m_centralWidget.isNull());
    m_centralWidget = new DPKGRepairWidget;
}
