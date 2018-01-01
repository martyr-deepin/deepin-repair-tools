#include "scanningpage.h"

#include <QVBoxLayout>

ScanningPage::ScanningPage(QWidget *parent)
    : QWidget(parent)

    , m_centerTips(new QLabel)
    , m_bottomTips(new QLabel)
{
    m_centerTips->setText(tr("Scanning..."));
    m_centerTips->setAlignment(Qt::AlignCenter);
    m_bottomTips->setText(tr("Broken..."));
    m_bottomTips->setAlignment(Qt::AlignCenter);
    m_bottomTips->setVisible(false);

    QVBoxLayout *centralLayout = new QVBoxLayout;
    centralLayout->addWidget(m_centerTips);
    centralLayout->addWidget(m_bottomTips);

    setLayout(centralLayout);
}
