#ifndef REPAIRTOOLS_H
#define REPAIRTOOLS_H

#include <DMainWindow>

DWIDGET_USE_NAMESPACE

class RepairTools : public DMainWindow
{
    Q_OBJECT

public:
    explicit RepairTools(QWidget *parent = nullptr);

private:
    void keyPressEvent(QKeyEvent *e);
};

#endif // REPAIRTOOLS_H
