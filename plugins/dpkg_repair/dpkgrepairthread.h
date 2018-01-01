#ifndef DPKGREPAIRTHREAD_H
#define DPKGREPAIRTHREAD_H

#include <QThread>

class DPKGRepairThread : public QThread
{
    Q_OBJECT

public:
    explicit DPKGRepairThread(QObject *parent = nullptr);

    void appendRoot(const QString &root) { m_rootList << root; }

    void run();

signals:
    void finished();
    void processInfo(const QString &info) const;

private:
    QStringList m_rootList;
};

#endif // DPKGREPAIRTHREAD_H
