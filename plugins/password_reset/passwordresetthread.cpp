#include "passwordresetthread.h"

#include <QDebug>

PasswordResetThread::PasswordResetThread(QObject *parent)
    : QThread(parent)
{

}

void PasswordResetThread::run()
{
    qDebug() << Q_FUNC_INFO << m_root << m_user << m_pwd;

    emit finished();
}
