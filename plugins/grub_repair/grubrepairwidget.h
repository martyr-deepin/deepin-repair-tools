#ifndef GRUBREPAIRWIDGET_H
#define GRUBREPAIRWIDGET_H

#include "../../src/repairtoolsproxy.h"

#include <QWidget>
#include <QPushButton>

class GrubRepairWidget : public QWidget
{
    Q_OBJECT

public:
    explicit GrubRepairWidget(QWidget *parent = nullptr);

    void setToolsProxy(RepairToolsProxy *proxy) { m_toolsProxy = proxy; }

private slots:
    void onRepairClicked();
    void onRepairFinished();

private:
    QPushButton *m_repairButton;
    RepairToolsProxy *m_toolsProxy;
};

#endif // GRUBREPAIRWIDGET_H
