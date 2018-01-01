#include "dpkgrepairthread.h"

#include <QDebug>
#include <QProcess>

DPKGRepairThread::DPKGRepairThread(QObject *parent)
    : QThread(parent)
{

}

void DPKGRepairThread::run()
{
    const QString sh = "/usr/lib/deepin-repair-tools/plugins/dpkg_repair/dpkg_repair.sh";

    for (const auto &root : m_rootList)
    {
        qDebug() << Q_FUNC_INFO << "processing:" << root;

        QProcess process;
        connect(&process, &QProcess::readyReadStandardOutput, this, [&] { emit processInfo(process.readAllStandardOutput()); });
        connect(&process, &QProcess::readyReadStandardError, this, [&] { emit processInfo(process.readAllStandardError()); });

        process.start("/bin/sh", QStringList() << sh << root);
        process.waitForFinished(-1);
    }

    emit finished();
}
