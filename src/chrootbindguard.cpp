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

        qDebug() << "bind in" << m_rootPath << bp << procBind.exitCode() << procBind.readAllStandardOutput() << procBind.readAllStandardError();
    }
}

ChrootBindGuard::~ChrootBindGuard()
{
    if (m_rootPath.isEmpty() || m_rootPath == "/")
        return;

    // do unbind
    for (const auto &bp : m_bindList)
    {
        QProcess procBind;
        procBind.start("umount", QStringList() << m_rootPath + bp);
        procBind.waitForFinished(-1);

        qDebug() << "unbind in" << m_rootPath << bp << procBind.exitCode() << procBind.readAllStandardOutput() << procBind.readAllStandardError();
    }
}
