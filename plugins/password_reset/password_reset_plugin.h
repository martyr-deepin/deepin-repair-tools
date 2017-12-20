#ifndef PASSWORD_RESET_PLUGIN_H
#define PASSWORD_RESET_PLUGIN_H

#include "../../src/repairtoolsinterface.h"

class PasswordReset : public QObject, public RepairToolsInterface
{
    Q_OBJECT
    Q_INTERFACES(RepairToolsInterface)
    Q_PLUGIN_METADATA(IID "com.deepin.dock.PluginsItemInterface" FILE "password-reset.json")

public:
    QWidget *centralWidget();
    QString name();
};

#endif // PASSWORD_RESET_PLUGIN_H
