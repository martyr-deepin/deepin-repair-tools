#include "repairtools.h"

#include <QApplication>
#include <QScreen>
#include <QKeyEvent>

RepairTools::RepairTools(QWidget *parent)
    : QWidget(parent)
{
    setFixedSize(800, 600);
    move(qApp->primaryScreen()->geometry().center() - rect().center());
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
