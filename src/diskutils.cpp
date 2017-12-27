#include "diskutils.h"

#include <QFile>
#include <QDebug>
#include <QDir>

#include <random>

#include <sys/stat.h>

#define MOUNTS_PATH     "/proc/mounts"
#define BLOCK_DEVS_PATH "/dev/block"

inline bool is_block_device(const QString &path)
{
    struct stat sb;

    return stat(path.toStdString().c_str(), &sb) || S_ISBLK(sb.st_mode);
}

const QString generate_mount_dir()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(10000, 10000000);

    do {
        const QString path(QString("/tmp/mount-%1").arg(dis(gen)));

        if (!QFile(path).exists())
            return path;
    } while(true);

    Q_UNREACHABLE();
}

void mount_block_devices()
{
    QDir block_dir(BLOCK_DEVS_PATH);

    for (const auto &info : block_dir.entryList(QDir::NoDotAndDotDot))
    {
        const QString &path = block_dir.absoluteFilePath(info);
        const QString &mount_point = generate_mount_dir();

        qDebug() << path << mount_point;
    }
}

QList<DiskInfo> list_mounted_devices()
{
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

        mount_info_list << std::move(DiskInfo { info[0], info[1], info[2] });
    }

    return mount_info_list;
}

DiskUtils::DiskUtils(QObject *parent)
    : QObject(parent)

    , m_scannerRunning(true)
{

}

void DiskUtils::initilize()
{
    m_diskInfos = list_mounted_devices();

    emit scanFinished();
}
