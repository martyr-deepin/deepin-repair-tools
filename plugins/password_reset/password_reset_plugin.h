#ifndef PASSWORD_RESET_PLUGIN_H
#define PASSWORD_RESET_PLUGIN_H

#include "../../src/repairtoolsinterface.h"
#include "../../src/repairtoolsproxy.h"
#include "passwordresetwidget.h"

#include <QPointer>

class PasswordReset : public QObject, public RepairToolsInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID ModuleInterface_iid FILE "password-reset.json")
    Q_INTERFACES(RepairToolsInterface)

public:
    explicit PasswordReset();

    void init(RepairToolsProxy *toolsProxy);
    QWidget *centralWidget() { return m_centralWidget.data(); }
    const QString name() const { return tr("Password Reset"); }

private:
    QPointer<PasswordResetWidget> m_centralWidget;
};

#endif // PASSWORD_RESET_PLUGIN_H
