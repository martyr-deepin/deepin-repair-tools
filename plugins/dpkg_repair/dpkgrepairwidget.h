#ifndef DPKGREPAIRWIDGET_H
#define DPKGREPAIRWIDGET_H

#include <QWidget>
#include <QPushButton>

class DPKGRepairWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DPKGRepairWidget(QWidget *parent = nullptr);

private slots:
    void onRepairClicked();
    void onRepairFinished();

private:
    QPushButton *m_repairButton;
};

#endif // DPKGREPAIRWIDGET_H
