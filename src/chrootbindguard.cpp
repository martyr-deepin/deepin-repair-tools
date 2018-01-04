#include "chrootbindguard.h"

#include <QProcess>
#include <QDebug>

ChrootBindGuard::ChrootBindGuard(const QString &rootPath)
    : m_rootPath(rootPath)
    , m_bindList({ "/proc", "/dev", "/run", "/sys" })
{
    if (m_rootPath.isEmpty() || m_rootPath == "/")
        return;

    // do bind
    for (const auto &bp : m_bindList)
    {
        QProcess procBind;
        procBind.start("mount", QStringList() << "--bind" << "--make-shared" << bp << m_rootPath + bp);
        procBind.waitForFinished(-1);

        qDebug() << "bind" << bp << procBind.exitCode() << procBind.readAllStandardOutput() << procBind.readAllStandardError();
    }
}

ChrootBindGuard::~ChrootBindGuard()
{
    if (m_rootPath.isEmpty() || m_rootPath == "/")
        return;

    // do unbind
    for (auto it(m_bindList.crbegin()); it != m_bindList.crend(); ++it)
        QProcess::startDetached("umount", QStringList() << m_rootPath + *it);
}
