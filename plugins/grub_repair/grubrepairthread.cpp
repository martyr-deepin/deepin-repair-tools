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

    do {
        if (primary_info.first.isEmpty() || primary_info.second.isEmpty())
        {
            qWarning() << "Primary system root not found";
            break;
        }

        const auto &mountPath = primary_info.first;
        const auto &r = m_toolsProxy->execAsChrootAynchronous(primary_info.second, sh, QStringList() << mountPath);

        emit outputPrinted(r.standardOutput);

    } while (false);
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
