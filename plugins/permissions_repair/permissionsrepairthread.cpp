#include "permissionsrepairthread.h"

PermissionsRepairThread::PermissionsRepairThread(QObject *parent)
    : QThread(parent)
{

}

void PermissionsRepairThread::run()
{
    const QString sh = "/usr/lib/deepin-repair-tools/plugins/permissions-repair/permissions-repair.sh";

    for (const auto &info : m_toolsProxy->diskInfos())
    {
        if (!info.osName.contains("deepin", Qt::CaseInsensitive))
            continue;

        QStringList users;
        for (const auto user : info.userList)
            users << user.name;

        m_toolsProxy->execAsChrootAynchronous(info.mountPoint, sh, users);
    }

    emit finished();
}
