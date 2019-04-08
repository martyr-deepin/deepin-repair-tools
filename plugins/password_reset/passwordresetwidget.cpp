#include "passwordresetwidget.h"
#include "passwordresetthread.h"

#include <QVBoxLayout>
#include <QTimer>
#include <QDebug>
#include <QApplication>

PasswordResetWidget::PasswordResetWidget(QWidget *parent)
    : QWidget(parent)

    , m_icon(new QLabel)
    , m_tips(new QLabel)
    , m_status(new QLabel)
    , m_resetButton(new QPushButton)
    , m_confirmButton(new QPushButton)
    , m_okButton(new QPushButton)
    , m_userChooseBox(new QComboBox)
    , m_passwordEdit(new DPasswordEdit)
    , m_passwordRepeat(new DPasswordEdit)
{
    m_icon->setPixmap(QIcon(":/resources/repair_password.svg").pixmap(128, 128));
    m_icon->setAlignment(Qt::AlignHCenter);
    m_icon->setObjectName("PluginIcon");
    m_tips->setAlignment(Qt::AlignHCenter);
    m_tips->setObjectName("TipsLabel");
    m_status->setAlignment(Qt::AlignHCenter);
    m_resetButton->setText(tr("Reset password"));
    m_confirmButton->setText(tr("Confirm"));
    m_okButton->setText(tr("Done"));
    m_passwordEdit->setPlaceholderText(tr("Password"));
    m_passwordRepeat->setPlaceholderText(tr("Confirm password"));

    QHBoxLayout *btnsLayout = new QHBoxLayout;
    btnsLayout->addStretch();
    btnsLayout->addWidget(m_resetButton);
    btnsLayout->addWidget(m_confirmButton);
    btnsLayout->addWidget(m_okButton);
    btnsLayout->addStretch();

    QVBoxLayout *centralLayout = new QVBoxLayout;
    centralLayout->addWidget(m_icon);
    centralLayout->addSpacing(10);
    centralLayout->addWidget(m_tips);
    centralLayout->addSpacing(5);
    centralLayout->addWidget(m_userChooseBox);
    centralLayout->setAlignment(m_userChooseBox, Qt::AlignHCenter);
    centralLayout->addSpacing(5);
    centralLayout->addWidget(m_passwordEdit);
    centralLayout->setAlignment(m_passwordEdit, Qt::AlignHCenter);
    centralLayout->addSpacing(5);
    centralLayout->addWidget(m_passwordRepeat);
    centralLayout->setAlignment(m_passwordRepeat, Qt::AlignHCenter);
    centralLayout->addStretch();
    centralLayout->addWidget(m_status);
    centralLayout->addSpacing(10);
    centralLayout->addLayout(btnsLayout);
    centralLayout->setSpacing(0);
    centralLayout->setContentsMargins(0, 0, 0, 0);

    setLayout(centralLayout);

    connect(m_resetButton, &QPushButton::clicked, this, &PasswordResetWidget::onResetClicked);
    connect(m_confirmButton, &QPushButton::clicked, this, &PasswordResetWidget::onPasswdSubmitted);
    connect(m_okButton, &QPushButton::clicked, this, &PasswordResetWidget::resetUI);
    connect(m_passwordEdit, &DPasswordEdit::textEdited, this, &PasswordResetWidget::onPasswordChanged);

    QTimer::singleShot(1, this, &PasswordResetWidget::initUserInfo);
    QTimer::singleShot(1, this, &PasswordResetWidget::resetUI);
}

void PasswordResetWidget::showEvent(QShowEvent *e)
{
    QWidget::showEvent(e);

    m_resetButton->setFocus();
}

void PasswordResetWidget::onResetClicked()
{
    m_tips->setText(tr("Please enter a new password"));
    m_resetButton->setVisible(false);
    m_confirmButton->setVisible(true);

    m_passwordEdit->setVisible(true);
    m_passwordRepeat->setVisible(true);

    if (m_userChooseBox->count() > 1)
        m_userChooseBox->setVisible(true);
}

void PasswordResetWidget::onPasswdSubmitted()
{
    const QString &pwd1 = m_passwordEdit->text();
    const QString &pwd2 = m_passwordRepeat->text();
    if (pwd1 != pwd2)
        return m_passwordRepeat->showAlertMessage(tr("Passwords do not match"));

    m_confirmButton->setVisible(false);
    m_okButton->setVisible(false);
    m_tips->setVisible(false);

    m_userChooseBox->setVisible(false);
    m_passwordEdit->setVisible(false);
    m_passwordRepeat->setVisible(false);

    resetUserPassword();
}

void PasswordResetWidget::resetUserPassword()
{
    const auto data = m_userChooseBox->currentData().value<QPair<QString, QString>>();

    PasswordResetThread *thrd = new PasswordResetThread;
    thrd->setToolsProxy(m_toolsProxy);
    // Base64 encoding password to escape characters
    const QString encoded_password = m_passwordEdit->text().toUtf8().toBase64();
    thrd->setTaskInfo(data.first, data.second, encoded_password);

    connect(thrd, &PasswordResetThread::finished, thrd, &PasswordResetThread::deleteLater, Qt::QueuedConnection);
    connect(thrd, &PasswordResetThread::finished, this, &PasswordResetWidget::onResetPasswordFinished);

    thrd->start();
}

void PasswordResetWidget::onResetPasswordFinished()
{
    m_okButton->setVisible(true);
    m_status->setVisible(true);
    m_status->setText(tr("Reset password successfully"));
    m_status->setStyleSheet("color: #3da219;");
}

void PasswordResetWidget::initUserInfo()
{
    for (const auto &diskInfo : m_toolsProxy->diskInfos())
    {
        if (diskInfo.userList.isEmpty())
            continue;

        const QString &disk = diskInfo.diskPath;
        for (const auto &userInfo : diskInfo.userList)
        {
            const QString &text = tr("%1 (on %2)").arg(userInfo.name).arg(disk);
            const QVariant &data = QVariant::fromValue(QPair<QString, QString>(diskInfo.mountPoint, userInfo.name));
            m_userChooseBox->addItem(text, data);
        }
    }
}

void PasswordResetWidget::resetUI()
{
    m_passwordEdit->clear();
    m_passwordRepeat->clear();

    m_tips->setText(tr("Forgot login password"));
    m_tips->setVisible(true);
    m_status->setVisible(false);
    m_resetButton->setVisible(true);
    m_confirmButton->setVisible(false);
    m_okButton->setVisible(false);
    m_userChooseBox->setVisible(false);
    m_passwordEdit->setVisible(false);
    m_passwordRepeat->setVisible(false);

    onPasswordChanged(QString());
}
