#ifndef DPKGREPAIRWIDGET_H
#define DPKGREPAIRWIDGET_H

#include "../../src/repairtoolsproxy.h"

#include <QWidget>
#include <QPushButton>
#include <QLabel>

class DPKGRepairWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DPKGRepairWidget(QWidget *parent = nullptr);

    void setToolsProxy(RepairToolsProxy *proxy) { m_toolsProxy = proxy; }

private slots:
    void onRepairClicked();
    void onRepairFinished();

private:
    RepairToolsProxy *m_toolsProxy;
    QLabel *m_icon;
    QPushButton *m_repairButton;
};

#endif // DPKGREPAIRWIDGET_H
