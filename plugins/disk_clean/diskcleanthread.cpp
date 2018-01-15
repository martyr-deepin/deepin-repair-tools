#include "diskcleanthread.h"

#include "../../src/repairtoolsproxy.h"

#include <QDebug>
#include <QProcess>
#include <QRegularExpression>
#include <QElapsedTimer>

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

    QElapsedTimer elapsedTimer;
    elapsedTimer.start();

    for (const auto &p : m_diskList)
    {
        if (p.osName.isEmpty() || !p.osName.contains("deepin", Qt::CaseInsensitive))
            continue;
        emit processDisk(p.diskPath);
        qDebug() << "cleaning:" << p.diskPath << p.mountPoint << p.osName;

        const auto r = m_toolsProxy->execAsChrootSynchronous(p.mountPoint, sh);

        for (const QString &line : r.standardOutput.split('\n'))
            if (line.startsWith("DiskClean:"))
                recordClearedSize(line);

        emit processInfo(r.standardOutput);
        emit processInfo(r.standardError);
    }

    const int msec = elapsedTimer.elapsed();
    if (m_totalClearedSize && msec < 2 * 1000)
        msleep(2 * 1000 - msec);

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
