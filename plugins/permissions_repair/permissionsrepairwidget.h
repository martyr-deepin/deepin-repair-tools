#ifndef PERMISSIONSREPAIRWIDGET_H
#define PERMISSIONSREPAIRWIDGET_H

#include "../../src/repairtoolsproxy.h"

#include <QWidget>
#include <QPushButton>

class PermissionsRepairWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PermissionsRepairWidget(QWidget *parent = nullptr);

    void setToolsRroxy(RepairToolsProxy *toolsProxy) { m_toolsProxy = toolsProxy; }

private slots:
    void onRepairButtonClicked();
    void onRepairFinished();

private:
    RepairToolsProxy *m_toolsProxy;
    QPushButton *m_repairButton;
};

#endif // PERMISSIONSREPAIRWIDGET_H
