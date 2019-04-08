#include "scanningpage.h"
#include "toolspage.h"
#include "fscheckthread.h"
#include "fsrepairthread.h"

#include <QVBoxLayout>
#include <QIcon>
#include <QDebug>
#include <QApplication>

ScanningPage::ScanningPage(QWidget *parent)
    : QWidget(parent)

    , m_spinner(new DSpinner)
    , m_icon(new QLabel)
    , m_centerTips(new QLabel)
    , m_bottomTips(new QLabel)
    , m_cancel(new QPushButton)
    , m_repair(new QPushButton)
    , m_finish(new QPushButton)
    , m_reboot(new QPushButton)
{
    m_spinner->setFixedSize(24, 24);
    m_spinner->start();
    m_icon->setPixmap(QIcon(":/resources/repair_deepindisk.svg").pixmap(128, 128));
    m_icon->setObjectName("Icon");
    m_centerTips->setText(tr("Checking hard disk status, please wait..."));
    m_centerTips->setAlignment(Qt::AlignCenter);
    m_bottomTips->setAlignment(Qt::AlignCenter);
    m_bottomTips->setVisible(false);
    m_bottomTips->setStyleSheet("QLabel {"
                                "color: #f3a21d;"
                                "}");
    m_cancel->setText(tr("Cancel"));
    m_cancel->setObjectName("DeactivedButton");
    m_cancel->setVisible(false);
    m_repair->setText(tr("Repair"));
    m_repair->setVisible(false);
    m_finish->setText(tr("Done"));
    m_finish->setVisible(false);
    m_reboot->setText(tr("Restart Now"));
    m_reboot->setVisible(false);

    QHBoxLayout *buttonsLayout = new QHBoxLayout;
    buttonsLayout->addStretch();
    buttonsLayout->addWidget(m_cancel);
    buttonsLayout->addWidget(m_repair);
    buttonsLayout->addWidget(m_finish);
    buttonsLayout->addWidget(m_reboot);
    buttonsLayout->addStretch();
    buttonsLayout->setSpacing(20);

    QVBoxLayout *centralLayout = new QVBoxLayout;
    centralLayout->addWidget(m_icon);
    centralLayout->setAlignment(m_icon, Qt::AlignHCenter);
    centralLayout->addSpacing(15);
    centralLayout->addWidget(m_centerTips);
    centralLayout->addStretch();
    centralLayout->addWidget(m_bottomTips);
    centralLayout->addSpacing(10);
    centralLayout->addWidget(m_spinner);
    centralLayout->setAlignment(m_spinner, Qt::AlignHCenter);
    centralLayout->addLayout(buttonsLayout);
    centralLayout->setSpacing(0);
    centralLayout->setContentsMargins(0, 0, 0, 40);

    setLayout(centralLayout);
    setObjectName("ScanPage");
    setStyleSheet("#ScanPage QPushButton {"
                  "width: 152px;"
                  "height: 26px;"
                  "background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #FFFFFF, stop:1 #FBFBFB);"
                  "border-color: rgba(0, 131, 255, 0.4);"
                  "border-radius: 4;"
                  "border-style: solid;"
                  "border-width: 1;"
                  "outline: none;"
                  "padding: 3px 24px 4px 24px;"
                  "color: #0699ff;"
                  "font-size: 14px;"
                  "}"
                  ""
                  "#ScanPage QPushButton:hover {"
                  "background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #8ccfff, stop:1 #4bb8ff);"
                  "color: #FFFFFF;"
                  "border-color: rgba(0, 117, 243, 0.2);"
                  "}"
                  ""
                  "#ScanPage QPushButton:pressed {"
                  "background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #0b8cff, stop:1 #0aa1ff);"
                  "color: #FFFFFF;"
                  "border-color: rgba(29, 129, 255, 0.3);"
                  "}"
                  ""
                  "#ScanPage QPushButton#DeactivedButton {"
                  "border-color: rgba(0, 0, 0, .1);"
                  "color: #303030;"
                  "}"
                  ""
                  "#ScanPage QPushButton#DeactivedButton:hover {"
                  "color: white;"
                  "}"
                  ""
                  "#ScanPage QLineEdit {"
                  "width: 210px;"
                  "height: 25px;"
                  "}"
                  ""
                  "#ScanPage #TipsLabel {"
                  "font-size: 9pt;"
                  "font-weight: 500;"
                  "}"
                  ""
                  "#ScanPage #Icon {"
                  "margin: 70px 0 0 0;"
                  "}"
                  "");

    connect(m_cancel, &QPushButton::clicked, qApp, &QApplication::quit);
    connect(m_finish, &QPushButton::clicked, qApp, &QApplication::quit);
    connect(m_reboot, &QPushButton::clicked, this, &ScanningPage::onRebootClicked);
}

void ScanningPage::startScan()
{
    FSCheckThread *thrd = new FSCheckThread;
    thrd->setDiskUtils(m_diskUtils);

    connect(thrd, &FSCheckThread::checkFinished, this, &ScanningPage::onScanFinsihed);
    connect(thrd, &FSCheckThread::finished, thrd, &FSCheckThread::deleteLater, Qt::QueuedConnection);

    thrd->start();
}

void ScanningPage::onScanFinsihed(const QString &errorPartion)
{
#ifndef QT_DEBUG
    if (errorPartion.isEmpty())
#endif
    {
        emit scanDone();
        return;
    }

    m_spinner->setVisible(false);
    m_spinner->stop();
    m_cancel->setVisible(true);
    m_repair->setVisible(true);
    m_centerTips->setText(tr("Hard disk partition %1 error, please repair").arg(errorPartion));
    m_bottomTips->setText(tr("The repair may cause data loss, please confirm and continue"));
    m_bottomTips->setVisible(true);

    connect(m_repair, &QPushButton::clicked, this, [=] { repairPartion(errorPartion); });
}

void ScanningPage::onRepairFinished(bool success)
{
    if (success)
    {
        m_centerTips->setText(tr("Repair successful"));
        m_bottomTips->setText(tr("Please restart your computer to finish repairing"));
        m_reboot->setVisible(true);
    }
    else
    {
        m_centerTips->setText(tr("Repair failed"));
        m_finish->setVisible(true);
    }

    m_spinner->stop();
    m_spinner->setVisible(false);
}

void ScanningPage::onRebootClicked()
{
    QProcess::startDetached("reboot");
}

void ScanningPage::repairPartion(const QString &errorPartion)
{
    m_repair->disconnect();
    m_repair->setVisible(false);
    m_cancel->setVisible(false);
    m_spinner->start();
    m_spinner->setVisible(true);
    m_bottomTips->clear();
    m_centerTips->setText(tr("Repairing hard disk, please wait..."));

    FSRepairThread *thrd = new FSRepairThread;
    thrd->setRepairPartion(errorPartion);

    connect(thrd, &FSRepairThread::repairFinished, this, &ScanningPage::onRepairFinished);
    connect(thrd, &FSRepairThread::finished, thrd, &FSRepairThread::deleteLater, Qt::QueuedConnection);

    thrd->start();
}
