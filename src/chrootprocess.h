#ifndef CHROOTPROCESS_H
#define CHROOTPROCESS_H

#include "chrootbindguard.h"

#include <QProcess>

class ChrootProcess : public QProcess
{
    Q_OBJECT

public:
    explicit ChrootProcess(const QString &root, QObject *parent = nullptr);

private:
    ChrootBindGuard _bind_guard;
};

#endif // CHROOTPROCESS_H
