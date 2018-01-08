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

private slots:
    void initUserInfo();
    void resetUI();
    void resetUserPassword();
    void onResetClicked();
    void onPasswdSubmitted();
    void onResetPasswordFinished();

private:
    inline void onPasswordEmpty() { showErrorTips(m_passwordEdit, tr("Password can't be empty")); }
    inline void onPasswordRepeatEmpty() { showErrorTips(m_passwordRepeat, tr("Password can't be empty")); }
    inline void onPasswordNotMatch() { showErrorTips(m_passwordRepeat, tr("Password not match")); }
    inline void showErrorTips(DLineEdit * const edit, const QString &text) { edit->showAlertMessage(text); }

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
