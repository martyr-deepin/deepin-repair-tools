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
    m_output->setReadOnly(true);
    m_output->setFixedHeight(240);
    m_output->setStyleSheet("QTextEdit {"
                            "color: #609dc8;"
                            "font-size: 8pt;"
                            "padding: 0 20px 0 20px;"
                            "}");

    m_tips->setAlignment(Qt::AlignCenter);
    m_tips->setObjectName("TipsLabel");
    m_result->setAlignment(Qt::AlignCenter);
    m_showDetail->setText(tr("Show details"));
    m_hideDetail->setText(tr("Hide details"));
    m_spinner->setFixedSize(24, 24);
    m_icon->setPixmap(QIcon(":/resources/repair_dpkg.svg").pixmap(128, 128));
    m_icon->setAlignment(Qt::AlignHCenter);
    m_icon->setObjectName("PluginIcon");
    m_repairButton->setText(tr("Repair"));
    m_okButton->setText(tr("Done"));

    QHBoxLayout *btnsLayout = new QHBoxLayout;
    btnsLayout->addStretch();
    btnsLayout->addWidget(m_okButton);
    btnsLayout->addWidget(m_repairButton);
    btnsLayout->addStretch();

    QVBoxLayout *centralLayout = new QVBoxLayout;
    centralLayout->addWidget(m_output);
    centralLayout->addWidget(m_icon);
    centralLayout->addSpacing(10);
    centralLayout->addWidget(m_tips);
    centralLayout->addWidget(m_showDetail);
    centralLayout->addWidget(m_hideDetail);
    centralLayout->addStretch();
    centralLayout->addWidget(m_result);
    centralLayout->addSpacing(10);
    centralLayout->addWidget(m_spinner);
    centralLayout->setAlignment(m_spinner, Qt::AlignHCenter);
    centralLayout->addLayout(btnsLayout);
    centralLayout->setSpacing(0);
    centralLayout->setContentsMargins(0, 0, 0, 0);

    setLayout(centralLayout);

    connect(m_repairButton, &QPushButton::clicked, this, &DPKGRepairWidget::onRepairClicked);
    connect(m_okButton, &QPushButton::clicked, this, &DPKGRepairWidget::reset);
    connect(m_showDetail, &QPushButton::clicked, this, &DPKGRepairWidget::showDetail);
    connect(m_hideDetail, &QPushButton::clicked, this, &DPKGRepairWidget::hideDetail);

    QTimer::singleShot(1, this, &DPKGRepairWidget::reset);
}

void DPKGRepairWidget::showEvent(QShowEvent *e)
{
    QWidget::showEvent(e);

    m_repairButton->setFocus();
}

void DPKGRepairWidget::reset()
{
    m_tips->setText(tr("Cannot install or upgrade application"));
    m_tips->setVisible(true);
    m_result->setVisible(false);
    m_output->setVisible(false);
    m_output->clear();
    m_showDetail->setVisible(false);
    m_hideDetail->setVisible(false);
    m_spinner->setVisible(false);
    m_okButton->setVisible(false);
    m_repairButton->setVisible(true);
    m_icon->setVisible(true);
}

void DPKGRepairWidget::onRepairClicked()
{
    m_toolsProxy->registerExclusive(true);

    m_repairButton->setVisible(false);
    m_spinner->start();
    m_spinner->setVisible(true);
    m_showDetail->setVisible(true);
    m_result->setText(tr("Repairing, please wait..."));
    m_result->setVisible(true);
    m_result->setStyleSheet("color: black;");

    DPKGRepairThread *thrd = new DPKGRepairThread;
    thrd->setToolsProxy(m_toolsProxy);

    for (const auto &info : m_toolsProxy->diskInfos())
    {
        if (info.osName.contains("deepin", Qt::CaseInsensitive))
            thrd->appendRoot(info.mountPoint);
    }

    connect(thrd, &DPKGRepairThread::finished, thrd, &DPKGRepairThread::deleteLater, Qt::QueuedConnection);
    connect(thrd, &DPKGRepairThread::processFinished, this, &DPKGRepairWidget::onRepairFinished);
    connect(thrd, &DPKGRepairThread::outputPrinted, m_output, &QTextEdit::append);

    thrd->start();
}

void DPKGRepairWidget::onRepairFinished(const bool success)
{
    m_toolsProxy->registerExclusive(false);

    if (!success)
    {
        m_result->setStyleSheet("color: #f3a21d;");
        m_result->setText(tr("Repair failed"));
    } else {
        m_result->setStyleSheet("color: #3da219;");
        m_result->setText(tr("Repair successful"));
    }

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
