#include "diskcleanthread.h"

#include "../../src/repairtoolsproxy.h"

#include <QDebug>
#include <QProcess>
#include <QRegularExpression>

unsigned long sizeUnit(const QString &unit)
{
    if (!unit.compare("b", Qt::CaseInsensitive))
        return 1ul;
    if (!unit.compare("k", Qt::CaseInsensitive))
        return 1024ul;
    if (!unit.compare("m", Qt::CaseInsensitive))
        return 1024ul * 1024;
    if (!unit.compare("g", Qt::CaseInsensitive))
        return 1024ul * 1024 * 1024;

    // t
    return 1024ul * 1024 * 1024 * 1024;
}

DiskCleanThread::DiskCleanThread(QObject *parent)
    : QThread(parent)

    , m_totalClearedSize(0)
{

}

void DiskCleanThread::run()
{
    const QString sh = "/usr/lib/deepin-repair-tools/plugins/disk-clean/disk_clean.sh";

    for (const auto &p : m_diskList)
    {
        if (p.osName.isEmpty() || !p.osName.contains("deepin", Qt::CaseInsensitive))
            continue;
        qDebug() << "cleaning:" << p.diskPath << p.mountPoint << p.osName;

        const auto r = m_toolsProxy->execAsChrootSynchronous(p.mountPoint, sh);

        for (const QString &line : r.standardOutput.split('\n'))
            if (line.startsWith("DiskClean:"))
                recordClearedSize(line);

        emit processInfo(r.standardOutput);
        emit processInfo(r.standardError);
    }

    emit processDone(m_totalClearedSize);
}

void DiskCleanThread::recordClearedSize(const QString &log)
{
    QRegularExpression sizeExp("([\\.\\d]+)([kKbBmMgGtT])");
    const auto match = sizeExp.match(log);
    if (!match.isValid())
        return;

    const double nums = match.captured(1).toDouble();
    const int mul = sizeUnit(match.captured(2));

    m_totalClearedSize += nums * mul;
}
