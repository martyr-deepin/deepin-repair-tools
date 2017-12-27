#include "repairtools.h"
#include "toolspage.h"

#include <QApplication>
#include <QScreen>
#include <QKeyEvent>

#include <DTitlebar>

RepairTools::RepairTools(QWidget *parent)
    : DMainWindow(parent)
    , m_diskUtils(new DiskUtils(this))
    , m_toolsModel(new ToolsModel(this))
{
    m_diskUtils->initilize();

    auto *tbar = titlebar();
    tbar->setTitle(QString());

    resize(800, 600);
    move(qApp->primaryScreen()->geometry().center() - rect().center());

    ToolsPage *p = new ToolsPage;
    p->setDiskUtils(m_diskUtils);
    p->setModel(m_toolsModel);

    setCentralWidget(p);
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
