#ifndef PASSWORDRESETWIDGET_H
#define PASSWORDRESETWIDGET_H

#include <QWidget>

class PasswordResetWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PasswordResetWidget(QWidget *parent = nullptr);

private:
    void paintEvent(QPaintEvent *e);
};

#endif // PASSWORDRESETWIDGET_H
