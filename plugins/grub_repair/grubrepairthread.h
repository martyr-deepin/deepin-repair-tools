#ifndef DPKGREPAIRTHREAD_H
#define DPKGREPAIRTHREAD_H

#include "../../src/repairtoolsproxy.h"

#include <QThread>

class GrubRepairThread : public QThread
{
    Q_OBJECT

public:
    explicit GrubRepairThread(QObject *parent = nullptr);

    void setToolsProxy(RepairToolsProxy *proxy) { m_toolsProxy = proxy; }
    void appendRoot(const QString &root) { m_rootList << root; }

    void run();

signals:
    void finished();

private:
    RepairToolsProxy *m_toolsProxy;
    QStringList m_rootList;
};

#endif // DPKGREPAIRTHREAD_H
