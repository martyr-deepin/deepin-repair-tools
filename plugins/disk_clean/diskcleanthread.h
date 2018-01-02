#ifndef DISKCLEANTHREAD_H
#define DISKCLEANTHREAD_H

#include "../../src/repairtoolsproxy.h"

#include <QThread>
#include <QList>

class DiskCleanThread : public QThread
{
    Q_OBJECT

public:
    explicit DiskCleanThread(QObject *parent = nullptr);
    void appendDir(const DiskInfo &info) { m_diskList << info; }
    void setToolsProxy(RepairToolsProxy *proxy) { m_toolsProxy = proxy; }

    void run();

signals:
    void finished() const;
    void processInfo(const QString &info) const;

private:
    RepairToolsProxy *m_toolsProxy;
    QList<DiskInfo> m_diskList;
};

#endif // DISKCLEANTHREAD_H
