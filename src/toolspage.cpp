#include "toolspage.h"

#include <QTimer>
#include <QHBoxLayout>
#include <QPushButton>
#include <QDebug>

ToolsPage::ToolsPage(QWidget *parent)
    : QWidget(parent)
    , m_navButtonsGroup(new QButtonGroup)
    , m_navigationLayout(new QVBoxLayout)
    , m_pluginsLayout(new QStackedLayout)
{
    m_navButtonsGroup->setExclusive(true);
    m_navigationLayout->setMargin(0);
    m_navigationLayout->setSpacing(10);

    QVBoxLayout *rightPanelLayout = new QVBoxLayout;
    rightPanelLayout->setMargin(0);
    rightPanelLayout->setSpacing(0);
    rightPanelLayout->addLayout(m_pluginsLayout);

    QWidget *navWidget = new QFrame;
    navWidget->setLayout(m_navigationLayout);
    navWidget->setStyleSheet("QFrame {"
                             "border: solid #eee;"
                             "border-right-width: 1px;"
                             "}");

    QHBoxLayout *centralLayout = new QHBoxLayout;
    centralLayout->setMargin(0);
    centralLayout->setSpacing(1);
    centralLayout->addWidget(navWidget);
    centralLayout->addLayout(rightPanelLayout);

    setLayout(centralLayout);
    setStyleSheet("QPushButton {"
                  "margin: 0;"
                  "text-align: left;"
                  "padding: 10px 15px;"
                  "border: none;"
                  "border-right: 3px solid transparent;"
                  "}"
                  ""
                  "QPushButton:checked {"
                  "color: #1779bd;"
                  "background-color: #d5edfe;"
                  "border: 1px solid #279bed;"
                  "border-right: 3px solid #279bed;"
                  "border-left: none;"
                  "}");
}

ToolsPage::~ToolsPage()
{

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

    m_navigationLayout->addStretch();
    for (int i(0); i != m_model->rowCount(QModelIndex()); ++i)
    {
        QPushButton *b = new QPushButton;
        b->setCheckable(true);
        b->setText(m_model->data(m_model->index(i), ToolsModel::ToolsName).toString());
        m_navigationLayout->addWidget(b);
        m_navButtonsGroup->addButton(b);

        connect(b, &QPushButton::clicked, this, [=] { onNavButtonClicked(i); });
    }
    m_navigationLayout->addStretch();
}

void ToolsPage::onNavButtonClicked(const int index)
{
    QWidget *w = m_model->data(m_model->index(index), ToolsModel::ToolsWidget).value<QWidget *>();
    Q_ASSERT(w);

    m_pluginsLayout->insertWidget(0, w);
    m_pluginsLayout->setCurrentWidget(w);
}
