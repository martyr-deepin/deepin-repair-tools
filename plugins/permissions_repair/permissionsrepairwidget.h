#ifndef PERMISSIONSREPAIRWIDGET_H
#define PERMISSIONSREPAIRWIDGET_H

#include <QWidget>
#include <QPushButton>

class PermissionsRepairWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PermissionsRepairWidget(QWidget *parent = nullptr);

private slots:
    void onRepairButtonClicked();

private:
    QPushButton *m_repairButton;
};

#endif // PERMISSIONSREPAIRWIDGET_H
