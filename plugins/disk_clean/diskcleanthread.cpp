#include "diskcleanthread.h"

#include "../../src/repairtoolsproxy.h"

#include <QDebug>
#include <QProcess>

DiskCleanThread::DiskCleanThread(QObject *parent)
    : QThread(parent)
{

}

void DiskCleanThread::run()
{
    const QString sh = "/usr/lib/deepin-repair-tools/plugins/disk-clean/disk_clean.sh";

    for (const auto &p : m_diskList)
    {
        qDebug() << "cleaning:" << p.diskPath << p.mountPoint << p.osName;
        const auto r = m_toolsProxy->execAsChrootAynchronous(p.mountPoint, sh);

        emit processInfo(r.standardOutput);
        emit processInfo(r.standardError);
    }

    emit finished();
}
