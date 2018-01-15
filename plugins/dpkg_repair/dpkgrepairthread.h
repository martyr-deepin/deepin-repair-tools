#ifndef DPKGREPAIRTHREAD_H
#define DPKGREPAIRTHREAD_H

#include "../../src/repairtoolsproxy.h"

#include <QThread>

class DPKGRepairThread : public QThread
{
    Q_OBJECT

public:
    explicit DPKGRepairThread(QObject *parent = nullptr);

    void setToolsProxy(RepairToolsProxy *proxy) { m_toolsProxy = proxy; }
    void appendRoot(const QString &root) { m_rootList << root; }

    void run();

signals:
    void processFinished(const bool success);
    void outputPrinted(const QString &output);

private slots:
    void outputProcess(const QString &output);

private:
    RepairToolsProxy *m_toolsProxy;
    QStringList m_rootList;
};

#endif // DPKGREPAIRTHREAD_H
