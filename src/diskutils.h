#ifndef DISKUTILS_H
#define DISKUTILS_H

#include "repairtoolsproxy.h"

#include <QObject>

class DiskUtils : public QObject
{
    Q_OBJECT

public:
    explicit DiskUtils(QObject *parent = nullptr);
    ~DiskUtils();

    bool scannerRunning() const { return m_scannerRunning; }

    const QList<DiskInfo> diskInfos() const { return m_diskInfos; }

signals:
    void scanFinished() const;

public slots:
    void initilize();
    void onInitFinished();

private:
    bool m_scannerRunning;
    QList<DiskInfo> m_diskInfos;
};

#endif // DISKUTILS_H
