#include "toolspage.h"
#include "chrootbindguard.h"
#include "chrootprocess.h"

#include <QTimer>
#include <QHBoxLayout>
#include <QPushButton>
#include <QDebug>
#include <QApplication>
#include <QProcess>
#include <QThread>

const QString chroot_hook_script = "/usr/lib/deepin-repair-tools/chroot_hook.sh";

const RunResult execAsChrootSynchronous(const QString &root, const QString &script, const QStringList &args)
{
    QProcess &process = *execAsChrootAsynchronous(root, script, args);
    process.start();
    process.waitForFinished(-1);

    const RunResult r { process.exitCode(), process.readAllStandardOutput(), process.readAllStandardError() };

    qInfo().noquote() << Q_FUNC_INFO
                      << "exitCode =" << r.exitCode
                      << "output =" << r.standardOutput
                      << "error =" << r.standardError;

    process.deleteLater();

    return r;
}

QProcess* execAsChrootAsynchronous(const QString &root, const QString &script, const QStringList &args)
{
    qInfo().noquote() << Q_FUNC_INFO
                      << root
                      << script
                      << args;

    ChrootProcess *process = new ChrootProcess(root);
    process->setProgram("/bin/sh");
    process->setArguments(QStringList() << chroot_hook_script << root << script << args);

    return process;
}

ToolsPage::ToolsPage(QWidget *parent)
    : QWidget(parent)
    , m_navButtonsGroup(new QButtonGroup)
    , m_navigationLayout(new QVBoxLayout)
    , m_pluginsLayout(new QStackedLayout)
{
    m_navButtonsGroup->setExclusive(true);
    m_navigationLayout->setMargin(0);
    m_navigationLayout->setSpacing(10);

    QWidget *navWidget = new QFrame;
    navWidget->setObjectName("NavWidget");
    navWidget->setLayout(m_navigationLayout);

    QWidget *pluginWidget = new QWidget;
    pluginWidget->setObjectName("PluginWidget");
    pluginWidget->setLayout(m_pluginsLayout);

    QHBoxLayout *centralLayout = new QHBoxLayout;
    centralLayout->addWidget(navWidget);
    centralLayout->addWidget(pluginWidget);
    centralLayout->setSpacing(0);
    centralLayout->setMargin(0);

    setLayout(centralLayout);
    setStyleSheet("#NavWidget {"
                  "border: solid #eee;"
                  "border-right-width: 1px;"
                  "}"
                  ""
                  "#NavWidget > QPushButton {"
                  "margin: 0;"
                  "text-align: left;"
                  "padding: 10px 15px;"
                  "border: none;"
                  "border-right: 3px solid transparent;"
                  "}"
                  ""
                  "#NavWidget > QPushButton:checked {"
                  "color: #1779bd;"
                  "background-color: #d5edfe;"
                  "border: 1px solid #72bef4;"
                  "border-right: 3px solid #279bed;"
                  "border-left: none;"
                  "}"
                  ""
                  "#PluginWidget QComboBox {"
                  "width: 180px;"
                  "}"
                  ""
                  "#PluginWidget QPushButton {"
//                  "padding: 10px 0;"
                  "width: 180px;"
                  "color: #1779bd;"
                  "}"
//                  ""
                  "#PluginWidget QPushButton:hover {"
                  "color: white;"
                  "}"
                  ""
                  "#PluginWidget QLineEdit {"
                  "width: 210px;"
                  "height: 25px;"
                  "}"
                  "");
}

ToolsPage::~ToolsPage()
{

}

void ToolsPage::setModel(const QPointer<ToolsModel> &model)
{
    m_model = model;

    connect(model, &ToolsModel::pluginsLoadFinished, this, &ToolsPage::refreshNavBar);

    m_model->initPlugins(this);
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

    // FIXME: select first page for default
    m_navButtonsGroup->buttons().first()->click();
}

void ToolsPage::onNavButtonClicked(const int index)
{
    QWidget *w = m_model->data(m_model->index(index), ToolsModel::ToolsWidget).value<QWidget *>();
    Q_ASSERT(w);

    m_pluginsLayout->insertWidget(0, w);
    m_pluginsLayout->setCurrentWidget(w);
}
