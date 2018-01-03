#include "grub_repair_plugin.h"

GrubRepair::GrubRepair()
{

}

void GrubRepair::init(RepairToolsProxy *toolsProxy)
{
    Q_ASSERT(m_centralWidget.isNull());
    m_toolsProxy = toolsProxy;
    m_centralWidget = new GrubRepairWidget;
    m_centralWidget->setToolsProxy(m_toolsProxy);
}
