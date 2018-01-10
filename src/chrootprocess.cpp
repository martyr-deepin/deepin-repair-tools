#include "chrootprocess.h"

#include <QDebug>

ChrootProcess::ChrootProcess(const QString &root, QObject *parent)
    : QProcess(parent)

    , _bind_guard(root)
{

}
