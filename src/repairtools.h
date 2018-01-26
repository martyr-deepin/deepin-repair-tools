#ifndef REPAIRTOOLS_H
#define REPAIRTOOLS_H

#include "toolsmodel.h"
#include "diskutils.h"

#include <QPointer>

#include <DMainWindow>

DWIDGET_USE_NAMESPACE

class RepairTools : public DMainWindow
{
    Q_OBJECT

public:
    explicit RepairTools(QWidget *parent = nullptr);

public slots:
    bool onLinkClicked(const QUrl &link);

private:
    void keyPressEvent(QKeyEvent *e);

private slots:
    void onScanDone();

private:
    QPointer<DiskUtils> m_diskUtils;
    QPointer<ToolsModel> m_toolsModel;
};

#endif // REPAIRTOOLS_H
