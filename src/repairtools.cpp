#include "repairtools.h"
#include "toolspage.h"
#include "scanningpage.h"

#include <QApplication>
#include <QScreen>
#include <QKeyEvent>

#include <DTitlebar>

RepairTools::RepairTools(QWidget *parent)
    : DMainWindow(parent)
    , m_diskUtils(new DiskUtils(this))
    , m_toolsModel(new ToolsModel(this))
{
    auto *tbar = titlebar();
    tbar->setTitle(QString());
    tbar->setIcon(QIcon::fromTheme("deepin-repair-tools"));

    ScanningPage *sp = new ScanningPage;

    setFixedSize(700, 450);
    move(qApp->primaryScreen()->geometry().center() - rect().center());
    setCentralWidget(sp);

    connect(m_diskUtils, &DiskUtils::scanFinished, sp, &ScanningPage::startScan);
    connect(sp, &ScanningPage::scanDone, this, &RepairTools::onScanDone);

    m_diskUtils->initilize();
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
