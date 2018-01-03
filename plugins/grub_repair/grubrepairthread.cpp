#include "grubrepairthread.h"

#include <QDebug>
#include <QProcess>

GrubRepairThread::GrubRepairThread(QObject *parent)
    : QThread(parent)
{

}

void GrubRepairThread::run()
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
