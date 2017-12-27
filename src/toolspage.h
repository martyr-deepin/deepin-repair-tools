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

class ToolsPage : public QWidget, public RepairToolsProxy
{
    Q_OBJECT

public:
    explicit ToolsPage(QWidget *parent = nullptr);
    ~ToolsPage();

    void setModel(const QPointer<ToolsModel> &model);
    void setDiskUtils(const QPointer<DiskUtils> &diskUtils) { m_diskUtils = diskUtils; }

public:
    void ping();
    const QList<DiskInfo> diskInfos() const { return m_diskUtils->diskInfos(); }

private slots:
    void refreshNavBar();
    void onNavButtonClicked(const int index);

private:
    QPointer<DiskUtils> m_diskUtils;
    QPointer<ToolsModel> m_model;
    QButtonGroup *m_navButtonsGroup;
    QVBoxLayout *m_navigationLayout;
    QStackedLayout *m_pluginsLayout;
};

#endif // TOOLSPAGE_H
