#include "fscheckthread.h"
#include "toolspage.h"

#include <QDebug>

FSCheckThread::FSCheckThread(QObject *parent)
    : QThread(parent)
{

}

void FSCheckThread::run()
{
    const QString sh = "/usr/lib/deepin-repair-tools/fsck.sh";

    const auto r = execAsChrootAynchronous("/", sh, QStringList());

    qDebug() << r.standardOutput << r.standardError;
}
