#include "fscheckthread.h"
#include "toolspage.h"

#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#define RET_ERROR_DETECTED  4

FSCheckThread::FSCheckThread(QObject *parent)
    : QThread(parent)
{

}

void FSCheckThread::run()
{
    const QString sh = "/usr/lib/deepin-repair-tools/fsck_check.sh";

    for (const QString &device : listUnmountDevices())
    {
        const QString &partion = QString("/dev/%1").arg(device);
        const RunResult &r = execAsChrootSynchronous("/", sh, QStringList() << partion);

        if (!(r.exitCode & RET_ERROR_DETECTED))
            continue;

        emit checkFinished(partion);
        return exit(-1);
    }

    emit checkFinished(QString());
}

QStringList FSCheckThread::listUnmountDevices()
{
    QStringList ret;

    QProcess process;
    process.start("lsblk", QStringList() << "-J");
    process.waitForFinished(-1);

    const auto &mountInfo = process.readAll();
    const auto &devInfo = QJsonDocument::fromJson(mountInfo).object();
    const auto &blockDevs = devInfo.value("blockdevices").toArray();

    for (const auto &dev : blockDevs)
    {
        const auto &children = dev.toObject().value("children").toArray();
        for (const auto &child : children)
        {
            const auto &info = child.toObject();
            if (!info.value("mountpoint").toString().isEmpty())
                continue;

            ret << info.value("name").toString();
        }
    }

    return ret;
}
