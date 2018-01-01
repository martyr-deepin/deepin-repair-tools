#include "diskcleanthread.h"

#include <QDebug>

DiskCleanThread::DiskCleanThread(QObject *parent)
    : QThread(parent)
{

}

void DiskCleanThread::run()
{
    for (const auto &p : m_diskList)
    {
        qDebug() << "cleaning:" << p.diskPath << p.mountPoint << p.osName;
        sleep(1);
    }

    emit finished();
}
