#ifndef FSCHECKTHREAD_H
#define FSCHECKTHREAD_H

#include <QThread>

class FSCheckThread : public QThread
{
    Q_OBJECT

public:
    explicit FSCheckThread(QObject *parent = nullptr);

    void run();
};

#endif // FSCHECKTHREAD_H
