#ifndef GRUBREPAIRWIDGET_H
#define GRUBREPAIRWIDGET_H

#include "../../src/repairtoolsproxy.h"

#include <QWidget>
#include <QPushButton>
#include <QLabel>

#include <dloadingindicator.h>

DWIDGET_USE_NAMESPACE

class GrubRepairWidget : public QWidget
{
    Q_OBJECT

public:
    explicit GrubRepairWidget(QWidget *parent = nullptr);

    void setToolsProxy(RepairToolsProxy *proxy) { m_toolsProxy = proxy; }

private slots:
    void onRepairClicked();
    void onRepairFinished();
    void resetUi();

private:
    RepairToolsProxy *m_toolsProxy;
    QLabel *m_icon;
    QLabel *m_centerTips;
    QLabel *m_statusTips;
    QPushButton *m_repairButton;
    QPushButton *m_okButton;
    DLoadingIndicator *m_loadingIndicator;
};

#endif // GRUBREPAIRWIDGET_H
