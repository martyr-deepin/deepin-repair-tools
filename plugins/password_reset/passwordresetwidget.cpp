#include "passwordresetwidget.h"

#include <QPainter>

PasswordResetWidget::PasswordResetWidget(QWidget *parent)
    : QWidget(parent)
{

}

void PasswordResetWidget::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);

    painter.fillRect(rect(), Qt::cyan);
}
