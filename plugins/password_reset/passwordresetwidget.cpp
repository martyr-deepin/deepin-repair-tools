#include "passwordresetwidget.h"

#include <QVBoxLayout>
#include <QTimer>

PasswordResetWidget::PasswordResetWidget(QWidget *parent)
    : QWidget(parent)
    , m_normalTips(new QLabel)
    , m_successTips(new QLabel)
    , m_resetButton(new QPushButton)
    , m_confirmButton(new QPushButton)
    , m_okButton(new QPushButton)
    , m_userChooseBox(new QComboBox)
    , m_passwordEdit(new DPasswordEdit)
    , m_passwordRepeat(new DPasswordEdit)
{
    m_resetButton->setText(tr("Password Reset"));
    m_confirmButton->setText(tr("Confirm"));
    m_confirmButton->setVisible(false);
    m_okButton->setText(tr("OK"));
    m_okButton->setVisible(false);

    m_userChooseBox->setVisible(false);
    m_passwordEdit->setVisible(false);
    m_passwordRepeat->setVisible(false);

    QHBoxLayout *btnsLayout = new QHBoxLayout;
    btnsLayout->addWidget(m_resetButton);
    btnsLayout->setAlignment(m_resetButton, Qt::AlignHCenter);
    btnsLayout->addWidget(m_confirmButton);
    btnsLayout->setAlignment(m_confirmButton, Qt::AlignHCenter);
    btnsLayout->addWidget(m_okButton);
    btnsLayout->setAlignment(m_okButton, Qt::AlignHCenter);

    QVBoxLayout *centralLayout = new QVBoxLayout;
    centralLayout->addWidget(m_userChooseBox);
    centralLayout->setAlignment(m_userChooseBox, Qt::AlignHCenter);
    centralLayout->addWidget(m_passwordEdit);
    centralLayout->setAlignment(m_passwordEdit, Qt::AlignHCenter);
    centralLayout->addWidget(m_passwordRepeat);
    centralLayout->setAlignment(m_passwordRepeat, Qt::AlignHCenter);
    centralLayout->addLayout(btnsLayout);

    setLayout(centralLayout);

    connect(m_resetButton, &QPushButton::clicked, this, &PasswordResetWidget::onResetClicked);
    connect(m_confirmButton, &QPushButton::clicked, this, &PasswordResetWidget::onPasswdSubmitted);

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
    m_okButton->setVisible(true);

    m_userChooseBox->setVisible(false);
    m_passwordEdit->setVisible(false);
    m_passwordRepeat->setVisible(false);
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
            const QString text = tr("%1 (on %2)").arg(userInfo.name).arg(disk);
            m_userChooseBox->addItem(text);
        }
    }
}
