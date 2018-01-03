#include "dpkgrepairwidget.h"
#include "dpkgrepairthread.h"

#include <QVBoxLayout>
#include <QDebug>

DPKGRepairWidget::DPKGRepairWidget(QWidget *parent)
    : QWidget(parent)

    , m_icon(new QLabel)
    , m_repairButton(new QPushButton)
{
    m_icon->setPixmap(QIcon(":/resources/repair_dpkg.svg").pixmap(128, 128));
    m_icon->setAlignment(Qt::AlignHCenter);
    m_repairButton->setText(tr("Repair"));

    QVBoxLayout *centralLayout = new QVBoxLayout;
    centralLayout->addWidget(m_icon);
    centralLayout->addWidget(m_repairButton);
    centralLayout->setAlignment(m_repairButton, Qt::AlignHCenter);
    centralLayout->setSpacing(0);
    centralLayout->setContentsMargins(0, 60, 0, 40);

    setLayout(centralLayout);

    connect(m_repairButton, &QPushButton::clicked, this, &DPKGRepairWidget::onRepairClicked);
}

void DPKGRepairWidget::onRepairClicked()
{
    m_repairButton->setVisible(false);

    DPKGRepairThread *thrd = new DPKGRepairThread;
    thrd->setToolsProxy(m_toolsProxy);

    for (const auto &info : m_toolsProxy->diskInfos())
    {
        if (info.osName.contains("deepin", Qt::CaseInsensitive))
            thrd->appendRoot(info.mountPoint);
    }

    connect(thrd, &DPKGRepairThread::finished, thrd, &DPKGRepairThread::deleteLater);
    connect(thrd, &DPKGRepairThread::finished, this, &DPKGRepairWidget::onRepairFinished);

    thrd->start();
}

void DPKGRepairWidget::onRepairFinished()
{
    m_repairButton->setVisible(true);
}
