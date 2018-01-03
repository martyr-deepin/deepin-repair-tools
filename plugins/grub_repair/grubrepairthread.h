#ifndef GRUBREPAIRTHREAD_H
#define GRUBREPAIRTHREAD_H

#include "../../src/repairtoolsproxy.h"

#include <QThread>

class GrubRepairThread : public QThread
{
    Q_OBJECT

public:
    explicit GrubRepairThread(QObject *parent = nullptr);

    void setToolsProxy(RepairToolsProxy *proxy) { m_toolsProxy = proxy; }

    void run();

signals:
    void finished();

private:
    QPair<QString, QString> primarySystemRoot();

private:
    RepairToolsProxy *m_toolsProxy;
};

#endif // GRUBREPAIRTHREAD_H
