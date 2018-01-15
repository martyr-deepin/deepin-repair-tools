#ifndef TOOLSPAGE_H
#define TOOLSPAGE_H

#include "repairtoolsproxy.h"
#include "toolsmodel.h"
#include "diskutils.h"

#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QStackedLayout>
#include <QButtonGroup>
#include <QProcess>

const RunResult execAsChrootSynchronous(const QString &root, const QString &script, const QStringList &args);
QProcess* execAsChrootAsynchronous(const QString &root, const QString &script, const QStringList &args);

class ToolsPage : public QWidget, public RepairToolsProxy
{
    Q_OBJECT

public:
    explicit ToolsPage(QWidget *parent = nullptr);
    ~ToolsPage();

    void setModel(const QPointer<ToolsModel> &model);
    void setDiskUtils(const QPointer<DiskUtils> &diskUtils) { m_diskUtils = diskUtils; }

public:
    void registerExclusive(const bool exclusive) { setNavgationEnabled(!exclusive); }
    const RunResult execAsChrootSynchronous(const QString &root, const QString &script, const QStringList &args = QStringList()) const
    { return ::execAsChrootSynchronous(root, script, args); }
    QProcess* execAsChrootAsynchronous(const QString &root, const QString &script, const QStringList &args = QStringList()) const
    { return ::execAsChrootAsynchronous(root, script, args); }
    const QList<DiskInfo> diskInfos() const { return m_diskUtils->diskInfos(); }

private slots:
    void refreshNavBar();
    void setNavgationEnabled(const bool enabled);
    void onNavButtonClicked(const int index);

private:
    QPointer<DiskUtils> m_diskUtils;
    QPointer<ToolsModel> m_model;
    QButtonGroup *m_navButtonsGroup;
    QVBoxLayout *m_navigationLayout;
    QStackedLayout *m_pluginsLayout;
};

#endif // TOOLSPAGE_H
