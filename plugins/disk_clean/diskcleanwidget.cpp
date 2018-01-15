#include "diskcleanwidget.h"

#include <QPainter>
#include <QHBoxLayout>
#include <QDebug>
#include <QApplication>

DiskCleanWidget::DiskCleanWidget(QWidget *parent)
    : QWidget(parent)

    , m_icon(new QLabel)
    , m_tips(new QLabel)
    , m_statusTips(new QLabel)
    , m_diskSelectBox(new QComboBox)
    , m_cleanButton(new QPushButton)
    , m_cancelButton(new QPushButton)
    , m_okButton(new QPushButton)
{
    m_icon->setPixmap(QIcon(":/resources/repair_deepindisk.svg").pixmap(128, 128));
    m_icon->setAlignment(Qt::AlignHCenter);
    m_icon->setObjectName("PluginIcon");
    m_tips->setAlignment(Qt::AlignHCenter);
    m_tips->setText(tr("System partition"));
    m_statusTips->setAlignment(Qt::AlignHCenter);
    m_cleanButton->setText(tr("Clean"));
    m_cancelButton->setText(tr("Cancel"));
    m_cancelButton->setVisible(false);
    m_okButton->setText(tr("Finish"));
    m_okButton->setVisible(false);

    QHBoxLayout *buttonsLayout = new QHBoxLayout;
    buttonsLayout->addWidget(m_cleanButton);
    buttonsLayout->setAlignment(m_cleanButton, Qt::AlignHCenter);
    buttonsLayout->addWidget(m_cancelButton);
    buttonsLayout->setAlignment(m_cancelButton, Qt::AlignHCenter);
    buttonsLayout->addWidget(m_okButton);
    buttonsLayout->setAlignment(m_okButton, Qt::AlignHCenter);

    QVBoxLayout *centralLayout = new QVBoxLayout;
    centralLayout->addWidget(m_icon);
    centralLayout->addWidget(m_tips);
    centralLayout->addWidget(m_diskSelectBox);
    centralLayout->setAlignment(m_diskSelectBox, Qt::AlignHCenter);
    centralLayout->addStretch();
    centralLayout->addWidget(m_statusTips);
    centralLayout->addSpacing(10);
    centralLayout->addLayout(buttonsLayout);
    centralLayout->setSpacing(0);
    centralLayout->setContentsMargins(0, 0, 0, 0);

    setLayout(centralLayout);

    connect(m_cleanButton, &QPushButton::clicked, this, &DiskCleanWidget::cleanStart);
    connect(m_cancelButton, &QPushButton::clicked, this, &DiskCleanWidget::cleanCancel);
    connect(m_okButton, &QPushButton::clicked, this, &DiskCleanWidget::reset);
}

void DiskCleanWidget::setToolsProxy(RepairToolsProxy *proxy)
{
    m_toolsProxy = proxy;

    // init disk info list
    for (const auto diskInfo : m_toolsProxy->diskInfos())
    {
        const QString &os_name = diskInfo.osName;
        const QString &disk = diskInfo.diskPath;

        if (!os_name.contains("deepin", Qt::CaseInsensitive))
            continue;
        m_diskSelectBox->addItem(tr("%1 (on %2)").arg(os_name).arg(disk), QVariant::fromValue(diskInfo));
    }

    m_diskSelectBox->setVisible(m_diskSelectBox->count() > 1);
    refreshDiskSize();
}

void DiskCleanWidget::showEvent(QShowEvent *e)
{
    QWidget::showEvent(e);

    m_cleanButton->setFocus();
}

void DiskCleanWidget::reset()
{
    m_statusTips->clear();
    m_cleanButton->setVisible(true);
    m_cancelButton->setVisible(false);
    m_okButton->setVisible(false);
}

void DiskCleanWidget::cleanStart()
{
    m_toolsProxy->registerExclusive(true);

    m_statusTips->clear();
    m_statusTips->setStyleSheet("QLabel { color: black;} ");
    m_cleanButton->setVisible(false);
    m_cancelButton->setVisible(true);

    m_worker = new DiskCleanThread;
    m_worker->setToolsProxy(m_toolsProxy);
    m_worker->setCleanInfo(m_diskSelectBox->currentData().value<DiskInfo>());

    connect(m_worker, &DiskCleanThread::finished, m_worker, &DiskCleanThread::deleteLater, Qt::QueuedConnection);
    connect(m_worker, &DiskCleanThread::processDone, this, &DiskCleanWidget::cleanEnd);
    connect(m_worker, &DiskCleanThread::processDisk, this, [=](const QString &disk) { m_statusTips->setText(tr("Cleaning %1").arg(disk)); });

    m_worker->start();
}

void DiskCleanWidget::cleanCancel()
{
    m_toolsProxy->registerExclusive(false);

    m_worker->terminate();
    m_worker->wait();

    reset();
}

void DiskCleanWidget::refreshDiskSize()
{
    const auto sh = "/usr/lib/deepin-repair-tools/plugins/disk-clean/disk_size.sh";
    const DiskInfo &diskInfo = m_diskSelectBox->currentData().value<DiskInfo>();

    const RunResult &r = m_toolsProxy->execAsChrootSynchronous("/", sh, QStringList() << diskInfo.diskPath);
    const QStringList &output = r.standardOutput.split(' ');

    unsigned used = 0;
    unsigned size = 0;
    if (output.size() == 2)
    {
        used = output[0].toUInt();
        size = output[1].toUInt();
    }
}

void DiskCleanWidget::cleanEnd(const quint64 clearedSize)
{
    m_toolsProxy->registerExclusive(false);

    m_cancelButton->setVisible(false);
    m_okButton->setVisible(true);

    const double sizeMb = double(clearedSize) / 1024 / 1024;
    const QString sizeStr = QString("%1MB").arg(sizeMb, 0, 'f', 2);

    m_statusTips->setText(tr("%1 has been cleaned up").arg(sizeStr));
    m_statusTips->setStyleSheet("QLabel { color: green;} ");
}
