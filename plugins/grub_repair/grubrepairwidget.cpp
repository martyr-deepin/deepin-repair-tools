#include "grubrepairwidget.h"
#include "grubrepairthread.h"

#include <QVBoxLayout>
#include <QDebug>

GrubRepairWidget::GrubRepairWidget(QWidget *parent)
    : QWidget(parent)

    , m_icon(new QLabel)
    , m_centerTips(new QLabel)
    , m_statusTips(new QLabel)
    , m_repairButton(new QPushButton)
    , m_okButton(new QPushButton)
    , m_loadingIndicator(new DLoadingIndicator)
{
    m_repairButton->setText(tr("Repair"));
    m_centerTips->setText(tr("Cannot find boot menu"));
    m_centerTips->setAlignment(Qt::AlignHCenter);
    m_statusTips->setAlignment(Qt::AlignHCenter);
    m_statusTips->setVisible(false);
    m_okButton->setText(tr("Finish"));
    m_okButton->setVisible(false);
    m_loadingIndicator->setVisible(false);

    QHBoxLayout *btnsLayout = new QHBoxLayout;
    btnsLayout->addStretch();
    btnsLayout->addWidget(m_repairButton);
    btnsLayout->addWidget(m_okButton);
    btnsLayout->addStretch();

    QVBoxLayout *centralLayout = new QVBoxLayout;
    centralLayout->addWidget(m_icon);
    centralLayout->addWidget(m_centerTips);
    centralLayout->addWidget(m_statusTips);
    centralLayout->addLayout(btnsLayout);
    centralLayout->addWidget(m_loadingIndicator);
    centralLayout->setAlignment(m_loadingIndicator, Qt::AlignHCenter);

    setLayout(centralLayout);

    connect(m_repairButton, &QPushButton::clicked, this, &GrubRepairWidget::onRepairClicked);
    connect(m_okButton, &QPushButton::clicked, this, &GrubRepairWidget::resetUi);
}

void GrubRepairWidget::onRepairClicked()
{
    m_repairButton->setVisible(false);

    GrubRepairThread *thrd = new GrubRepairThread;
    thrd->setToolsProxy(m_toolsProxy);

    connect(thrd, &GrubRepairThread::finished, thrd, &GrubRepairThread::deleteLater);
    connect(thrd, &GrubRepairThread::finished, this, &GrubRepairWidget::onRepairFinished);

    thrd->start();
}

void GrubRepairWidget::resetUi()
{
    m_okButton->setVisible(false);
    m_statusTips->setVisible(false);
    m_repairButton->setVisible(true);
}

void GrubRepairWidget::onRepairFinished()
{
    m_statusTips->setText(tr("Repair success"));
    m_statusTips->setVisible(true);
    m_okButton->setVisible(true);
}
