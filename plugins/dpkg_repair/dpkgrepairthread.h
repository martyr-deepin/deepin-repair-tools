#ifndef DPKGREPAIRTHREAD_H
#define DPKGREPAIRTHREAD_H

#include <QThread>

class DPKGRepairThread : public QThread
{
    Q_OBJECT

public:
    explicit DPKGRepairThread(QObject *parent = nullptr);

    void run();

signals:
    void finished();
    void processInfo(const QString &info) const;
};

#endif // DPKGREPAIRTHREAD_H
