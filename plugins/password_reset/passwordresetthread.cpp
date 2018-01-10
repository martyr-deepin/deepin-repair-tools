#include "passwordresetthread.h"

#include <QDebug>

PasswordResetThread::PasswordResetThread(QObject *parent)
    : QThread(parent)
{

}

void PasswordResetThread::run()
{
    const QString sh = "/usr/lib/deepin-repair-tools/plugins/password-reset/password_reset.sh";

    qDebug() << Q_FUNC_INFO << m_root << m_user << m_pwd;

    m_toolsProxy->execAsChrootSynchronous(m_root, sh, QStringList() << m_user << m_pwd);
}
