#ifndef CHROOTBINDGUARD_H
#define CHROOTBINDGUARD_H

#include <QStringList>

class ChrootBindGuard
{
public:
    ChrootBindGuard(const QString &rootPath);
    ~ChrootBindGuard();

private:
    QString m_rootPath;
    QStringList m_bindList;
};

#endif // CHROOTBINDGUARD_H
