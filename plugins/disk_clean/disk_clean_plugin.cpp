#include "disk_clean_plugin.h"

DiskClean::DiskClean()
{

}

void DiskClean::init(RepairToolsProxy *toolsProxy)
{
    Q_ASSERT(m_centralWidget.isNull());
    m_toolsProxy = toolsProxy;
    m_centralWidget = new DiskCleanWidget;
    m_centralWidget->setToolsProxy(m_toolsProxy);
}
