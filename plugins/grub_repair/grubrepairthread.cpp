#include "grubrepairthread.h"

#include <QDebug>
#include <QProcess>

QString partition_disk(QString partition)
{
    while (!partition.isEmpty())
    {
        if (partition[partition.size() - 1].isDigit())
            partition = partition.left(partition.size() - 1);
        else
            break;
    }

    return partition;
}

GrubRepairThread::GrubRepairThread(QObject *parent)
    : QThread(parent)
{

}

void GrubRepairThread::run()
{
    const auto &primary_info = primarySystemRoot();
    const QString sh = "/usr/lib/deepin-repair-tools/plugins/grub-repair/grub-repair.sh";

    bool failed = false;

    do {
        if (primary_info.first.isEmpty() || primary_info.second.isEmpty())
        {
            qWarning() << "Primary system root not found";
            break;
        }

        const auto &mountPath = primary_info.first;
        QProcess &process = *m_toolsProxy->execAsChrootAsynchronous(primary_info.second, sh, QStringList() << mountPath);

        connect(&process, &QProcess::readyReadStandardOutput, this, [&] { processOutput(process.readAllStandardOutput()); });
        connect(&process, &QProcess::readyReadStandardError, this, [&] { processOutput(process.readAllStandardError()); });

        process.start();
        process.waitForFinished(-1);
        process.deleteLater();

        failed |= process.exitCode();
    } while (false);

    emit commandFinished(!failed);
}

void GrubRepairThread::processOutput(const QString &output)
{
    const auto trimmed = output.trimmed();
    if (!trimmed.isEmpty())
        emit outputPrinted(trimmed);
}

QPair<QString, QString> GrubRepairThread::primarySystemRoot()
{
    for (const auto &disk : m_toolsProxy->diskInfos())
    {
        if (!disk.osName.contains("deepin", Qt::CaseInsensitive))
            continue;

        return QPair<QString, QString>(partition_disk(disk.diskPath), disk.mountPoint);
    }

    return QPair<QString, QString>();
}
