#include "permissionsrepairwidget.h"
#include "permissionsrepairthread.h"

#include <QVBoxLayout>
#include <QDebug>

PermissionsRepairWidget::PermissionsRepairWidget(QWidget *parent)
    : QWidget(parent)

    , m_icon(new QLabel)
    , m_repairButton(new QPushButton)
{
    m_icon->setPixmap(QIcon(":/resources/repair_permission.svg").pixmap(128, 128));
    m_icon->setAlignment(Qt::AlignHCenter);
    m_repairButton->setText(tr("Permissions Repair"));

    QVBoxLayout *centralLayout = new QVBoxLayout;
    centralLayout->addWidget(m_icon);
    centralLayout->addWidget(m_repairButton);
    centralLayout->setAlignment(m_repairButton, Qt::AlignHCenter);
    centralLayout->setSpacing(0);
    centralLayout->setContentsMargins(0, 60, 0, 0);

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
