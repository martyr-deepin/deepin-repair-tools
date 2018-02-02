#include "repairtools.h"
#include "toolspage.h"
#include "scanningpage.h"

#include <QApplication>
#include <QScreen>
#include <QKeyEvent>
#include <QDebug>
#include <QDesktopServices>

#include <DTitlebar>

#include <pwd.h>

RepairTools::RepairTools(QWidget *parent)
    : DMainWindow(parent)
    , m_diskUtils(new DiskUtils(this))
    , m_toolsModel(new ToolsModel(this))
{
    auto *tbar = titlebar();
    tbar->setTitle(QString());
    tbar->setIcon(QIcon::fromTheme("deepin-repair-tools"));
    tbar->setBackgroundTransparent(true);

    ScanningPage *sp = new ScanningPage;
    sp->setDiskUtils(m_diskUtils);

    setFixedSize(700, 450);
    move(qApp->primaryScreen()->geometry().center() - rect().center());
    setCentralWidget(sp);

    connect(m_diskUtils, &DiskUtils::scanFinished, sp, &ScanningPage::startScan);
    connect(sp, &ScanningPage::scanDone, this, &RepairTools::onScanDone);

    m_diskUtils->initilize();
}

bool RepairTools::onLinkClicked(const QUrl &link)
{
    struct passwd *pwd = getpwuid(qEnvironmentVariableIntValue("PKEXEC_UID"));

    QProcess proc;
    proc.setProgram("bash");
    proc.setArguments(QStringList() << "/usr/lib/deepin-repair-tools/open_link.sh" << link.toString() << pwd->pw_name);
    proc.start();
    proc.waitForFinished();

    return true;
}

void RepairTools::keyPressEvent(QKeyEvent *e)
{
    switch (e->key())
    {
#ifdef QT_DEBUG
    case Qt::Key_Escape:    qApp->quit();       break;
#endif
    default:;
    }

    return QWidget::keyPressEvent(e);
}

void RepairTools::onScanDone()
{
    delete centralWidget();

    ToolsPage *p = new ToolsPage;
    p->setDiskUtils(m_diskUtils);
    p->setModel(m_toolsModel);

    setCentralWidget(p);
}
