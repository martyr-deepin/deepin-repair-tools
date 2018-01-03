#include "grubrepairwidget.h"
#include "grubrepairthread.h"

#include <QVBoxLayout>
#include <QDebug>

GrubRepairWidget::GrubRepairWidget(QWidget *parent)
    : QWidget(parent)

    , m_repairButton(new QPushButton)
{
    m_repairButton->setText(tr("Repair"));

    QVBoxLayout *centralLayout = new QVBoxLayout;
    centralLayout->addWidget(m_repairButton);
    centralLayout->setAlignment(m_repairButton, Qt::AlignHCenter);

    setLayout(centralLayout);

    connect(m_repairButton, &QPushButton::clicked, this, &GrubRepairWidget::onRepairClicked);
}

void GrubRepairWidget::onRepairClicked()
{
    m_repairButton->setVisible(false);

    GrubRepairThread *thrd = new GrubRepairThread;
    thrd->setToolsProxy(m_toolsProxy);

    for (const auto &info : m_toolsProxy->diskInfos())
    {
        if (info.osName.contains("deepin", Qt::CaseInsensitive))
            thrd->appendRoot(info.mountPoint);
    }

    connect(thrd, &GrubRepairThread::finished, thrd, &GrubRepairThread::deleteLater);
    connect(thrd, &GrubRepairThread::finished, this, &GrubRepairWidget::onRepairFinished);

    thrd->start();
}

void GrubRepairWidget::onRepairFinished()
{
    m_repairButton->setVisible(true);
}
