#include "dpkgrepairthread.h"

DPKGRepairThread::DPKGRepairThread(QObject *parent)
    : QThread(parent)
{

}

void DPKGRepairThread::run()
{
    sleep(1);

    emit finished();
}
