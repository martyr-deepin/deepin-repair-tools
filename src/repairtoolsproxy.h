#ifndef REPAIRTOOLSPROXY_H
#define REPAIRTOOLSPROXY_H

#include <QList>

class UserInfo
{
public:
    QString name;
    QString homeDir;
};

class DiskInfo
{
public:
    QString diskPath;
    QString mountPoint;
    QString format;
    QString osName;
    QList<UserInfo> userList;
};

class RunResult
{
public:
    int exitCode;
    QString standardOutput;
    QString standardError;
};

class RepairToolsProxy
{
public:
    virtual ~RepairToolsProxy() {}

    virtual const QList<DiskInfo> diskInfos() const = 0;
    virtual const RunResult execAsChrootAynchronous(const QString &root, const QString &script, const QStringList &args = QStringList()) const = 0;
};

#endif // REPAIRTOOLSPROXY_H
