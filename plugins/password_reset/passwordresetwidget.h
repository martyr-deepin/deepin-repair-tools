#ifndef PASSWORDRESETWIDGET_H
#define PASSWORDRESETWIDGET_H

#include "../../src/repairtoolsproxy.h"

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QComboBox>

#include <dpasswordedit.h>

DWIDGET_USE_NAMESPACE

class PasswordResetWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PasswordResetWidget(QWidget *parent = nullptr);

    void setToolsProxy(RepairToolsProxy *proxy) { m_toolsProxy = proxy; }

protected:
    void showEvent(QShowEvent *e);

private slots:
    void onPasswordChanged(const QString &pwd) { m_confirmButton->setEnabled(!pwd.isEmpty()); }
    void initUserInfo();
    void resetUI();
    void resetUserPassword();
    void onResetClicked();
    void onPasswdSubmitted();
    void onResetPasswordFinished();

private:
    RepairToolsProxy *m_toolsProxy;
    QLabel *m_icon;
    QLabel *m_tips;
    QLabel *m_status;
    QPushButton *m_resetButton;
    QPushButton *m_confirmButton;
    QPushButton *m_okButton;
    QComboBox *m_userChooseBox;
    DPasswordEdit *m_passwordEdit;
    DPasswordEdit *m_passwordRepeat;
};

#endif // PASSWORDRESETWIDGET_H
