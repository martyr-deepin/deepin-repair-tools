#include "diskcleanwidget.h"

#include <QPainter>

DiskCleanWidget::DiskCleanWidget(QWidget *parent)
    : QWidget(parent)
{

}

void DiskCleanWidget::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);

    painter.fillRect(rect(), Qt::green);
}
