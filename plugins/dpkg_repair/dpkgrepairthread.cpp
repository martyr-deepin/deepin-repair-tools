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

    bool failed = false;

    for (const auto &info : m_toolsProxy->diskInfos())
    {
        if (!info.osName.contains("deepin", Qt::CaseInsensitive))
            continue;

        QProcess &process = *m_toolsProxy->execAsChrootAsynchronous(info.mountPoint, sh);

        connect(&process, &QProcess::readyReadStandardOutput, this, [&] { emit outputPrinted(process.readAllStandardOutput()); });
        connect(&process, &QProcess::readyReadStandardError, this, [&] { emit outputPrinted(process.readAllStandardError()); });

        process.start();
        process.waitForFinished(-1);
        process.deleteLater();

        failed |= process.exitCode();
    }

    emit processFinished(!failed);
}
