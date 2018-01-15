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
    void setCleanInfo(const DiskInfo &info) { m_cleanInfo = info; }
    void setToolsProxy(RepairToolsProxy *proxy) { m_toolsProxy = proxy; }

    void run();

signals:
    void processDisk(const QString &dir) const;
    void processDone(const quint64 cleared) const;
    void processInfo(const QString &info) const;

private:
    void recordClearedSize(const QString &log);

private:
    quint64 m_totalClearedSize;
    RepairToolsProxy *m_toolsProxy;
    DiskInfo m_cleanInfo;
};

#endif // DISKCLEANTHREAD_H
