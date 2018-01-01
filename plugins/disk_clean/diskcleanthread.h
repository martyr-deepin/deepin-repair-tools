#ifndef DISKCLEANTHREAD_H
#define DISKCLEANTHREAD_H

#include "../../src/diskutils.h"

#include <QThread>
#include <QList>

class DiskCleanThread : public QThread
{
    Q_OBJECT

public:
    explicit DiskCleanThread(QObject *parent = nullptr);
    void appendDir(const DiskInfo &info) { m_diskList << info; }

    void run();

signals:
    void finished() const;

private:
    QList<DiskInfo> m_diskList;
};

#endif // DISKCLEANTHREAD_H
