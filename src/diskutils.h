#ifndef DISKUTILS_H
#define DISKUTILS_H

#include <QObject>

class DiskUtils : public QObject
{
    Q_OBJECT

public:
    explicit DiskUtils(QObject *parent = nullptr);

public slots:
    void initilize();
};

#endif // DISKUTILS_H
