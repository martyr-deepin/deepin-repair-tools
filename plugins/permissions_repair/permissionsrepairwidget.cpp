#include "permissionsrepairwidget.h"
#include "permissionsrepairthread.h"

#include <QVBoxLayout>
#include <QDebug>

PermissionsRepairWidget::PermissionsRepairWidget(QWidget *parent)
    : QWidget(parent)

    , m_icon(new QLabel)
    , m_okButton(new QPushButton)
    , m_repairButton(new QPushButton)
{
    m_icon->setPixmap(QIcon(":/resources/repair_permission.svg").pixmap(128, 128));
    m_icon->setAlignment(Qt::AlignHCenter);
    m_repairButton->setText(tr("Permissions Repair"));
    m_okButton->setText(tr("Finish"));
    m_okButton->setVisible(false);

    QHBoxLayout *btnsLayout = new QHBoxLayout;
    btnsLayout->addStretch();
    btnsLayout->addWidget(m_okButton);
    btnsLayout->addWidget(m_repairButton);
    btnsLayout->addStretch();

    QVBoxLayout *centralLayout = new QVBoxLayout;
    centralLayout->addWidget(m_icon);
    centralLayout->addLayout(btnsLayout);
    centralLayout->setSpacing(0);
    centralLayout->setContentsMargins(0, 60, 0, 40);

    setLayout(centralLayout);

    connect(m_repairButton, &QPushButton::clicked, this, &PermissionsRepairWidget::onRepairButtonClicked);
    connect(m_okButton, &QPushButton::clicked, this, &PermissionsRepairWidget::resetUi);
}

void PermissionsRepairWidget::onRepairButtonClicked()
{
    m_repairButton->setVisible(false);

    PermissionsRepairThread *thrd = new PermissionsRepairThread;
    thrd->setToolsProxy(m_toolsProxy);

    connect(thrd, &PermissionsRepairThread::finished, thrd, &PermissionsRepairThread::deleteLater, Qt::QueuedConnection);
    connect(thrd, &PermissionsRepairThread::finished, this, &PermissionsRepairWidget::onRepairFinished);

    thrd->start();
}

void PermissionsRepairWidget::onRepairFinished()
{
    m_okButton->setVisible(true);
}

void PermissionsRepairWidget::resetUi()
{
    m_okButton->setVisible(false);
    m_repairButton->setVisible(true);
}
