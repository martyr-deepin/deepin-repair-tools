#include "password_reset_plugin.h"

PasswordReset::PasswordReset()
{

}

void PasswordReset::init()
{
    Q_ASSERT(m_centralWidget.isNull());
    m_centralWidget = new PasswordResetWidget;
}
