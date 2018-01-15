#include "diskcleanwidget.h"

#include <QPainter>
#include <QHBoxLayout>
#include <QDebug>
#include <QApplication>

DiskCleanWidget::DiskCleanWidget(QWidget *parent)
    : QWidget(parent)

    , m_icon(new QLabel)
    , m_tips(new QLabel)
    , m_cleanButton(new QPushButton)
    , m_cancelButton(new QPushButton)
    , m_okButton(new QPushButton)
{
    m_icon->setPixmap(QIcon(":/resources/repair_deepindisk.svg").pixmap(128, 128));
    m_icon->setAlignment(Qt::AlignHCenter);
    m_icon->setObjectName("PluginIcon");
    m_tips->setAlignment(Qt::AlignHCenter);
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
    centralLayout->addStretch();
    centralLayout->addWidget(m_tips);
    centralLayout->addSpacing(10);
    centralLayout->addLayout(buttonsLayout);
    centralLayout->setSpacing(0);
    centralLayout->setContentsMargins(0, 0, 0, 0);

    setLayout(centralLayout);

    connect(m_cleanButton, &QPushButton::clicked, this, &DiskCleanWidget::cleanStart);
    connect(m_cancelButton, &QPushButton::clicked, this, &DiskCleanWidget::cleanCancel);
    connect(m_okButton, &QPushButton::clicked, this, &DiskCleanWidget::reset);
}

void DiskCleanWidget::showEvent(QShowEvent *e)
{
    QWidget::showEvent(e);

    m_cleanButton->setFocus();
}

void DiskCleanWidget::reset()
{
    m_tips->clear();
    m_cleanButton->setVisible(true);
    m_cancelButton->setVisible(false);
    m_okButton->setVisible(false);
}

void DiskCleanWidget::cleanStart()
{
    m_toolsProxy->registerExclusive(true);

    m_tips->setText(tr("Cleaning %1"));
    m_tips->setStyleSheet("QLabel { color: black;} ");
    m_cleanButton->setVisible(false);
    m_cancelButton->setVisible(true);

    m_worker = new DiskCleanThread;
    m_worker->setToolsProxy(m_toolsProxy);
    for (const auto &info : m_toolsProxy->diskInfos())
    {
        if (info.osName.contains("deepin", Qt::CaseInsensitive))
            m_worker->appendDir(info);
    }

    connect(m_worker, &DiskCleanThread::finished, m_worker, &DiskCleanThread::deleteLater, Qt::QueuedConnection);
    connect(m_worker, &DiskCleanThread::processDone, this, &DiskCleanWidget::cleanEnd);

    m_worker->start();
}

void DiskCleanWidget::cleanCancel()
{
    m_toolsProxy->registerExclusive(false);

    m_worker->terminate();
    m_worker->wait();

    reset();
}

void DiskCleanWidget::cleanEnd(const quint64 clearedSize)
{
    m_toolsProxy->registerExclusive(false);

    m_cancelButton->setVisible(false);
    m_okButton->setVisible(true);

    const double sizeMb = double(clearedSize) / 1024 / 1024;
    const QString sizeStr = QString("%1MB").arg(sizeMb, 0, 'f', 2);

    m_tips->setText(tr("%1 has been cleaned up").arg(sizeStr));
    m_tips->setStyleSheet("QLabel { color: green;} ");
}
