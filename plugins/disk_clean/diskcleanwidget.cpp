#include "diskcleanwidget.h"

#include <QPainter>
#include <QHBoxLayout>
#include <QDebug>
#include <QApplication>

QString formatDiskSize(const quint64 sizeInByte)
{
    QString ret;
    QString unit;

    if (sizeInByte < 1024)
    {
        return QString("%1B").arg(sizeInByte);
    }
    else if (sizeInByte < 1024ull * 1024)
    {
        ret = QString("%1").arg(double(sizeInByte) / 1024, 0, 'f', 1);
        unit = "KB";
    }
    else if (sizeInByte < 1024ull * 1024 * 1024)
    {
        ret = QString("%1").arg(double(sizeInByte) / 1024 / 1024, 0, 'f', 1);
        unit = "MB";
    }
    else if (sizeInByte < 1024ull * 1024 * 1024 * 1024)
    {
        ret = QString("%1").arg(double(sizeInByte) / 1024 / 1024 / 1024, 0, 'f', 1);
        unit = "GB";
    }
    else
    {
        ret = QString("%1").arg(double(sizeInByte) / 1024 / 1024 / 1024 / 1024, 0, 'f', 1);
        unit = "TB";
    }

    ret.remove(QRegularExpression("0+$"));
    ret.remove(QRegularExpression("\\.$"));

    return ret + unit;
}

DiskCleanWidget::DiskCleanWidget(QWidget *parent)
    : QWidget(parent)

    , m_icon(new QLabel)
    , m_tips(new QLabel)
    , m_sysInfo(new QLabel)
    , m_capacity(new QLabel)
    , m_statusTips(new QLabel)
    , m_capacityBar(new QSlider(Qt::Horizontal))
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
    m_okButton->setText(tr("Done"));
    m_okButton->setVisible(false);
    m_diskSelectBox->setFixedHeight(25);
    m_capacityBar->setFixedHeight(4);
    m_capacityBar->setMinimum(0);
    m_capacityBar->setMaximum(1000);
    m_capacityBar->setPageStep(1);
    m_capacityBar->setEnabled(false);

    QHBoxLayout *buttonsLayout = new QHBoxLayout;
    buttonsLayout->addStretch();
    buttonsLayout->addWidget(m_cleanButton);
    buttonsLayout->addWidget(m_cancelButton);
    buttonsLayout->addWidget(m_okButton);
    buttonsLayout->addStretch();

    QHBoxLayout *infoTextLayout = new QHBoxLayout;
    infoTextLayout->addWidget(m_sysInfo);
    infoTextLayout->addStretch();
    infoTextLayout->addWidget(m_capacity);

    QVBoxLayout *infoLayout = new QVBoxLayout;
    infoLayout->addLayout(infoTextLayout);
    infoLayout->addWidget(m_capacityBar);
    infoLayout->addWidget(m_diskSelectBox);

    QWidget *infoWidget = new QWidget;
    infoWidget->setFixedWidth(210);
    infoWidget->setLayout(infoLayout);
    infoWidget->setStyleSheet("QLabel {"
                              "color: #aaa;"
                              "font-size: 7pt;"
                              "}"
                              ""
                              "QComboBox {"
                              "font-size: 8pt;"
                              "}"
                              ""
                              "QSlider {"
                              "border: none;"
                              "}"
                              ""
                              "QSlider::groove {"
                              "background-color: rgba(0, 0, 0, 0.05);"
                              "border: 1px solid rgba(0, 0, 0, 0.1);"
                              "border-radius: 2px;"
                              "}"
                              ""
                              "QSlider::sub-page {"
                              "background-color: rgba(255, 139, 35, 0.8);"
                              "border-width: 0px;"
                              "border-radius: 2px;"
                              "}");

    QVBoxLayout *centralLayout = new QVBoxLayout;
    centralLayout->addWidget(m_icon);
    centralLayout->addWidget(m_tips);
    centralLayout->addWidget(infoWidget);
    centralLayout->setAlignment(infoWidget, Qt::AlignHCenter);
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
    connect(m_diskSelectBox, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &DiskCleanWidget::refreshDiskInfo);
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

    const int count = m_diskSelectBox->count();
    m_diskSelectBox->setVisible(count > 1);
    if (count)
        m_diskSelectBox->setCurrentIndex(0);
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

void DiskCleanWidget::refreshDiskInfo()
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

    m_capacityBar->setValue(double(used) / size * m_capacityBar->maximum());

    m_sysInfo->setText(QString("%1(%2)").arg(diskInfo.osName).arg(diskInfo.diskPath));
    m_capacity->setText(QString("%1/%2G").arg(used / 1024 / 1024).arg(size / 1024 / 1024));
}

void DiskCleanWidget::cleanEnd(const quint64 clearedSize)
{
    m_toolsProxy->registerExclusive(false);

    m_cancelButton->setVisible(false);
    m_okButton->setVisible(true);

    const QString sizeStr = formatDiskSize(clearedSize);

    m_statusTips->setText(tr("%1 has been cleaned up").arg(sizeStr));
    m_statusTips->setStyleSheet("QLabel { color: #3da219;} ");

    refreshDiskInfo();
}
