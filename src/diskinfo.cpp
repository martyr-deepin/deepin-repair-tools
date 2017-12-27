#include "diskinfo.h"

QDebug operator<<(QDebug debug, const DiskInfo &rhs)
{
    debug.noquote() << rhs.diskPath << ":" << rhs.mountPoint << ":" << rhs.format;

    return debug;
}
