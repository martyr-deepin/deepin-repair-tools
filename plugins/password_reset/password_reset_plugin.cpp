#include "password_reset_plugin.h"

PasswordReset::PasswordReset()
{

}

void PasswordReset::init(RepairToolsProxy *toolsProxy)
{
    Q_ASSERT(m_centralWidget.isNull());
    m_toolsProxy = toolsProxy;
    m_centralWidget = new PasswordResetWidget;
    m_centralWidget->setToolsProxy(toolsProxy);
}
