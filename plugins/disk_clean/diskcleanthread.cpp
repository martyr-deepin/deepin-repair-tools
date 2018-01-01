#include "diskcleanthread.h"

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

        QProcess process;
        connect(&process, &QProcess::readyReadStandardOutput, this, [&] { emit processInfo(process.readAllStandardOutput()); });
        connect(&process, &QProcess::readyReadStandardError, this, [&] { emit processInfo(process.readAllStandardError()); });

        process.start("/bin/sh", QStringList() << sh << p.mountPoint);
        process.waitForFinished(-1);
    }

    emit finished();
}
