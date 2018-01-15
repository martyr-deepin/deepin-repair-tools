#include "fsrepairthread.h"

#include "toolspage.h"

#define RET_NOERROR         0
#define RET_REPAIR_SUCCESS  1

FSRepairThread::FSRepairThread(QObject *parent)
    : QThread(parent)
{

}

void FSRepairThread::run()
{
    const QString sh = "/usr/lib/deepin-repair-tools/fsck_repair.sh";

    const RunResult &r = execAsChrootSynchronous("/", sh, QStringList() << m_partion);

    emit repairFinished(r.exitCode == RET_NOERROR || r.exitCode & RET_REPAIR_SUCCESS);
}
