#ifndef REPAIRTOOLSPROXY_H
#define REPAIRTOOLSPROXY_H

#include <QList>

class DiskInfo
{
public:
    QString diskPath;
    QString mountPoint;
    QString format;
};

class RepairToolsProxy
{
public:
    virtual ~RepairToolsProxy() {}

    virtual void ping() = 0;
    virtual const QList<DiskInfo> diskInfos() const = 0;
};

#endif // REPAIRTOOLSPROXY_H
