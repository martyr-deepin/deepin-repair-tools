#ifndef PASSWORD_RESET_PLUGIN_H
#define PASSWORD_RESET_PLUGIN_H

#include "../../src/repairtoolsinterface.h"

class PasswordReset : public QObject, public RepairToolsInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID ModuleInterface_iid FILE "password-reset.json")
    Q_INTERFACES(RepairToolsInterface)

public:
    QWidget *centralWidget();
    QString name();
};

#endif // PASSWORD_RESET_PLUGIN_H
