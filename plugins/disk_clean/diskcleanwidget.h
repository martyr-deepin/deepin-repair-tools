#ifndef DISKCLEANWIDGET_H
#define DISKCLEANWIDGET_H

#include "../../src/repairtoolsproxy.h"

#include <QWidget>
#include <QPushButton>
#include <QPointer>
#include <QProcess>

class DiskCleanWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DiskCleanWidget(QWidget *parent = nullptr);

    void setToolsProxy(RepairToolsProxy *proxy) { m_toolsProxy = proxy; }

private slots:
    void cleanStart();
    void cleanCancel();
    void cleanEnd();

private:
    QPointer<QProcess> m_process;
    RepairToolsProxy *m_toolsProxy;
    QPushButton *m_cleanButton;
    QPushButton *m_cancelButton;
    QPushButton *m_okButton;
};

#endif // DISKCLEANWIDGET_H
