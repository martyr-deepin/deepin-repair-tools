#ifndef PASSWORDRESETTHREAD_H
#define PASSWORDRESETTHREAD_H

#include "../../src/repairtoolsproxy.h"

#include <QThread>

class PasswordResetThread : public QThread
{
    Q_OBJECT

public:
    explicit PasswordResetThread(QObject *parent = nullptr);

    void setToolsProxy(RepairToolsProxy *proxy) { m_toolsProxy = proxy; }
    void setTaskInfo(const QString &root, const QString &user, const QString &pwd)
    { m_root = root; m_user = user; m_pwd = pwd; }

    void run();

private:
    RepairToolsProxy *m_toolsProxy;
    QString m_root;
    QString m_user;
    QString m_pwd;
};

#endif // PASSWORDRESETTHREAD_H
