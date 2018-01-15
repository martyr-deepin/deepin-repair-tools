#ifndef FSCHECKTHREAD_H
#define FSCHECKTHREAD_H

#include "diskutils.h"

#include <QThread>
#include <QPointer>

class FSCheckThread : public QThread
{
    Q_OBJECT

public:
    explicit FSCheckThread(QObject *parent = nullptr);

    void setDiskUtils(QPointer<DiskUtils> &utils) { m_diskUtils = utils; }

    void run();

signals:
    void checkFinished(const QString &errorPartion);

private:
    QStringList listUnmountDevices();

private:
    QPointer<DiskUtils> m_diskUtils;
};

#endif // FSCHECKTHREAD_H
