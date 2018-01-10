#include "dpkgrepairwidget.h"
#include "dpkgrepairthread.h"

#include <QVBoxLayout>
#include <QDebug>
#include <QTimer>
#include <QApplication>

DPKGRepairWidget::DPKGRepairWidget(QWidget *parent)
    : QWidget(parent)

    , m_output(new QTextEdit)
    , m_icon(new QLabel)
    , m_tips(new QLabel)
    , m_result(new QLabel)
    , m_spinner(new DSpinner)
    , m_showDetail(new DLinkButton)
    , m_hideDetail(new DLinkButton)
    , m_repairButton(new QPushButton)
    , m_okButton(new QPushButton)
{
    m_output->setStyleSheet("QTextEdit {"
                            "color: #1077d7;"
                            "}");

    m_tips->setAlignment(Qt::AlignCenter);
    m_result->setAlignment(Qt::AlignCenter);
    m_showDetail->setText(tr("Show detailed progress"));
    m_hideDetail->setText(tr("Hide detailed progress"));
    m_spinner->setFixedSize(24, 24);
    m_icon->setPixmap(QIcon(":/resources/repair_dpkg.svg").pixmap(128, 128));
    m_icon->setAlignment(Qt::AlignHCenter);
    m_repairButton->setText(tr("Repair"));
    m_okButton->setText(tr("OK"));

    QHBoxLayout *btnsLayout = new QHBoxLayout;
    btnsLayout->addStretch();
    btnsLayout->addWidget(m_okButton);
    btnsLayout->addWidget(m_repairButton);
    btnsLayout->addStretch();

    QVBoxLayout *centralLayout = new QVBoxLayout;
    centralLayout->addWidget(m_output);
    centralLayout->addWidget(m_icon);
    centralLayout->addWidget(m_tips);
    centralLayout->addWidget(m_showDetail);
    centralLayout->addWidget(m_hideDetail);
    centralLayout->addStretch();
    centralLayout->addWidget(m_result);
    centralLayout->addWidget(m_spinner);
    centralLayout->setAlignment(m_spinner, Qt::AlignHCenter);
    centralLayout->addLayout(btnsLayout);
    centralLayout->setSpacing(0);
    centralLayout->setContentsMargins(0, 60, 0, 40);

    setLayout(centralLayout);

    connect(m_repairButton, &QPushButton::clicked, this, &DPKGRepairWidget::onRepairClicked);
    connect(m_okButton, &QPushButton::clicked, this, &DPKGRepairWidget::reset);
    connect(m_showDetail, &QPushButton::clicked, this, &DPKGRepairWidget::showDetail);
    connect(m_hideDetail, &QPushButton::clicked, this, &DPKGRepairWidget::hideDetail);

    QTimer::singleShot(1, this, &DPKGRepairWidget::reset);
}

void DPKGRepairWidget::reset()
{
    m_tips->setText(tr("Unexpected interruption when install/upgrade"));
    m_tips->setVisible(true);
    m_result->setVisible(false);
    m_output->setVisible(false);
    m_output->clear();
    m_showDetail->setVisible(false);
    m_hideDetail->setVisible(false);
    m_spinner->setVisible(false);
    m_okButton->setVisible(false);
    m_repairButton->setVisible(true);
}

void DPKGRepairWidget::onRepairClicked()
{
    m_repairButton->setVisible(false);
    m_spinner->start();
    m_spinner->setVisible(true);
    m_showDetail->setVisible(true);

    DPKGRepairThread *thrd = new DPKGRepairThread;
    thrd->setToolsProxy(m_toolsProxy);

    for (const auto &info : m_toolsProxy->diskInfos())
    {
        if (info.osName.contains("deepin", Qt::CaseInsensitive))
            thrd->appendRoot(info.mountPoint);
    }

    connect(thrd, &DPKGRepairThread::finished, thrd, &DPKGRepairThread::deleteLater, Qt::QueuedConnection);
    connect(thrd, &DPKGRepairThread::finished, this, &DPKGRepairWidget::onRepairFinished);
    connect(thrd, &DPKGRepairThread::outputPrinted, this, &DPKGRepairWidget::appendOutput);

    thrd->start();
}

void DPKGRepairWidget::onRepairFinished()
{
    m_result->setStyleSheet("color: green;");
    m_result->setText(tr("Repair success"));
    m_result->setVisible(true);
    m_okButton->setVisible(true);
    m_spinner->stop();
    m_spinner->setVisible(false);
}

void DPKGRepairWidget::showDetail()
{
    m_showDetail->setVisible(false);
    m_hideDetail->setVisible(true);
    m_output->setVisible(true);
    m_icon->setVisible(false);
    m_tips->setVisible(false);
}

void DPKGRepairWidget::hideDetail()
{
    m_showDetail->setVisible(true);
    m_hideDetail->setVisible(false);
    m_output->setVisible(false);
    m_icon->setVisible(true);
    m_tips->setVisible(true);
}

void DPKGRepairWidget::appendOutput(const QString &output)
{
    m_output->append(output);
}
