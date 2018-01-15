#ifndef FSREPAIRTHREAD_H
#define FSREPAIRTHREAD_H

#include <QThread>

class FSRepairThread : public QThread
{
    Q_OBJECT

public:
    explicit FSRepairThread(QObject *parent = nullptr);

    void setRepairPartion(const QString &partion) { m_partion = partion; }

    void run();

signals:
    void repairFinished(bool success) const;

private:
    QString m_partion;
};

#endif // FSREPAIRTHREAD_H
