#ifndef DISKCLEANWIDGET_H
#define DISKCLEANWIDGET_H

#include <QWidget>
#include <QPushButton>

class DiskCleanWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DiskCleanWidget(QWidget *parent = nullptr);

private:
    QPushButton *m_cleanButton;
    QPushButton *m_cancelButton;
    QPushButton *m_okButton;
};

#endif // DISKCLEANWIDGET_H
