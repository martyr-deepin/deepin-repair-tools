#ifndef REPAIRTOOLSPROXY_H
#define REPAIRTOOLSPROXY_H

#include <QVariant>

class RepairToolsProxy
{
public:

    enum RepairToolsRole
    {
        ToolsVersionRole,
    };


    virtual ~RepairToolsProxy() {}

    virtual void ping() = 0;
    virtual QVariant data(const RepairToolsRole role) = 0;
};

#endif // REPAIRTOOLSPROXY_H
