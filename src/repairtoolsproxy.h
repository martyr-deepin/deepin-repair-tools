#ifndef REPAIRTOOLSPROXY_H
#define REPAIRTOOLSPROXY_H

#include <QList>
#include <QMetaType>

class QProcess;

class UserInfo
{
public:
    QString name;
    QString homeDir;
};
Q_DECLARE_METATYPE(UserInfo)

class DiskInfo
{
public:
    QString diskPath;
    QString mountPoint;
    QString format;
    QString osName;
    QList<UserInfo> userList;
};
Q_DECLARE_METATYPE(DiskInfo)

class RunResult
{
public:
    int exitCode;
    QString standardOutput;
    QString standardError;
};
Q_DECLARE_METATYPE(RunResult)

class RepairToolsProxy
{
public:
    virtual ~RepairToolsProxy() {}

    virtual void registerExclusive(const bool exclusive) = 0;
    virtual const QList<DiskInfo> diskInfos() const = 0;
    virtual const RunResult execAsChrootSynchronous(const QString &root, const QString &script, const QStringList &args = QStringList()) const = 0;
    virtual QProcess* execAsChrootAsynchronous(const QString &root, const QString &script, const QStringList &args = QStringList()) const = 0;
};

#endif // REPAIRTOOLSPROXY_H
