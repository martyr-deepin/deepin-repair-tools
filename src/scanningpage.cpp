#include "scanningpage.h"
#include "toolspage.h"
#include "fscheckthread.h"

#include <QVBoxLayout>
#include <QIcon>

ScanningPage::ScanningPage(QWidget *parent)
    : QWidget(parent)

    , m_spinner(new DSpinner)
    , m_icon(new QLabel)
    , m_centerTips(new QLabel)
    , m_bottomTips(new QLabel)
{
    m_spinner->setFixedSize(24, 24);
    m_spinner->start();
    m_icon->setPixmap(QIcon::fromTheme("drive-harddisk").pixmap(128, 128));
    m_centerTips->setText(tr("Checking hard disk status, please wait..."));
    m_centerTips->setAlignment(Qt::AlignCenter);
    m_bottomTips->setText(tr("Hard disk error, please repair"));
    m_bottomTips->setAlignment(Qt::AlignCenter);
    m_bottomTips->setVisible(false);

    QVBoxLayout *centralLayout = new QVBoxLayout;
    centralLayout->addWidget(m_icon);
    centralLayout->setAlignment(m_icon, Qt::AlignHCenter);
    centralLayout->addWidget(m_centerTips);
    centralLayout->addWidget(m_bottomTips);
    centralLayout->addWidget(m_spinner);
    centralLayout->setAlignment(m_spinner, Qt::AlignHCenter);
    centralLayout->setSpacing(0);
    centralLayout->setContentsMargins(0, 60, 0, 40);

    setLayout(centralLayout);
}

void ScanningPage::startScan()
{
    FSCheckThread *thrd = new FSCheckThread;

    connect(thrd, &FSCheckThread::finished, this, &ScanningPage::onScanFinsihed);
    connect(thrd, &FSCheckThread::finished, thrd, &FSCheckThread::deleteLater, Qt::QueuedConnection);

    thrd->start();
}

void ScanningPage::onScanFinsihed()
{
    emit scanDone();
}
