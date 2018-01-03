#include "permissionsrepairwidget.h"
#include "permissionsrepairthread.h"

#include <QVBoxLayout>
#include <QDebug>

PermissionsRepairWidget::PermissionsRepairWidget(QWidget *parent)
    : QWidget(parent)

    , m_repairButton(new QPushButton)
{
    m_repairButton->setText(tr("Permissions Repair"));

    QVBoxLayout *centralLayout = new QVBoxLayout;
    centralLayout->addWidget(m_repairButton);
    centralLayout->setAlignment(m_repairButton, Qt::AlignHCenter);

    setLayout(centralLayout);

    connect(m_repairButton, &QPushButton::clicked, this, &PermissionsRepairWidget::onRepairButtonClicked);
}

void PermissionsRepairWidget::onRepairButtonClicked()
{
    PermissionsRepairThread *thrd = new PermissionsRepairThread;
    thrd->setToolsProxy(m_toolsProxy);

    connect(thrd, &PermissionsRepairThread::finished, thrd, &PermissionsRepairThread::deleteLater);
    connect(thrd, &PermissionsRepairThread::finished, this, &PermissionsRepairWidget::onRepairFinished);

    thrd->start();
}

void PermissionsRepairWidget::onRepairFinished()
{
}
