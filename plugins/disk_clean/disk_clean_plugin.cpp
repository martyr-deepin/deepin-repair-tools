#include "disk_clean_plugin.h"

DiskClean::DiskClean()
{

}

void DiskClean::init()
{
    Q_ASSERT(m_centralWidget.isNull());
    m_centralWidget = new DiskCleanWidget;
}
