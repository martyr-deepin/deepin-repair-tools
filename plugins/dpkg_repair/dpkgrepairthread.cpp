#include "dpkgrepairthread.h"

#include <QDebug>
#include <QProcess>

DPKGRepairThread::DPKGRepairThread(QObject *parent)
    : QThread(parent)
{

}

void DPKGRepairThread::run()
{
    const QString sh = "/usr/lib/deepin-repair-tools/plugins/dpkg-repair/dpkg_repair.sh";

    for (const auto &info : m_toolsProxy->diskInfos())
    {
        if (!info.osName.contains("deepin", Qt::CaseInsensitive))
            continue;

        m_toolsProxy->execAsChrootAynchronous(info.mountPoint, sh);
    }

    emit finished();
}
