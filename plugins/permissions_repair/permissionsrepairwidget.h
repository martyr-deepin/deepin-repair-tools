#ifndef PERMISSIONSREPAIRWIDGET_H
#define PERMISSIONSREPAIRWIDGET_H

#include "../../src/repairtoolsproxy.h"

#include <QWidget>
#include <QPushButton>
#include <QLabel>

class PermissionsRepairWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PermissionsRepairWidget(QWidget *parent = nullptr);

    void setToolsRroxy(RepairToolsProxy *toolsProxy) { m_toolsProxy = toolsProxy; }

protected:
    void showEvent(QShowEvent *e);

private slots:
    void onRepairButtonClicked();
    void onRepairFinished(const bool success);
    void resetUi();

private:
    RepairToolsProxy *m_toolsProxy;
    QLabel *m_icon;
    QLabel *m_tips;
    QLabel *m_status;
    QWidget *m_spinnerPlaceHolder;
    QPushButton *m_okButton;
    QPushButton *m_repairButton;
};

#endif // PERMISSIONSREPAIRWIDGET_H
