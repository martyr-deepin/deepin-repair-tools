#ifndef DPKGREPAIRWIDGET_H
#define DPKGREPAIRWIDGET_H

#include "../../src/repairtoolsproxy.h"

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QTextEdit>

#include <DSpinner>
#include <dlinkbutton.h>

DWIDGET_USE_NAMESPACE

class DPKGRepairWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DPKGRepairWidget(QWidget *parent = nullptr);

    void setToolsProxy(RepairToolsProxy *proxy) { m_toolsProxy = proxy; }

protected:
    void showEvent(QShowEvent *e);

private slots:
    void reset();
    void onRepairClicked();
    void onRepairFinished(const bool success);
    void showDetail();
    void hideDetail();

private:
    RepairToolsProxy *m_toolsProxy;
    QTextEdit *m_output;
    QLabel *m_icon;
    QLabel *m_tips;
    QLabel *m_result;
    DSpinner *m_spinner;
    DLinkButton *m_showDetail;
    DLinkButton *m_hideDetail;
    QPushButton *m_repairButton;
    QPushButton *m_okButton;
};

#endif // DPKGREPAIRWIDGET_H
