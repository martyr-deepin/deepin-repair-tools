#include "diskcleanwidget.h"

#include <QPainter>
#include <QHBoxLayout>
#include <QDebug>

DiskCleanWidget::DiskCleanWidget(QWidget *parent)
    : QWidget(parent)
    , m_cleanButton(new QPushButton)
    , m_cancelButton(new QPushButton)
    , m_okButton(new QPushButton)
{
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
    centralLayout->addLayout(buttonsLayout);

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

    for (const auto &info : m_toolsProxy->diskInfos())
    {
        qDebug() << info.diskPath << info.mountPoint << info.format;
    }

    cleanEnd();
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
