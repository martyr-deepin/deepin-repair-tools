#include "diskutils.h"
#include "diskinfo.h"

#include <QFile>
#include <QDebug>
#include <QDir>

#include <sys/stat.h>

#define MOUNTS_PATH     "/proc/mounts"
#define BLOCK_DEVS_PATH "/dev/block"

inline bool is_block_device(const QString &path)
{
    struct stat sb;

    return stat(path.toStdString().c_str(), &sb) || S_ISBLK(sb.st_mode);
}

QList<DiskInfo> list_mounted_devices()
{
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

void mount_block_devices()
{
    QDir block_dir(BLOCK_DEVS_PATH);

    for (const auto &info : block_dir.entryList(QDir::NoDotAndDotDot))
    {
        qDebug() << block_dir.absoluteFilePath(info);
    }
}

DiskUtils::DiskUtils(QObject *parent)
    : QObject(parent)

    , m_scanFinished(false)
{

}

void DiskUtils::initilize()
{
    mount_block_devices();
    qDebug() << list_mounted_devices();
}
