#include "pluginnavgation.h"

#include <QPainter>

PluginNavgation::PluginNavgation(QWidget *parent)
    : QPushButton(parent)
{

}

void PluginNavgation::paintEvent(QPaintEvent *e)
{
    QPushButton::paintEvent(e);

    if (!isChecked())
        return;

    // draw right side border
    QRect r = rect();
    r.setLeft(r.right() - 2);

    QPainter painter(this);
    painter.fillRect(r, QColor(44, 167, 248));
}
