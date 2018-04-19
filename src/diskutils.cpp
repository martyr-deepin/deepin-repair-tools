#include "diskutils.h"

#include <QFile>
#include <QDebug>
#include <QDir>
#include <QProcess>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QSettings>
#include <QtConcurrent>
#include <QFutureWatcher>

#include <random>

#include <mntent.h>
#include <sys/stat.h>

#define MOUNT_PREFIX    "/tmp/D.R.T./"
#define MOUNTS_PATH     "/proc/mounts"
#define BLOCK_DEVS_PATH "/dev/block"

class OSProberInfo
{
public:
    QString mountPoint;
    QString osName;
};

inline bool is_block_device(const QString &path)
{
    struct stat sb;

    return stat(path.toStdString().c_str(), &sb) || S_ISBLK(sb.st_mode);
}

const QString generate_mount_dir(const QString &devName)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(10000, 10000000);

    const QDir baseDir(MOUNT_PREFIX);
    do {
        const QString mountName = QString("mount-%1-%2").arg(devName).arg(dis(gen));
        if (!baseDir.exists(mountName) && baseDir.mkpath(mountName))
            return baseDir.absoluteFilePath(mountName);
    } while(true);

    Q_UNREACHABLE();
}

QSet<QString> mounted_drives()
{
    QSet<QString> r;

    auto *p = setmntent("/etc/mtab", "r");
    if (!p)
        return r;

    mntent *mnt;
    while ((mnt = getmntent(p)) != nullptr)
    {
        const QString &fs_name = mnt->mnt_fsname;
        if (fs_name.startsWith("/dev/"))
            r << fs_name;
    }

    endmntent(p);
    return r;
}

void mount_block_devices()
{
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
            const QString &mountPoint = info.value("mountpoint").toString();

            QProcess procMount;

            if (!mountPoint.isEmpty())
            {
                procMount.start("mount", QStringList() << "-o" << "remount,rw" << mountPoint);

                qDebug() << "remount" << mountPoint << "as rw";
            }
            else
            {
                const QString &devName = info.value("name").toString();
                const QString &devPath = QString("/dev/%1").arg(devName);
                const QString &dst_point = generate_mount_dir(devName);

                procMount.start("mount", QStringList() << devPath << dst_point);

                qDebug() << "mounting" << devPath << "to" << dst_point;
            }

            // do mount
            procMount.waitForFinished(-1);

            qDebug() << "mount" << procMount.exitCode() << procMount.readAllStandardOutput() << procMount.readAllStandardError();
        }
    }
}

QList<OSProberInfo> list_os_info()
{
    QProcess process;
    process.start("os-prober");
    process.waitForFinished(-1);

    QList<OSProberInfo> r;
    for (const QString &line : process.readAllStandardOutput().split('\n'))
    {
        const QStringList &info = line.split(':');
        if (info.size() != 4)
            continue;

        r << OSProberInfo { info[0], info[1] };
    }

    return r;
}

QString host_info()
{
    QSettings os_info("/etc/lsb-release", QSettings::IniFormat);

    const QString &id = os_info.value("DISTRIB_ID").toString();
    const QString &version = os_info.value("DISTRIB_RELEASE").toString();
    const QString &info = QString("%1 %2").arg(id).arg(version);

    if (!info.isEmpty())
        return info;
    else
        return "known";
}

QList<UserInfo> list_os_user_info(const QString &rootPath, const QString &osName)
{
    QList<UserInfo> r;

    // NOTE(sbw): We only check deepin OS now, but I think we should works for any linux distro.
    if (osName.isEmpty() || !osName.contains("deepin", Qt::CaseInsensitive))
        return r;

    const QString chroot_hook_script = "/usr/lib/deepin-repair-tools/chroot_hook.sh";
    const QString list_users_script = "/usr/lib/deepin-repair-tools/list_user_info.sh";

    QProcess process;
    process.start("/bin/sh", QStringList() << chroot_hook_script << rootPath << list_users_script);
    process.waitForFinished(-1);

    const QString &output = process.readAllStandardOutput();
    for (const QString &line : output.split('\n'))
    {
        const QStringList &info = line.split(' ');
        if (info.size() < 2)
            continue;

        r << UserInfo { info[0], info[1] };
    }

    return r;
}

QList<DiskInfo> list_mounted_devices_info()
{
    QMap<QString, QString> os_names;
    for (const auto &info : list_os_info())
        os_names.insert(info.mountPoint, info.osName);
    mount_block_devices();

    QFile mounts(MOUNTS_PATH);
    if (!mounts.open(QIODevice::ReadOnly))
        return QList<DiskInfo>();

    const auto &mount_list = mounts.readAll().split('\n');
    QList<DiskInfo> mount_info_list;
    mount_info_list.reserve(mount_list.size());

    for (const QString &line : mount_list)
    {
        const QStringList &info = line.split(' ');
        if (info.size() != 6)
            continue;
        if (!QFile(info.first()).exists())
            continue;
        if (!is_block_device(info.first()))
            continue;

        const QString &drivePath = QDir(info[0]).canonicalPath();
        const QString &mountPath = info[1];
        const QString os_name = mountPath == "/" ? host_info() : os_names.value(drivePath);
        const auto &userInfos = list_os_user_info(mountPath, os_name);

        DiskInfo d { drivePath, mountPath, info[2], os_name, userInfos };

        qDebug() << d.diskPath << d.mountPoint << d.format << d.osName;
        mount_info_list << std::move(d);
    }

    return mount_info_list;
}

DiskUtils::DiskUtils(QObject *parent)
    : QObject(parent)

    , m_scannerRunning(true)
{
    qRegisterMetaType<UserInfo>();
    qRegisterMetaType<DiskInfo>();
    qRegisterMetaType<RunResult>();
}

DiskUtils::~DiskUtils()
{
    for (const auto &info : m_diskInfos)
    {
        if (!info.mountPoint.startsWith(MOUNT_PREFIX))
            continue;

        QProcess::startDetached("umount", QStringList() << info.mountPoint);
    }
}

void DiskUtils::initilize()
{
    auto future = QtConcurrent::run(list_mounted_devices_info);
    auto *watcher = new QFutureWatcher<QList<DiskInfo>>();
    watcher->setFuture(future);

    connect(watcher, &QFutureWatcher<QList<DiskInfo>>::finished, this, &DiskUtils::onInitFinished);
}

void DiskUtils::onInitFinished()
{
    QFutureWatcher<QList<DiskInfo>> *watcher = static_cast<QFutureWatcher<QList<DiskInfo>> *>(sender());
    if (!watcher)
        return;

    m_diskInfos = watcher->result();
    watcher->deleteLater();

    emit scanFinished();
}
