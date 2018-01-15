#ifndef GRUBREPAIRTHREAD_H
#define GRUBREPAIRTHREAD_H

#include "../../src/repairtoolsproxy.h"

#include <QThread>

class GrubRepairThread : public QThread
{
    Q_OBJECT

public:
    explicit GrubRepairThread(QObject *parent = nullptr);

    void setToolsProxy(RepairToolsProxy *proxy) { m_toolsProxy = proxy; }

    void run();

signals:
    void commandFinished(const bool success) const;
    void outputPrinted(const QString &output) const;

private slots:
    void processOutput(const QString &output);

private:
    QPair<QString, QString> primarySystemRoot();

private:
    RepairToolsProxy *m_toolsProxy;
};

#endif // GRUBREPAIRTHREAD_H
