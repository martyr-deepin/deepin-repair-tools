#include "toolspage.h"

#include <QTimer>
#include <QHBoxLayout>
#include <QPushButton>

ToolsPage::ToolsPage(QWidget *parent)
    : QWidget(parent)
    , m_navigationLayout(new QVBoxLayout)
    , m_pluginsLayout(new QStackedLayout)
    , m_bottomTips(new QLabel)
    , m_bottomButton(new QPushButton)
{
    m_bottomTips->setText("Bottom Tips");
    m_bottomTips->setAlignment(Qt::AlignHCenter);
    m_bottomButton->setText("Bottom Button");

    QVBoxLayout *rightPanelLayout = new QVBoxLayout;
    rightPanelLayout->setMargin(0);
    rightPanelLayout->setSpacing(0);
    rightPanelLayout->addLayout(m_pluginsLayout);
    rightPanelLayout->addWidget(m_bottomTips);
    rightPanelLayout->addWidget(m_bottomButton);
    rightPanelLayout->setAlignment(m_bottomButton, Qt::AlignCenter);

    QHBoxLayout *centralLayout = new QHBoxLayout;
    centralLayout->setMargin(0);
    centralLayout->setSpacing(0);
    centralLayout->addLayout(m_navigationLayout);
    centralLayout->addLayout(rightPanelLayout);

    setLayout(centralLayout);
}

void ToolsPage::setModel(const QPointer<ToolsModel> &model)
{
    m_model = model;

    QTimer::singleShot(1, this, &ToolsPage::refreshNavBar);
}

void ToolsPage::refreshNavBar()
{
    // TODO:
    // clear old widgets

    for (int i(0); i != m_model->rowCount(QModelIndex()); ++i)
    {
        QPushButton *b = new QPushButton;
        b->setText(m_model->data(m_model->index(i), ToolsModel::ToolsName).toString());
        m_navigationLayout->addWidget(b);
    }
}
