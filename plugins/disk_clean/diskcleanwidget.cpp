#include "diskcleanwidget.h"
#include "diskcleanthread.h"

#include <QPainter>
#include <QHBoxLayout>
#include <QDebug>

DiskCleanWidget::DiskCleanWidget(QWidget *parent)
    : QWidget(parent)

    , m_icon(new QLabel)
    , m_cleanButton(new QPushButton)
    , m_cancelButton(new QPushButton)
    , m_okButton(new QPushButton)
{
    m_icon->setPixmap(QIcon::fromTheme("drive-harddisk").pixmap(128, 128));
    m_icon->setAlignment(Qt::AlignHCenter);
    m_cleanButton->setText(tr("Clean"));
    m_cancelButton->setText(tr("Cancel"));
    m_cancelButton->setVisible(false);
    m_okButton->setText(tr("OK"));
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
    centralLayout->addLayout(buttonsLayout);
    centralLayout->setSpacing(0);
    centralLayout->setContentsMargins(0, 60, 0, 40);

    setLayout(centralLayout);

    connect(m_cleanButton, &QPushButton::clicked, this, &DiskCleanWidget::cleanStart);
    connect(m_cancelButton, &QPushButton::clicked, this, &DiskCleanWidget::cleanCancel);
    connect(m_okButton, &QPushButton::clicked, this, &DiskCleanWidget::reset);
}

void DiskCleanWidget::reset()
{
    m_cleanButton->setVisible(true);
    m_cancelButton->setVisible(false);
    m_okButton->setVisible(false);
}

void DiskCleanWidget::cleanStart()
{
    m_cleanButton->setVisible(false);
    m_cancelButton->setVisible(true);

    DiskCleanThread *thrd = new DiskCleanThread;
    thrd->setToolsProxy(m_toolsProxy);
    for (const auto &info : m_toolsProxy->diskInfos())
    {
        if (info.osName.contains("deepin", Qt::CaseInsensitive))
            thrd->appendDir(info);
    }

    connect(thrd, &DiskCleanThread::finished, thrd, &DiskCleanThread::deleteLater);
    connect(thrd, &DiskCleanThread::finished, this, &DiskCleanWidget::cleanEnd);
    connect(thrd, &DiskCleanThread::processInfo, this, [&](const QString &info) { qDebug() << info; });

    thrd->start();
}

void DiskCleanWidget::cleanCancel()
{
    m_cancelButton->setVisible(false);
    m_cleanButton->setVisible(true);
}

void DiskCleanWidget::cleanEnd()
{
    m_cancelButton->setVisible(false);
    m_okButton->setVisible(true);
}
