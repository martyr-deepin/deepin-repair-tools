#include "permissionsrepairwidget.h"

#include <QVBoxLayout>
#include <QDebug>

PermissionsRepairWidget::PermissionsRepairWidget(QWidget *parent)
    : QWidget(parent)

    , m_repairButton(new QPushButton)
{
    m_repairButton->setText(tr("Permissions Repair"));

    QVBoxLayout *centralLayout = new QVBoxLayout;
    centralLayout->addWidget(m_repairButton);
    centralLayout->setAlignment(m_repairButton, Qt::AlignHCenter);

    setLayout(centralLayout);

    connect(m_repairButton, &QPushButton::clicked, this, &PermissionsRepairWidget::onRepairButtonClicked);
}

void PermissionsRepairWidget::onRepairButtonClicked()
{
}
