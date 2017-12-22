#ifndef PASSWORDRESETWIDGET_H
#define PASSWORDRESETWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>

#include <dpasswordedit.h>

DWIDGET_USE_NAMESPACE

class PasswordResetWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PasswordResetWidget(QWidget *parent = nullptr);

private:
    void onResetClicked();

private:
    QLabel *m_normalTips;
    QLabel *m_successTips;
    QPushButton *m_resetButton;
    QPushButton *m_confirmButton;
    QPushButton *m_okButton;
    DPasswordEdit *m_passwordEdit;
    DPasswordEdit *m_passwordRepeat;
};

#endif // PASSWORDRESETWIDGET_H
