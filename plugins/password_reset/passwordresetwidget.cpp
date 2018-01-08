#include "passwordresetwidget.h"
#include "passwordresetthread.h"

#include <QVBoxLayout>
#include <QTimer>
#include <QDebug>

PasswordResetWidget::PasswordResetWidget(QWidget *parent)
    : QWidget(parent)

    , m_icon(new QLabel)
    , m_normalTips(new QLabel)
    , m_successTips(new QLabel)
    , m_resetButton(new QPushButton)
    , m_confirmButton(new QPushButton)
    , m_okButton(new QPushButton)
    , m_userChooseBox(new QComboBox)
    , m_passwordEdit(new DPasswordEdit)
    , m_passwordRepeat(new DPasswordEdit)
{
    m_icon->setPixmap(QIcon(":/resources/repair_password.svg").pixmap(128, 128));
    m_icon->setAlignment(Qt::AlignHCenter);
    m_resetButton->setText(tr("Password Reset"));
    m_confirmButton->setText(tr("Confirm"));
    m_okButton->setText(tr("OK"));

    QHBoxLayout *btnsLayout = new QHBoxLayout;
    btnsLayout->addWidget(m_resetButton);
    btnsLayout->setAlignment(m_resetButton, Qt::AlignHCenter);
    btnsLayout->addWidget(m_confirmButton);
    btnsLayout->setAlignment(m_confirmButton, Qt::AlignHCenter);
    btnsLayout->addWidget(m_okButton);
    btnsLayout->setAlignment(m_okButton, Qt::AlignHCenter);

    QVBoxLayout *centralLayout = new QVBoxLayout;
    centralLayout->addWidget(m_icon);
    centralLayout->addWidget(m_userChooseBox);
    centralLayout->setAlignment(m_userChooseBox, Qt::AlignHCenter);
    centralLayout->addWidget(m_passwordEdit);
    centralLayout->setAlignment(m_passwordEdit, Qt::AlignHCenter);
    centralLayout->addWidget(m_passwordRepeat);
    centralLayout->setAlignment(m_passwordRepeat, Qt::AlignHCenter);
    centralLayout->addLayout(btnsLayout);
    centralLayout->setSpacing(0);
    centralLayout->setContentsMargins(0, 60, 0, 40);

    setLayout(centralLayout);
    resetUI();

    connect(m_resetButton, &QPushButton::clicked, this, &PasswordResetWidget::onResetClicked);
    connect(m_confirmButton, &QPushButton::clicked, this, &PasswordResetWidget::onPasswdSubmitted);
    connect(m_okButton, &QPushButton::clicked, this, &PasswordResetWidget::resetUI);

    QTimer::singleShot(1, this, &PasswordResetWidget::initUserInfo);
}

void PasswordResetWidget::onResetClicked()
{
    m_resetButton->setVisible(false);
    m_confirmButton->setVisible(true);

    m_userChooseBox->setVisible(true);
    m_passwordEdit->setVisible(true);
    m_passwordRepeat->setVisible(true);
}

void PasswordResetWidget::onPasswdSubmitted()
{
    const QString &pwd1 = m_passwordEdit->text();
    if (pwd1.isEmpty())
        return onPasswordEmpty();

    const QString &pwd2 = m_passwordRepeat->text();
    if (pwd2.isEmpty())
        return onPasswordRepeatEmpty();

    if (pwd1 != pwd2)
        return onPasswordNotMatch();

    m_confirmButton->setVisible(false);
    m_okButton->setVisible(false);

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
    thrd->setTaskInfo(data.first, data.second, m_passwordEdit->text());

    connect(thrd, &PasswordResetThread::finished, thrd, &PasswordResetThread::deleteLater, Qt::QueuedConnection);
    connect(thrd, &PasswordResetThread::finished, this, &PasswordResetWidget::onResetPasswordFinished);

    thrd->start();
}

void PasswordResetWidget::onResetPasswordFinished()
{
    m_okButton->setVisible(true);
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

    m_resetButton->setVisible(true);
    m_confirmButton->setVisible(false);
    m_okButton->setVisible(false);
    m_userChooseBox->setVisible(false);
    m_passwordEdit->setVisible(false);
    m_passwordRepeat->setVisible(false);
}
