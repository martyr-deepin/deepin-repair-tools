#ifndef GRUBREPAIRWIDGET_H
#define GRUBREPAIRWIDGET_H

#include "../../src/repairtoolsproxy.h"

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QTextEdit>

#include <DSpinner>
#include <dlinkbutton.h>

DWIDGET_USE_NAMESPACE

class GrubRepairWidget : public QWidget
{
    Q_OBJECT

public:
    explicit GrubRepairWidget(QWidget *parent = nullptr);

    void setToolsProxy(RepairToolsProxy *proxy) { m_toolsProxy = proxy; }

protected:
    void showEvent(QShowEvent *e);

private slots:
    void onRepairClicked();
    void onRepairFinished(const bool success);
    void resetUi();
    void showDetail();
    void hideDetail();

private:
    RepairToolsProxy *m_toolsProxy;
    QLabel *m_icon;
    QLabel *m_centerTips;
    QLabel *m_statusTips;
    DSpinner *m_spinner;
    QTextEdit *m_output;
    DLinkButton *m_showDetail;
    DLinkButton *m_hideDetail;
    QPushButton *m_repairButton;
    QPushButton *m_okButton;
};

#endif // GRUBREPAIRWIDGET_H
