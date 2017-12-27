#ifndef DISKUTILS_H
#define DISKUTILS_H

#include <QObject>

class DiskUtils : public QObject
{
    Q_OBJECT

public:
    explicit DiskUtils(QObject *parent = nullptr);

    bool scanFinished() const { return m_scanFinished; }

public slots:
    void initilize();

private:
    bool m_scanFinished;
};

#endif // DISKUTILS_H
