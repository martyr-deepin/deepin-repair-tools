#ifndef DISKCLEANWIDGET_H
#define DISKCLEANWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QPointer>
#include <QProcess>

class DiskCleanWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DiskCleanWidget(QWidget *parent = nullptr);

private slots:
    void cleanStart();
    void cleanCancel();
    void cleanEnd();

private:
    QPointer<QProcess> m_process;
    QPushButton *m_cleanButton;
    QPushButton *m_cancelButton;
    QPushButton *m_okButton;
};

#endif // DISKCLEANWIDGET_H
