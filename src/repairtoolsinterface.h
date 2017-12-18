#ifndef REPAIRTOOLSINTERFACE_H
#define REPAIRTOOLSINTERFACE_H

#include <QWidget>

class RepairToolsInterface
{
public:
    virtual ~RepairToolsInterface() = 0;

    virtual QWidget *centralWidget() = 0;
};

#endif // REPAIRTOOLSINTERFACE_H
