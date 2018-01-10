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

        const auto &r = m_toolsProxy->execAsChrootSynchronous(info.mountPoint, sh);

        emit outputPrinted(r.standardOutput);
    }
}
