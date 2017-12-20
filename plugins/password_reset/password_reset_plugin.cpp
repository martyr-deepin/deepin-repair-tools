#include "password_reset_plugin.h"

QWidget *PasswordReset::centralWidget()
{
    return nullptr;
}

QString PasswordReset::name()
{
    return tr("Password Reset");
}
