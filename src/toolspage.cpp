#include "toolspage.h"
#include "chrootbindguard.h"
#include "chrootprocess.h"
#include "pluginnavgation.h"

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
    centralLayout->setContentsMargins(0, 0, 0, 40);

    setLayout(centralLayout);
    setStyleSheet("#NavWidget {"
                  "border: solid #eee;"
                  "border-right-width: 1px;"
                  "}"
                  ""
                  "#NavWidget > QPushButton {"
                  "margin: 0;"
                  "text-align: left;"
                  "padding: 5px 15px;"
                  "border: none;"
                  "}"
                  ""
                  "#NavWidget > QPushButton:checked {"
                  "color: #2ca7f8;"
                  "font-weight: 500;"
                  "background-color: #d5edfe;"
                  "border: 1px solid rgba(44, 167, 248, .1);"
                  "border-left: none;"
                  "border-right: none;"
                  "}"
                  ""
                  "#PluginWidget QComboBox {"
                  "width: 180px;"
                  "}"
                  ""
                  "#PluginWidget QPushButton {"
                  "width: 152px;"
                  "height: 26px;"
                  "background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #FFFFFF, stop:1 #FBFBFB);"
                  "border-color: rgba(0, 131, 255, 0.4);"
                  "border-radius: 4;"
                  "border-style: solid;"
                  "border-width: 1;"
                  "outline: none;"
                  "padding: 3px 24px 4px 24px;"
                  "color: #0699ff;"
                  "font-size: 14px;"
                  "}"
                  ""
                  "#PluginWidget QPushButton:hover {"
                  "background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #8ccfff, stop:1 #4bb8ff);"
                  "color: #FFFFFF;"
                  "border-color: rgba(0, 117, 243, 0.2);"
                  "}"
                  ""
                  "#PluginWidget QPushButton:pressed {"
                  "background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #0b8cff, stop:1 #0aa1ff);"
                  "color: #FFFFFF;"
                  "border-color: rgba(29, 129, 255, 0.3);"
                  "}"
                  ""
                  "#PluginWidget QPushButton:disabled {"
                  "border-color: rgba(0, 131, 255, 0.2);"
                  "color: rgba(6, 153, 255, .4);"
                  "}"
                  ""
                  "#PluginWidget QLineEdit {"
                  "width: 210px;"
                  "height: 25px;"
                  "}"
                  ""
                  "#PluginWidget #TipsLabel {"
                  "font-size: 9pt;"
                  "font-weight: 500;"
                  "}"
                  ""
                  "#PluginWidget Dtk--Widget--DLinkButton {"
                  "font-size: 12px;"
                  "}"
                  ""
                  "#PluginWidget QLabel#PluginIcon {"
                  "margin: 70px 0 0 0;"
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
        QPushButton *b = new PluginNavgation;
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

void ToolsPage::setNavgationEnabled(const bool enabled)
{
    for (auto *button : m_navButtonsGroup->buttons())
        button->setEnabled(enabled);
}

void ToolsPage::onNavButtonClicked(const int index)
{
    QWidget *w = m_model->data(m_model->index(index), ToolsModel::ToolsWidget).value<QWidget *>();
    Q_ASSERT(w);

    m_pluginsLayout->insertWidget(0, w);
    m_pluginsLayout->setCurrentWidget(w);
}
