#ifndef DISKINFO_H
#define DISKINFO_H

#include <QString>
#include <QDebug>

class DiskInfo
{
public:
    friend QDebug operator<<(QDebug debug, const DiskInfo &rhs);

    QString diskPath;
    QString mountPoint;
    QString format;
};

#endif // DISKINFO_H
