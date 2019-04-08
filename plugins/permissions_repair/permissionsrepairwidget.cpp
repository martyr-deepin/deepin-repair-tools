#include "permissionsrepairwidget.h"
#include "permissionsrepairthread.h"

#include <QVBoxLayout>
#include <QDebug>
#include <QTimer>
#include <QApplication>

PermissionsRepairWidget::PermissionsRepairWidget(QWidget *parent)
    : QWidget(parent)

    , m_icon(new QLabel)
    , m_tips(new QLabel)
    , m_status(new QLabel)
    , m_spinnerPlaceHolder(new QWidget)
    , m_okButton(new QPushButton)
    , m_repairButton(new QPushButton)
{
    m_icon->setPixmap(QIcon(":/resources/repair_permission.svg").pixmap(128, 128));
    m_icon->setAlignment(Qt::AlignHCenter);
    m_icon->setObjectName("PluginIcon");
    m_tips->setAlignment(Qt::AlignHCenter);
    m_tips->setText(tr("Cannot enter desktop due to privilege error"));
    m_tips->setObjectName("TipsLabel");
    m_status->setAlignment(Qt::AlignHCenter);
    m_repairButton->setText(tr("Repair"));
    m_okButton->setText(tr("Done"));
    m_okButton->setVisible(false);
    m_spinnerPlaceHolder->setFixedSize(32, 32);

    QHBoxLayout *btnsLayout = new QHBoxLayout;
    btnsLayout->addStretch();
    btnsLayout->addWidget(m_okButton);
    btnsLayout->addWidget(m_repairButton);
    btnsLayout->addStretch();

    QVBoxLayout *centralLayout = new QVBoxLayout;
    centralLayout->addWidget(m_icon);
    centralLayout->addSpacing(10);
    centralLayout->addWidget(m_tips);
    centralLayout->addStretch();
    centralLayout->addWidget(m_status);
    centralLayout->addSpacing(10);
    centralLayout->addWidget(m_spinnerPlaceHolder);
    centralLayout->setAlignment(m_spinnerPlaceHolder, Qt::AlignHCenter);
    centralLayout->addLayout(btnsLayout);
    centralLayout->setSpacing(0);
    centralLayout->setContentsMargins(0, 0, 0, 0);

    setLayout(centralLayout);

    connect(m_repairButton, &QPushButton::clicked, this, &PermissionsRepairWidget::onRepairButtonClicked);
    connect(m_okButton, &QPushButton::clicked, this, &PermissionsRepairWidget::resetUi);

    QTimer::singleShot(1, this, &PermissionsRepairWidget::resetUi);
}

void PermissionsRepairWidget::showEvent(QShowEvent *e)
{
    QWidget::showEvent(e);

    m_repairButton->setFocus();
}

void PermissionsRepairWidget::onRepairButtonClicked()
{
    m_toolsProxy->registerExclusive(true);

    m_spinnerPlaceHolder->setVisible(true);
    m_repairButton->setVisible(false);
    m_status->setText(tr("Repairing, please wait..."));
    m_status->setVisible(true);
    m_status->setStyleSheet("color: black;");

    PermissionsRepairThread *thrd = new PermissionsRepairThread;
    thrd->setToolsProxy(m_toolsProxy);

    connect(thrd, &PermissionsRepairThread::finished, thrd, &PermissionsRepairThread::deleteLater, Qt::QueuedConnection);
    connect(thrd, &PermissionsRepairThread::commandFinished, this, &PermissionsRepairWidget::onRepairFinished);

    thrd->start();
}

void PermissionsRepairWidget::onRepairFinished(const bool success)
{
    m_toolsProxy->registerExclusive(false);

    if (success)
    {
        m_status->setText(tr("Reset privilege successfully"));
        m_status->setStyleSheet("color: #3da219;");
    } else {
        m_status->setText(tr("Reset failed"));
        m_status->setStyleSheet("color: #f3a21d;");
    }

    m_status->setVisible(true);
    m_okButton->setVisible(true);
    m_spinnerPlaceHolder->setVisible(false);
}

void PermissionsRepairWidget::resetUi()
{
    m_status->setVisible(false);
    m_okButton->setVisible(false);
    m_repairButton->setVisible(true);
    m_spinnerPlaceHolder->setVisible(false);
}
